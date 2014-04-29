 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/GLInfo.h"
#include "Locus/Rendering/ShaderVariables.h"

namespace Locus
{

namespace ShaderSource
{
      
namespace VariableSpecifiers
{

static const char* const In = "in";
static const char* const Out = "out";
static const char* const Uniform = "uniform";
static const char* const Attribute = "attribute";
static const char* const Varying = "varying";

}

namespace VariableTypes
{

static const char* const Vec4 = "vec4";
static const char* const Vec3 = "vec3";
static const char* const Vec2 = "vec2";
static const char* const Mat4 = "mat4";
static const char* const Mat3 = "mat3";
static const char* const Sampler2D = "sampler2D";
static const char* const Float = "float";
static const char* const Int = "int";

}

namespace PrecisionQualifiers
{

static const char* const High = "highp";
static const char* const Medium = "mediump";
static const char* const Low = "lowp";

}

static const std::string FragColorVar130 = "mygl_FragColor";

static std::string Vert_1_30(bool textured, unsigned int numLights);
static std::string Frag_1_30(bool textured, unsigned int numLights);
static std::string Vert_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights);
static std::string Frag_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights);

static void AddVersion(std::string& source, GLInfo::GLSLVersion version);
static void AddPrecision(std::string& source, const char* precisionQualifier, const char* varType);
static void AddVar(std::string& source, const char* varSpecifier, const char* varType, const std::string& variableName);
static void AddInOutVar(std::string& source, const char* varType, const std::string& variableName, GLInfo::GLSLVersion version);

static const char* GetGLSLVersionString(GLInfo::GLSLVersion version);
static std::string GetOutVariableName(const std::string& variableName);

std::string GetMultiVariableName(const std::string& variableName, unsigned int index)
{
   return variableName + "_" + std::to_string(index);
}

static std::string GetOutVariableName(const std::string& variableName)
{
   return std::string("out_") + variableName;
}

static const char* GetGLSLVersionString(GLInfo::GLSLVersion version)
{
   switch(version)
   {
   case GLInfo::GLSLVersion::V_110:
      return "110";

   case GLInfo::GLSLVersion::V_120:
      return "120";

   case GLInfo::GLSLVersion::V_130:
   default:
      return "130";
   }
}

static void AddVersion(std::string& source, GLInfo::GLSLVersion version)
{
   source.append("#version ");
   source.append(GetGLSLVersionString(version));
   source.append("\n");
}

static void AddPrecision(std::string& source, const char* precisionQualifier, const char* varType)
{
   source.append("precision ");
   source.append(precisionQualifier);
   source.append(" ");
   source.append(varType);
   source.append(";\n");
}

static void AddVar(std::string& source, const char* varSpecifier, const char* varType, const std::string& variableName)
{
   source.append(varSpecifier);
   source.append(" ");
   source.append(varType);
   source.append(" ");
   source.append(variableName);
   source.append(";\n");
}

static void AddInOutVar(std::string& source, const char* varType, const std::string& variableName, GLInfo::GLSLVersion version)
{
   if (version == GLInfo::GLSLVersion::V_130)
   {
      AddVar(source, VariableSpecifiers::In, varType, variableName);
      AddVar(source, VariableSpecifiers::Out, varType, GetOutVariableName(variableName));
   }
   else
   {
      AddVar(source, VariableSpecifiers::Attribute, varType, variableName);
      AddVar(source, VariableSpecifiers::Varying, varType, GetOutVariableName(variableName));
   }
}

std::string Vert(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)
{
   if (version == GLInfo::GLSLVersion::V_130)
   {
      return Vert_1_30(textured, numLights);
   }
   else
   {
      return Vert_Pre_1_30(version, textured, numLights);
   }
}

std::string Frag(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)
{
   if (version == GLInfo::GLSLVersion::V_130)
   {
      return Frag_1_30(textured, numLights);
   }
   else
   {
      return Frag_Pre_1_30(version, textured, numLights);
   }
}

///////////////////////////// 1.30 Shaders ////////////////////////////////////

static std::string Vert_1_30(bool textured, unsigned int numLights)
{
   GLInfo::GLSLVersion version = GLInfo::GLSLVersion::V_130;

   bool shouldUseLights = (numLights > 0);

   std::string source;

   AddVersion(source, version);
   AddPrecision(source, PrecisionQualifiers::Medium, VariableTypes::Float);

   AddVar(source, VariableSpecifiers::In, VariableTypes::Vec4, Vert_Pos);

   AddInOutVar(source, VariableTypes::Vec4, Color, version);

   AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Mat4, Mat_MVP);

   if (textured)
   {
      AddInOutVar(source, VariableTypes::Vec2, Vert_Tex, version);
   }

   if (shouldUseLights)
   {
      AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Mat4, Mat_MV);

      AddInOutVar(source, VariableTypes::Vec3, Vert_Normal, version);

      AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Mat3, Mat_Normal);

      //AddVar(source, VariableSpecifiers::Out, VariableTypes::Vec3, ToEyeDirection);

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Vec3, GetMultiVariableName(Light_EyePos, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_Attenuation, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_LinearAttenuation, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_QuadraticAttenuation, lightNum));

         AddVar(source, VariableSpecifiers::Out, VariableTypes::Vec3, GetMultiVariableName(ToLightDirection, lightNum));
         AddVar(source, VariableSpecifiers::Out, VariableTypes::Float, GetMultiVariableName(CalculatedLightAttenuation, lightNum));
      }
   }

   source.append("void main()\n");
   source.append("{\n");

   source.append(GetOutVariableName(Color) + " = " + Color + ";\n");

   if (textured)
   {
      source.append(GetOutVariableName(Vert_Tex) + " = " + Vert_Tex  + ";\n");
   }

   if (shouldUseLights)
   {
      std::string positionInViewSpaceVar = "positionInViewSpace";

      source.append(GetOutVariableName(Vert_Normal) + " = normalize(" + Mat_Normal + " * " + Vert_Normal + ");\n");

      source.append(std::string("vec3 ") + positionInViewSpaceVar + " = vec3(" + Mat_MV + " * " + Vert_Pos + ");\n");
      //source.append(ToEyeDirection + " = -" + positionInViewSpaceVar + ";\n");

      std::string lightDistanceVar = "lightDistance";

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         source.append(GetMultiVariableName(ToLightDirection, lightNum) + " = " + GetMultiVariableName(Light_EyePos, lightNum) + " - " + positionInViewSpaceVar + ";\n");

         if (lightNum == 0)
         {
            source.append("float ");
         }

         source.append(lightDistanceVar + " = length(" + GetMultiVariableName(ToLightDirection, lightNum) + ");\n");

         source.append(GetMultiVariableName(CalculatedLightAttenuation, lightNum) + " = 1.0 / (" +
                           GetMultiVariableName(Light_Attenuation, lightNum) + " + " +
                              "(" + GetMultiVariableName(Light_LinearAttenuation, lightNum) + " * " + lightDistanceVar + ") + " +
                              "(" + GetMultiVariableName(Light_QuadraticAttenuation, lightNum) + " * " + lightDistanceVar + " * " + lightDistanceVar + ")" +
                        ");\n");
      }
   }

   source.append(std::string("gl_Position = ") + Mat_MVP + " * " + Vert_Pos + ";\n");

   source.append("}");

   return source;
}

static std::string Frag_1_30(bool textured, unsigned int numLights)
{
   GLInfo::GLSLVersion version = GLInfo::GLSLVersion::V_130;

   std::string source;

   bool shouldUseLights = (numLights > 0);

   AddVersion(source, version);
   AddPrecision(source, PrecisionQualifiers::Medium, VariableTypes::Float);

   std::string fromVertColorVar = GetOutVariableName(Color);
   std::string fromVertTexCoordVar = GetOutVariableName(Vert_Tex);
   std::string fromVertNormalVar = GetOutVariableName(Vert_Normal);

   AddVar(source, VariableSpecifiers::In, VariableTypes::Vec4, fromVertColorVar);

   if (textured)
   {
      AddVar(source, VariableSpecifiers::In, VariableTypes::Vec2, fromVertTexCoordVar);
      AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Sampler2D, Map_Diffuse);
   }

   if (shouldUseLights)
   {
      AddVar(source, VariableSpecifiers::In, VariableTypes::Vec3, fromVertNormalVar);

      //AddVar(source, VariableSpecifier::In, VariableType::Vec3, fromVertToEyeDirectionVar);

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         AddVar(source, VariableSpecifiers::In, VariableTypes::Vec3, GetMultiVariableName(ToLightDirection, lightNum));
         AddVar(source, VariableSpecifiers::In, VariableTypes::Float, GetMultiVariableName(CalculatedLightAttenuation, lightNum));

         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Vec4, GetMultiVariableName(Light_Diffuse, lightNum));
         //AddVar(source, VariableSpecifiers::Uniform, VariableType::Vec4, GetMultiVariableName(Light_Ambient, lightNum));
         //AddVar(source, VariableSpecifiers::Uniform, VariableType::Vec4, GetMultiVariableName(Light_Specular, lightNum));
      }
   }

   AddVar(source, VariableSpecifiers::Out, VariableTypes::Vec4, FragColorVar130);

   source.append("void main()\n");
   source.append("{\n");

   std::string texturedInitialDiffuseColor = fromVertColorVar + " * " + "texture2D(" + Map_Diffuse + ", " + fromVertTexCoordVar + ")";

   if (shouldUseLights)
   {
      std::string diffuseColorVar = "diffuseColor";

      if (textured)
      {
         source.append(std::string("vec4 ") + diffuseColorVar + " = " + texturedInitialDiffuseColor + ";\n");
      }
      else
      {
         source.append(std::string("vec4 ") + diffuseColorVar + " = " + fromVertColorVar + ";\n");
      }

      std::string normalVar = "normal";

      source.append(std::string("vec3 ") + normalVar + " = normalize(" + fromVertNormalVar + ");\n");

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         source.append(diffuseColorVar + " += clamp(dot(" + normalVar + ", normalize(" + GetMultiVariableName(ToLightDirection, lightNum) + ")), 0.0, 1.0) * " + GetMultiVariableName(CalculatedLightAttenuation, lightNum) + " * " + GetMultiVariableName(Light_Diffuse, lightNum) + ";\n");
      }

      source.append(FragColorVar130 + " = " + diffuseColorVar + ";\n");
   }
   else
   {
      if (textured)
      {
         source.append(FragColorVar130 + " = " + texturedInitialDiffuseColor + ";\n");
      }
      else
      {
         source.append(FragColorVar130 + " = " + fromVertColorVar + ";\n");
      }
   }

   source.append("}");

   return source;
}

static std::string Vert_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)
{
   bool shouldUseLights = (numLights > 0);

   std::string source;

   AddVersion(source, version);

   if (shouldUseLights)
   {
      AddVar(source, VariableSpecifiers::Varying, VariableTypes::Vec3, GetOutVariableName(Vert_Normal));

      //AddVar(source, VariableSpecifiers::Varying, VariableTypes::Vec3, ToEyeDirection);

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Vec3, GetMultiVariableName(Light_EyePos, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_Attenuation, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_LinearAttenuation, lightNum));
         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Float, GetMultiVariableName(Light_QuadraticAttenuation, lightNum));

         AddVar(source, VariableSpecifiers::Varying, VariableTypes::Vec3, GetMultiVariableName(ToLightDirection, lightNum));
         AddVar(source, VariableSpecifiers::Varying, VariableTypes::Float, GetMultiVariableName(CalculatedLightAttenuation, lightNum));
      }
   }

   source.append("void main()\n");
   source.append("{\n");

   source.append("gl_FrontColor = gl_Color;\n");

   if (textured)
   {
      source.append("gl_TexCoord[0] = gl_MultiTexCoord0;\n");
   }

   if (shouldUseLights)
   {
      std::string positionInViewSpaceVar = "positionInViewSpace";

      source.append(GetOutVariableName(Vert_Normal) + " = normalize(gl_NormalMatrix * gl_Normal);\n");

      source.append(std::string("vec3 ") + positionInViewSpaceVar + " = vec3(gl_ModelViewMatrix * gl_Vertex);\n");
      //source.append(ToEyeDirection + " = -" + positionInViewSpaceVar + ";\n");

      std::string lightDistanceVar = "lightDistance";

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         source.append(GetMultiVariableName(ToLightDirection, lightNum) + " = " + GetMultiVariableName(Light_EyePos, lightNum) + " - " + positionInViewSpaceVar + ";\n");

         if (lightNum == 0)
         {
            source.append("float ");
         }

         source.append(lightDistanceVar + " = length(" + GetMultiVariableName(ToLightDirection, lightNum) + ");\n");

         source.append(GetMultiVariableName(CalculatedLightAttenuation, lightNum) + " = 1.0 / (" +
                           GetMultiVariableName(Light_Attenuation, lightNum) + " + " +
                              "(" + GetMultiVariableName(Light_LinearAttenuation, lightNum) + " * " + lightDistanceVar + ") + " +
                              "(" + GetMultiVariableName(Light_QuadraticAttenuation, lightNum) + " * " + lightDistanceVar + " * " + lightDistanceVar + ")" +
                        ");\n");
      }
   }

   source.append("gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n");

   source.append("}");

   return source;
}

static std::string Frag_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)
{
   std::string source;

   bool shouldUseLights = (numLights > 0);

   AddVersion(source, version);

   std::string fromVertNormalVar = GetOutVariableName(Vert_Normal);

   if (textured)
   {
      AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Sampler2D, Map_Diffuse);
   }

   if (shouldUseLights)
   {
      AddVar(source, VariableSpecifiers::Varying, VariableTypes::Vec3, fromVertNormalVar);

      //AddVar(source, VariableSpecifier::Varying, VariableType::Vec3, fromVertToEyeDirectionVar);

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         AddVar(source, VariableSpecifiers::Varying, VariableTypes::Vec3, GetMultiVariableName(ToLightDirection, lightNum));
         AddVar(source, VariableSpecifiers::Varying, VariableTypes::Float, GetMultiVariableName(CalculatedLightAttenuation, lightNum));

         AddVar(source, VariableSpecifiers::Uniform, VariableTypes::Vec4, GetMultiVariableName(Light_Diffuse, lightNum));
         //AddVar(source, VariableSpecifiers::Uniform, VariableType::Vec4, GetMultiVariableName(Light_Ambient, lightNum));
         //AddVar(source, VariableSpecifiers::Uniform, VariableType::Vec4, GetMultiVariableName(Light_Specular, lightNum));
      }
   }

   source.append("void main()\n");
   source.append("{\n");

   std::string texturedInitialDiffuseColor = std::string("gl_Color * texture2D(") + Map_Diffuse + ", gl_TexCoord[0].st)";

   if (shouldUseLights)
   {
      std::string diffuseColorVar = "diffuseColor";

      if (textured)
      {
         source.append(std::string("vec4 ") + diffuseColorVar + " = " + texturedInitialDiffuseColor + ";\n");
      }
      else
      {
         source.append(std::string("vec4 ") + diffuseColorVar + " = gl_Color;\n");
      }

      std::string normalVar = "normal";

      source.append(std::string("vec3 ") + normalVar + " = normalize(" + fromVertNormalVar + ");\n");

      for (unsigned int lightNum = 0; lightNum < numLights; ++lightNum)
      {
         source.append(diffuseColorVar + " += clamp(dot(" + normalVar + ", normalize(" + GetMultiVariableName(ToLightDirection, lightNum) + ")), 0.0, 1.0) * " + GetMultiVariableName(CalculatedLightAttenuation, lightNum) + " * " + GetMultiVariableName(Light_Diffuse, lightNum) + ";\n");
      }

      source.append("gl_FragColor = " + diffuseColorVar + ";\n");
   }
   else
   {
      if (textured)
      {
         source.append("gl_FragColor = " + texturedInitialDiffuseColor + ";\n");
      }
      else
      {
         source.append("gl_FragColor = gl_Color;\n");
      }
   }

   source.append("}");

   return source;
}

}

}