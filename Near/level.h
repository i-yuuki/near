#pragma once

#include <NearLib/asset.h>
#include <NearLib/buffer.h>
#include <NearLib/scene.h>

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
  std::vector<LevelPortal> portals;
  void loadObject(std::istream& is);
  bool loadEntityToken(std::istream& is, const std::string& token, LevelEntity& ent);
  std::string readToken(std::istream& is);
  int readInt(std::istream& is);
  float readFloat(std::istream& is);
};
