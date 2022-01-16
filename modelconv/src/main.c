// convert 3d model files to bmod format

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cglm/cglm.h>

// structs

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct MeshFileHeader {
    unsigned int        vertex_count;
    unsigned int        index_count;
    int                 material;
    // Vertices begin immediately after this header.
    // Indices begin immediately after that
};

// static functions

// extern functions

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s (input file) (output file)\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const struct aiScene *scene = aiImportFile(argv[1], aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_GenUVCoords | aiProcess_PreTransformVertices);
    if (scene == NULL) {
        fprintf(stderr, "file import error: %s\n", aiGetErrorString());
        exit(EXIT_FAILURE);
    }

    const char *name = argv[2];

    fprintf(stderr, "found %d children, %d materials\n", scene->mRootNode->mNumChildren, scene->mNumMaterials);

    for (int i = 0; i < scene->mRootNode->mNumChildren; i++) {
        fprintf(stderr, "node mesh count: %d\n", scene->mRootNode->mChildren[i]->mNumMeshes);
        for (int j = 0; j < scene->mRootNode->mChildren[i]->mNumMeshes; j++) {
            fprintf(stderr, "\t mesh name: %d\n", scene->mRootNode->mChildren[i]->mMeshes[j]);
        }
    }

    for (int i = 0; i < scene->mNumMeshes; i++) {
        char buf[256];
        sprintf_s(buf, 255, "%s_%d.bmod_mesh", name, i);
        fprintf(stderr, "mesh vertex count: %d\n", scene->mMeshes[i]->mNumVertices);

        FILE *fp = fopen(buf, "wb");
        if (fp == NULL) {
            fprintf(stderr, "unable to create file\n");
            exit(EXIT_FAILURE);
        }

        struct MeshFileHeader header = {
            .index_count = scene->mMeshes[i]->mNumFaces * 3, // all faces have 3 vertices with triangulation
            .vertex_count = scene->mMeshes[i]->mNumVertices,
        };

        fwrite(&header, sizeof(struct MeshFileHeader), 1, fp);
        for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
            fwrite(scene->mMeshes[i]->mFaces[j].mIndices, sizeof(unsigned int), 3, fp);
        }
        printf("ftell: %d\n", ftell(fp));
        for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
            fwrite(&scene->mMeshes[i]->mVertices[j].x, sizeof(float), 1, fp);
            fwrite(&scene->mMeshes[i]->mVertices[j].y, sizeof(float), 1, fp);
            fwrite(&scene->mMeshes[i]->mVertices[j].z, sizeof(float), 1, fp);
            
            fwrite(&scene->mMeshes[i]->mNormals[j].x, sizeof(float), 1, fp);
            fwrite(&scene->mMeshes[i]->mNormals[j].y, sizeof(float), 1, fp);
            fwrite(&scene->mMeshes[i]->mNormals[j].z, sizeof(float), 1, fp);
            fwrite(&scene->mMeshes[i]->mTextureCoords[0][j], sizeof(float), 2, fp);
        }

        for (int j = 0; j < header.vertex_count; j++) {
            printf("vertex %d: %f, %f, %f\n", j, scene->mMeshes[i]->mNormals[j].x, scene->mMeshes[i]->mNormals[j].y, scene->mMeshes[i]->mNormals[j].z);
        }

        fclose(fp);

	}

    aiReleaseImport(scene);

    return 0;
}