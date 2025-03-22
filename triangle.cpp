#include "triangle.h"
#include "display.h"
#include <algorithm>
void draw_texel(int x, int y, uint32_t* texture, vec4_t point_a, vec4_t point_b, vec4_t point_c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv);

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inv_slope1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope2 = (float)(x2 - x0) / (y2 - y0);

	float x_start = x0;
	float x_end = x0;

	for (int y = y0; y <= y2; y++) {
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inv_slope1 = (float)(x2 - x0) / (y2 - y0);
	float inv_slope2 = (float)(x2 - x1) / (y2 - y1);
	float x_start = x2;
	float x_end = x2;

	for (int y = y2; y >= y0; y--) {
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}

}
void triangle_t::draw_filled_triangle(uint32_t color)
{
	float y0 = points[0].y;
	float y1 = points[1].y;
	float y2 = points[2].y;
	float x0 = points[0].x;
	float x1 = points[1].x;
	float x2 = points[2].x;

	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}


	if (y1 == y2) {
		// Draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		// Draw flat-top triangle
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		// Calculate the new vertex (Mx,My) using triangle similarity
		int My = y1;
		int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

		// Draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

		// Draw flat-top triangle
		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
	}
}

void triangle_t::draw_triangle(uint32_t color)
{
	draw_line(points[0].x, points[0].y, points[1].x, points[1].y, color);
	draw_line(points[1].x, points[1].y, points[2].x, points[2].y, color);
	draw_line(points[2].x, points[2].y, points[0].x, points[0].y, color);
}

void triangle_t::draw_triangle()
{
	this->draw_triangle(color);
}

void triangle_t::draw_filled_triangle()
{
	this->draw_filled_triangle(color);
}

void triangle_t::draw_textured_triangle(uint32_t* texture)
{
	int x0 = points[0].x;
	int y0 = points[0].y;
	float z0 = points[0].z;
	float w0 = points[0].w;

	int x1 = points[1].x;
	int y1 = points[1].y;
	float z1 = points[1].z;
	float w1 = points[1].w;

	int x2 = points[2].x;
	int y2 = points[2].y;
	float z2 = points[2].z;
	float w2 = points[2].w;

	float u0 = texcoords[0].u;
	float v0 = texcoords[0].v;
	float u1 = texcoords[1].u;
	float v1 = texcoords[1].v;
	float u2 = texcoords[2].u;
	float v2 = texcoords[2].v;
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
		std::swap(z0, z1);
		std::swap(w0, w1);
		std::swap(u0, u1);
		std::swap(v0, v1);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(z1, z2);
		std::swap(w1, w2);
		std::swap(u1, u2);
		std::swap(v1, v2);
	}
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
		std::swap(z0, z1);
		std::swap(w0, w1);
		std::swap(u0, u1);
		std::swap(v0, v1);
	}

	// Create vector points and texture coords after we sort the vertices
	vec4_t point_a = { (float)x0, (float)y0, z0, w0 };
	vec4_t point_b = { (float)x1, (float)y1, z1, w1 };
	vec4_t point_c = { (float)x2, (float)y2, z2, w2 };
	tex2_t a_uv = { u0, v0 };
	tex2_t b_uv = { u1, v1 };
	tex2_t c_uv = { u2, v2 };


	// Render the top part of the triangle
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

	if (y1 - y0 != 0) {

		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2 ;

			if (x_end < x_start) {
				std::swap(x_start, x_end);
			}
			for (int x = x_start; x < x_end; x++) {
				draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
			}
		}
	}

	// Render the bottom part of the triangle
	 inv_slope_1 = 0;
	 inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / (y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

	if (y2 - y1 != 0) {

		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				std::swap(x_start, x_end);
			}
			for (int x = x_start; x < x_end; x++) {
				// Draw our pixel with the color that comes from the texture
				draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
			}
		}
	}
}


// Return the barycentric weights alpha, beta, and gamma for point p
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
	// Find the vectors between the vertices ABC and point p
	vec2_t ac = c.sub(a);
	vec2_t ab = b.sub(a);
	vec2_t ap = p.sub(a);
	vec2_t pc = c.sub(p);
	vec2_t pb = b.sub(p);

	// Compute the area of the full parallegram/triangle ABC using 2D cross product
	float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x);// || AC x AB ||

	// Alpha is the area of the small parallelogram/triangle PBC divided by the area of the full parallelogram/triangle ABC
	float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

	// Beta is the area of the samll parallelogram/triangle APC divided by the area of the full parallelogram/triangle ABC
	float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

	// Weight gamma is easily found since barycentric coordinates always add up to 1.0
	float gamma = 1 - alpha - beta;

	return vec3_t{ alpha, beta, gamma };
}

void draw_texel(int x, int y, uint32_t* texture, vec4_t point_a, vec4_t point_b, vec4_t point_c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv) {
	vec2_t p = { (float)x , (float)y };
	vec2_t a = point_a.to_vec2();
	vec2_t b = point_b.to_vec2();
	vec2_t c = point_c.to_vec2();

	// Calcuate the barycentric coordinates of our point 'p' inside the triangle
	vec3_t weights = barycentric_weights(a, b, c, p);
	
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	// Variables to store the interpolated values of U, V, and also 1/w for the current pixel
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;
	// Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
	interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

	// Also interpolate the value of 1/w for the current pixel
	interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

	// Now we can divide back both interpolated values by 1/w
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;



	// Map the UV coordinate to the full texture width and hiegt
	int tex_x = std::abs((int)(interpolated_u * texture_width));
	int tex_y = std::abs((int)(interpolated_v * texture_height));

	draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);

}