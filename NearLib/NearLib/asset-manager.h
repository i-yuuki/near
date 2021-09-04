#pragma once

#include <memory>
#include <unordered_map>

#include "asset.h"

namespace Near{

template<class T>
class AssetManager{
public:
  AssetManager();
  std::shared_ptr<T> get(const std::string& path);
  std::shared_ptr<T> getOrLoad(const std::string& path);
  void unloadUnused();
private:
  std::unordered_map<std::string, std::shared_ptr<T>> assets;
};

template<class T>
AssetManager<T>::AssetManager(){
}

template<class T>
std::shared_ptr<T> AssetManager<T>::get(const std::string& path){
  auto it = assets.find(path);
  if(it == assets.end()) return nullptr;
  return it->second;
}

template<class T>
std::shared_ptr<T> AssetManager<T>::getOrLoad(const std::string& path){
  auto asset = get(path);
  if(!asset){
    asset = std::make_shared<T>();
    try{
      asset->load(path);
    }catch(const std::exception& ex){
      printf_s("%s failed to load asset! %s (%s)\n", typeid(*this).name(), ex.what(), path.c_str());
      // spdlog::warn("{} failed to load asset! {} ({})", typeid(*this).name(), ex.what(), path);
    }catch(...){
      // spdlog::warn("{} failed to load asset! ({})", typeid(*this).name(), path);
    }
    assets.insert({path, asset});
  }
  return asset;
}

template<class T>
void AssetManager<T>::unloadUnused(){
  for(auto it = assets.begin();it != assets.end();){
    if(it->second.use_count() == 1){
      it = assets.erase(it);
    }else{
      it ++;
    }
  }
}

}
