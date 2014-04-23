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

#include "Locus/Rendering/Viewpoint.h"
#include "Locus/Rendering/TransformationStack.h"

namespace Locus
{

Viewpoint::Viewpoint()
   :  forward(Vector3::NegativeZAxis()), up(Vector3::YAxis()), right(Vector3::XAxis())
{
}

const Vector3& Viewpoint::GetPosition() const
{
   return position;
}

const Vector3& Viewpoint::GetRight() const
{
   return right;
}

const Vector3& Viewpoint::GetUp() const
{
   return up;
}

const Vector3& Viewpoint::GetForward() const
{
   return forward;
}

const Transformation& Viewpoint::GetRotation() const
{
   return rotations;
}

const DualTransformation& Viewpoint::GetTransformation() const
{
   return transformation;
}

void Viewpoint::TranslateBy(const Vector3& translation)
{
   transformation.TranslateBy(translation);

   position += translation.x * right;
   position += translation.y * up;
   position += -translation.z * forward;
}

void Viewpoint::RotateBy(const Vector3& rotation)
{
   transformation.RotateBy(rotation);
   rotations.RotateBy(rotation);

   forward = rotations.MultVector(Vector3::NegativeZAxis());
   up = rotations.MultVector(Vector3::YAxis());
   right = forward.cross(up);
}

void Viewpoint::Activate(TransformationStack& transformationStack) const
{
   transformationStack.Push( transformation.GetInverse() );
}

void Viewpoint::Deactivate(TransformationStack& transformationStack) const
{
   transformationStack.Pop();
}

}