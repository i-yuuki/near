#pragma once

#include "asset.h"

namespace Near{

class Texture : public Asset{
public:
  ~Texture();
  void load(const std::string& path) override;
  ID3D11ShaderResourceView* texture = nullptr;
};

}
