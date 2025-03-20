#pragma once


class vec4_t;
//typedef struct {
//	float x, y;
//} vec2_t;
//
//typedef struct {
//	float x, y, z;
//} vec3_t;


class vec2_t {
public:
	float x, y;
	vec2_t() : x(0), y(0) {}
	vec2_t(float x, float y) : x(x), y(y) {}

	float len();

	vec2_t add(const vec2_t& other);
	vec2_t sub(const vec2_t& other);
	vec2_t mul(float m);
	vec2_t div(float m);
	vec2_t& normalize();
	float dot(const vec2_t& other);

};

class vec3_t {
public:
	float x, y, z;
	vec3_t() : x(0), y(0), z(0) {}
	vec3_t(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3_t vec3_rotate_x( float angle);
	vec3_t vec3_rotate_y(float angle);

	vec3_t vec3_rotate_z( float angle);

	float len();
	vec3_t add(const vec3_t& other);
	vec3_t sub(const vec3_t& other);
	vec3_t mul(float m);
	vec3_t div(float m);
	vec3_t cross(const vec3_t& other);
	vec3_t& normalize();
	float dot(const vec3_t& other);

	vec4_t vec3_to_vec4();
};


class vec4_t {
public:
	float x, y, z, w;
	vec4_t() : x(0), y(0), z(0), w(0) {}
	vec4_t(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

	vec3_t vec4_to_vec3();


};