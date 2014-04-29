 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusSimulationAPI.h"

#include "UserEvents.h"

namespace Locus
{

class SceneManager;

class LOCUS_SIMULATION_API Scene
{
public:
   Scene(SceneManager& sceneManager);
   Scene(const Scene&) = delete;
   Scene& operator=(const Scene&) = delete;
   virtual ~Scene();

   virtual void Activate();

   virtual bool Update(double DT);
   virtual void Draw();

   virtual void InitializeRenderingState();

   virtual void KeyPressed(Key_t key);
   virtual void KeyReleased(Key_t key);

   virtual void MousePressed(MouseButton_t button, int x, int y);
   virtual void MouseReleased(MouseButton_t button, int x, int y);
   virtual void MouseMoved(int x, int y);

   virtual void Resized(int width, int height);

protected:
   SceneManager& sceneManager;
};

}