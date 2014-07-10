/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Preprocessor/CompilerDefinitions.h"

#ifdef LOCUS_WINDOWS
   #define NOMINMAX
   #include <windows.h>
#endif

#include "Locus/Simulation/SceneManager.h"
#include "Locus/Simulation/Window.h"
#include "Locus/Simulation/WindowContext.h"

#include "Locus/Common/Exception.h"

#include <Locus/Rendering/Locus_glew.h>

#include "CollisionScene.h"

#include <string>

#ifndef LOCUS_WINDOWS
   #include <iostream>
#endif

#include <stdlib.h>

static const char* Window_Name = "Locus Collisions Example";

void ShowFatalError(const std::string& error)
{
#ifdef LOCUS_WINDOWS
   MessageBox(GetActiveWindow(), error.c_str(), "Fatal Error", MB_ICONEXCLAMATION | MB_OK);
#else
   std::cout << "Fatal Error: " << error.c_str() << std::endl;
#endif
}

#if defined(LOCUS_WINDOWS)
  int CALLBACK WinMain(
  _In_  HINSTANCE /*hInstance*/,
  _In_  HINSTANCE /*hPrevInstance*/,
  _In_  LPSTR /*lpCmdLine*/,
  _In_  int /*nCmdShow*/
  )
#else
   int main(int argc, char** argv)
#endif
{
   try
   {
      Locus::WindowContext windowContext;

      int monitorWidth, monitorHeight;
      windowContext.GetPrimaryMonitorWindowSize(monitorWidth, monitorHeight);

      int targetRefreshRate = 60;

      Locus::Window window(windowContext, monitorWidth, monitorHeight, Window_Name, true, &targetRefreshRate);

      window.MakeContextCurrent();

      if (!GLEW_VERSION_2_0)
      {
         ShowFatalError("OpenGL 2.0 is required to run this application");
         return EXIT_FAILURE;
      }

      Locus::SceneManager sceneManager(window);

      sceneManager.RunSimulation( std::unique_ptr<Locus::Scene>(new Locus::Examples::CollisionScene(sceneManager, monitorWidth, monitorHeight)) );
   }
   catch (Locus::Exception& locusException)
   {
      ShowFatalError(locusException.Message());
      return EXIT_FAILURE;
   }
   catch (std::exception& stdException)
   {
      ShowFatalError(stdException.what());
      return EXIT_FAILURE;
   }
   catch (...)
   {
      ShowFatalError("Unknown Error");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}