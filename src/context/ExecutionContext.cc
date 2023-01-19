#include "ExecutionContext.hh"
#include <thread>

namespace mymatrixtoolbox {

ExecutionContext::ExecutionContext() {}
ExecutionContext::~ExecutionContext() {}

void ExecutionContext::launch(std::function<void()> executionMain) {
  Window::initGLFW();

  std::thread executionThread([&]() {
    executionMain();
    markExecutionFinished();
  });
  executionThread.detach();

  while(!(isExecutionFinished() && shouldClose())) {
    auto windowReq = windowRequests.pop();
    while(windowReq.has_value()) {
      windows.push_back(windowReq.value());
      windowReq->get()->init();
      windowReq = windowRequests.pop();
    }

    for(std::shared_ptr<Window>& window : windows) {
      uint32_t id = window->begin();
      if(id == -1) continue;
      window->drawRenderables();
      window->end(id);
    }
  }

  Window::terminateGLFW();
}

void ExecutionContext::markExecutionFinished() {
  std::lock_guard<std::mutex> lock(executionNotifierMutex);
  executionThreadFinished = true;
}

bool ExecutionContext::isExecutionFinished() {
  std::lock_guard<std::mutex> lock(executionNotifierMutex);
  return executionThreadFinished;
}

std::shared_ptr<Window> ExecutionContext::createWindow(std::string name) {
  std::shared_ptr<Window> window = std::make_shared<Window>(name);
  windowRequests.push(window);
  return window;
}

bool ExecutionContext::shouldClose() {
  for(int i=windows.size()-1;i>=0;i--) {
    if(windows[i]->shouldClose()) {
      windows[i]->terminate();
      windows.erase(windows.begin() + i);
    }
  }

  return windows.size() == 0;
}

}