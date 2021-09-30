#pragma once

#include "component.h"

namespace Near::GUI{

class ContainerBase : public Component{
public:
  virtual void layout();
  virtual void draw() override;
protected:
  std::vector<std::shared_ptr<Component>> children;
  virtual void sizeChanged() override;
  virtual void childAdded(std::shared_ptr<Component> child);
};

template<class This>
class TContainer : public ContainerBase, public std::enable_shared_from_this<This>{
public:
  std::shared_ptr<This> add(std::shared_ptr<Component> child){
    if(!child->getParent().expired()){
      // 既にどこかに追加されていたらなんかする？
    }
    children.push_back(child);
    childAdded(child);
    return this->shared_from_this();
  }
};

class Container : public TContainer<Container>{};

}
