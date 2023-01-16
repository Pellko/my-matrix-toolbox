#ifndef _MY_MATRIX_TOOLBOX_CONTEXT_EXECUTION_CONTEXT_H_
#define _MY_MATRIX_TOOLBOX_CONTEXT_EXECUTION_CONTEXT_H_

#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <mutex>
#include "src/graphics/Window.hh"
#include "src/types/SafeQueue.hh"

namespace mymatrixtoolbox {

class ExecutionContext {
 public:
  ExecutionContext();
  ~ExecutionContext();

  void launch(std::function<void()> executionMain);
  std::shared_ptr<Window> createWindow(std::string name);
  
 private:
  std::mutex executionNotifierMutex;
  bool executionThreadFinished = false;
  void markExecutionFinished();
  bool isExecutionFinished();

  SafeQueue<std::shared_ptr<Window>> windowRequests;
  std::vector<std::shared_ptr<Window>> windows; 

  bool shouldClose();
};

}

#endif