#pragma once

class IActivatable{
protected:
  virtual void activate() = 0;
  virtual void deactivate() = 0;
};

class Activatable : public IActivatable{
public:
  bool isActive();
  void setActive(bool active);
protected:
  bool active;
};
