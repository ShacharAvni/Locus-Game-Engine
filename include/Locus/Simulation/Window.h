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

#include <string>
#include <memory>

extern "C"
{

struct GLFWwindow;

struct GLEWContextStruct;
typedef struct GLEWContextStruct GLEWContext;

}

namespace Locus
{

class WindowContext;
class WindowEventListener;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_SIMULATION_API Window
{
public:
   Window(WindowContext& windowContext, int width, int height, const std::string& name, bool fullScreen, int* targetRefreshRate);
   Window(const Window&) = delete;
   Window& operator=(const Window&) = delete;
   ~Window();

   void RegisterEventListener(WindowEventListener* windowEventListener);

   void SetSize(int width, int height);
   void GetSize(int& width, int& height);

   void MakeFullScreen(bool fitToScreen);
   void MakeWindowed();

   void Close();

   void MakeContextCurrent();

   void StartPollingEvents();
   void StopPollingEvents();

   enum class MouseState
   {
      Normal,
      Hidden,
      Disabled
   };

   void SetMouseState(MouseState state);
   void SetMousePosition(int x, int y);
   void GetMousePosition(int& x, int& y);

private:
   WindowContext& windowContext;

   std::string name;
   std::unique_ptr<GLEWContext> glewContext;
   std::unique_ptr<int> targetRefreshRate;

   GLFWwindow* glfwWindow;
   WindowEventListener* windowEventListener;
   bool fullScreen;
   bool pollingEvents;

   void MakeWindow(int width, int height);
   void DestroyWindow();
   void SwapFullScreenOrWindowedState(int width, int height);

   void GetMousePositionInternal(double& x, double& y);

   static void ExternalKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
   static void ExternalMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
   static void ExternalMousePositionCallback(GLFWwindow* glfwWindow, double x, double y);
   static void ExternalSizeCallback(GLFWwindow* glfwWindow, int width, int height);

   void KeyCallback(int key, int scancode, int action, int mods);
   void MouseButtonCallback(int button, int action, int mods);
   void MousePositionCallback(double x, double y);
   void SizeCallback(int width, int height);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}