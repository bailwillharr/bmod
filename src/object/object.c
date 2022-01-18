#include <stdlib.h>

#include "../logger.h"

#include "object.h"

static void initObjectData(struct GameObject *obj)
{
    obj->transform.position[0] = 0.0f;
    obj->transform.position[1] = 0.0f;
    obj->transform.position[2] = 0.0f;
    obj->transform.rotation[0] = 0.0f;
    obj->transform.rotation[1] = 0.0f;
    obj->transform.rotation[2] = 0.0f;
    obj->transform.scale[0] = 1.0f;
    obj->transform.scale[1] = 1.0f;
    obj->transform.scale[2] = 1.0f;
}

// If NULL, this creates an entirely new hierarchy.
struct GameObject *object_create_sibling(struct GameObject *prev, const char *name)
{

    struct GameObject *new = calloc(1, sizeof(struct GameObject));
    if (new == NULL) {
        log_error("calloc() failed for new object '%s'\n", name);
        return NULL;
    }
    new->name = name;
    new->component_head = NULL;
    new->child = NULL;
    new->prev = prev;

    if (prev != NULL) {
        // insert new element in between
        new->parent = prev->parent;
        new->next = prev->next;
        prev->next = new;
    } else {
#ifndef NDEBUG
        log_log("Created new object hierarchy\n");
#endif
        new->parent = NULL;
        new->next = NULL;
    }

    initObjectData(new);

    return new;

}

// The parent object doesn't need to be childless for this to succeed.
struct GameObject *object_create_child(struct GameObject *parent, const char *name)
{
    if (parent == NULL) {
        return NULL;
    }

    struct GameObject *new = calloc(1, sizeof(struct GameObject));
    if (new == NULL) {
        log_error("calloc() failed for new object '%s'\n", name);
        return NULL;
    }

    new->name = name;
    new->component_head = NULL;
    new->child = NULL;
    new->parent = parent;
    new->next = NULL;

    if (parent->child == NULL) {
        parent->child = new;
        new->prev = NULL;
    } else {
        struct GameObject *end; // end of children
        for (end = parent->child; end->next != NULL; end = end->next) ;
        new->prev = end;
        end->next = new;
    }

    initObjectData(new);

    return new;
}

// Destroy a World Object, recursively destroying all its children, grandchildren, etc.
// Returns a pointer to whatever was to the right of this object
struct GameObject *object_destroy(struct GameObject *obj)
{
    while (obj->child != NULL) {
        // first delete all the object's children
        object_destroy(obj->child);
    }

    // if obj is left-most of many children
    if (obj->parent != NULL) {
        if (obj->parent->child == obj) {
            if (obj->next != NULL) {
                obj->parent->child = obj->next;
            } else {
                obj->parent-> child = NULL;
            }
        }
    }

    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
    if (obj->next != NULL) { 
        obj->next->prev = obj->prev;
    }

    struct GameObject *right = obj->next;
    free(obj);
    return right;
    
}

static void print_level(struct GameObject *left, int level)
{
    for (struct GameObject *current = left; current != NULL; current = current->next) {
        for (int i = 0; i < level; i++) {
            fprintf(stderr, "\t");
        }
        fprintf(stderr, "%s\n", current->name);
        if (current->child != NULL) {
            print_level(current->child, level+1);
        }
    }
}

void object_print_hier(struct GameObject *root)
{
    print_level(root, 0);
}