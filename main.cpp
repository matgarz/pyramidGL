#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "Mesh.h"

// --- Transformation Variables ---
glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
float rotationZ = 0.0f; 
glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f);

// --- Frame Timing ---
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- Shaders ---
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0f);
}
)glsl";

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Speed scaled by deltaTime for smooth movement across frame rates
    float moveSpeed = 1.5f * deltaTime;
    float rotSpeed  = 90.0f * deltaTime; // degrees per second
    float scaleSpeed = 0.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) translation.y += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) translation.y -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) translation.x -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) translation.x += moveSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationZ += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationZ -= rotSpeed;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) scaleVector.z += scaleSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) scaleVector.z = std::max(0.01f, scaleVector.z - scaleSpeed);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "COMP 371 - 3D Perspective Pyramid", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    // --- Enable 3D Depth Testing & Face Culling ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // --- Create Shader Program (OOP) ---
    Shader shader(vertexShaderSource, fragmentShaderSource);

    // --- Vertex Data (3 Pos, 3 Color per vertex) ---
    float vertices[] = {
        // --- Front Face (Bright Red) ---
        -0.5f, -0.5f,  0.5f,  0.9f, 0.1f, 0.1f,
         0.5f, -0.5f,  0.5f,  0.9f, 0.1f, 0.1f,
         0.0f,  0.5f,  0.0f,  1.0f, 0.4f, 0.4f,

        // --- Right Face (Vibrant Green) ---
         0.5f, -0.5f,  0.5f,  0.1f, 0.8f, 0.1f,
         0.5f, -0.5f, -0.5f,  0.1f, 0.8f, 0.1f,
         0.0f,  0.5f,  0.0f,  0.4f, 1.0f, 0.4f,

        // --- Back Face (Deep Blue) ---
         0.5f, -0.5f, -0.5f,  0.1f, 0.2f, 0.9f,
        -0.5f, -0.5f, -0.5f,  0.1f, 0.2f, 0.9f,
         0.0f,  0.5f,  0.0f,  0.4f, 0.5f, 1.0f,

        // --- Left Face (Bright Yellow) ---
        -0.5f, -0.5f, -0.5f,  0.9f, 0.8f, 0.1f,
        -0.5f, -0.5f,  0.5f,  0.9f, 0.8f, 0.1f,
         0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.5f,

        // --- Base Triangle 1 (Grey) ---
        -0.5f, -0.5f, -0.5f,  0.25f, 0.25f, 0.25f,
        -0.5f, -0.5f,  0.5f,  0.25f, 0.25f, 0.25f,
         0.5f, -0.5f,  0.5f,  0.25f, 0.25f, 0.25f,

        // --- Base Triangle 2 (Grey) ---
        -0.5f, -0.5f, -0.5f,  0.25f, 0.25f, 0.25f,
         0.5f, -0.5f,  0.5f,  0.25f, 0.25f, 0.25f,
         0.5f, -0.5f, -0.5f,  0.25f, 0.25f, 0.25f
    };

    // Corrected indices ensuring CCW order for outer-facing triangles
    unsigned int indices[] = {
        0, 1, 2,       // Front
        3, 4, 5,       // Right
        6, 7, 8,       // Back
        9, 10, 11,     // Left
        12, 14, 13,    // Base 1 (CCW facing down/out)
        15, 17, 16     // Base 2 (CCW facing down/out)
    };

    // --- Create Mesh ---
    Mesh pyramid(vertices, sizeof(vertices) / sizeof(float),
                 indices, sizeof(indices) / sizeof(unsigned int));

    while (!glfwWindowShouldClose(window)) {
        // Calculate Delta Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.08f, 0.1f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // 1. PROJECTION MATRIX
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        // 2. VIEW MATRIX
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -3.0f));

        // 3. MODEL MATRIX
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scaleVector);

        // Combined MVP Matrix
        glm::mat4 MVP = projection * view * model;

        shader.setMat4("transform", glm::value_ptr(MVP));

        pyramid.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}