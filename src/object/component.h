#ifndef OBJECT_COMPONENT_H
#define OBJECT_COMPONENT_H

enum ObjectComponentTypes {
    COMPONENT_MESH,
    COMPONENT_MESH_RENDERER,
    COMPONENT_LIGHT,
};

// generic object component, to be overloaded
struct ObjectComponent {
    enum ObjectComponentTypes type;
    void *data; // casted to specific component struct, e.g. (struct LightComponent *)

    struct ObjectComponent *next;
    struct ObjectComponent *prev;
};

struct ObjectComponent *component_create(struct ObjectComponent *prev);
void component_destroy(struct ObjectComponent *comp);

#endif