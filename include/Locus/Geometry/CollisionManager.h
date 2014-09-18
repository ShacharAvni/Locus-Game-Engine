/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include <memory>

namespace Locus
{

class Collidable;

struct CollisionManager_Impl;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

//{CodeReview:BroadPhaseCollisions}
/// Responsible for broad phase collision detection between Collidable objects.
class LOCUS_GEOMETRY_API CollisionManager
{
public:
   CollisionManager();
   ~CollisionManager();

   CollisionManager(const CollisionManager&) = delete;
   CollisionManager& operator=(const CollisionManager&) = delete;

   /*!
    * \brief Adds a Collidable object to the underlying
    * collection.
    *
    * \note It is a no-op if the Collidable is already
    * in the collection.
    *
    * \sa StartAddRemoveBatch FinishAddRemoveBatch
    */
   void Add(Collidable* collidable);

   /*!
    * \brief Update the CollisionManager's copy
    * of the broad collision extent for the given
    * Collidable. This extent will be used in the
    * next call to UpdateCollisions.
    *
    * \note It is an error to call Update on a
    * Collidable that has not yet been added to
    * the underlying collection.
    *
    * \details This method should be called when
    * the Collidable's broad collision extent
    * changes.
    *
    * \sa Add UpdateCollisions
    */
   void Update(Collidable* collidable);

   /*!
    * \brief Removes a Collidable object from the underlying
    * collection.
    *
    * \note It is a no-op if the Collidable is not
    * in the collection.
    *
    * \sa StartAddRemoveBatch FinishAddRemoveBatch
    */
   void Remove(Collidable* collidable);

   /*!
    * \brief Update the active collision list. This list
    * consists of all pairs of Collidables whose broad
    * collision extents intersect.
    *
    * \sa Update
    */
   void UpdateCollisions();

   /*!
    * \brief ResolveCollision is called on all the pairs
    * of Collidables in the active collision list.
    *
    * \details ResolveCollision is called once
    * for each colliding pair. For example, if
    * the broad collision extents of two Collidables,
    * C1 and C2, were found to intersect, then only
    * C1.ResolveCollision(C2) is called.
    *
    * \sa UpdateCollisions Collidable::ResolveCollision
    */
   void TransmitCollisions();

   /*!
    * \brief Should be called for performance reasons
    * before multiple calls to Add or Remove.
    *
    * \details This call should be paired with a
    * corresponding call to FinishAddRemoveBatch after
    * the calls to Add or Remove.
    *
    * \sa Add Remove
    */
   void StartAddRemoveBatch();

   /// \sa StartAddRemoveBatch
   void FinishAddRemoveBatch();

   /// Clears the underlying collection of Collidable objects.
   void Clear();

private:
   std::unique_ptr<CollisionManager_Impl> impl;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}