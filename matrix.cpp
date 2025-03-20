#include "matrix.h"
#include <math.h>
mat4_t::mat4_t()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}
mat4_t& mat4_t::identity()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                m[i][j] = 1;
            }
            else {
                m[i][j] = 0;
            }
        }
    }
    return *this;
}

mat4_t mat4_t::mat4_identity()
{
    mat4_t m = mat4_t(1,1,1,1);
    return m;
}

mat4_t& mat4_t::make_scale(float sx, float sy, float sz)
{
    this->identity();
    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;
    return *this;
}

mat4_t& mat4_t::make_translation(float tx, float ty, float tz)
{

    this->identity();
    m[0][3] = tx;
    m[1][3] = ty;
    m[2][3] = tz;
    return *this;
}

mat4_t& mat4_t::make_rotation_x(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // | 1  0  0  0 |
    // | 0  c -s  0 |
    // | 0  s  c  0 |
    // | 0  0  0  1 |
    this->identity();
    m[1][1] = c;
    m[1][2] = -s;
    m[2][1] = s;
    m[2][2] = c;
    return *this;

}

mat4_t& mat4_t::make_rotation_y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // |  c  0  s  0 |
    // |  0  1  0  0 |
    // | -s  0  c  0 |
    // |  0  0  0  1 |
    this->identity();
    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;
    return *this;
}

mat4_t& mat4_t::make_rotation_z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // | c -s  0  0 |
    // | s  c  0  0 |
    // | 0  0  1  0 |
    // | 0  0  0  1 |
    this->identity();
    m[0][0] = c;
    m[0][1] = -s;
    m[1][0] = s;
    m[1][1] = c;
    return *this;

}

vec4_t mat4_t::mul_vec4(vec4_t v)
{
    vec4_t result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return result;
}

mat4_t mat4_t::mul_mat4(mat4_t b)
{
    mat4_t r;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + m[i][2] * b.m[2][j] + m[i][3] * b.m[3][j];
        }
    }
    
    return r;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
    // | (h/w)*1/tan(fov/2)             0              0                 0 |
    // |                  0  1/tan(fov/2)              0                 0 |
    // |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
    // |                  0             0              1                 0 |
    mat4_t m = { {{0}} };
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;
    return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
    // multiply the projection martix by our original vector
    vec4_t result = mat_proj.mul_vec4(v);

    // perform perspective divide with original z-value that is now stored in w
    if (result.w != 0.0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

