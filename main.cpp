#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>

// --- Transformation Variables ---
const float d = 0.005f; 
const float s = 0.005f; 

glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
float rotationZ = 0.0f; 
glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) translation.y += d;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) translation.y -= d;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) translation.x -= d;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) translation.x += d;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationZ += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationZ -= 1.0f;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) scaleVector.z += s;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) scaleVector.z = std::max(0.01f, scaleVector.z - s);
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

    // Compile Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --- Vertex Data (All Front-Facing CCW Order) ---
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

    // --- EBO INDICES (18 triangles total) ---
    unsigned int indices[] = {
        0, 1, 2,   // Front
        3, 4, 5,   // Right
        6, 7, 8,   // Back
        9, 10, 11, // Left
        12, 13, 14, // Base 1
        15, 16, 17  // Base 2
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Positions (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.08f, 0.1f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // --- 1. PROJECTION MATRIX ---
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        // --- 2. VIEW MATRIX ---
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -3.0f));

        // --- 3. MODEL MATRIX ---
        glm::mat4 model = glm::mat4(1.0f);
        
        // User Translation (W, S, A, D)
        model = glm::translate(model, translation);
        
        // Initial 3D Tilt angles
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        // User Z-rotation (Q, E)
        model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
        
        // User Scaling (R, F)
        model = glm::scale(model, scaleVector);

        // --- Combined MVP Matrix ---
        glm::mat4 MVP = projection * view * model;

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}