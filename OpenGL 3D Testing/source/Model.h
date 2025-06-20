#ifndef MODEL_H
#define MODEL_H

#include <tiny_gltf.h>

#include "GLTFUtils.h"
#include "AnimationUtils.h"
#include "Shader.h"
#include "Mesh.h"

class Model {
public:
  Model(const char *filepath);
  ~Model();

  void setAnimationIndex(std::size_t index);

  void update(float deltaTime);
  void draw(Shader &shader);
private:
  void loadInverseBindMatrices();
  void loadModel();

  const char *m_filepath;

  tinygltf::Model m_gltfModel;
  std::vector<Mesh> m_meshes;
  std::vector<glm::mat4> m_inverseBindMatrices;
  tinygltf::Skin m_skin;
  tinygltf::Animation m_animation;
  std::vector<glm::mat4> m_jointMatrices;
  float m_currentAnimationTime;

  bool m_isAnimated = false;
};

#endif