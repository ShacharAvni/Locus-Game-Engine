 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Simulation/Window.h"
#include "Locus/Simulation/WindowContext.h"
#include "Locus/Simulation/WindowEventListener.h"

#include "Locus/Common/Exception.h"

#include <Locus/Rendering/Locus_glew.h>

#include <GLFW/glfw3.h>

#include <cmath>

namespace Locus
{

void Window::ExternalKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
   Window* locusWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
   locusWindow->KeyCallback(key, scancode, action, mods);
}

void Window::ExternalMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
   Window* locusWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
   locusWindow->MouseButtonCallback(button, action, mods);
}

void Window::ExternalMousePositionCallback(GLFWwindow* glfwWindow, double x, double y)
{
   Window* locusWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
   locusWindow->MousePositionCallback(static_cast<int>(x), static_cast<int>(y));
}

void Window::ExternalSizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
   Window* locusWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
   locusWindow->SizeCallback(width, height);
}

Window::Window(WindowContext& windowContext, int width, int height, const std::string& name, bool fullScreen, int* targetRefreshRate)
   : windowContext(windowContext), name(name), glewContext(new GLEWContext()), glfwWindow(nullptr), windowEventListener(nullptr),
     fullScreen(fullScreen), pollingEvents(false)
{
   if (targetRefreshRate != nullptr)
   {
      this->targetRefreshRate.reset(new int(*targetRefreshRate));
   }

   MakeWindow(width, height);
}

Window::~Window()
{
   DestroyWindow();
}

void Window::MakeWindow(int width, int height)
{
   if (targetRefreshRate.get() != nullptr)
   {
      glfwWindowHint(GLFW_REFRESH_RATE, *targetRefreshRate);
   }
   else
   {
      glfwDefaultWindowHints();
   }

#ifdef _DEBUG
   GLFWmonitor* monitor = nullptr;
#else
   GLFWmonitor* monitor = fullScreen ? windowContext.primaryMonitor : nullptr;
#endif

   glfwWindow = glfwCreateWindow(width, height, name.c_str(), monitor, windowContext.sharedWindow);

   if (glfwWindow == nullptr)
   {
      throw Exception(std::string("Failed to create window: ") + name);
   }

   glfwSetWindowUserPointer(glfwWindow, this);
   glfwSetKeyCallback(glfwWindow, ExternalKeyCallback);
   glfwSetMouseButtonCallback(glfwWindow, ExternalMouseButtonCallback);
   glfwSetCursorPosCallback(glfwWindow, ExternalMousePositionCallback);
   glfwSetWindowSizeCallback(glfwWindow, ExternalSizeCallback);
}

void Window::DestroyWindow()
{
   ContextDestroyed(glewContext.get());
   glfwDestroyWindow(glfwWindow);
}

void Window::SwapFullScreenOrWindowedState(int width, int height)
{
   fullScreen = !fullScreen;

   bool contextIsCurrent = (glewGetContext() == glewContext.get());

   int widthBefore = 0;
   int heightBefore = 0;
   GetSize(widthBefore, heightBefore);

   DestroyWindow();

   MakeWindow(width, height);

   if (contextIsCurrent)
   {
      MakeContextCurrent();
   }

   int widthAfter = 0;
   int heightAfter = 0;
   GetSize(widthAfter, heightAfter);

   if ((widthAfter != widthBefore) || (heightAfter != heightBefore))
   {
      SizeCallback(widthAfter, heightAfter);
   }
}

void Window::SetSize(int width, int height)
{
   glfwSetWindowSize(glfwWindow, width, height);
}

void Window::GetSize(int& width, int& height)
{
   glfwGetWindowSize(glfwWindow, &width, &height);
}

void Window::MakeFullScreen(bool fitToScreen)
{
   if (!fullScreen)
   {
      int width = 0;
      int height = 0;

      if (fitToScreen)
      {
         windowContext.GetPrimaryMonitorWindowSize(width, height);
      }
      else
      {
         GetSize(width, height);
      }

      SwapFullScreenOrWindowedState(width, height);
   }
}

void Window::MakeWindowed()
{
   if (fullScreen)
   {
      int width = 0;
      int height = 0;
      GetSize(width, height);

      SwapFullScreenOrWindowedState(width, height);
   }
}

void Window::Close()
{
   glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
}

void Window::RegisterEventListener(WindowEventListener* windowEventListener)
{
   this->windowEventListener = windowEventListener;
}

void Window::MakeContextCurrent()
{
   glfwMakeContextCurrent(glfwWindow);
   ChangeCurrentContext(glewContext.get());

   if (glewInit() != GLEW_OK)
   {
      throw Exception("Failed to initialize GLEW");
   }
}

void Window::StartPollingEvents()
{
   pollingEvents = true;

   while (pollingEvents && !glfwWindowShouldClose(glfwWindow))
   {
      if (windowEventListener != nullptr)
      {
         windowEventListener->NextFrame();
      }

      glfwSwapBuffers(glfwWindow);

      if (pollingEvents)
      {
         glfwPollEvents();
      }
   }
}

void Window::StopPollingEvents()
{
   pollingEvents = false;
}

void Window::SetMouseState(MouseState state)
{
   switch (state)
   {
   case MouseState::Normal:
   default:
      glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;

   case MouseState::Hidden:
      glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      break;

   case MouseState::Disabled:
      glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      break;
   }
}

void Window::SetMousePosition(int x, int y)
{
   glfwSetCursorPos(glfwWindow, x, y);
}

void Window::GetMousePositionInternal(double& x, double& y)
{
   glfwGetCursorPos(glfwWindow, &x, &y);
}

void Window::GetMousePosition(int& x, int& y)
{
   double doubleX = 0.0;
   double doubleY = 0.0;
   GetMousePositionInternal(doubleX, doubleY);

   x = static_cast<int>(std::floor(doubleX));
   y = static_cast<int>(std::floor(doubleY));
}

void Window::KeyCallback(int key, int /*scancode*/, int action, int /*mods*/)
{
   if (windowEventListener != nullptr)
   {
      if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
      {
         windowEventListener->KeyPressed(key);
      }
      else
      {
         windowEventListener->KeyReleased(key);
      }
   }
}

void Window::MouseButtonCallback(int button, int action, int /*mods*/)
{
   if (windowEventListener != nullptr)
   {
      if (action == GLFW_PRESS)
      {
         windowEventListener->MousePressed(button);
      }
      else
      {
         windowEventListener->MouseReleased(button);
      }
   }
}

void Window::MousePositionCallback(double x, double y)
{
   if (windowEventListener != nullptr)
   {
      windowEventListener->MouseMoved( static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)) );
   }
}

void Window::SizeCallback(int width, int height)
{
   if (windowEventListener != nullptr)
   {
      windowEventListener->WindowSized(width, height);
   }
}

}