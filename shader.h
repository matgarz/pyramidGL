#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>

// The Shader class is responsible for:
// - Compiling the vertex shader
// - Compiling the fragment shader
// - Linking both into a single GPU program
// - Allowing you to activate the shader and send uniforms
class Shader {
public:
// The OpenGL ID of the shader program stored on the GPU
    unsigned int ID;
    // Constructor:
    // Takes the vertex shader source code and fragment shader source code,
    // compiles them, and links them into a usable shader program.
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