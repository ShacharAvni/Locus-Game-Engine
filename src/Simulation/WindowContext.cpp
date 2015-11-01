/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Simulation/WindowContext.h"

#include "Locus/Common/ScopeFinalizer.h"
#include "Locus/Common/Exception.h"

#include <GLFW/glfw3.h>

namespace Locus
{

WindowContext::WindowContext()
{
   if (glfwInit() == 0)
   {
      throw Exception("Failed to initialize GLFW");
   }

   ScopeFinalizer terminateGLFWOnError([]()
   {
      glfwTerminate();
   });

   primaryMonitor = glfwGetPrimaryMonitor();

   if (primaryMonitor == nullptr)
   {
      throw Exception("Failed to retrieve the primary monitor");
   }

   primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

   if (primaryMonitorVideoMode == nullptr)
   {
      throw Exception("Failed to access the video mode of the primary monitor");
   }

   glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

   sharedWindow = glfwCreateWindow(5, 5, "Locus Hidden Window", NULL, NULL);

   glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

   if (sharedWindow == nullptr)
   {
      throw Exception("Failed to create the shared window");
   }

   terminateGLFWOnError.Cancel();
}

WindowContext::~WindowContext()
{
   glfwDestroyWindow(sharedWindow);

   glfwTerminate();
}

void WindowContext::GetPrimaryMonitorWindowSize(int& width, int &height) const
{
   width = primaryMonitorVideoMode->width;
   height = primaryMonitorVideoMode->height;
}

GLFWwindow* WindowContext::GetSharedWindow() const
{
   return sharedWindow;
}

GLFWmonitor* WindowContext::GetPrimaryMonitor() const
{
   return primaryMonitor;
}

}