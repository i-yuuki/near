#include "pch.h"
#include "font.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "utils.h"

namespace Near::Font{

Font::~Font(){
}

static void readKeyValuePair(std::istream& s, std::string& key, std::string& value){
  while(std::isspace(s.peek())){
    s.ignore();
  }
  std::getline(s, key, u8'=');
  s >> std::quoted(value);
}

void Font::load(const std::string& path){
  std::ifstream s(path);
  while(s){
    std::string line, type, key, value;
    std::getline(s, line);
    std::istringstream ss(line);
    ss >> type;
    if(type == u8"info"){
      while(ss){
        readKeyValuePair(ss, key, value);
        if(key == u8"size"){
          fontSize = parseInt<int>(value);
        }
      }
    }else if(type == u8"common"){
      while(ss){
        readKeyValuePair(ss, key, value);
        if(key == u8"lineHeight"){
          lineHeight = parseInt<int>(value);
        }else if(key == u8"base"){
          baseline = parseInt<int>(value);
        }else if(key == u8"pages"){
          textures.resize(parseInt<size_t>(value));
        }
      }
    }else if(type == u8"page"){
      size_t page = 0;
      std::string file;
      while(ss){
        readKeyValuePair(ss, key, value);
        if(key == u8"id"){
          page = parseInt<size_t>(value);
        }else if(key == u8"file"){
          file = value;
        }
      }
      textures[page].load((std::filesystem::path(path).parent_path() / file).string());
    }else if(type == u8"chars"){
      // 特に何もしなくていい
    }else if(type == u8"char"){
      FontChar ch = {};
      while(ss){
        readKeyValuePair(ss, key, value);
        if(key == u8"id"){
          ch.codepoint = parseInt<uint32_t>(value);
        }else if(key == u8"x"){
          ch.x = parseInt<uint16_t>(value);
        }else if(key == u8"y"){
          ch.y = parseInt<uint16_t>(value);
        }else if(key == u8"width"){
          ch.width = parseInt<uint16_t>(value);
        }else if(key == u8"height"){
          ch.height = parseInt<uint16_t>(value);
        }else if(key == u8"xoffset"){
          ch.xOffset = parseInt<int16_t>(value);
        }else if(key == u8"yoffset"){
          ch.yOffset = parseInt<int16_t>(value);
        }else if(key == u8"xadvance"){
          ch.xAdvance = parseInt<int16_t>(value);
        }else if(key == u8"page"){
          ch.page = parseInt<uint8_t>(value);
        }else if(key == u8"chnl"){
          ch.channel = parseInt<uint8_t>(value);
        }
      }
      characters.emplace(ch.codepoint, ch);
    }else if(type == u8"kernings"){
      // 特に何もしなくていい
    }else if(type == u8"kerning"){
      // いつか対応するかも
    }
  }
}

}
