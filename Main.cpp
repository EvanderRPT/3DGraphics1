#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include <iostream>
#include <algorithm>  
#include "matrix.h"
#include "light.h"
#include "texture.h"

//#define DRAW_SHAPE
#define DRAW_CUBE
#define FULL_ROTATION

//#define TEST
#define DEV



std::vector<triangle_t> triangles_to_render;


vec3_t camera_position = { 0, 0, 0 };
mat4_t proj_matrix;

bool is_running = false;

int previous_frame_time = 0;

enum cull_method cull_method = CULL_NONE;
enum render_method render_method = RENDER_WIRE;
void setup(void) {

	// Initialize render mode and triangle culling method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;


	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	// Initialize the perspective projection matrix
	float fov = M_PI / 3.0; // the same as 180/3, or 60 deg
	float aspect = (float)window_height / (float)window_width;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

#ifdef DRAW_CUBE
	load_cube_mesh_data();
#endif 

#ifdef DRAW_SHAPE
	load_obj_file("D:\\Codes\\Dev\\C_CPP\\3dGraphics\\3dGraphics\\f22.obj");
#endif // DRAW_SHAPE

	// Load the hardcoded texature array in the global mesh texture variable
	mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		if (event.key.keysym.sym == SDLK_1)
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4)
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_5)
			render_method = RENDER_TEXTURED;
		if (event.key.keysym.sym == SDLK_6)
			render_method = RENDER_TEXTURED_WIRE;
		if (event.key.keysym.sym == SDLK_c)
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			cull_method = CULL_NONE;
		break;
	}
}



void update(void) {
	// Wait some time to have a constant frame rate
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}
	previous_frame_time = SDL_GetTicks();

	triangles_to_render = {};

	// Initialize the triangles to render
#ifdef FULL_ROTATION
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;
#endif // 


	mesh.rotation.x += 0.01;
	//mesh.translation.x += 0.01;
	mesh.translation.z = 5;
	//mesh.scale.x += 0.02;


	mat4_t scale_matrix;
	scale_matrix.make_scale(mesh.scale. x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix;
	translation_matrix.make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x;
	rotation_matrix_x.make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y;
	rotation_matrix_y.make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z;
	rotation_matrix_z.make_rotation_z(mesh.rotation.z);



	int num_faces = mesh.faces.size();
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3] = {
			mesh.vertices[mesh_face.a - 1],
			mesh.vertices[mesh_face.b - 1],
			mesh.vertices[mesh_face.c - 1]
		};

		vec4_t transformed_vertices[3];

		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = face_vertices[j].vec3_to_vec4();

			// Create a World Matrix combining scale, rotation, and translation matrices
			mat4_t world_matrix;
			world_matrix.identity();

			world_matrix = scale_matrix.mul_mat4(world_matrix);
			world_matrix = rotation_matrix_z.mul_mat4(world_matrix);
			world_matrix = rotation_matrix_y.mul_mat4(world_matrix);
			world_matrix = rotation_matrix_x.mul_mat4(world_matrix);
			world_matrix = translation_matrix.mul_mat4(world_matrix);

			transformed_vertex = world_matrix.mul_vec4(transformed_vertex);
			
			
			// Translate the vertex away from the camera
			// Save the transformed vertex in the array of transformed vertexs
			transformed_vertices[j] = transformed_vertex;
		}
			vec3_t vector_a = transformed_vertices[0].vec4_to_vec3(); /*   A   */
			vec3_t vector_b = transformed_vertices[1].vec4_to_vec3(); /*  / \  */
			vec3_t vector_c = transformed_vertices[2].vec4_to_vec3(); /* B---C */

			vec3_t vector_ab = vector_b.sub(vector_a);
			vec3_t vector_ac = vector_c.sub(vector_a);
			vector_ab.normalize();
			vector_ac.normalize();

			// Compute the normal of the face
			vec3_t normal = vector_ab.cross(vector_ac);
			normal.normalize();
			// Find the vector between a point in the triangle and the camera
			vec3_t camera_ray = camera_position.sub(vector_a);

			// Calculate how aligned the camera ray is with the face normal 
			float dot_normal_camera = normal.dot(camera_ray);

			if (dot_normal_camera < 0) {
				continue;
			}
		
		vec4_t projected_points[3];

		// Loop all three vertices to perform projection
		for (int j = 0; j < 3; j++) {
			// Project the current vertex
			projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);


			// Scale into the view
			projected_points[j].x *= (window_width / 2.0);
			projected_points[j].y *= (window_height / 2.0);

			// Scale and translate the projected points to the middle of the screen
			projected_points[j].x += (window_width / 2);
			projected_points[j].y += (window_height / 2);
		}

		//	Calculate the average depth for each face based on the vertices after transformation
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		// Calculaate the shade intensity based on how aliged is the face normal and the opposite of the light direction
		float light_intensity_factor = -normal.dot(light.direction);

		// Calculate the triangle color based on the light angle
		uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

		triangle_t projected_triangle{ 
			{ projected_points[0].x, projected_points[0].y },
			{ projected_points[1].x, projected_points[1].y },
			{ projected_points[2].x, projected_points[2].y },
			{ mesh_face.a_uv.u,  mesh_face.a_uv.v },
			{ mesh_face.b_uv.u,  mesh_face.b_uv.v },
			{ mesh_face.c_uv.u,  mesh_face.c_uv.v },
			triangle_color,
			avg_depth
		};

		// Save the projected triangle in the array of triangles to render
		triangles_to_render.push_back(projected_triangle);
	}

	// Sort the triangles to render by their avg_depth

	std::sort(triangles_to_render.begin(), triangles_to_render.end(),
		[](const triangle_t& a, const triangle_t& b) {
			return a.avg_depth > b.avg_depth;
		});

}

void free_resources(void) {
	////free(color_buffer);
	//std::cout << "free(color_buffer);" << std::endl;

	mesh.faces.clear();
#ifdef DEBUG
	std::cout << "mesh.faces.clear();" << std::endl;
#endif // DEBUG


	mesh.vertices.clear();
#ifdef DEBUG
	std::cout << "mesh.vertices.clear();" << std::endl;
#endif // DEBUG
}

void render(void) {
	SDL_RenderClear(renderer);

	draw_grid();
	// Loop all projected triangles and render them
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			triangle.draw_filled_triangle(triangle.color);
		}

		// Draw textured triangle
		if (render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE) {
			triangle.draw_textured_triangle(mesh_texture);
		}

		// Draw triangle wireframe
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_TEXTURED_WIRE) {
			triangle.draw_triangle(0xFFFFFFFF);
		}

		// Draw triangle vertex points
		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000); // vertex A
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000); // vertex B
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000); // vertex C
		}
	}

	

	//draw_triangle(300, 100, 50, 400, 500, 700, 0xFF00FF00);

	// Clear the array of triangles to render
	triangles_to_render.clear();
	
	render_color_buffer();

	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

#ifdef TEST

typedef struct {
	float m[4][4];
} mat4_t_t;
mat4_t_t mat4_identity(void) {
	// | 1 0 0 0 |
	// | 0 1 0 0 |
	// | 0 0 1 0 |
	// | 0 0 0 1 |
	mat4_t_t m = { {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	 } };
	return m;
}


mat4_t_t mat4_make_rotation_x(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	// | 1  0  0  0 |
	// | 0  c -s  0 |
	// | 0  s  c  0 |
	// | 0  0  0  1 |
	mat4_t_t m = mat4_identity();
	m.m[1][1] = c;
	m.m[1][2] = -s;
	m.m[2][1] = s;
	m.m[2][2] = c;
	return m;
}
#endif // 




int main(int args, char* argr[]) {

#ifdef DEV

	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
#ifdef DEBUG
	std::cout << "destroy_window" << std::endl;
#endif // DEBUG


	free_resources();
#ifdef DEBUG
	std::cout << "free_resources" << std::endl;
#endif // DEBUG


#endif // DEV


#ifdef TEST
	std::cout << " start" << std::endl;
	mat4_t m1 = {};
	mat4_t_t m2;
	m1.make_rotation_x(0.1);
	m2 = mat4_make_rotation_x(0.1);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m1.m[i][j] != m2.m[i][j]) {
				std::cout << "Mismatch at (" << i << "," << j << "): "
					<< m1.m[i][j] << " vs " << m2.m[i][j] << std::endl;
			}
		}
	}
#endif // TEST

	return 0;
}
