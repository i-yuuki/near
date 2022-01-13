#include "pch.h"
#include "assets.h"

#include "utils.h"

namespace Near::Assets{

namespace{

AssetManager<VertexShader>* g_vertexShaders = nullptr;
AssetManager<PixelShader>* g_pixelShaders = nullptr;
AssetManager<Texture>* g_textures = nullptr;
NamedAssetManager<Font::Font>* g_fonts = nullptr;

}

void init(){
  g_vertexShaders = new AssetManager<VertexShader>();
  g_pixelShaders = new AssetManager<PixelShader>();
  g_textures = new AssetManager<Texture>();
  g_fonts = new NamedAssetManager<Font::Font>();
}

void uninit(){
  safeDelete(g_vertexShaders);
  safeDelete(g_pixelShaders);
  safeDelete(g_textures);
  safeDelete(g_fonts);
}

AssetManager<VertexShader>* vertexShaders(){
  return g_vertexShaders;
}

AssetManager<PixelShader>* pixelShaders(){
  return g_pixelShaders;
}

AssetManager<Texture>* textures(){
  return g_textures;
}

NamedAssetManager<Font::Font>* fonts(){
  return g_fonts;
}

}
