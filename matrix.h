#pragma once

#include "vector.h"
#include <algorithm>
#include <array>
class mat4_t {
public:
	std::array<std::array<float, 4>, 4> m;

	mat4_t();

	mat4_t(std::array<std::array<float, 4>, 4> m) :m(m) {};

	mat4_t(float m1[4][4]) {
		std::copy(&m1[0][0], &m1[0][0] + 16, &m[0][0]);
	}
	mat4_t(float x, float y, float z, float w = 1.0f) {
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		m[2][2] = w;
	}

	mat4_t& identity();
	mat4_t mat4_identity();

	mat4_t& make_scale(float sx, float sy, float sz);
	mat4_t& make_translation(float tx, float ty, float tz);
	mat4_t& make_rotation_x(float angle);
	mat4_t& make_rotation_y(float angle);
	mat4_t& make_rotation_z(float angle);
	vec4_t mul_vec4(vec4_t v);
	mat4_t mul_mat4(mat4_t b);
	
	
	//static mat4_t mat4_make_rotation_x(float angle);
	//static mat4_t rotation_y(float angle);
};

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);
mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up);