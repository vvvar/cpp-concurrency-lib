#pragma once
#include "cpp-concurrency-lib/Concurrency.hpp"
#include <memory>

namespace cppconcurrencylib
{

std::shared_ptr<concurrency::Concurrency> CreateConcurrency()
{
  return std::make_shared<concurrency::Concurrency>();
}

}
