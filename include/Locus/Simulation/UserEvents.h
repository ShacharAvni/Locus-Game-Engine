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

//This header wraps some user events in glfw3.h

/*************************************************************************
 * GLFW 3.0 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#pragma once

namespace Locus
{

typedef int Key_t;

//The unknown key
const Key_t Key_UNKNOWN = -1;

//Printable keys
const Key_t Key_SPACE =             32;
const Key_t Key_APOSTROPHE =        39;
const Key_t Key_COMMA =             44;
const Key_t Key_MINUS =             45;
const Key_t Key_PERIOD =            46;
const Key_t Key_SLASH =             47;
const Key_t Key_0 =                 48;
const Key_t Key_1 =                 49;
const Key_t Key_2 =                 50;
const Key_t Key_3 =                 51;
const Key_t Key_4 =                 52;
const Key_t Key_5 =                 53;
const Key_t Key_6 =                 54;
const Key_t Key_7 =                 55;
const Key_t Key_8 =                 56;
const Key_t Key_9 =                 57;
const Key_t Key_SEMICOLON =         59;
const Key_t Key_EQUAL =             61;
const Key_t Key_A =                 65;
const Key_t Key_B =                 66;
const Key_t Key_C =                 67;
const Key_t Key_D =                 68;
const Key_t Key_E =                 69;
const Key_t Key_F =                 70;
const Key_t Key_G =                 71;
const Key_t Key_H =                 72;
const Key_t Key_I =                 73;
const Key_t Key_J =                 74;
const Key_t Key_K =                 75;
const Key_t Key_L =                 76;
const Key_t Key_M =                 77;
const Key_t Key_N =                 78;
const Key_t Key_O =                 79;
const Key_t Key_P =                 80;
const Key_t Key_Q =                 81;
const Key_t Key_R =                 82;
const Key_t Key_S =                 83;
const Key_t Key_T =                 84;
const Key_t Key_U =                 85;
const Key_t Key_V =                 86;
const Key_t Key_W =                 87;
const Key_t Key_X =                 88;
const Key_t Key_Y =                 89;
const Key_t Key_Z =                 90;
const Key_t Key_LEFT_BRACKET =      91;
const Key_t Key_BACKSLASH =         92;
const Key_t Key_RIGHT_BRACKET =     93;
const Key_t Key_GRAVE_ACCENT =      96;
const Key_t Key_WORLD_1 =          161;
const Key_t Key_WORLD_2 =          162;

//Function keys
const Key_t Key_ESCAPE =           256;
const Key_t Key_ENTER =            257;
const Key_t Key_TAB =              258;
const Key_t Key_BACKSPACE =        259;
const Key_t Key_INSERT =           260;
const Key_t Key_DELETE =           261;
const Key_t Key_RIGHT =            262;
const Key_t Key_LEFT =             263;
const Key_t Key_DOWN =             264;
const Key_t Key_UP =               265;
const Key_t Key_PAGE_UP =          266;
const Key_t Key_PAGE_DOWN =        267;
const Key_t Key_HOME =             268;
const Key_t Key_END =              269;
const Key_t Key_CAPS_LOCK =        280;
const Key_t Key_SCROLL_LOCK =      281;
const Key_t Key_NUM_LOCK =         282;
const Key_t Key_PRINT_SCREEN =     283;
const Key_t Key_PAUSE =            284;
const Key_t Key_F1 =               290;
const Key_t Key_F2 =               291;
const Key_t Key_F3 =               292;
const Key_t Key_F4 =               293;
const Key_t Key_F5 =               294;
const Key_t Key_F6 =               295;
const Key_t Key_F7 =               296;
const Key_t Key_F8 =               297;
const Key_t Key_F9 =               298;
const Key_t Key_F10 =              299;
const Key_t Key_F11 =              300;
const Key_t Key_F12 =              301;
const Key_t Key_F13 =              302;
const Key_t Key_F14 =              303;
const Key_t Key_F15 =              304;
const Key_t Key_F16 =              305;
const Key_t Key_F17 =              306;
const Key_t Key_F18 =              307;
const Key_t Key_F19 =              308;
const Key_t Key_F20 =              309;
const Key_t Key_F21 =              310;
const Key_t Key_F22 =              311;
const Key_t Key_F23 =              312;
const Key_t Key_F24 =              313;
const Key_t Key_F25 =              314;
const Key_t Key_KP_0 =             320;
const Key_t Key_KP_1 =             321;
const Key_t Key_KP_2 =             322;
const Key_t Key_KP_3 =             323;
const Key_t Key_KP_4 =             324;
const Key_t Key_KP_5 =             325;
const Key_t Key_KP_6 =             326;
const Key_t Key_KP_7 =             327;
const Key_t Key_KP_8 =             328;
const Key_t Key_KP_9 =             329;
const Key_t Key_KP_DECIMAL =       330;
const Key_t Key_KP_DIVIDE =        331;
const Key_t Key_KP_MULTIPLY =      332;
const Key_t Key_KP_SUBTRACT =      333;
const Key_t Key_KP_ADD =           334;
const Key_t Key_KP_ENTER =         335;
const Key_t Key_KP_EQUAL =         336;
const Key_t Key_LEFT_SHIFT =       340;
const Key_t Key_LEFT_CONTROL =     341;
const Key_t Key_LEFT_ALT =         342;
const Key_t Key_LEFT_SUPER =       343;
const Key_t Key_RIGHT_SHIFT =      344;
const Key_t Key_RIGHT_CONTROL =    345;
const Key_t Key_RIGHT_ALT =        346;
const Key_t Key_RIGHT_SUPER =      347;
const Key_t Key_MENU =             348;
const Key_t Key_KEY_LAST =    Key_MENU;

//Mouse buttons
typedef int MouseButton_t;

const MouseButton_t Mouse_Button_Left =   0;
const MouseButton_t Mouse_Button_Right =  1;
const MouseButton_t Mouse_Button_Middle = 2;

}