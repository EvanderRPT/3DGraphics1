#pragma once

#include "vector.h"
#include "triangle.h"
#include <vector>

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];
void load_cube_mesh_data(void);
void load_obj_file(const char* filename);
class mesh_t {
public:
	std::vector<vec3_t> vertices;
	std::vector<face_t> faces;
	vec3_t rotation;
	vec3_t scale;       // scale with x, y, and z values
	vec3_t translation; // translation with x, y, and z values
	mesh_t() : rotation({0,0,0}) {}
	mesh_t(std::vector<vec3_t> vertices, std::vector<face_t> faces) : vertices(vertices), faces(faces) {}
	mesh_t(std::vector<vec3_t> vertices, std::vector<face_t> faces, vec3_t rotation, vec3_t scale, vec3_t translation) : vertices(vertices), faces(faces), rotation(rotation), scale(scale), translation(translation) {}
};
 
extern mesh_t mesh;

