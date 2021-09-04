#pragma once

#include "asset-manager.h"
#include "shader.h"
#include "texture.h"

namespace Near::Assets{

AssetManager<VertexShader>* vertexShaders();
AssetManager<PixelShader>* pixelShaders();
AssetManager<Texture>* textures();

}
