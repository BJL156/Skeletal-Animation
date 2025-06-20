#include "AnimationUtils.h"

float AnimationUtils::getAnimationTime(const tinygltf::Model &model, const tinygltf::Animation &anim, float currentTime) {
  const auto &inputAccessor = model.accessors[anim.samplers[0].input];
  const auto &inputView = model.bufferViews[inputAccessor.bufferView];
  const auto &inputBuffer = model.buffers[inputView.buffer];

  const float *inputData = reinterpret_cast<const float *>(&inputBuffer.data[inputView.byteOffset + inputAccessor.byteOffset]);

  return fmod(currentTime, inputData[inputAccessor.count - 1]);
}

glm::mat4 AnimationUtils::interpolateChannelTransform(const tinygltf::Model &model, const tinygltf::Animation &anim, const tinygltf::AnimationChannel &channel, float animationTime) {
  const auto &sampler = anim.samplers[channel.sampler];

  const auto &inputAccessor = model.accessors[sampler.input];
  const auto &outputAccessor = model.accessors[sampler.output];
  const auto &inputView = model.bufferViews[inputAccessor.bufferView];
  const auto &outputView = model.bufferViews[outputAccessor.bufferView];
  const auto &inputBuffer = model.buffers[inputView.buffer];
  const auto &outputBuffer = model.buffers[outputView.buffer];

  const float *inputData = reinterpret_cast<const float *>(&inputBuffer.data[inputView.byteOffset + inputAccessor.byteOffset]);
  const float *outputData = reinterpret_cast<const float *>(&outputBuffer.data[outputView.byteOffset + outputAccessor.byteOffset]);

  size_t inputCount = inputAccessor.count;
  if (inputCount < 2) return glm::mat4(1.0f);

  size_t frameIndex = 0;
  while (frameIndex < inputCount - 1 && animationTime >= inputData[frameIndex + 1]) {
    frameIndex++;
  }

  float t0 = inputData[frameIndex];
  float t1 = inputData[frameIndex + 1];
  float factor = (animationTime - t0) / (t1 - t0);

  int components = (channel.target_path == "rotation") ? 4 : 3;
  const float *out0 = outputData + frameIndex * components;
  const float *out1 = outputData + (frameIndex + 1) * components;

  if (channel.target_path == "translation") {
    glm::vec3 v0 = glm::make_vec3(out0);
    glm::vec3 v1 = glm::make_vec3(out1);
    return glm::translate(glm::mat4(1.0f), glm::mix(v0, v1, factor));
  } else if (channel.target_path == "scale") {
    glm::vec3 v0 = glm::make_vec3(out0);
    glm::vec3 v1 = glm::make_vec3(out1);
    return glm::scale(glm::mat4(1.0f), glm::mix(v0, v1, factor));
  } else if (channel.target_path == "rotation") {
    glm::quat q0 = glm::make_quat(out0);
    glm::quat q1 = glm::make_quat(out1);
    return glm::mat4_cast(glm::slerp(q0, q1, factor));
  }

  return glm::mat4(1.0f);
}

std::map<int, glm::mat4> AnimationUtils::calculateLocalPose(const tinygltf::Model &model, const tinygltf::Animation &anim, float animationTime) {
  std::map<int, glm::mat4> localPose;

  for (const auto &channel : anim.channels) {
    int nodeIndex = channel.target_node;
    glm::mat4 transform = interpolateChannelTransform(model, anim, channel, animationTime);

    localPose[nodeIndex] = localPose.count(nodeIndex)
      ? localPose[nodeIndex] * transform
      : transform;
  }

  return localPose;
}

void AnimationUtils::calculateGlobalPose(const tinygltf::Model &model, int nodeIndex, glm::mat4 parentTransform,
  const std::map<int, glm::mat4> &localPose,
  std::map<int, glm::mat4> &globalPose) {
  glm::mat4 local = localPose.count(nodeIndex) ? localPose.at(nodeIndex) : glm::mat4(1.0f);
  glm::mat4 global = parentTransform * local;
  globalPose[nodeIndex] = global;

  for (int child : model.nodes[nodeIndex].children) {
    calculateGlobalPose(model, child, global, localPose, globalPose);
  }
}

std::vector<glm::mat4> AnimationUtils::calculateJointMatrices(const tinygltf::Skin &skin,
  const std::map<int, glm::mat4> &globalPose,
  const std::vector<glm::mat4> &inverseBindMatrices) {
  std::vector<glm::mat4> jointMatrices;

  for (size_t i = 0; i < skin.joints.size(); ++i) {
    int nodeIndex = skin.joints[i];
    glm::mat4 global = globalPose.count(nodeIndex) ? globalPose.at(nodeIndex) : glm::mat4(1.0f);
    jointMatrices.push_back(global * inverseBindMatrices[i]);
  }

  return jointMatrices;
}