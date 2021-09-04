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

  for(int i = 0;i < VERTEX_BUFFER_RECTS * 4;i ++){
    vertices[i] = {Math::Vector3::Zero, Math::Vector3::Forward, Near::Math::Color(), Near::Math::Vector2::Zero};
  }
  vertexBuffer.init(true, VERTEX_BUFFER_RECTS * 4, vertices);

  auto indexCount = VERTEX_BUFFER_RECTS * 6;
  uint32_t* indices = new uint32_t[indexCount];
  for(int i = 0, ii = 0, vi = 0;i < VERTEX_BUFFER_RECTS;i ++){
    indices[ii ++] = vi;
    indices[ii ++] = vi + 1;
    indices[ii ++] = vi + 2;
    indices[ii ++] = vi + 2;
    indices[ii ++] = vi + 1;
    indices[ii ++] = vi + 3;
    vi += 4;
  }
  indexBuffer.init(false, indexCount, indices);
  delete[] indices;

  pixelShader = Assets::pixelShaders()->getOrLoad("assets/nearlib/shaders/ps.hlsl");
}

void Font::drawText(const std::string_view text, const Math::Vector2& position, const Math::Vector2& origin, float size, const Math::Color& color){
  rectIdx = 0;
  texture = nullptr;
  float scale = size / fontSize;
  auto* r = renderer();

  // 行数と各行の幅を求める
  std::vector<LineMetrics> lines;
  calcLineWidths(text, lines);

  r->pushWorldTransform();
  r->applyWorldTransform(Math::Matrix::CreateScale(size / fontSize) * Math::Matrix::CreateTranslation(position.x, position.y, 0));
  Math::Vector2 drawPos(0, lines.size() * lineHeight * -origin.y);
  for(auto& line : lines){
    auto it = line.text.data();
    auto itEnd = it + line.text.size();
    drawPos.x = line.width * -origin.x;
    while(it != itEnd){
      auto codepoint = utf8::next(it, itEnd);
      if(auto* charInfo = findChar(codepoint)){
        addChar(charInfo, drawPos, color);
        drawPos.x += charInfo->xAdvance;
      }
    }
    drawPos.y += lineHeight;
  }
  flush();
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

void Font::addChar(const FontChar* ch, const Math::Vector2& pos, const Math::Color& color){
  auto* texture = textures[ch->page].get();
  if(texture != this->texture || rectIdx >= VERTEX_BUFFER_RECTS){
    flush();
    this->texture = texture;
  }
  int vertexIdx = rectIdx * 4;
  float x = pos.x + ch->xOffset;
  float y = pos.y + ch->yOffset;
  float uvX = ch->x / static_cast<float>(textureWidth);
  float uvY = ch->y / static_cast<float>(textureHeight);
  float uvW = ch->width  / static_cast<float>(textureWidth);
  float uvH = ch->height / static_cast<float>(textureHeight);
  vertices[vertexIdx].position = Math::Vector3(x,             y + ch->height, 0);
  vertices[vertexIdx].uv       = Math::Vector2(uvX,       uvY + uvH);
  vertices[vertexIdx ++].color = color;
  vertices[vertexIdx].position = Math::Vector3(x + ch->width, y + ch->height, 0);
  vertices[vertexIdx].uv       = Math::Vector2(uvX + uvW, uvY + uvH);
  vertices[vertexIdx ++].color = color;
  vertices[vertexIdx].position = Math::Vector3(x,             y,              0);
  vertices[vertexIdx].uv       = Math::Vector2(uvX,       uvY);
  vertices[vertexIdx ++].color = color;
  vertices[vertexIdx].position = Math::Vector3(x + ch->width, y,              0);
  vertices[vertexIdx].uv       = Math::Vector2(uvX + uvW, uvY);
  vertices[vertexIdx ++].color = color;
  rectIdx ++;
}

void Font::flush(){
  if(!texture){
    rectIdx = 0;
    return;
  }
  auto* r = renderer();
  r->setPixelShader(pixelShader.get());
  r->setTexture(texture);
  vertexBuffer.set(vertices);
  vertexBuffer.draw(0, rectIdx * 6, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexBuffer);
  rectIdx = 0;
}

}
