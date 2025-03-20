# Vector Operations

## Back-face Culling Motivation

## Vector Magnitude



## Vector Cross Product

![image-20250309083323898](assets/image-20250309083323898.png)  

![image-20250309083351034](assets/image-20250309083351034.png)





## Back-face Culling Algorithm

![image-20250310063745430](assets/image-20250310063745430.png)



## Back-face Culling Code

```c++

vec3_t vector_a = transformed_vertexs[0]; /*   A   */
vec3_t vector_b = transformed_vertexs[1]; /*  / \  */
vec3_t vector_c = transformed_vertexs[2]; /* B---C */

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
```









# Flat Shading

