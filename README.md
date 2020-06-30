# cpp-concurrency-lib
> Simple concurrency implementation for C++.

This library provides you with Event Loop that can execute lambda functions in FIFO manner.

## Installation

Add to your project using Cmake:

```sh
add_subdirectory(cpp-concurrency-lib)
```

## Usage example

Let's imagine we have this simple function we would like to execute asynchronously:
```cpp
/**
 * Sleeps for 2 seconds,
 * and then prints current thread ID
 */
void TestFn()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));auto thread_id = std::this_thread::get_id();std::stringstream ss;
    ss << thread_id;
    std::string mystring = ss.str();
    std::cout << mystring + " - Executed.\n";
}
```

First, include root `CppConcurrencyLib.hpp` header file to your project:
```cpp
#include <cpp-concurrency-lib/CppConcurrencyLib.hpp>
```

Create instance of `Concurrency` class using `CreateConcurrency` builder function from `cppconcurrencylib` namespace:
```cpp
const auto concurrency = cppconcurrencylib::CreateConcurrency();
```

From now on, we can schedule execution of our functions using `createTask` method:
```cpp
auto task = concurrency->createTask<void>(TestFn);
```
here, we're providing it with our `TestFn` and getting shared pointer(`std::shared_ptr` is used) to instance of `Task<void>` which wraps `std::future` so we can use it to wait for task execution to be completed:
```cpp
task->getFuture().get(); // wait until task is completed
```
Please note that task will be added to queue immediatley after invocation of `createTask` and will be executed in separate thread. You **should be cearful** with functions you're scheduling.

We can create whenever instances of `Concurrency` we want, eash instance will create thread pool and it's own EventLoop so they can work separatley:
```cpp
// create two separate event loops
const auto concurrency_1 = cppconcurrencylib::CreateConcurrency();
const auto concurrency_2 = cppconcurrencylib::CreateConcurrency();

// schedule task for both loops
auto task_1 = concurrency_1->createTask<void>(TestFn);
auto task_2 = concurrency_2->createTask<void>(TestFn);

// wait for task completion
task_1->getFuture().get();
task_2->getFuture().get();
```

Also, we can schedule as many task as we want, Event Loop will execute them:
```cpp
const auto concurrency_1 = cppconcurrencylib::CreateConcurrency();
auto task_1 = concurrency_1->createTask<void>(TestFn);
auto task_2 = concurrency_1->createTask<void>(TestFn);
  
const auto concurrency_2 = cppconcurrencylib::CreateConcurrency();
auto task_3 = concurrency_2->createTask<void>(TestFn);
auto task_4 = concurrency_2->createTask<void>(TestFn);

task_1->getFuture().get();
task_2->getFuture().get();
task_3->getFuture().get();
task_4->getFuture().get();
```

As far as `Task<void>` wraps `std::future`, we can use it in same manner to get result returned from function:
```cpp
auto lambda_int_fn = []() -> int {
  return 1234;
};
const auto concurrency = cppconcurrencylib::CreateConcurrency();
auto task = concurrency->createTask<int>(lambda_int_fn);
// use std::future interface to get result
int result = task->getFuture().get();
std::cout << "Result of function: " << result << std::endl;
```
and also to catch exceptions:
```cpp
auto throws_error = []() -> void {
  throw std::runtime_error("Dummy error");
};
const auto concurrency = cppconcurrencylib::CreateConcurrency();
auto task = concurrency->createTask<void>(throws_error);
try {
    task->getFuture().get();
} catch (std::runtime_error& err) {
    std::cerr << "Task incompleted, error: " << err.what() <<  std::endl;
}
```

Complete example:
```cpp
#include <cpp-concurrency-lib/CppConcurrencyLib.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

/**
 * Sleeps for 2 seconds,
 * and then prints current thread ID
 */
auto lambda_void_fn = []() -> void {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto thread_id = std::this_thread::get_id();
    std::stringstream ss;
    ss << thread_id;
    std::string mystring = ss.str();
    std::cout << mystring + " - Task function called." + "\n";
};

int main()
{
    // Create two event loops
    const auto concurrency_1 = cppconcurrencylib::CreateConcurrency();const auto concurrency_2 = cppconcurrencylib::CreateConcurrency();
    
    // schedule tasks for the Event Loop 1
    auto task_1 = concurrency_1->createTask<void>(lambda_void_fn);
    auto task_2 = concurrency_1->createTask<void>(lambda_void_fn);
    
    // schedule tasks for the Event Loop 2
    auto task_3 = concurrency_2->createTask<void>(lambda_void_fn);
    auto task_4 = concurrency_2->createTask<void>(lambda_void_fn);
    
    // Wait for them
    try 
    {
        task_1->getFuture().get();
        task_2->getFuture().get();
        task_3->getFuture().get();
        task_4->getFuture().get();
        std::cout << "All tasks completed!" <<  std::endl;
    } 
    catch (std::runtime_error& err)
    {
        std::cerr << "Tasks error: " << err.what() <<  std::endl;
    }

    return 0;
}
```