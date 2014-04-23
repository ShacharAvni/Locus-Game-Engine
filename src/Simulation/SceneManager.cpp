 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#include "Locus/Simulation/SceneManager.h"
#include "Locus/Simulation/Scene.h"
#include "Locus/Simulation/UserEvents.h"

#include "Locus/Common/Util.h"

namespace Locus
{

SceneManager::SceneManager(Window& window)
   : window(window), lastMouseX(0), lastMouseY(0)
{
   window.RegisterEventListener(this);
}

void SceneManager::AddScene(std::unique_ptr<Scene> scene)
{
   sceneStack.push( std::move(scene) );
   sceneStack.top()->Activate();
}

void SceneManager::HideMouse()
{
   window.SetMouseState(Window::MouseState::Hidden);
}

void SceneManager::CenterMouse()
{
   int windowWidth = 0;
   int windowHeight = 0;
   window.GetSize(windowWidth, windowHeight);

   int midX = windowWidth/2;
   int midY = windowHeight/2;

   window.SetMousePosition(midX, midY);

   lastMouseX = midX;
   lastMouseY = midY;
}

void SceneManager::SyncWindowSizeToSceneSize()
{
   int width = 0;
   int height = 0;
   window.GetSize(width, height);

   sceneStack.top()->Resized(width, height);
}

void SceneManager::MakeWindowed()
{
   window.MakeWindowed();
   sceneStack.top()->InitializeRenderingState();
}

void SceneManager::MakeFullScreen()
{
   window.MakeFullScreen(true);
   sceneStack.top()->InitializeRenderingState();
}

double SceneManager::ComputeDT() 
{
   std::chrono::time_point<std::chrono::system_clock> thisTime = std::chrono::system_clock::now();

   double DT = (std::chrono::duration_cast<std::chrono::milliseconds> (thisTime - lastUpdateTime).count()) * 0.001;

   if (DT < 0.001) DT = 0.001; //Less than 1 millisecond (the resolution of "timeNow").
   if (DT > 0.2) DT = 0.2; //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.

   lastUpdateTime = thisTime;

   return DT;
}

void SceneManager::ResetLastUpdateTime()
{
   lastUpdateTime = std::chrono::system_clock::now();
}

void SceneManager::KeyPressed(int key)
{
   sceneStack.top()->KeyPressed(key);
}

void SceneManager::KeyReleased(int key)
{
   sceneStack.top()->KeyReleased(key);
}

void SceneManager::MousePressed(int button)
{
   sceneStack.top()->MousePressed(button, lastMouseX, lastMouseY);
}

void SceneManager::MouseReleased(int button)
{
   sceneStack.top()->MouseReleased(button, lastMouseX, lastMouseY);
}

void SceneManager::MouseMoved(int x, int y)
{
   int moveX = x - lastMouseX;
   int moveY = y - lastMouseY;

   lastMouseX = moveX;
   lastMouseY = moveY;

   sceneStack.top()->MouseMoved(moveX, moveY);
}

void SceneManager::WindowSized(int width, int height)
{
   sceneStack.top()->Resized(width, height);
}

void SceneManager::NextFrame()
{
   double DT = ComputeDT();

   if (sceneStack.top()->Update(DT))
   {
      sceneStack.top()->Draw();
   }
   else
   {
      sceneStack.pop();

      if (sceneStack.size() > 0)
      {
         sceneStack.top()->Activate();
         SyncWindowSizeToSceneSize();
         ResetLastUpdateTime();
      }
      else
      {
         window.StopPollingEvents();
      }
   }
}

void SceneManager::RunSimulation(std::unique_ptr<Scene> scene)
{
   AddScene( std::move(scene) );

   SyncWindowSizeToSceneSize();

   ResetLastUpdateTime();

   window.StartPollingEvents();

   Util::ClearStack(sceneStack);
}

}