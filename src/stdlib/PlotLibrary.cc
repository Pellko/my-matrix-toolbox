#include "PlotLibrary.hh"
#include "src/graphics/Window.hh"
#include "src/graphics/LineRenderer.hh"
#include "src/graphics/FontRenderer.hh"
#include "src/context/ExecutionContext.hh"
#include "src/math/ObjectMatrix.hh"
#include "src/vm/RuntimeException.hh"

namespace mymatrixtoolbox {
namespace stdlib {

static Value plot(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args) {
  ObjectMatrix* xData = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* yData = static_cast<ObjectMatrix*>(args[1].as.object);

  std::vector<double> x(xData->getHeight());
  std::vector<double> y(xData->getHeight());

  for(int i=0;i<xData->getHeight();i++) {
    x[i] = xData->get(0, i).as.number;
    y[i] = yData->get(0, i).as.number;
  }

  std::shared_ptr<Window> window = context->createWindow("My Matrix Toolbox");
  std::shared_ptr<LineRenderer> lineRenderer = std::make_shared<LineRenderer>(window, 0);
  std::shared_ptr<FontRenderer> fontRenderer = std::make_shared<FontRenderer>(window, 0);

  window->addInitializer([=]() {
    const int padding = 64;
    const glm::vec3 axisColor = {150, 150, 150};

    glm::vec2 max = {0,0};
    glm::vec2 min = {0,0};

    for(int i=0;i<x.size();i++) {
      // Get maximum values
      if(x[i] > max.x) {
        max.x = x[i];
      }
      if(y[i] > max.y) {
        max.y = y[i];
      }

      // Get minimum values
      if(x[i] < min.x) {
        min.x = x[i];
      }
      if(y[i] < min.y) {
        min.y = y[i];
      }
    }

    double xAxis = max.x - min.x;
    double yAxis = max.y - min.y;
    double screenWidth = window->getExtent().width - padding * 2;
    double screenHeight = window->getExtent().height - padding * 2;

    // Transform min and max
    glm::vec2 screenMax = {(max.x/xAxis)*screenWidth, (max.y/yAxis)*screenHeight};
    glm::vec2 screenMin = {(min.x/xAxis)*screenWidth, (min.y/yAxis)*screenHeight};

    // Add plot transformed to screen coordinates to GUI
    for(int i=0;i<x.size()-1;i++) {
      // Get raw coordinates
      glm::vec2 start = {x[i], y[i]};
      glm::vec2 end = {x[i+1], y[i+1]};


      // Transform
      start.x = (start.x / xAxis) * screenWidth + padding + screenMin.x;
      end.x = (end.x / xAxis) * screenWidth + padding + screenMin.x;

      start.y = screenMin.y + (window->getExtent().height - padding) - (start.y / yAxis) * screenHeight;
      end.y = screenMin.y + (window->getExtent().height - padding) - (end.y / yAxis) * screenHeight;

      lineRenderer->addLineSegment(
        start,
        end,
        {255*(float)rand()/RAND_MAX,255*(float)rand()/RAND_MAX,255*(float)rand()/RAND_MAX},
        1
      );
    }

    // Add axis
    lineRenderer->addLineSegment(
      {padding, window->getExtent().height - padding},
      {window->getExtent().width - padding, window->getExtent().height - padding},
      axisColor,
      1
    );

    lineRenderer->addLineSegment(
      {padding, padding},
      {padding, window->getExtent().height - padding},
      axisColor,
      1
    );
  });

  window->addRenderable(lineRenderer);
  window->addRenderable(fontRenderer);
  return Value::nil();
}

Library createPlotLibrary() {
  Library library;
  library.registerFunction("plot", &plot);
  return library;
}

}
}