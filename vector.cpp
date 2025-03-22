#include "vector.h"
#include <corecrt_math.h>

float vec2_t::len()
{
	return sqrt((x * x) + (y * y));
}

vec2_t vec2_t::add(const vec2_t& other)
{
	return vec2_t(x + other.x, y + other.y);
}

vec2_t vec2_t::sub(const vec2_t& other)
{
	return vec2_t(x - other.x, y - other.y);
}

vec2_t vec2_t::mul(float m)
{
	return vec2_t(x * m, y * m);
}

vec2_t vec2_t::div(float m)
{
	return vec2_t(x / m, y / m);
}

vec2_t& vec2_t::normalize()
{
	float l = len();
	if (l != 0.0f) { // Avoid division by zero
		this->x /= l;
		this->y /= l;
	}
	return *this;
}

float vec2_t::dot(const vec2_t& other)
{
	return (x * other.x )+ (y * other.y);

}




// ------------------------------
vec3_t vec3_t::vec3_rotate_x( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = this->x;
	rotated_vector.y = (this->y * cos(angle)) - (this->z * sin(angle));
	rotated_vector.z = (this->y * sin(angle)) + (this->z * cos(angle));
	return rotated_vector;
}

vec3_t vec3_t::vec3_rotate_y( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = (this->x * cos(angle)) - (this->z * sin(angle));
	rotated_vector.y = this->y;
	rotated_vector.z = (this->x * sin(angle)) + (this->z * cos(angle));
	return rotated_vector;
}

vec3_t vec3_t::vec3_rotate_z( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = (this->x * cos(angle)) - (this->y * sin(angle));
	rotated_vector.y = (this->x * sin(angle)) + (this->y * cos(angle));
	rotated_vector.z = this->z;
	return rotated_vector;
}

float vec3_t::len()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

vec3_t vec3_t::add(const vec3_t& other)
{
	return vec3_t(x + other.x, y + other.y, z + other.z);
}

vec3_t vec3_t::sub(const vec3_t& other)
{
	return vec3_t(x - other.x, y - other.y, z - other.z);
}

vec3_t vec3_t::mul(float m)
{
	return vec3_t(x * m, y * m, z * m);
}

vec3_t vec3_t::div(float m)
{
	return vec3_t(x / m, y / m, z / m);
}

vec3_t vec3_t::cross(const vec3_t& other)
{
	return vec3_t(
		(y * other.z) - (z * other.y),
		(z * other.x) - (x * other.z),
		(x * other.y) - (y * other.x)
	);
}

 vec3_t& vec3_t::normalize() 
{
	float l = len();
	if (l != 0.0f) { // Avoid division by zero
		this->x /= l;
		this->y /= l;
		this->z /= l;
	}
	return *this;
}

float vec3_t::dot(const vec3_t& other)
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

vec4_t vec3_t::vec3_to_vec4()
{
	vec4_t r(x, y, z);

	return r;
}

vec3_t vec4_t::vec4_to_vec3()
{
	vec3_t r(x, y, z);
	return r;

}
vec2_t vec4_t::to_vec2()
{
	vec2_t r(x, y);
	return r;
}
