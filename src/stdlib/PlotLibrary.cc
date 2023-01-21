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
  ObjectMatrix* a = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* b = static_cast<ObjectMatrix*>(args[1].as.object);

  std::shared_ptr<Window> window = context->createWindow("My Matrix Toolbox");
  std::shared_ptr<LineRenderer> lineRenderer = std::make_shared<LineRenderer>(window, 0);
  std::shared_ptr<FontRenderer> fontRenderer = std::make_shared<FontRenderer>(window, 0);

  window->addInitializer([=]() {
    int padding = 32;
    glm::vec3 axisColor = {150, 150, 150};

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

    for(int i=0;i<a->getHeight() - 1;i++) {
      // Get raw coordinates
      double xs = a->get(0, i).as.number;
      double ys = b->get(0, i).as.number;
      double xe = a->get(0, i+1).as.number;
      double ye = b->get(0, i+1).as.number;

      // Transform
      xs += padding;
      xe += padding;
      ys = (window->getExtent().height - padding) - ys;
      ye = (window->getExtent().height - padding) - ye;

      lineRenderer->addLineSegment(
        {xs, ys},
        {xe, ye},
        {255*(float)rand()/RAND_MAX,255*(float)rand()/RAND_MAX,255*(float)rand()/RAND_MAX},
        1
      );
    }
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