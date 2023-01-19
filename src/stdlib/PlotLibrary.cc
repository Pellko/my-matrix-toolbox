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

  for(int i=0;i<a->getHeight() - 1;i++) {
    lineRenderer->addLineSegment(
      {a->get(0, i).as.number, b->get(0, i).as.number},
      {a->get(0, i+1).as.number, b->get(0, i+1).as.number}
    );
  }

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