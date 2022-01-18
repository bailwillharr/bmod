#include <stdlib.h>

#include "../../logger.h"
#include "../component.h"

#include "component_mesh_renderer.h"

struct GameComponent *component_mesh_renderer_create(struct GameObject *object)
{
	// first check if a mesh already exists in object
	for (struct GameComponent *current = object->component_head; current != NULL; current = current->next) {
		if (current->type == COMPONENT_MESH_RENDERER) {
			log_error("cannot add mesh renderer to an object with a mesh renderer already\n");
			return NULL;
		}
	}

	struct GameComponent *newComp = component_create(object);
	if (newComp == NULL) {
		log_error("Unable to create new component object\n");
		return NULL;
	}

	newComp->type = COMPONENT_MESH_RENDERER;

	newComp->data = malloc(sizeof(struct ComponentMeshRenderer));
	if (newComp->data == NULL) {
		log_error("Unable to allocate memory for ComponentMeshRenderer data\n");
		return NULL;
	}

	return newComp;

}

struct GameComponent *component_mesh_renderer_destroy(struct GameComponent *comp)
{
	free(comp->data);
	component_destroy(comp);
}