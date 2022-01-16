// convert 3d model files to bmod format

#include <stdio.h>
#include <stdlib.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s (input file)\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const struct aiScene *scene = aiImportFile(argv[1], 0);
    if (scene == NULL) {
        fprintf(stderr, "file import error: %s\n", aiGetErrorString());
        exit(EXIT_FAILURE);
    }

    

    aiReleaseImport(scene);

    return 0;
}