//
//  BaseTask.hpp
//  cpp-concurrency
//
//  Created by Vladislav Voinov on 28.06.20.
//
#pragma once

namespace task
{

class AbstractTask
{
protected:
  bool _is_done;
public:
  AbstractTask(): _is_done(false) {};
  ~AbstractTask() {};
  AbstractTask(const AbstractTask&)              = delete;
  AbstractTask& operator = (const AbstractTask&) = delete;
  bool isDone() { return _is_done; };
  virtual AbstractTask& operator ()() = 0;
  virtual AbstractTask& execute()     = 0;
};

}
