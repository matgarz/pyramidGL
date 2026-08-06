#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();

    // =========================================================================
    // ADDED METHOD: ACTIVATES THE SHADER PROGRAM IN OPENGL VIA glUseProgram.
    // REQUIRED BEFORE SETTING UNIFORMS OR DRAWING WITH THIS SHADER.
    // =========================================================================
    void use() const;

    // =========================================================================
    // ADDED METHOD: PASSES 4X4 MATRIX UNIFORMS (LIKE MVP) DOWN TO GLSL SHADERS.
    // NECESSARY TO BIND THE "transform" UNIFORM IN MAIN.
    // =========================================================================
    void setMat4(const std::string& name, const float* matrixData) const;

private:
    // =========================================================================
    // ADDED HELPER: CHECKS GLSL COMPILATION & LINKING ERRORS AT RUNTIME.
    // PRINTS DETAILED GLSL COMPILER LOGS TO THE CONSOLE IF A SHADER FAILS.
    // =========================================================================
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif