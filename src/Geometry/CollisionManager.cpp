/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/CollisionManager.h"
#include "Locus/Geometry/Collidable.h"

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <forward_list>
#include <algorithm>
#include <list>
#include <functional>

#include <cassert>

namespace Locus
{

struct CollisionInterval
{
   CollisionInterval()
      : owner(nullptr), min(0.0), max(0.0)
   {
   }

   CollisionInterval(Collidable* owner, float min, float max)
      : owner(owner), min(min), max(max)
   {
   }

   bool Intersects(const CollisionInterval& other) const
   {
      return(! (FGreater<float>(min, other.max) || FLess<float>(max, other.min)) );
   }

   bool operator <(const CollisionInterval& other) const
   {
      return FLess<float>(min, other.min);
   }

   Collidable* owner;
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

   std::unordered_set<Collidable*> collidables;

   std::unordered_map<Collidable*, std::unique_ptr<CollisionInterval>> xIntervals;
   std::unordered_map<Collidable*, std::unique_ptr<CollisionInterval>> yIntervals;
   std::unordered_map<Collidable*, std::unique_ptr<CollisionInterval>> zIntervals;

   std::vector<CollisionInterval*> xSortedIntervals;
   std::vector<CollisionInterval*> ySortedIntervals;
   std::vector<CollisionInterval*> zSortedIntervals;

   std::unordered_map<Collidable*, std::size_t> collidableToIndex;

   //collisions tables
   std::vector<std::vector<bool>> xCollisions;
   std::vector<std::vector<bool>> yCollisions;

   std::forward_list<std::pair<Collidable*, Collidable*>> collisionList;

   bool doUpdateCollisionCollections;

   void UpdateCollisionCollections();
   void UpdateCollisions(std::vector<CollisionInterval*>& sortedIntervals, const std::function<void(std::size_t, std::size_t, Collidable*, Collidable*)>& intervalIntersectionFunction);
};

CollisionManager::CollisionManager()
   : impl(std::make_unique<CollisionManager_Impl>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Add(Collidable* collidable)
{
   bool inserted = impl->collidables.insert(collidable).second;

   if (inserted)
   {
      const Vector3& broadCollisionExtentMin = collidable->GetBroadCollisionExtentMin();
      const Vector3& broadCollisionExtentMax = collidable->GetBroadCollisionExtentMax();

      impl->xIntervals[collidable] = std::make_unique<CollisionInterval>(collidable, broadCollisionExtentMin.x, broadCollisionExtentMax.x);
      impl->yIntervals[collidable] = std::make_unique<CollisionInterval>(collidable, broadCollisionExtentMin.y, broadCollisionExtentMax.y);
      impl->zIntervals[collidable] = std::make_unique<CollisionInterval>(collidable, broadCollisionExtentMin.z, broadCollisionExtentMax.z);

      if (impl->doUpdateCollisionCollections)
      {
         impl->UpdateCollisionCollections();
      }
   }
}

void CollisionManager_Impl::UpdateCollisionCollections()
{
   std::size_t numCollidables = collidables.size();

   collidableToIndex.clear();

   xSortedIntervals.clear();
   xSortedIntervals.reserve(numCollidables);

   ySortedIntervals.clear();
   ySortedIntervals.reserve(numCollidables);

   zSortedIntervals.clear();
   zSortedIntervals.reserve(numCollidables);

   std::size_t index = 0;
   for (Collidable* collidable : collidables)
   {
      //"index" is an index into the collisions tables
      collidableToIndex[collidable] = index;

      xSortedIntervals.push_back( xIntervals[collidable].get() );
      ySortedIntervals.push_back( yIntervals[collidable].get() );
      zSortedIntervals.push_back( zIntervals[collidable].get() );

      ++index;
   }

   std::sort(xSortedIntervals.begin(), xSortedIntervals.end(), CollisionIntervalComparator());
   std::sort(ySortedIntervals.begin(), ySortedIntervals.end(), CollisionIntervalComparator());
   std::sort(zSortedIntervals.begin(), zSortedIntervals.end(), CollisionIntervalComparator());

   xCollisions = std::vector<std::vector<bool>>(numCollidables, std::vector<bool>(numCollidables));
   yCollisions = std::vector<std::vector<bool>>(numCollidables, std::vector<bool>(numCollidables));
}

//{CodeReview:BroadPhaseCollisions}
void CollisionManager_Impl::UpdateCollisions(std::vector<CollisionInterval*>& sortedIntervals, const std::function<void(std::size_t, std::size_t, Collidable*, Collidable*)>& intervalIntersectionFunction)
{
   InsertionSort<CollisionInterval*>(sortedIntervals, CollisionIntervalComparator());

   std::list<const CollisionInterval*> intervalCheckList;

   for (CollisionInterval* interval : sortedIntervals)
   {
      std::list<const CollisionInterval*>::iterator checkListIter = intervalCheckList.begin();

      while (checkListIter != intervalCheckList.end())
      {
         if (interval->Intersects(**checkListIter))
         {
            Collidable* collidable1 = interval->owner;
            Collidable* collidable2 = (*checkListIter)->owner;

            std::size_t index1 = collidableToIndex[collidable1];
            std::size_t index2 = collidableToIndex[collidable2];

            intervalIntersectionFunction(index1, index2, collidable1, collidable2);

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

void CollisionManager::Update(Collidable* collidable)
{
   assert(impl->collidables.find(collidable) != impl->collidables.end());

   const Vector3& broadCollisionExtentMin = collidable->GetBroadCollisionExtentMin();
   const Vector3& broadCollisionExtentMax = collidable->GetBroadCollisionExtentMax();

   impl->xIntervals[collidable]->min = broadCollisionExtentMin.x;
   impl->xIntervals[collidable]->max = broadCollisionExtentMax.x;

   impl->yIntervals[collidable]->min = broadCollisionExtentMin.y;
   impl->yIntervals[collidable]->max = broadCollisionExtentMax.y;

   impl->zIntervals[collidable]->min = broadCollisionExtentMin.z;
   impl->zIntervals[collidable]->max = broadCollisionExtentMax.z;
}

void CollisionManager::Remove(Collidable* collidable)
{
   bool erased = (impl->collidables.erase(collidable) == 1);

   if (erased)
   {
      impl->xIntervals.erase(collidable);
      impl->yIntervals.erase(collidable);
      impl->zIntervals.erase(collidable);

      if (impl->doUpdateCollisionCollections)
      {
         impl->UpdateCollisionCollections();
      }
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

   impl->xIntervals.clear();
   impl->yIntervals.clear();
   impl->zIntervals.clear();
}

//{CodeReview:BroadPhaseCollisions}
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

      impl->UpdateCollisions(impl->xSortedIntervals, [this](std::size_t index1, std::size_t index2, Collidable* /*collidable1*/, Collidable* /*collidable2*/)
      {
         impl->xCollisions[index1][index2] = true;
         impl->xCollisions[index2][index1] = true;
      });

      impl->UpdateCollisions(impl->ySortedIntervals, [this](std::size_t index1, std::size_t index2, Collidable* /*collidable1*/, Collidable* /*collidable2*/)
      {
         impl->yCollisions[index1][index2] = true;
         impl->yCollisions[index2][index1] = true;
      });

      impl->UpdateCollisions(impl->zSortedIntervals, [this](std::size_t index1, std::size_t index2, Collidable* collidable1, Collidable* collidable2)
      {
         if (impl->xCollisions[index1][index2] && impl->yCollisions[index1][index2])
         {
            impl->collisionList.emplace_front(std::pair<Collidable*, Collidable*>(collidable1, collidable2));
         }
      });
   }
}

//{CodeReview:BroadPhaseCollisions}
void CollisionManager::TransmitCollisions()
{
   for (const std::pair<Collidable*, Collidable*>& collisionPair : impl->collisionList)
   {
      if (collisionPair.first->CollidesWith(*(collisionPair.second)))
      {
         collisionPair.first->ResolveCollision(*(collisionPair.second));
      }
   }
}

}