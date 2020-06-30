//
//  EventLoop.hpp
//  cpp-concurrency
//
//  Created by Vladislav Voinov on 28.06.20.
//
#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include "cpp-concurrency-lib/AbstractTask.hpp"

namespace eventloop
{

using TaskPtr = std::shared_ptr<task::AbstractTask>;

class EventLoop
{
private:
  std::thread t1;
  std::thread t2;
  std::thread t3;
  std::queue<TaskPtr> tasks;
  std::mutex queue_mutex;
  bool is_working;
  void worker();
  void clear();
public:
  EventLoop();
  ~EventLoop();
  void    push(const TaskPtr task_ptr);
  TaskPtr pop();
};

}
