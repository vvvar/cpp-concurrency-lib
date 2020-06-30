//
//  Task.hpp
//  cpp-concurrency
//
//  Created by Vladislav Voinov on 28.06.20.
//
#pragma once
#include <future>
#include <functional>
#include "cpp-concurrency-lib/AbstractTask.hpp"

namespace task
{

template<typename TaskType>
class Task: public AbstractTask
{
using PackagedTask = std::packaged_task<TaskType()>;
using Lambda       = std::function<TaskType()>;
private:
  PackagedTask _packed_task;
public:
  Task(const Lambda& fn): AbstractTask(), _packed_task(fn) {}
  ~Task() {}
  Task& execute() override
  {
    _packed_task();
    _is_done = true;
    return *this;
  }
  Task& operator ()() override {
    return execute();
  };
  std::future<TaskType> getFuture()
  {
    return _packed_task.get_future();
  }
};

}
