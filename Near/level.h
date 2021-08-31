#pragma once

#include <NearLib/asset.h>
#include <NearLib/buffer.h>
#include <NearLib/scene.h>

#include "activatable.h"

class LevelBlock{
public:
  Near::Math::Vector3 position;
  Near::Math::Vector3 size;
  Near::Math::Vector3 rotation;
  int materials[6];
};

class LevelEntity{
public:
  std::string name;
  Near::Math::Vector3 position;
  Near::Math::Vector3 rotation;
};

class LevelActivatable : public LevelEntity{
public:
  std::vector<std::string> targets;
};

class LevelPortal : public LevelEntity{
public:
  std::string partner;
  Near::Math::Vector2 size;
};

class Level : public Near::Asset{
public:
  void load(const std::string& path) override;
  const Near::Math::Vector3& getSpawnPosition() const;
  const Near::Math::Vector3& getSpawnRotation() const;
  const std::vector<LevelBlock>& getBlocks() const;
  const std::vector<LevelPortal>& getPortals() const;
  void createGameObjects(Near::Scene& scene);
private:
  Near::Math::Vector3 spawnPosition;
  Near::Math::Vector3 spawnRotation;
  std::vector<LevelBlock> blocks;
  std::vector<LevelEntity> cubes;
  std::vector<LevelActivatable> floorButtons;
  std::vector<LevelEntity> doors;
  std::vector<LevelPortal> portals;
  std::unordered_map<std::string, std::shared_ptr<Near::GameObject>> objectsCreated;
  void loadObject(std::istream& is);
  bool loadEntityToken(std::istream& is, const std::string& token, LevelEntity& ent);
  std::string readToken(std::istream& is);
  int readInt(std::istream& is);
  float readFloat(std::istream& is);
  template<class Ent>
  void addEntityObject(std::shared_ptr<Near::GameObject> obj, const Ent& entity){
    obj->transform.position = entity.position;
    obj->transform.rotation = Near::createEularRotation(entity.rotation);
    if(!entity.name.empty()){
      objectsCreated.emplace(entity.name, obj);
    }
  }
  template<class Obj, class Ent>
  void createEntityObjects(const std::vector<Ent>& entities, Near::Layer& layer){
    for(auto& ent : entities){
      addEntityObject(layer.createGameObject<Obj>(), ent);
    }
  }
  template<class Obj>
  std::shared_ptr<Obj> findObjectByName(const std::string& key){
    auto it = objectsCreated.find(key);
    if(it == objectsCreated.end()) return nullptr;
    return std::dynamic_pointer_cast<Obj>(it->second);
  }
  template<class Ent>
  void connectObjects(std::vector<Ent>& entities){
    for(auto& ent : entities){
      auto obj = findObjectByName<Activator>(ent.name);
      for(auto& target : ent.targets){
        if(auto targetObj = findObjectByName<Activatable>(target)){
          obj->addTarget(targetObj);
        }
      }
    }
  }
};
