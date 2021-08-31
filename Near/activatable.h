#pragma once

#include <memory>
#include <vector>

class Activatable{
public:
  bool isActive();
  void setActive(bool active);
protected:
  bool active;
  virtual void activate() = 0;
  virtual void deactivate() = 0;
};

class Activator{
public:
  void addTarget(std::weak_ptr<Activatable> target);
  void activateTargets();
  void deactivateTargets();
protected:
  std::vector<std::weak_ptr<Activatable>> targets;
};
