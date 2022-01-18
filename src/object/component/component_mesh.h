#ifndef OBJECT_COMPONENT_COMPONENT_MESH_H
#define OBJECT_COMPONENT_COMPONENT_MESH_H

#include "../object.h"
#include "../component.h"

struct ComponentMesh {
	// pointer to the loaded mesh in the game's list of allocated meshes
	struct GameMesh *mesh;
};

struct GameComponent *component_mesh_create(struct GameObject *object, struct GameMesh *mesh);
struct GameComponent *component_mesh_destroy(struct GameComponent *comp);

#endif