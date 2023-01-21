#include "Vertex.hh"

namespace mymatrixtoolbox {

CameraData CameraData::getStandardOrtho(std::shared_ptr<Window> window) {
  glm::mat4 projection = glm::ortho<float>(0.0f, window->getExtent().width, 0.0f, window->getExtent().height);
  CameraData data;
  data.projection = projection;
  return data;
}
 
vkutil::VertexInputDescription PolygonVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(PolygonVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
  positionAttribute.offset = offsetof(PolygonVertex, position);

  VkVertexInputAttributeDescription colorAttribute = {};
  colorAttribute.binding = 0;
  colorAttribute.location = 1;
  colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
  colorAttribute.offset = offsetof(PolygonVertex, color);

  VkVertexInputAttributeDescription uvAttribute = {};
  uvAttribute.binding = 0;
  uvAttribute.location = 2;
  uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  uvAttribute.offset = offsetof(PolygonVertex, uv);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(colorAttribute);
  description.attributes.push_back(uvAttribute);

  return description;
}

vkutil::VertexInputDescription LineVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(LineVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputBindingDescription instanceBinding = {};
  instanceBinding.binding = 1;
  instanceBinding.stride = sizeof(LineInstance);
  instanceBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
  description.bindings.push_back(instanceBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  positionAttribute.offset = offsetof(LineVertex, position);

  VkVertexInputAttributeDescription pointAAttribute = {};
  pointAAttribute.binding = 1;
  pointAAttribute.location = 1;
  pointAAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointAAttribute.offset = offsetof(LineInstance, pointA);

  VkVertexInputAttributeDescription pointBAttribute = {};
  pointBAttribute.binding = 1;
  pointBAttribute.location = 2;
  pointBAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointBAttribute.offset = offsetof(LineInstance, pointB);

  VkVertexInputAttributeDescription colorAttribute = {};
  colorAttribute.binding = 1;
  colorAttribute.location = 3;
  colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
  colorAttribute.offset = offsetof(LineInstance, color);

  VkVertexInputAttributeDescription widthAttribute = {};
  widthAttribute.binding = 1;
  widthAttribute.location = 4;
  widthAttribute.format = VK_FORMAT_R32_SFLOAT;
  widthAttribute.offset = offsetof(LineInstance, width);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(pointAAttribute);
  description.attributes.push_back(pointBAttribute);
  description.attributes.push_back(colorAttribute);
  description.attributes.push_back(widthAttribute);

  return description;
}

vkutil::VertexInputDescription FontVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(FontVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  positionAttribute.offset = offsetof(FontVertex, position);

  VkVertexInputAttributeDescription uvAttribute = {};
  uvAttribute.binding = 0;
  uvAttribute.location = 1;
  uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  uvAttribute.offset = offsetof(FontVertex, uv);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(uvAttribute);

  return description;
}

}