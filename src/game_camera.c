#include "game.h"

#include "game_camera.h"

void game_camera_update(struct Game * g, double frameDelta)
{
	vec3 dxRotated = {g->input.moveDX * frameDelta * 60.0f, 0.0f, 0.0f};
    glm_vec3_rotate(dxRotated, g->ren.cam.rot[1], GLM_YUP);
    vec3 dzRotated = {0.0f, 0.0f, g->input.moveDY * frameDelta * 60.0f};
    glm_vec3_rotate(dzRotated, g->ren.cam.rot[1], GLM_YUP);

	vec3 oldCameraPos;
	glm_vec3_copy(g->ren.cam.pos, oldCameraPos);

    glm_vec3_add(g->ren.cam.pos, dxRotated, g->ren.cam.pos);
    glm_vec3_add(g->ren.cam.pos, dzRotated, g->ren.cam.pos);

    // ascend / descend
    g->ren.cam.pos[1] += (g->input.buttons[INPUT_BUTTON_JUMP] ? 10.0f : 0.0f) * frameDelta;
    g->ren.cam.pos[1] += (g->input.buttons[INPUT_BUTTON_CROUCH] ? -10.0f : 0.0f) * frameDelta;

	// look around
    float rotDX = (float)g->input.lookAxisDY * -0.005f;
    g->ren.cam.rot[0] += rotDX;
    if (g->ren.cam.rot[0] >= GLM_PI_2 || g->ren.cam.rot[0] <= -GLM_PI_2) {
        g->ren.cam.rot[0] -= rotDX;
    }
    g->ren.cam.rot[1] += (float)g->input.lookAxisDX * -0.005f;
}
