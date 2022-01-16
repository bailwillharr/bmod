#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <cglm/cglm.h>

struct Camera {
	float fov;
	float aspect; // width / height
	mat4 p, v;
	vec2 rot;
	vec3 pos;
};

void camera_init(struct Camera *cam, float aspect, float fov, vec3 pos, vec2 rot);
void camera_update(struct Camera *cam);

#endif
