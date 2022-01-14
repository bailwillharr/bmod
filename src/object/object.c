#include <stdlib.h>

#include "../logger.h"

#include "object.h"

// If NULL, this creates an entirely new hierarchy.
struct WorldObject *object_create_sibling(struct WorldObject *prev, const char *name)
{

    struct WorldObject *new = calloc(1, sizeof(struct WorldObject));
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

    return new;

}

// The parent object doesn't need to be childless for this to succeed.
struct WorldObject *object_create_child(struct WorldObject *parent, const char *name)
{
    if (parent == NULL) {
        return NULL;
    }

    struct WorldObject *new = calloc(1, sizeof(struct WorldObject));
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
        struct WorldObject *end; // end of children
        for (end = parent->child; end->next != NULL; end = end->next) ;
        new->prev = end;
        end->next = new;
    }

    return new;
}

// Destroy a World Object, recursively destroying all its children, grandchildren, etc.
// Returns a pointer to whatever was to the right of this object
struct WorldObject *object_destroy(struct WorldObject *obj)
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

    struct WorldObject *right = obj->next;
    free(obj);
    return right;
    
}

static void print_level(struct WorldObject *left, int level)
{
    for (struct WorldObject *current = left; current != NULL; current = current->next) {
        for (int i = 0; i < level; i++) {
            fprintf(stderr, "\t");
        }
        fprintf(stderr, "%s\n", current->name);
        if (current->child != NULL) {
            print_level(current->child, level+1);
        }
    }
}

void object_print_hier(struct WorldObject *root)
{
    print_level(root, 0);
}