#version 450

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 pointA;
layout (location = 2) in vec2 pointB;

void main() {
  float width = 0.01;
  vec2 xBasis = pointB - pointA;
  vec2 yBasis = normalize(vec2(-xBasis.y, xBasis.x));
  vec2 point = pointA + xBasis * vPosition.x + yBasis * width * vPosition.y;
  gl_Position = vec4(point, 0, 1);
}