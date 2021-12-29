#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

struct Camera {
	float fov;
	float aspect; // width / height
	mat4 p, v;
	vec2 rot;
	vec3 pos;
};

void camera_init(struct Camera *cam, float aspect, float fov);
void camera_update(struct Camera *cam);

#endif
