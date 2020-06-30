//
//  EventLoop.cpp
//  cpp-concurrency
//
//  Created by Vladislav Voinov on 28.06.20.
//
#include "cpp-concurrency-lib/EventLoop.hpp"

namespace eventloop
{

EventLoop::EventLoop():
  t1([=]() { worker(); }),
  t2([=]() { worker(); }),
  t3([=]() { worker(); }),
  is_working(true)
{};

EventLoop::~EventLoop()
{
  clear();
  is_working = false;
  t1.join();
  t2.join();
  t3.join();
};

void EventLoop::worker()
{
  while(is_working) {
    try {
      auto task = pop();
      if (task) {
        task->execute();
      }
    } catch(...) {
      // error here means no tasks or internal task error, ignore them
    }
  }
};

void EventLoop::push(const TaskPtr task_ptr)
{
  if (task_ptr == nullptr) {
      return;
  }
  const std::lock_guard<std::mutex> lock(queue_mutex);
  tasks.push(task_ptr);
  return task_ptr;
};

TaskPtr EventLoop::pop()
{
  std::lock_guard<std::mutex> lock(queue_mutex);
  if (tasks.empty()) {
    throw std::runtime_error("No tasks");
  }
  auto task_ptr = tasks.front();
  tasks.pop();
  return task_ptr;
};

void EventLoop::clear()
{
  std::lock_guard<std::mutex> lock(queue_mutex);
  std::queue<TaskPtr> empty;
  std::swap(tasks, empty);
};

}
