#include "Model.h"

Model::Model(const char *filepath)
  : m_filepath(filepath) {
  loadModel();
}

Model::~Model() {
  for (auto &mesh : m_meshes) {
    mesh.deleteBuffers();
  }
}

void Model::setAnimationIndex(std::size_t index) {
  if (index >= m_gltfModel.animations.size()) {
    std::cout << "Model \"" << m_filepath << "\"::Animation[" << index << "] is out of range.\n";
    return;
  }

  m_animation = m_gltfModel.animations[index];
  m_currentAnimationTime = 0.0f;
}

void Model::update(float deltaTime) {
  if (m_gltfModel.animations.empty()) {
    return;
  }

  m_currentAnimationTime += deltaTime;

  float animationTime = AnimationUtils::getAnimationTime(m_gltfModel, m_animation, m_currentAnimationTime);
  auto localPose = AnimationUtils::calculateLocalPose(m_gltfModel, m_animation, animationTime);

  std::map<int, glm::mat4> globalPose;
  int rootNode = m_skin.skeleton >= 0 ? m_skin.skeleton : m_skin.joints[0];
  AnimationUtils::calculateGlobalPose(m_gltfModel, rootNode, glm::mat4(1.0f), localPose, globalPose);

  m_jointMatrices = AnimationUtils::calculateJointMatrices(m_skin, globalPose, m_inverseBindMatrices);
}

void Model::draw(Shader &shader) {
  shader.use();

  shader.setInt("u_isAnimated", m_isAnimated);

  for (std::size_t i = 0; i < m_jointMatrices.size(); ++i) {
    std::string uniform = "u_jointMatrices[" + std::to_string(i) + "]";
    shader.setMat4(uniform.c_str(), m_jointMatrices[i]);
  }

  for (auto &mesh : m_meshes) {
    mesh.draw(shader);
  }
}

void Model::loadInverseBindMatrices() {
  const auto &accessor = m_gltfModel.accessors[m_skin.inverseBindMatrices];
  const auto &view = m_gltfModel.bufferViews[accessor.bufferView];
  const auto &buffer = m_gltfModel.buffers[view.buffer];

  const float *data = reinterpret_cast<const float *>(&buffer.data[view.byteOffset + accessor.byteOffset]);
  for (std::size_t i = 0; i < accessor.count; i++) {
    m_inverseBindMatrices.push_back(glm::make_mat4(data + i * 16));
  }
}

void Model::loadModel() {
  m_gltfModel = GLTFUtils::readModel(m_filepath);

  m_meshes = GLTFUtils::loadMeshes(m_gltfModel);

  if (!m_gltfModel.animations.empty()) {
    m_skin = m_gltfModel.skins[0];
    m_animation = m_gltfModel.animations[0];
    loadInverseBindMatrices();

    std::cout << "Model \"" << m_filepath << "\" has " << m_gltfModel.animations.size() << " animations.\n";
    for (std::size_t i = 0; i < m_gltfModel.animations.size(); ++i) {
      std::cout << "\tAnimation [" << i << "] \"" << m_gltfModel.animations[i].name << "\"\n";
    }

    m_isAnimated = true;
  }
}