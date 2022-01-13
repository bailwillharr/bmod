#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include "component.h"
#include "cglm/cglm.h"

struct ObjectTransform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

// world object
// functions as a doubly-linked list
struct WorldObject {
    const char *name;

    struct ObjectTransform transform; // all objects have a transform component
    struct ObjectComponent *component_head; // also a doubly-linked list
    
    struct WorldObject *child; // child is left-most of a new linked-list
    struct WorldObject *parent;

    struct WorldObject *next;
    struct WorldObject *prev; // if NULL, object is left-most of siblings
};

// If NULL, this creates an entirely new hierarchy.
struct WorldObject *object_create_sibling(struct WorldObject *prev, const char *name);
// The parent object doesn't need to be childless for this to succeed.
struct WorldObject *object_create_child(struct WorldObject *parent, const char *name);

// Destroy a World Object, recursively destroying all its children, grandchildren, etc.
// Returns a pointer to whatever was to the right of this object
struct WorldObject *object_destroy(struct WorldObject *obj);

// Print a list of all objects ('root' being top-left in hierarchy)
void object_print_hier(struct WorldObject *root);

#endif