#ifndef OBJECT_COMPONENT_H
#define OBJECT_COMPONENT_H

enum GameComponentType {
    COMPONENT_MESH,
    COMPONENT_MESH_RENDERER,
    COMPONENT_LIGHT,
};

// generic object component, to be overloaded
struct GameComponent {
    enum GameComponentType type;
    void *data; // casted to specific component struct, e.g. (struct LightComponent *)

    struct GameComponent *next;
    struct GameComponent *prev;
    struct GameObject *object;
};

// if NULL, this creates a new component list
struct GameComponent *component_create(struct GameObject *object);
// Returns a pointer to whatever was to the right of this component.
struct GameComponent *component_destroy(struct GameComponent *comp);

#endif