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
const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0); // white wireframe
}
)";

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Speed scaled by deltaTime for smooth movement across frame rates
    float moveSpeed = 1.5f * deltaTime;
    float rotSpeed  = 90.0f * deltaTime;
    float scaleSpeed = 0.5f * deltaTime;

    // Translation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) translation.y += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) translation.y -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) translation.x -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) translation.x += moveSpeed;

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationZ += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationZ -= rotSpeed;

    // Scaling
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) scaleVector.z += scaleSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        scaleVector.z = std::max(0.01f, scaleVector.z - scaleSpeed);
}

int main() {
    
     /*
    GLFW is teh library to control the window on OPENGL, without this library there is no window
    1. glfwInit();
    2. glfwCreateWindow(...);
    3. glfwMakeContextCurrent(window);
    */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "COMP 371 - Assignment 3 Chair", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
 /*
    GLEW is the library that load teh recent functions/extensions that OPENGL uses.
    It allows yto use shaders such as  VBO, VAO, EBO, etc.
    1. glewInit()
    */
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);

    // Wireframe mode (required)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        /*
    SHADER
    Compile vertex and fragment shader.
    shader.use();
    shader.setMat4(...);
    Shader decides how we draw each vertex and their color
    */
    Shader shader(vertexShaderSrc, fragmentShaderSrc);

    // Load your OBJ chair
    Mesh chair("chair.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.08f, 0.1f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        /*
        GLM 
        This library allows the mathematics matrix operation 
        Translation, scaling , rotation etc. 
        We create 3 matrices: 
            Model: Position, rotation other transformations.
            View: Place the camera view position.
            Projection: Generetes the perspective projection so it looks 3D in a 2D screen.
        */

        // 1. PROJECTION MATRIX
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        // 2.  VIEW MATRIX (camera)
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -5.0f));

        // 3. MODEL MATRIX
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scaleVector);

        // Combined MVP Matrix
        glm::mat4 MVP = projection * view * model;

        shader.setMat4("transform", glm::value_ptr(MVP));

        chair.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}