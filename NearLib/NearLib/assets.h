#pragma once

#include "asset-manager.h"
#include "named-asset-manager.h"
#include "shader.h"
#include "texture.h"
#include "font.h"

namespace Near::Assets{

AssetManager<VertexShader>* vertexShaders();
AssetManager<PixelShader>* pixelShaders();
AssetManager<Texture>* textures();
NamedAssetManager<Font::Font>* fonts();

}
