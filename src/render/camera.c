#include "../game.h"

static void gen_proj_matrix(struct Camera *cam)
{
	const float CLIP_NEAR = 0.1f;
	const float CLIP_FAR = 500.0f;

	float vertical_fov_rads = 2.0f * atanf( (1.0f / cam->aspect) * tanf(cam->fov * GLM_PI / 360.0f));
	glm_perspective(vertical_fov_rads, cam->aspect, CLIP_NEAR, CLIP_FAR, cam->p);
}

static void gen_view_matrix(struct Camera *cam)
{
	vec3 direction = { 0.0f, 0.0f, -1.0f };
	glm_vec3_rotate(direction, cam->rot[0], (vec3){1.0f, 0.0f, 0.0f});
	glm_vec3_rotate(direction, cam->rot[1], (vec3){0.0f, 1.0f, 0.0f});

	glm_look(cam->pos, direction, GLM_YUP, cam->v);
}

void camera_init(struct Camera *cam, float aspect, float fov)
{
	cam->aspect = aspect;
	cam->fov = fov;

	cam->pos[0] = 0.0f;
	cam->pos[1] = 1.2f;
	cam->pos[2] = 5.0f;

	cam->rot[0] = 0.0f;
	cam->rot[1] = 0.0f;

	gen_proj_matrix(cam);
}

void camera_update(struct Camera *cam)
{
	gen_view_matrix(cam);
}