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
   void MakeWindowed();
   void MakeFullScreen();

private:
   Window& window;
   int lastMouseX;
   int lastMouseY;

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