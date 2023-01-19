#version 450

layout (location = 0) in vec2 inUv;
layout (location = 0) out vec4 outFragColor;

layout(binding = 1) uniform sampler2D textureSampler;

void main() {
  if(texture(textureSampler, inUv) == vec4(0,0,0,1)) {
    discard;
  } else {
    outFragColor = texture(textureSampler, inUv);
  }
}