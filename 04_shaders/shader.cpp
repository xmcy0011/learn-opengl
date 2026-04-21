#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    exit(-1);
  }
  const char *vertexShaderCode = vertexCode.c_str();
  const char *fragmentShaderCode = fragmentCode.c_str();

  // 2. 创建和编译顶点着色器
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderCode,
                 NULL);          // 将着色器源码附加到着色器对象上
  glCompileShader(vertexShader); // 编译着色器代码
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 检查编译结果
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }

  // 3. 创建和编译片段着色器
  unsigned int fragmentShader =
      glCreateShader(GL_FRAGMENT_SHADER); // 创建片段着色器对象
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragmentShader); // 编译着色器代码
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }

  // 4. 创建着色器程序
  shaderProgramID = glCreateProgram();             // 创建着色器程序对象
  glAttachShader(shaderProgramID, vertexShader);   // 附加顶点着色器
  glAttachShader(shaderProgramID, fragmentShader); // 附加片段着色器
  glLinkProgram(shaderProgramID);                  // 链接着色器程序
  glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success); // 检查链接结果
  if (!success) {
    glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }
  glUseProgram(shaderProgramID); // 使用着色器程序
  glDeleteShader(vertexShader);  // 删除顶点着色器，程序对象创建成功后，不需要了
  glDeleteShader(fragmentShader); // 删除片段着色器，同上
}

void Shader::use() { glUseProgram(shaderProgramID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}