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

namespace Locus
{

class LOCUS_SIMULATION_API WindowEventListener
{
friend class Window;

public:
   virtual ~WindowEventListener()
   {
   }

private:
   virtual void KeyPressed(int key) = 0;
   virtual void KeyReleased(int key) = 0;
   virtual void MousePressed(int button) = 0;
   virtual void MouseReleased(int button) = 0;
   virtual void MouseMoved(int x, int y) = 0;
   virtual void WindowSized(int width, int height) = 0;
   virtual void NextFrame() = 0;
};

}