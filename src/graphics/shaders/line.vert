#version 450

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 pointA;
layout (location = 2) in vec2 pointB;
layout (location = 3) in vec3 color;
layout (location = 4) in float width;

layout (location = 0) out vec3 outColor;

layout (set = 0, binding = 0) uniform CameraData {
  mat4 projection;
} cameraData;

void main() {
  vec2 xBasis = pointB - pointA;
  vec2 yBasis = normalize(vec2(-xBasis.y, xBasis.x));
  vec2 point = pointA + xBasis * vPosition.x + yBasis * width * vPosition.y;
  gl_Position = cameraData.projection * vec4(point, 0, 1);
  outColor = color;
}