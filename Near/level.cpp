#define NOMINMAX
#include "level.h"

#include <charconv>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <NearLib/vertex.h>

#include "portal.h"

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
  portals.shrink_to_fit();
}

const std::vector<LevelBlock>& Level::getBlocks() const{
  return blocks;
}

void Level::createGameObjects(Near::Scene& scene){
  Near::Layer& layerTransparent = *scene.getLayer(Near::Scene::LAYER_TRANSPARENT_OBJECTS);
  std::unordered_map<std::string, std::shared_ptr<Portal>> portalObjects;
  for(auto& portal : portals){
    auto obj = layerTransparent.createGameObject<Portal>(portal.size / 2);
    obj->transform.position = portal.position;
    obj->transform.rotation = Near::Math::Quaternion::CreateFromYawPitchRoll(portal.rotation.y, portal.rotation.x, portal.rotation.z);
    portalObjects.emplace(portal.name, obj);
  }
  for(auto& portal : portals){
    auto partner = portalObjects.find(portal.partner);
    if(partner != portalObjects.end()){
      portalObjects[portal.name]->otherPortal = partner->second;
    }
  }
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
  }else if(token == u8"portal"){
    LevelPortal& portal = portals.emplace_back();
    while((token = readToken(is)) != u8"}"){
      if(loadEntityToken(is, token, portal)) continue;
      if(token == u8"partner"){
        portal.partner = readToken(is);
      }else if(token == u8"size"){
        portal.size.x = readFloat(is);
        portal.size.y = readFloat(is);
      }
    }
  }else{
    printf_s("Skipping unknown map object %s\n", token.c_str());
    while((token = readToken(is)) != u8"}"){}
  }
}

bool Level::loadEntityToken(std::istream& is, const std::string& token, LevelEntity& ent){
  if(token == u8"position"){
    ent.position.x = readFloat(is);
    ent.position.y = readFloat(is);
    ent.position.z = readFloat(is);
  }else if(token == u8"rotation"){
    ent.rotation.x = readFloat(is);
    ent.rotation.y = readFloat(is);
    ent.rotation.z = readFloat(is);
  }else if(token == u8"name"){
    ent.name = readToken(is);
  }else{
    return false;
  }
  return true;
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
