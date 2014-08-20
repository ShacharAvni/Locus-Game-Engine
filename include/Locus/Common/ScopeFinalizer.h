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

#include "LocusCommonAPI.h"

#include <functional>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/*!
 * \brief Calls the given function on destruction.
 *
 * \code{.cpp}
 * #include "Locus/Common/ScopeFinalizer.h"
 *
 * #include <iostream>
 *
 * void CallAtEndOfScope()
 * {
 *    std::cout << "Scope termination function." << std::endl;
 * }
 *
 * void CallingFunction()
 * {
 *    {
 *       Locus::ScopeFinalizer finalizer( CallAtEndOfScope );
 *
 *       //Do Stuff
 *
 *    } //Here, CallAtEndOfScope is called
 * }
 * \endcode
 */
class LOCUS_COMMON_API ScopeFinalizer
{
public:
   ScopeFinalizer(const std::function<void()>& finalizeFunc);

   /*!
    * \brief On destruction, the given function is called unless Cancel has been called.
    *
    * \sa Cancel
    */
   ~ScopeFinalizer();

   /// Makes it so the given function is NOT called on destruction. This cannot be undone.
   void Cancel();

private:
   std::function<void()> finalizeFunc;
   bool cancelled;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus