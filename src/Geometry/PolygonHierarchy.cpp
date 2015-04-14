/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/PolygonHierarchy.h"
#include "Locus/Geometry/Polygon.h"

#include "Locus/Math/Vectors.h"

#include "Locus/Common/Float.h"

namespace Locus
{

template <class PolygonType>
PolygonHierarchy<PolygonType>::PolygonHierarchy(std::vector<PolygonType*>& polygons, PolygonWinding winding, const FVector3& normal, float toleranceFactor)
   : root(std::make_unique<Node>())
{
   root->polygon = nullptr;

   std::vector<PolygonType*> wellDefinedPolygons;
   wellDefinedPolygons.reserve(polygons.size());

   for (PolygonType* polygon : polygons)
   {
      if ((polygon != nullptr) && polygon->IsWellDefined())
      {
         wellDefinedPolygons.push_back(polygon);
      }
   }

   if (winding == PolygonWinding::Undefined)
   {
      winding = PolygonWinding::CounterClockwise;
   }

   FormHierarchyAtNode_R(root, wellDefinedPolygons, winding, normal, toleranceFactor);
}

template <class PolygonType>
void PolygonHierarchy<PolygonType>::DetermineTopLevelPolygonsAndTheirChildren(std::vector<PolygonType*>& polygons, float toleranceFactor, std::list<PolygonType*>& topLevelPolygons, std::list< std::vector<PolygonType*> >& children)
{
   topLevelPolygons.clear();
   children.clear();

   std::size_t numPolygons = polygons.size();

   if (numPolygons == 1)
   {
      topLevelPolygons.push_front(polygons[0]);
      children.resize(1);
   }
   else
   {
      std::vector<bool> topLevelPolygonCandidates(numPolygons, true);
      std::vector< std::vector<bool> > polygonIsInsideRelationship(numPolygons, std::vector<bool>(numPolygons, false));

      //TODO: improve this
      for (std::size_t polygonIndex = 0; polygonIndex < numPolygons - 1; ++polygonIndex)
      {
         for (std::size_t otherPolygonIndex = 1; otherPolygonIndex < numPolygons; ++otherPolygonIndex)
         {
            if ( polygons[polygonIndex]->PointIsWithinPolygon( (*polygons[otherPolygonIndex])[0], toleranceFactor ) )
            {
               //polygons[otherPolygonIndex] is in polygons[polygonIndex]
               topLevelPolygonCandidates[otherPolygonIndex] = false;
               polygonIsInsideRelationship[polygonIndex][otherPolygonIndex] = true;
            }

            if ( polygons[otherPolygonIndex]->PointIsWithinPolygon( (*polygons[polygonIndex])[0], toleranceFactor ) )
            {
               //polygons[polygonIndex] is in polygons[otherPolygonIndex]
               topLevelPolygonCandidates[polygonIndex] = false;
               polygonIsInsideRelationship[otherPolygonIndex][polygonIndex] = true;
            }
         }
      }

      for (std::size_t polygonIndex = 0; polygonIndex < numPolygons; ++polygonIndex)
      {
         if (topLevelPolygonCandidates[polygonIndex])
         {
            topLevelPolygons.push_back(polygons[polygonIndex]);

            std::vector<PolygonType*> childrenForThisTopLevelPolygon;

            for (std::size_t checkChildPolygonIndex = 0; checkChildPolygonIndex < numPolygons; ++checkChildPolygonIndex)
            {
               if (polygonIsInsideRelationship[polygonIndex][checkChildPolygonIndex])
               {
                  childrenForThisTopLevelPolygon.push_back(polygons[checkChildPolygonIndex]);
               }
            }

            children.push_back( std::move(childrenForThisTopLevelPolygon) );
         }
      }
   }
}

template <class PolygonType>
void PolygonHierarchy<PolygonType>::FormHierarchyAtNode_R(std::unique_ptr<Node>& node, std::vector<PolygonType*>& polygons, PolygonWinding winding, const FVector3& normal, float toleranceFactor)
{
   if (polygons.size() > 0)
   {
      std::list<PolygonType*> topLevelPolygons;
      std::list< std::vector<PolygonType*> > grandChildren;

      DetermineTopLevelPolygonsAndTheirChildren(polygons, toleranceFactor, topLevelPolygons, grandChildren);

      PolygonWinding childrenPolygonWinding = ReversePolygonWinding(winding);

      std::size_t numTopLevelPolygons = topLevelPolygons.size();

      node->children.resize(numTopLevelPolygons);

      std::size_t topLevelPolygonIndex = 0;

      do
      {
         PolygonType*& topLevelPolygon = topLevelPolygons.front();

         if (topLevelPolygon->GetWinding(normal) != winding)
         {
            topLevelPolygon->Reverse();
         }

         node->children[topLevelPolygonIndex] = std::make_unique<Node>();
         node->children[topLevelPolygonIndex]->polygon = topLevelPolygon;

         FormHierarchyAtNode_R(node->children[topLevelPolygonIndex], grandChildren.front(), childrenPolygonWinding, normal, toleranceFactor);

         grandChildren.pop_front();
         topLevelPolygons.pop_front();

         ++topLevelPolygonIndex;
      } while (topLevelPolygonIndex < numTopLevelPolygons);
   }
}

template <class PolygonType>
const typename PolygonHierarchy<PolygonType>::Node* PolygonHierarchy<PolygonType>::Root() const
{
   return root.get();
}

template class LOCUS_GEOMETRY_API PolygonHierarchy<Polygon2D_t>;
template class LOCUS_GEOMETRY_API PolygonHierarchy<Polygon3D_t>;

}