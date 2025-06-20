#ifndef ANIMATION_H
#define ANIMATION_H

#include <tiny_gltf.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <map>

class AnimationUtils {
public:
  static float getAnimationTime(const tinygltf::Model &model, const tinygltf::Animation &anim, float currentTime);

  static glm::mat4 interpolateChannelTransform(const tinygltf::Model &model, const tinygltf::Animation &anim, const tinygltf::AnimationChannel &channel, float animationTime);

  static std::map<int, glm::mat4> calculateLocalPose(const tinygltf::Model &model, const tinygltf::Animation &anim, float animationTime);
  static void calculateGlobalPose(const tinygltf::Model &model, int nodeIndex, glm::mat4 parentTransform, const std::map<int, glm::mat4> &localPose, std::map<int, glm::mat4> &globalPose);

  static std::vector<glm::mat4> calculateJointMatrices(const tinygltf::Skin &skin, const std::map<int, glm::mat4> &globalPose, const std::vector<glm::mat4> &inverseBindMatrices);
};

#endif