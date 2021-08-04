#pragma once

#include <chrono>

namespace Near{

class FPSCounter{
public:
  static constexpr double FPS_UPDATE_INTERVAL = 500;
  FPSCounter();
  ~FPSCounter();
  void frame();
  double getLastFrameTime();
  double getFPS() const;
private:
  using clock = std::chrono::high_resolution_clock;
  clock::time_point timeLastFrame;
  double time = 0;
  double timeFPSUpdated = 0;
  double deltaTime = 0;
  double fps = 0;
  uint64_t totalFrames = 0;
  uint64_t framesSinceFPSUpdate = 0;
};

}
