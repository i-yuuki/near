#include "pch.h"
#include "level-manager.h"

#include <fstream>

LevelManager::LevelManager(){
}

void LevelManager::init(const std::string& path){
  levels.clear();
  std::ifstream is(path);
  std::string line;
  while(std::getline(is, line)){
    levels.push_back(line);
  }
}

void LevelManager::uninit(){
  levels.clear();
  level.reset();
}

int LevelManager::getLevelIdx(){
  return levelIdx;
}

std::shared_ptr<Level> LevelManager::getLevel(){
  return level;
}

int LevelManager::getLevelCount(){
  return static_cast<int>(levels.size());
}

bool LevelManager::hasNextLevel(){
  return nextLevelIdx < levels.size();
}

void LevelManager::setNextLevel(int idx){
  if(idx < 0 || idx >= levels.size()) throw std::out_of_range("Level index out of range");
  nextLevelIdx = idx;
}

void LevelManager::nextLevel(){
  if(!hasNextLevel()) return;
  levelIdx = nextLevelIdx ++;
  level.reset(new Level());
  level->load(levels[levelIdx]);
}
