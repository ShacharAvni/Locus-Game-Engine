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

#include "Locus/Math/Vectors.h"

namespace Locus
{

/*!
 * \brief Objects should derive from this class
 * to be usable with the CollisionManager (for
 * broad phase collision detection).
 *
 * \sa CollisionManager
 */
class LOCUS_GEOMETRY_API Collidable
{
public:
   Collidable();
   virtual ~Collidable();

   Collidable(const Collidable&) = default;
   Collidable& operator=(const Collidable&) = default;

   /// \sa collidableType
   unsigned int GetCollidableType() const;

   /*!
    * \return The min corner point of the axis-aligned
    * cube used as the broad phase collision extent
    * for this object.
    */
   const FVector3& GetBroadCollisionExtentMin() const;

   /*!
    * \return The max corner point of the axis-aligned
    * cube used as the broad phase collision extent
    * for this object.
    */
   const FVector3& GetBroadCollisionExtentMax() const;

   /*!
    * \brief Overriders should determine the new
    * broad phase collision extent and set it using
    * UpdateBroadCollisionExtent(const Vector3&, float)
    *
    * \sa UpdateBroadCollisionExtent(const Vector3&, float)
    */
   virtual void UpdateBroadCollisionExtent() = 0;

   /*!
    * \brief Overriders should return true if the given
    * Collidable can collide with this object. This
    * method returns true by default.
    *
    * \details Say we have a Player (collidableType = 0)
    * that can shoot Bullets (collidableType = 1) that can
    * ricochet and we have Enemies (collidableType = 2) and
    * we don't want the Bullets to hit the Player. Thus,
    * instanceOfPlayer.CollidesWith(instanceOfBullet) should
    * return false (and vice versa). instanceOfPlayer.CollidesWith(instanceOfEnemy)
    * should return true (and vice versa). instanceOfBullet.CollidesWith(instanceOfEnemy)
    * should return true (and vice versa). It is suggested to
    * use the collidableType of the given Collidable for determining
    * collidability.
    */
   virtual bool CollidesWith(Collidable& collidable) const;

   /*!
    * \brief Overriders should do narrow phase collision
    * detection and response.
    *
    * \details The collidableType of the given Collidable
    * should be used for determining an appropriate type
    * for a dynamic_cast, followed by a specialized narrow
    * phase collision detection and response.
    */
   virtual void ResolveCollision(Collidable& collidable) = 0;

protected:
   /*!
    * \brief Collidables would use this to identify
    * the type of the Collidable passed to CollidesWith
    * or ResolveCollision.
    *
    * \details This should be the same for each instance
    * of the same class and it should be unique for each
    * class that derives from Collidable.
    */
   unsigned int collidableType;

   /*!
    * \brief Sets the axis-aligned cube used for broad
    * phase collision detection.
    *
    * \param[in] centroid The centroid of the axis-aligned
    * cube.
    *
    * \param[in] halfLength Half the side length of the cube.
    */
   void UpdateBroadCollisionExtent(const FVector3& centroid, float halfLength);

private:
   FVector3 broadCollisionExtentMin;
   FVector3 broadCollisionExtentMax;
};

}