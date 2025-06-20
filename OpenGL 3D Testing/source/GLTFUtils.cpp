#include "GLTFUtils.h"

tinygltf::Model GLTFUtils::readModel(const char *filepath) {
  tinygltf::Model gltfModel;
  tinygltf::TinyGLTF loader;

  std::string err;
  std::string warn;

  bool ret = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, filepath);
  if (!warn.empty()) {
    std::cout << "Warning: " << warn << '\n';
  }
  if (!err.empty()) {
    std::cout << "Error: " << err << '\n';
  }
  if (!ret) {
    std::cout << "Failed to load GLTF.\n";
  }

  return gltfModel;
}

std::vector<Vertex> GLTFUtils::extractVertices(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive) {
  std::vector<Vertex> vertices;

  const auto &positionAccessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
  const auto &positionView = gltfModel.bufferViews[positionAccessor.bufferView];
  const auto &positionBuffer = gltfModel.buffers[positionView.buffer];

  const float *positionData = reinterpret_cast<const float *>(&positionBuffer.data[positionView.byteOffset + positionAccessor.byteOffset]);

  for (std::size_t i = 0; i < positionAccessor.count; i++) {
    Vertex vertex{};
    vertex.position = glm::make_vec3(&positionData[i * 3]);
    vertices.push_back(vertex);
  }

  if (primitive.attributes.count("NORMAL")) {
    const auto &normalAccessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
    const auto &normalView = gltfModel.bufferViews[normalAccessor.bufferView];
    const auto &normalBuffer = gltfModel.buffers[normalView.buffer];

    const float *normalData = reinterpret_cast<const float *>(&normalBuffer.data[normalView.byteOffset + normalAccessor.byteOffset]);

    std::size_t normalCount = std::min(normalAccessor.count, vertices.size());
    for (std::size_t i = 0; i < normalCount; i++) {
      vertices[i].normal = glm::make_vec3(&normalData[i * 3]);
    }
  }

  if (primitive.attributes.count("TEXCOORD_0")) {
    const auto &texCoordAccessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_0")];
    const auto &texCoordView = gltfModel.bufferViews[texCoordAccessor.bufferView];
    const auto &texCoordBuffer = gltfModel.buffers[texCoordView.buffer];

    const float *texCoordData = reinterpret_cast<const float *>(&texCoordBuffer.data[texCoordView.byteOffset + texCoordAccessor.byteOffset]);

    std::size_t texCoordCount = std::min(texCoordAccessor.count, vertices.size());
    for (std::size_t i = 0; i < texCoordCount; i++) {
      vertices[i].texCoords = glm::make_vec2(&texCoordData[i * 2]);
    }
  }

  if (primitive.attributes.count("JOINTS_0")) {
    const auto &jointAccessor = gltfModel.accessors[primitive.attributes.at("JOINTS_0")];
    const auto &jointView = gltfModel.bufferViews[jointAccessor.bufferView];
    const auto &jointBuffer = gltfModel.buffers[jointView.buffer];

    const unsigned char *jointData = &jointBuffer.data[jointView.byteOffset + jointAccessor.byteOffset];

    for (std::size_t i = 0; i < jointAccessor.count; i++) {
      if (jointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
        glm::u8vec4 joints = *reinterpret_cast<const glm::u8vec4 *>(jointData + i * 4);
        vertices[i].boneIDs = glm::uvec4(joints);
      }
      else if (jointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        const std::uint16_t *joints = reinterpret_cast<const std::uint16_t *>(jointData + i * 4 * sizeof(uint16_t));
        vertices[i].boneIDs = glm::uvec4(joints[0], joints[1], joints[2], joints[3]);
      }
    }
  }

  if (primitive.attributes.count("WEIGHTS_0")) {
    const auto &weightAccessor = gltfModel.accessors[primitive.attributes.at("WEIGHTS_0")];
    const auto &weightView = gltfModel.bufferViews[weightAccessor.bufferView];
    const auto &weightBuffer = gltfModel.buffers[weightView.buffer];

    const float *weightData = reinterpret_cast<const float *>(&weightBuffer.data[weightView.byteOffset + weightAccessor.byteOffset]);

    for (std::size_t i = 0; i < weightAccessor.count; i++) {
      glm::vec4 weights = glm::make_vec4(weightData + i * 4);
      vertices[i].boneWeights = weights;

      float totalWeight = vertices[i].boneWeights.x +
        vertices[i].boneWeights.y +
        vertices[i].boneWeights.z +
        vertices[i].boneWeights.w;

      if (totalWeight > 0.0f) {
        vertices[i].boneWeights /= totalWeight;
      }
    }
  }

  return vertices;
}

std::vector<unsigned int> GLTFUtils::extractIndices(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive) {
  std::vector<unsigned int> indices;

  const auto &indexAccessor = gltfModel.accessors[primitive.indices];
  const auto &indexView = gltfModel.bufferViews[indexAccessor.bufferView];
  const auto &indexBuffer = gltfModel.buffers[indexView.buffer];

  if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
    const std::uint16_t *buffer = reinterpret_cast<const std::uint16_t *>(&indexBuffer.data[static_cast<size_t>(indexView.byteOffset) + indexAccessor.byteOffset]);

    for (std::size_t i = 0; i < indexAccessor.count; i++) {
      indices.push_back(buffer[i]);
    }
  }
  else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
    const std::uint32_t *buffer = reinterpret_cast<const std::uint32_t *>(&indexBuffer.data[static_cast<size_t>(indexView.byteOffset) + indexAccessor.byteOffset]);

    for (std::size_t i = 0; i < indexAccessor.count; i++) {
      indices.push_back(buffer[i]);
    }
  }

  return indices;
}

unsigned int GLTFUtils::loadTexture(const tinygltf::Model &gltfModel, std::size_t textureIndex) {
  if (textureIndex >= gltfModel.textures.size()) {
    return 0;
  }

  const auto &tex = gltfModel.textures[textureIndex];
  const auto &image = gltfModel.images[tex.source];

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, image.component == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image.image.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

Mesh GLTFUtils::processPrimitive(const tinygltf::Model &gltfModel, const tinygltf::Primitive &primitive) {
  std::vector<Vertex> vertices = extractVertices(gltfModel, primitive);
  std::vector<unsigned int> indices = extractIndices(gltfModel, primitive);

  unsigned int textureID = 0;
  if (primitive.material >= 0) {
    const auto &material = gltfModel.materials[primitive.material];
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
      textureID = loadTexture(gltfModel, material.pbrMetallicRoughness.baseColorTexture.index);
    }
  }

  Mesh mesh(vertices, indices, textureID);

  return mesh;
}

std::vector<Mesh> GLTFUtils::loadMeshes(const tinygltf::Model &gltfModel) {
  std::vector<Mesh> meshes;

  for (const auto &gltfMesh : gltfModel.meshes) {
    for (const auto &primitive : gltfMesh.primitives) {
      meshes.push_back(processPrimitive(gltfModel, primitive));
    }
  }

  return meshes;
}