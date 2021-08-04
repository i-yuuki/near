#include "level.h"

#include <charconv>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <NearLib/vertex.h>

void Level::load(const std::string& path){
  blocks.clear();
  {
    std::ifstream is(path);
    if(!is) throw std::exception(("File open failed: " + path).c_str());
    std::string line, word;
    std::getline(is, line);
    if(line != "Near Map v1") throw std::exception(("Unsupported format: " + line).c_str());
    while(is){
      std::getline(is, line);
      loadLine(line);
    }
  }
  blocks.shrink_to_fit();
}

const std::vector<LevelBlock>& Level::getBlocks() const{
  return blocks;
}

void Level::loadLine(const std::string& line){
  std::istringstream ss(line);
  auto type = readWord(ss);
  if(type == "#") return;
  if(type == "b"){
    LevelBlock& block = blocks.emplace_back();
    block.position.x = readFloat(ss);
    block.position.y = readFloat(ss);
    block.position.z = readFloat(ss);
    block.size.x = readFloat(ss);
    block.size.y = readFloat(ss);
    block.size.z = readFloat(ss);
    block.rotation.x = readFloat(ss);
    block.rotation.y = readFloat(ss);
    block.rotation.z = readFloat(ss);
    for(int face = 0;face < 6;face ++){
      block.materials[face] = readInt(ss);
    }
  }
}

std::string Level::readWord(std::istream& is){
  std::string word;
  while(is && word.empty()){
    std::getline(is, word, ' ');
  }
  return word;
}

int Level::readInt(std::istream& is){
  auto word = readWord(is);
  int val;
  auto res = std::from_chars(word.c_str(), word.c_str() + word.size(), val, 10);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + word);
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + word);
  return val;
}

float Level::readFloat(std::istream& is){
  auto word = readWord(is);
  float val;
  auto res = std::from_chars(word.c_str(), word.c_str() + word.size(), val);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + word);
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + word);
  return val;
}
