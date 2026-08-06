#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
// The Mesh class stores everything needed to draw a 3D object:
// - VAO: Vertex Array Object (stores the vertex attribute configuration)
// - VBO: Vertex Buffer Object (stores the vertex data: positions + colors)
// - EBO: Element Buffer Object (stores the indices for triangles)
// 
// The class also provides a draw() function to render the mesh.
class Mesh {
public:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
// Constructor:
    // Takes arrays of vertices and indices,
    // creates VAO/VBO/EBO, and uploads the data to the GPU.
    Mesh(float* vertices, unsigned int vertexCount,
         unsigned int* indices, unsigned int indexCount);
   // Destructor:
    // Cleans up GPU buffers when the mesh is destroyed.
         ~Mesh();
// Draws the mesh using glDrawElements().
    void draw() const;
};

#endif