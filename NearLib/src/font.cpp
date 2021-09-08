#include "pch.h"
#include "font.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "utf8.h"
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
        }else if(key == u8"scaleW"){
          textureWidth = parseInt<int>(value);
        }else if(key == u8"scaleH"){
          textureHeight = parseInt<int>(value);
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
      textures[page] = Near::Assets::textures()->getOrLoad((std::filesystem::path(path).parent_path() / file).string());
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

  pixelShader = Assets::pixelShaders()->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void Font::drawText(const std::string_view text, const Math::Vector2& position, const Math::Vector2& origin, float size, const Math::Color& color){
  float scale = size / fontSize;
  float texW = static_cast<float>(textureWidth);
  float texH = static_cast<float>(textureHeight);
  auto* r = renderer();
  auto* r2d = renderer2D();

  // 行数と各行の幅を求める
  std::vector<LineMetrics> lines;
  calcLineWidths(text, lines);

  r->pushWorldTransform();
  r->applyWorldTransform(Math::Matrix::CreateScale(size / fontSize) * Math::Matrix::CreateTranslation(position.x, position.y, 0));
  r2d->begin();
  Math::Vector2 drawPos(0, lines.size() * lineHeight * -origin.y);
  for(auto& line : lines){
    auto it = line.text.data();
    auto itEnd = it + line.text.size();
    drawPos.x = line.width * -origin.x;
    while(it != itEnd){
      auto codepoint = utf8::next(it, itEnd);
      if(auto* charInfo = findChar(codepoint)){
        r2d->setTexture(textures[charInfo->page].get());
        r2d->fillRectUV(
          drawPos + Near::Math::Vector2(charInfo->xOffset, charInfo->yOffset),
          Near::Math::Vector2(charInfo->width, charInfo->height),
          Near::Math::Vector2(charInfo->x / texW, charInfo->y / texH),
          Near::Math::Vector2(charInfo->width / texW, charInfo->height / texH),
          Near::Math::Vector2::Zero,
          color
        );
        drawPos.x += charInfo->xAdvance;
      }
    }
    drawPos.y += lineHeight;
  }
  r2d->end();
  r->popWorldTransform();
}

FontChar* Font::findChar(uint32_t codepoint){
  auto info = characters.find(codepoint);
  return info == characters.end() ? nullptr : &info->second;
}

void Font::calcLineWidths(const std::string_view text, std::vector<LineMetrics>& out){
  uint32_t x = 0;
  uint32_t width = 0;
  auto it = text.data();
  auto itLineStart = it;
  auto itEnd = it + text.size();
  while(true){
    auto codepoint = utf8::next(it, itEnd);
    if(it == itEnd){
      out.push_back({width, std::string_view(itLineStart, it - itLineStart)});
      break;
    }
    if(codepoint == u8'\n'){
      out.push_back({width, std::string_view(itLineStart, it - itLineStart)});
      itLineStart = it;
      width = 0;
      x = 0;
      continue;
    }
    if(auto* charInfo = findChar(codepoint)){
      x += charInfo->xAdvance;
      width = std::max(x, width);
    }
  }
}

}
