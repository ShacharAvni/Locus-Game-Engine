/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Simulation/Scene.h"
#include "Locus/Simulation/SceneManager.h"

namespace Locus
{

Scene::Scene(SceneManager& sceneManager)
   : sceneManager(sceneManager)
{
}

Scene::~Scene()
{
}

void Scene::Activate()
{
}

bool Scene::Update(double /*DT*/)
{
   return true;
}

void Scene::Draw()
{
}

void Scene::InitializeRenderingState()
{
}

void Scene::KeyPressed(Key_t /*key*/)
{
}

void Scene::KeyReleased(Key_t /*key*/)
{
}

void Scene::MousePressed(MouseButton_t /*button*/, int /*x*/, int /*y*/)
{
}

void Scene::MouseReleased(MouseButton_t /*button*/, int /*x*/, int /*y*/)
{
}

void Scene::MouseMoved(int /*x*/, int /*y*/)
{
}

void Scene::Resized(int /*width*/, int /*height*/)
{
}

}