#include "pch.h"
#include "shader.h"

#include "near.h"
#include "utils.h"

namespace Near{

void VertexShader::load(const std::string& path){
  renderer()->compileVertexShader(&shader, &layout, path.c_str());
}

VertexShader::~VertexShader(){
  safeRelease(layout);
  safeRelease(shader);
}

void PixelShader::load(const std::string& path){
  renderer()->compilePixelShader(&shader, path.c_str());
}

PixelShader::~PixelShader(){
  safeRelease(shader);
}

}
