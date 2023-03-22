#ifndef OBJECT_COMPONENT_COMPONENT_MESH_RENDERER_H
#define OBJECT_COMPONENT_COMPONENT_MESH_RENDERER_H

#include "../object.h"
#include "../component.h"

struct ComponentMeshRenderer {
	struct GameMaterial *material;
};

struct GameComponent *component_mesh_renderer_create(struct GameObject *object);
void component_mesh_renderer_destroy(struct GameComponent *comp);

#endif
