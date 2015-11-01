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

extern "C"
{

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

}

namespace Locus
{

class LOCUS_SIMULATION_API WindowContext
{
public:
   WindowContext();
   ~WindowContext();

   void GetPrimaryMonitorWindowSize(int& width, int &height) const;

   GLFWwindow* GetSharedWindow() const;
   GLFWmonitor* GetPrimaryMonitor() const;

private:
   GLFWwindow* sharedWindow;
   GLFWmonitor* primaryMonitor;
   const GLFWvidmode* primaryMonitorVideoMode;
};

}