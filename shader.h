#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

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

    // Activates (uses) this shader program in the current OpenGL context.
    void use() const {
        glUseProgram(ID);
    }

    // Sends a 4x4 matrix (mat4) uniform to the shader.
    // Used for MVP (Model-View-Projection) transformations.
    void setMat4(const std::string& name, const float* value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
    }
};

#endif
