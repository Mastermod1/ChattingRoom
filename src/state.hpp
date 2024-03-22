#pragma once

#include <memory>

class Context;

class State
{
  public:
    virtual void render() const = 0;
    virtual void setContext(const std::shared_ptr<Context>& ctx) = 0;
    virtual ~State() = default;
};
