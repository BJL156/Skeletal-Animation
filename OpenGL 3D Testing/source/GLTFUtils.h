#ifndef GLTF_UTILS_H
#define GLTF_UTILS_H

#include <glad/glad.h>
#include <tiny_gltf.h>
#include <glm/gtc/type_ptr.hpp> 

#include "Mesh.h"

#include <iostream>
#include <vector>

class GLTFUtils {
public:
  static tinygltf::Model readModel(const char *filepath);
  static std::vector<Vertex> extractVertices(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive);
  static std::vector<unsigned int> extractIndices(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive);

  static unsigned int loadTexture(const tinygltf::Model &gltfModel, std::size_t textureIndex);

  static Mesh processPrimitive(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive);
  static std::vector<Mesh> loadMeshes(const tinygltf::Model &gltfModel);
};

#endif