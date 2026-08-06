#include "Shader.h"

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    // =========================================================================
    // ADDED: CHECK FOR VERTEX SHADER COMPILATION ERRORS
    // =========================================================================
    checkCompileErrors(vertexShader, "VERTEX");

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    // =========================================================================
    // ADDED: CHECK FOR FRAGMENT SHADER COMPILATION ERRORS
    // =========================================================================
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    // =========================================================================
    // ADDED: CHECK FOR PROGRAM LINKING ERRORS
    // =========================================================================
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// =========================================================================
// ADDED DESTRUCTOR: CLEANS UP THE SHADER PROGRAM FROM GPU MEMORY
// =========================================================================
Shader::~Shader() {
    glDeleteProgram(ID);
}

// =========================================================================
// ADDED IMPLEMENTATION: CALLS glUseProgram TO MAKE THIS SHADER ACTIVE
// =========================================================================
void Shader::use() const {
    glUseProgram(ID);
}

// =========================================================================
// ADDED IMPLEMENTATION: FINDS UNIFORM LOCATION AND SENDS MAT4 DATA TO GPU
// =========================================================================
void Shader::setMat4(const std::string& name, const float* matrixData) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrixData);
    }
}

// =========================================================================
// ADDED IMPLEMENTATION: QUERIES OPENGL FOR SHADER/PROGRAM COMPILE LOGS
// =========================================================================
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << infoLog << "\n---------------------------------------------------" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                      << infoLog << "\n---------------------------------------------------" << std::endl;
        }
    }
}