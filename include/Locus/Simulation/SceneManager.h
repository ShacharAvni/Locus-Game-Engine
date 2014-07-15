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

#include "LocusSimulationAPI.h"

#include "Window.h"
#include "WindowEventListener.h"

#include <stack>
#include <memory>
#include <chrono>

namespace Locus
{

class Scene;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_SIMULATION_API SceneManager : public WindowEventListener
{
public:
   SceneManager(Window& window);
   SceneManager(const SceneManager&) = delete;
   SceneManager& operator=(const SceneManager&) = delete;

   void AddScene(std::unique_ptr<Scene> scene);

   void RunSimulation(std::unique_ptr<Scene> scene);

   void HideMouse();
   void CenterMouse();
   void GetMousePosition(int& x, int &y);
   void SetMousePosition(int x, int y);
   void MakeWindowed();
   void MakeFullScreen();

private:
   Window& window;

   std::stack<std::unique_ptr<Scene>> sceneStack;
   std::chrono::time_point<std::chrono::system_clock> lastUpdateTime;

   void ResetLastUpdateTime();
   double ComputeDT();

   void SyncWindowSizeToSceneSize();

   virtual void KeyPressed(int key) override;
   virtual void KeyReleased(int key) override;
   virtual void MousePressed(int button) override;
   virtual void MouseReleased(int button) override;
   virtual void MouseMoved(int x, int y) override;
   virtual void WindowSized(int width, int height) override;
   virtual void NextFrame() override;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}