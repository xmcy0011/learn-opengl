/****************************************************************************************************
shader.h
  shader header file.
  Copyright (c) StarLeap Studio.(2026), All rights reserved.

Purpose:
  Header file to implement shader.

Author:
  Jake (xmcy0011@gmail.com)

Creating Time:
  2026-04-21
****************************************************************************************************/
#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);

public:
  // use/activate the shader
  void use();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

  // the program ID
  unsigned int shaderProgramID;
};

#endif // SHADER_H_