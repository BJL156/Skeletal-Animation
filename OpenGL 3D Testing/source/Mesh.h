#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::uvec4 boneIDs = glm::uvec4(0);
  glm::vec4 boneWeights = glm::vec4(0.0f);
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int textureID);

  void deleteBuffers();

  void draw(Shader &shader);
private:
  void setupRenderData();

  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  unsigned int m_textureID;

  unsigned int m_vbo;
  unsigned int m_vao;
  unsigned int m_ebo;
};

#endif