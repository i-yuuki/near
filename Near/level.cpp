#define NOMINMAX
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
      loadObject(is);
    }
  }
  blocks.shrink_to_fit();
}

const std::vector<LevelBlock>& Level::getBlocks() const{
  return blocks;
}

void Level::loadObject(std::istream& is){
  auto token = readToken(is);
  if(is.eof()) return;
  {
    auto brace = readToken(is);
    if(brace != u8"{") throw std::exception(("Map syntax error: expected \"{\" but read " + brace).c_str());
  }
  if(token == u8"block"){
    LevelBlock& block = blocks.emplace_back();
    while((token = readToken(is)) != u8"}"){
      if(token == u8"position"){
        block.position.x = readFloat(is);
        block.position.y = readFloat(is);
        block.position.z = readFloat(is);
      }else if(token == u8"size"){
        block.size.x = readFloat(is);
        block.size.y = readFloat(is);
        block.size.z = readFloat(is);
      }else if(token == u8"rotation"){
        block.rotation.x = readFloat(is);
        block.rotation.y = readFloat(is);
        block.rotation.z = readFloat(is);
      }else if(token == u8"faces"){
        for(int face = 0;face < 6;face ++){
          block.materials[face] = readInt(is);
        }
      }
    }
  }
}

std::string Level::readToken(std::istream& is){
  std::string word;
  while(true){
    if(!(is >> word)) return "";
    if(word == u8"#"){
      is.ignore(std::numeric_limits<std::streamsize>::max(), u8'\n');
      word.clear();
    }else{
      break;
    }
  }
  return word;
}

int Level::readInt(std::istream& is){
  auto word = readToken(is);
  int val;
  auto res = std::from_chars(word.c_str(), word.c_str() + word.size(), val, 10);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + word);
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + word);
  return val;
}

float Level::readFloat(std::istream& is){
  auto word = readToken(is);
  float val;
  auto res = std::from_chars(word.c_str(), word.c_str() + word.size(), val);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + word);
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + word);
  return val;
}
