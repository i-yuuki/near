#include "pch.h"
#include "fps-counter.h"

namespace Near{

FPSCounter::FPSCounter(){
}

FPSCounter::~FPSCounter(){
}

double FPSCounter::getLastFrameTime(){
  return deltaTime;
}

double FPSCounter::getFPS() const{
  return fps;
}

void FPSCounter::frame(){
  clock::time_point timePoint = clock::now();
  deltaTime = totalFrames == 0 ? 0 : std::chrono::duration<double, std::milli>(timePoint - timeLastFrame).count();
  // OutputDebugStringW(std::to_wstring(deltaTime).append(L"\n").c_str());
  time += deltaTime;
  totalFrames ++;
  framesSinceFPSUpdate ++;
  timeLastFrame = timePoint;
  double timeSinceUpdate = time - timeFPSUpdated;
  if(timeSinceUpdate >= FPS_UPDATE_INTERVAL){
    fps = 1000 * framesSinceFPSUpdate / timeSinceUpdate;
    timeFPSUpdated = time;
    framesSinceFPSUpdate = 0;
  }
}

}
