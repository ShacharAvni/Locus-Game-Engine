 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Geometry/CollisionManager.h"
#include "Locus/Geometry/Collidable.h"

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"
#include "Locus/Common/SequentialIDGenerator.h"
#include "Locus/Common/Exception.h"

#include <vector>
#include <set>
#include <unordered_map>
#include <forward_list>
#include <algorithm>
#include <list>
#include <functional>

#include <assert.h>

namespace Locus
{

struct CollisionInterval
{
   CollisionInterval()
   : collidableID(0), min(0.0), max(0.0)
   {
   }

   CollisionInterval(ID_t collidableID, float min, float max)
      : collidableID(collidableID), min(min), max(max)
   {
   }

   bool Intersects(const CollisionInterval& other) const
   {
      return(!(Float::FGreater<float>(min, other.max) || Float::FLess<float>(max, other.min)));
   }

   bool operator <(const CollisionInterval& other) const
   {
      return Float::FLess<float>(min, other.min);
   }

   ID_t collidableID;
   float min;
   float max;
};

struct CollisionIntervalComparator
{
   bool operator()(const CollisionInterval* interval1, const CollisionInterval* interval2) const
   {
      return *interval1 < *interval2;
   }
};

struct CollisionManager_Impl
{
   CollisionManager_Impl()
      : doUpdateCollisionCollections(true)
   {
   }

   SequentialIDGenerator idGenerator;

   std::set<ID_t> collidableIDSet;
   std::unordered_map<ID_t, Collidable*> collidables;

   std::unordered_map<ID_t, std::unique_ptr<CollisionInterval>> xIntervals;
   std::unordered_map<ID_t, std::unique_ptr<CollisionInterval>> yIntervals;
   std::unordered_map<ID_t, std::unique_ptr<CollisionInterval>> zIntervals;

   std::vector<CollisionInterval*> xSortedIntervals;
   std::vector<CollisionInterval*> ySortedIntervals;
   std::vector<CollisionInterval*> zSortedIntervals;

   std::unordered_map<ID_t, std::size_t> collidableIDToIndex;

   //collisions tables
   std::vector<std::vector<bool>> xCollisions;
   std::vector<std::vector<bool>> yCollisions;

   std::forward_list<std::pair<ID_t, ID_t>> collisionList;

   bool doUpdateCollisionCollections;

   void UpdateCollisionCollections();
   void UpdateCollisions(std::vector<CollisionInterval*>& sortedIntervals, const std::function<void(std::size_t, std::size_t, ID_t, ID_t)>& intervalIntersectionFunction);
};

CollisionManager::CollisionManager()
   : impl(new CollisionManager_Impl())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Add(Collidable& collidable)
{
   assert(impl->collidables.find(collidable.collidableID) == impl->collidables.end());

   collidable.collidableID = impl->idGenerator.NextID();

   if (collidable.collidableID == BAD_ID)
   {
      throw Exception("Bad ID generated for Collidable");
   }

   impl->collidables.insert(std::make_pair<ID_t, Collidable*>(std::move(collidable.collidableID), &collidable));

   impl->collidableIDSet.insert(collidable.collidableID);

   impl->xIntervals[collidable.collidableID] = std::unique_ptr<CollisionInterval>(new CollisionInterval(collidable.collidableID, collidable.broadCollisionExtentMin.x, collidable.broadCollisionExtentMax.x));
   impl->yIntervals[collidable.collidableID] = std::unique_ptr<CollisionInterval>(new CollisionInterval(collidable.collidableID, collidable.broadCollisionExtentMin.y, collidable.broadCollisionExtentMax.y));
   impl->zIntervals[collidable.collidableID] = std::unique_ptr<CollisionInterval>(new CollisionInterval(collidable.collidableID, collidable.broadCollisionExtentMin.z, collidable.broadCollisionExtentMax.z));

   if (impl->doUpdateCollisionCollections)
   {
      impl->UpdateCollisionCollections();
   }
}

void CollisionManager_Impl::UpdateCollisionCollections()
{
   std::size_t numCollidables = collidableIDSet.size();

   collidableIDToIndex.clear();

   xSortedIntervals.clear();
   xSortedIntervals.reserve(numCollidables);

   ySortedIntervals.clear();
   ySortedIntervals.reserve(numCollidables);

   zSortedIntervals.clear();
   zSortedIntervals.reserve(numCollidables);

   std::size_t index = 0;
   for (const ID_t& collidableID : collidableIDSet)
   {
      //"index" is an index into the collisions tables
      collidableIDToIndex[collidableID] = index;

      xSortedIntervals.push_back( xIntervals[collidableID].get() );
      ySortedIntervals.push_back( yIntervals[collidableID].get() );
      zSortedIntervals.push_back( zIntervals[collidableID].get() );

      ++index;
   }

   std::sort(xSortedIntervals.begin(), xSortedIntervals.end(), CollisionIntervalComparator());
   std::sort(ySortedIntervals.begin(), ySortedIntervals.end(), CollisionIntervalComparator());
   std::sort(zSortedIntervals.begin(), zSortedIntervals.end(), CollisionIntervalComparator());
      
   xCollisions = std::vector<std::vector<bool>>(numCollidables, std::vector<bool>(numCollidables));
   yCollisions = std::vector<std::vector<bool>>(numCollidables, std::vector<bool>(numCollidables));
}

void CollisionManager_Impl::UpdateCollisions(std::vector<CollisionInterval*>& sortedIntervals, const std::function<void(std::size_t, std::size_t, ID_t, ID_t)>& intervalIntersectionFunction)
{
   Util::InsertionSort<CollisionInterval*>(sortedIntervals, CollisionIntervalComparator());

   std::list<const CollisionInterval*> intervalCheckList;

   for (CollisionInterval* interval : sortedIntervals)
   {
      std::list<const CollisionInterval*>::iterator checkListIter = intervalCheckList.begin();

      while (checkListIter != intervalCheckList.end())
      {
         if (interval->Intersects(**checkListIter))
         {
            std::size_t index1 = collidableIDToIndex[interval->collidableID];
            std::size_t index2 = collidableIDToIndex[(*checkListIter)->collidableID];

            intervalIntersectionFunction(index1, index2, interval->collidableID, (*checkListIter)->collidableID);

            ++checkListIter;
         }
         else
         {
            checkListIter = intervalCheckList.erase(checkListIter);
         }
      }

      intervalCheckList.push_back(interval);
   }
}

void CollisionManager::Update(Collidable& collidable)
{
   assert(impl->collidables.find(collidable.collidableID) != impl->collidables.end());

   impl->xIntervals[collidable.collidableID]->min = collidable.broadCollisionExtentMin.x;
   impl->xIntervals[collidable.collidableID]->max = collidable.broadCollisionExtentMax.x;

   impl->yIntervals[collidable.collidableID]->min = collidable.broadCollisionExtentMin.y;
   impl->yIntervals[collidable.collidableID]->max = collidable.broadCollisionExtentMax.y;

   impl->zIntervals[collidable.collidableID]->min = collidable.broadCollisionExtentMin.z;
   impl->zIntervals[collidable.collidableID]->max = collidable.broadCollisionExtentMax.z;
}

void CollisionManager::Remove(Collidable& collidable)
{
   assert(impl->collidables.find(collidable.collidableID) != impl->collidables.end());

   impl->collidables.erase(collidable.collidableID);
   impl->collidableIDSet.erase(collidable.collidableID);

   impl->xIntervals.erase(collidable.collidableID);
   impl->yIntervals.erase(collidable.collidableID);
   impl->zIntervals.erase(collidable.collidableID);

   collidable.collidableID = BAD_ID;

   if (impl->doUpdateCollisionCollections)
   {
      impl->UpdateCollisionCollections();
   }
}

void CollisionManager::StartAddRemoveBatch()
{
   impl->doUpdateCollisionCollections = false;
}

void CollisionManager::FinishAddRemoveBatch()
{
   impl->doUpdateCollisionCollections = true;

   impl->UpdateCollisionCollections();
}

void CollisionManager::Clear()
{
   impl->collidables.clear();
   impl->collidableIDSet.clear();

   impl->xIntervals.clear();
   impl->yIntervals.clear();
   impl->zIntervals.clear();

   impl->idGenerator.Reset();
}

void CollisionManager::UpdateCollisions()
{
   impl->collisionList.clear();
      
   std::size_t numCollidables = impl->collidables.size();

   if (numCollidables > 0)
   {
      for (std::size_t i = 0; i < numCollidables; ++i)
      {
         std::fill(impl->xCollisions[i].begin(), impl->xCollisions[i].end(), false);
         std::fill(impl->yCollisions[i].begin(), impl->yCollisions[i].end(), false);
      }

      impl->UpdateCollisions(impl->xSortedIntervals, [this](std::size_t index1, std::size_t index2, ID_t /*collidableID1*/, ID_t /*collidableID2*/)
      {
         impl->xCollisions[index1][index2] = true;
         impl->xCollisions[index2][index1] = true;
      });

      impl->UpdateCollisions(impl->ySortedIntervals, [this](std::size_t index1, std::size_t index2, ID_t /*collidableID1*/, ID_t /*collidableID2*/)
      {
         impl->yCollisions[index1][index2] = true;
         impl->yCollisions[index2][index1] = true;
      });

      impl->UpdateCollisions(impl->zSortedIntervals, [this](std::size_t index1, std::size_t index2, ID_t collidableID1, ID_t collidableID2)
      {
         if (impl->xCollisions[index1][index2] && impl->yCollisions[index1][index2])
         {
            impl->collisionList.emplace_front(std::pair<ID_t, ID_t>(collidableID1, collidableID2));
         }
      });
   }
}

void CollisionManager::TransmitCollisions()
{
   for (const std::pair<ID_t, ID_t>& collisionPair : impl->collisionList)
   {
      impl->collidables[collisionPair.first]->HandleCollision(*(impl->collidables[collisionPair.second]));
   }
}

}