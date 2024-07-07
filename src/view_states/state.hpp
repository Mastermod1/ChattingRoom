#pragma once

#include <memory>

class Context;

class State
{
  public:
    virtual void render() = 0;
    virtual void setContext(const std::shared_ptr<Context>& ctx) = 0;
    virtual ~State() = default;
};
