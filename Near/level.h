#pragma once

#include <NearLib/asset.h>
#include <NearLib/buffer.h>

class LevelBlock{
public:
  Near::Math::Vector3 position;
  Near::Math::Vector3 size;
  Near::Math::Vector3 rotation;
  int materials[6];
};

class Level : public Near::Asset{
public:
  void load(const std::string& path) override;
  const std::vector<LevelBlock>& getBlocks() const;
private:
  std::vector<LevelBlock> blocks;
  void loadLine(const std::string& line);
  std::string readWord(std::istream& is);
  int readInt(std::istream& is);
  float readFloat(std::istream& is);
};
