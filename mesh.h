#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh {
public:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    Mesh(float* vertices, unsigned int vertexCount,
         unsigned int* indices, unsigned int indexCount);
    ~Mesh();

    void draw() const;
};

#endif