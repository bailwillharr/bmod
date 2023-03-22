#include <stdlib.h>
#include "../../logger.h"
#include "../component.h"

#include "component_mesh.h"

struct GameComponent *component_mesh_create(struct GameObject *object, struct GameMesh *mesh)
{
	// first check if a mesh already exists in object
	for (struct GameComponent *current = object->component_head; current != NULL; current = current->next) {
		if (current->type == COMPONENT_MESH) {
			log_error("cannot add mesh to an object with a mesh already\n");
			return NULL;
		}
	}

	struct GameComponent *newComp = component_create(object);
	if (newComp == NULL) {
		log_error("Unable to create new component object\n");
		return NULL;
	}

	newComp->type = COMPONENT_MESH;

	newComp->data = malloc(sizeof(struct ComponentMesh));
	if (newComp->data == NULL) {
		log_error("Unable to allocate memory for ComponentMesh data\n");
		return NULL;
	}

	// pointers lol
	((struct ComponentMesh *)newComp->data)->mesh = mesh;

	return newComp;

}

void component_mesh_destroy(struct GameComponent *comp)
{
	free(comp->data);
	component_destroy(comp);
}
