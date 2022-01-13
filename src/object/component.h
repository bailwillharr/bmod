#ifndef OBJECT_COMPONENT_H
#define OBJECT_COMPONENT_H

enum ObjectComponentType {
    COMPONENT_MESH,
    COMPONENT_MESH_RENDERER,
    COMPONENT_LIGHT,
};

// generic object component, to be overloaded
struct ObjectComponent {
    enum ObjectComponentType type;
    void *data; // casted to specific component struct, e.g. (struct LightComponent *)
    //void * (*init)(); // set to specific component init function, e.g. struct LightComponent * init()
    //void (*destroy)(void *); // set to specific component destroy function, e.g. void destroy(struct LightComponent *)
};

#endif