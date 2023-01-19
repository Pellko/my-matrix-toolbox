#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;

layout (location = 0) out vec2 outUv;

layout (set = 0, binding = 0) uniform CameraBuffer {
  mat4 projection;
} cameraData;

void main() {
  gl_Position = cameraData.projection * vec4(vPosition, 1.0f);
  outUv = vUv;
}