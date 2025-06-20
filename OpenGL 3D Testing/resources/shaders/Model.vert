#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;
layout (location = 3) in uvec4 a_boneIDs;
layout (location = 4) in vec4 a_weights;

out vec2 v_texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_jointMatrices[200];
uniform bool u_isAnimated;

void main() {
  v_texCoords = a_texCoords;

  vec4 position = vec4(a_position, 1.0);
  vec3 normal = a_normal;

  if (u_isAnimated) {
    mat4 skinMatrix =
	  a_weights.x * u_jointMatrices[a_boneIDs.x] +
	  a_weights.y * u_jointMatrices[a_boneIDs.y] +
	  a_weights.z * u_jointMatrices[a_boneIDs.z] +
	  a_weights.w * u_jointMatrices[a_boneIDs.w];
    
    position = skinMatrix * position;
	normal = mat3(skinMatrix) * a_normal;
  }

  gl_Position = u_projection * u_view * u_model * position;
}