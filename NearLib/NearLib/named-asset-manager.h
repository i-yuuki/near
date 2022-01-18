#pragma once

#include <memory>
#include <unordered_map>

#include "asset.h"

namespace Near{

template<class T>
class NamedAssetManager{
public:
  NamedAssetManager();
  std::shared_ptr<T> get(const std::string& name);
  std::shared_ptr<T> add(const std::string& name, const std::string& path);
private:
  std::unordered_map<std::string, std::shared_ptr<T>> assets;
};

template<class T>
NamedAssetManager<T>::NamedAssetManager(){
}

template<class T>
std::shared_ptr<T> NamedAssetManager<T>::get(const std::string& name){
  auto it = assets.find(name);
  if(it == assets.end()) return nullptr;
  return it->second;
}

template<class T>
std::shared_ptr<T> NamedAssetManager<T>::add(const std::string& name, const std::string& path){
  auto asset = get(name);
  if(asset) return asset;

  asset = std::make_shared<T>();
  try{
    asset->load(path);
  }catch(const std::exception& ex){
    printf_s("%s failed to load asset! %s (%s)\n", typeid(*this).name(), ex.what(), path.c_str());
  }catch(...){
    printf_s("%s failed to load asset! Unknown reason (%s)\n", typeid(*this).name(), path.c_str());
  }
  assets.insert({name, asset});
  return asset;
}

}
