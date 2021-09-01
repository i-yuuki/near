#pragma once

#include <string>
#include <vector>

#include "level.h"

class LevelManager{
public:
  LevelManager();
  void init(const std::string& path);
  void uninit();
  int getLevelIdx();
  std::shared_ptr<Level> getLevel();
  int getLevelCount();
  bool hasNextLevel();
  void setNextLevel(int idx);
  void nextLevel();
private:
  std::vector<std::string> levels;
  int levelIdx = 0;
  int nextLevelIdx = 0;
  std::shared_ptr<Level> level;
};
