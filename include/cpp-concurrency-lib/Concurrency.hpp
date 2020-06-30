//
//  Concurrency.hpp
//  cpp-concurrency
//
//  Created by Vladislav Voinov on 25.06.20.
//
#pragma once
#include <functional>
#include <memory>
#include "cpp-concurrency-lib/AbstractTask.hpp"
#include "cpp-concurrency-lib/Task.hpp"
#include "cpp-concurrency-lib/EventLoop.hpp"

namespace concurrency
{

class Concurrency
{
private:
  eventloop::EventLoop eventLoop;
public:
  Concurrency();
  ~Concurrency();
  template<typename LambdaReturns>
  std::shared_ptr<task::Task<LambdaReturns>> createTask(const std::function<LambdaReturns()>& fn)
  {
    auto task_ptr = std::make_shared<task::Task<LambdaReturns>>(fn);
    eventLoop.push(std::static_pointer_cast<task::AbstractTask>(task_ptr));
    return task_ptr;
  }
};

}
