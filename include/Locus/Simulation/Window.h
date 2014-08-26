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
   ~Window();

   Window(const Window&) = delete;
   Window& operator=(const Window&) = delete;

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