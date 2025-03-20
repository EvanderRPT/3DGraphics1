#include "mesh.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

mesh_t mesh = {
	{}, // Empty vector for vertices
	{}, // Empty vector for faces
	{ 0, 0, 0 }, // Rotation
	{ 1.0, 1.0, 1.0 }, // Scale
	{ 0, 0, 0 } // Translation
}; // Definition
vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ -1,  -1,  -1 }, // 1
	{ -1,  1,  -1 }, // 2
	{ 1,  1,  -1 }, // 3
	{ 1,  -1,  -1 }, // 4
	{ 1,  1,  1 }, // 5
	{ 1,  -1,  1 }, // 6
	{ -1,  1,  1 }, // 7
	{ -1,  -1,  1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{ 1,  2,  3,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 1,  3,  4,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF },
	// right
	{ 4,  3,  5,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 4,  5,  6,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF },
	// back
	{ 6,  5,  7,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 6,  7,  8,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF },
	// left
	{ 8,  7,  2,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 8,  2,  1,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF },
	// top
	{ 2,  7,  5,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 2,  5,  3,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF },
	// bottom
	{ 6,  8,  1,  { 0, 0 },  { 0, 1 },  { 1, 1 },  0xFFFFFFFF },
	{ 6,  1,  4,  { 0, 0 },  { 1, 1 },  { 1, 0 },  0xFFFFFFFF }
};


void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		mesh.vertices.push_back(cube_vertices[i]);
	}
	for (int i = 0; i < N_CUBE_FACES; i++) {
		mesh.faces.push_back(cube_faces[i]);
	}
}

void load_obj_file(const char* filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Could not open the file - " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);

		if (line.rfind("v ", 0) == 0) { //Vertex line
			vec3_t vertex;
			char type;
			iss >> type >> vertex.x >> vertex.y >> vertex.z;
			mesh.vertices.push_back(vertex);
		}
		else if (line.rfind("f ", 0) == 0) { // Face line
			face_t face;
			char type;
			int v1, v2, v3, t1, t2, t3, n1, n2, n3;
			iss >> type;
			char slash;
			if (line.find('/') != std::string::npos) {
				iss >> v1 >> slash >> t1 >> slash >> n1
					>> v2 >> slash >> t2 >> slash >> n2
					>> v3 >> slash >> t3 >> slash >> n3;
			}
			else {
				iss >> v1 >> v2 >> v3;
			} 
			face = { v1, v2, v3 };
			mesh.faces.push_back(face);
		}
		
	}
}
