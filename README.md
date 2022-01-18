## Data Structures

```c
// Components

// generic container for all components
struct Component;
// holds a pointer to a mesh, 1 per object
struct ComponentMesh;
// holds the materials for a mesh and indicates that it should be rendered, 1 per object
struct ComponentMeshRenderer;

// Game Constructs

// holds vertices, indices, vao, vbo, ebo for rendering a mesh
struct GameMesh;
// tells the renderer what shader, textures, and shader parameters to use for a ComponentMeshRenderer
struct GameMaterial;
```