#version 460 core

layout(location = 0) out vec4 fragColor;

in vec2 v_texCoords;

uniform sampler2D u_textureDiffuse;

void main() {
  fragColor = texture(u_textureDiffuse, v_texCoords);
}