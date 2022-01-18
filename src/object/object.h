#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include "component.h"
#include "cglm/cglm.h"

struct ObjectTransform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

// game object
// functions as a doubly-linked list
struct GameObject {
    const char *name;

    // all objects have a transform component
    struct ObjectTransform transform;
    // left-most of component list
    struct GameComponent *component_head;
    
    // child is left-most of a new linked-list
    struct GameObject *child;
    struct GameObject *parent;

    struct GameObject *next;
    // if NULL, object is left-most of siblings
    struct GameObject *prev;
};

// If NULL, this creates an entirely new hierarchy.
struct GameObject *object_create_sibling(struct GameObject *prev, const char *name);
// The parent object doesn't need to be childless for this to succeed.
struct GameObject *object_create_child(struct GameObject *parent, const char *name);

// Destroy a World Object, recursively destroying all its children, grandchildren, etc.
// Returns a pointer to whatever was to the right of this object
struct GameObject *object_destroy(struct GameObject *obj);

// Print a list of all objects ('root' being top-left in hierarchy)
void object_print_hier(struct GameObject *root);

#endif