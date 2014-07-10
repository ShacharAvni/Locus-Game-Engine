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

#include "Locus/Simulation/Scene.h"

#include "Locus/Geometry/CollisionManager.h"

#include "Locus/Rendering/Viewpoint.h"
#include "Locus/Rendering/LineCollection.h"

#include "CollidableMesh.h"

#include <memory>

namespace Locus
{

class SceneManager;
class RenderingState;

namespace Examples
{

class CollisionScene : public Locus::Scene
{
public:
   CollisionScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY);
   ~CollisionScene();

   virtual void Activate() override;

   virtual bool Update(double DT) override;
   virtual void Draw() override;

   virtual void InitializeRenderingState() override;

   virtual void KeyPressed(Locus::Key_t key) override;
   virtual void KeyReleased(Locus::Key_t key) override;

   virtual void MouseMoved(int x, int y) override;

   virtual void Resized(int width, int height) override;

private:
   std::unique_ptr< Locus::RenderingState > renderingState;

   Locus::CollisionManager collisionManager;

   Locus::Viewpoint viewpoint;

   std::vector<CollidableMesh> collidableMeshes;

   Locus::LineCollection boundary;

   bool dieOnNextFrame;

   bool paused;

   unsigned int resolutionX;
   unsigned int resolutionY;

   bool moveViewerAhead;
   bool moveViewerBack;
   bool moveViewerRight;
   bool moveViewerLeft;
   bool moveViewerUp;
   bool moveViewerDown;

   void InitializeCollidableMeshes();
   void InitializeBoundary();

   void DestroyRenderingState();

   void TickCollidableMeshes(double DT);
   void TickViewer(double DT);
};

}

}