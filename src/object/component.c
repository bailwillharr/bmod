#include <stdlib.h>

#include "../logger.h"

#include "object.h"

#include "component.h"

// if NULL, this creates a new component list
struct GameComponent *component_create(struct GameObject *object)
{
	/* ***********************  TODO: REDO THIS *******************/
	struct GameComponent *new = calloc(1, sizeof(struct GameComponent));
	if (new == NULL) {
		log_error("calloc() failed for new component\n");
		return NULL;
	}
	new->object = object;
	new->prev = NULL;
	new->next = object->component_head;
	object->component_head = new;
	if (new->next != NULL) {
		new->next->prev = new;
	}

	return new;
	
}

// Returns a pointer to whatever was to the right of this object.
struct GameComponent *component_destroy(struct GameComponent *comp)
{
	if (comp->prev == NULL) {
		// if left-most component, change HEAD pointer
		comp->object->component_head = comp->next;
	} else {
		comp->prev->next = comp->next;
	}
	if (comp->next != NULL) {
		comp->next->prev = comp->prev;
	}

	struct GameComponent *right = comp->next;
	free(comp);
	return right;
}