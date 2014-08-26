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
class LOCUS_GEOMETRY_API CollisionManager
{
public:
   CollisionManager();
   ~CollisionManager();

   CollisionManager(const CollisionManager&) = delete;
   CollisionManager& operator=(const CollisionManager&) = delete;

   void Add(Collidable* collidable);
   void Update(Collidable* collidable);
   void Remove(Collidable* collidable);
   void UpdateCollisions();
   void TransmitCollisions();

   void StartAddRemoveBatch();
   void FinishAddRemoveBatch();

   void Clear();

private:
   std::unique_ptr<CollisionManager_Impl> impl;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}