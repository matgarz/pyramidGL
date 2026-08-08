// tiny_obj_loader.h - Minimal single-header OBJ loader
// MIT License (open-source)

#ifndef TINY_OBJ_LOADER_H_
#define TINY_OBJ_LOADER_H_

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace tinyobj {

typedef struct {
    std::vector<float> vertices;   // x, y, z
    std::vector<float> normals;    // nx, ny, nz
    std::vector<float> texcoords;  // u, v
} attrib_t;

typedef struct {
    std::vector<unsigned int> indices; // vertex indices only
} mesh_t;

typedef struct {
    mesh_t mesh;
} shape_t;

typedef struct {
    std::string name;
} material_t;

// ============================================================================
// Minimal OBJ loader: supports v, vn, vt, f (triangles)
// ============================================================================
static bool LoadObj(
    attrib_t* attrib,
    std::vector<shape_t>* shapes,
    std::vector<material_t>* materials,
    std::string* warn,
    std::string* err,
    const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        if (err) *err = "Cannot open OBJ file.";
        return false;
    }

    std::vector<float> v;
    std::vector<float> vn;
    std::vector<float> vt;

    shape_t shape;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {

        // Vertex position
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            sscanf(line + 2, "%f %f %f", &x, &y, &z);
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
        }

        // Vertex normal
        else if (strncmp(line, "vn ", 3) == 0) {
            float x, y, z;
            sscanf(line + 3, "%f %f %f", &x, &y, &z);
            vn.push_back(x);
            vn.push_back(y);
            vn.push_back(z);
        }

        // Texture coordinate
        else if (strncmp(line, "vt ", 3) == 0) {
            float u, w;
            sscanf(line + 3, "%f %f", &u, &w);
            vt.push_back(u);
            vt.push_back(w);
        }

        // Face (triangle)
        else if (strncmp(line, "f ", 2) == 0) {
            unsigned int vi[3], ti[3], ni[3];

            int count = sscanf(
                line + 2,
                "%u/%u/%u %u/%u/%u %u/%u/%u",
                &vi[0], &ti[0], &ni[0],
                &vi[1], &ti[1], &ni[1],
                &vi[2], &ti[2], &ni[2]
            );

            if (count == 9) {
                // Store only vertex indices (OpenGL wireframe doesn't need normals/UVs)
                shape.mesh.indices.push_back(vi[0] - 1);
                shape.mesh.indices.push_back(vi[1] - 1);
                shape.mesh.indices.push_back(vi[2] - 1);
            }
        }
    }

    fclose(fp);

    attrib->vertices = v;
    attrib->normals = vn;
    attrib->texcoords = vt;

    shapes->push_back(shape);

    return true;
}

} // namespace tinyobj

#endif // TINY_OBJ_LOADER_H_
