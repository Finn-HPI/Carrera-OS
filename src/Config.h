#pragma once

struct Config {
  int calibrationSpeed;

  int fullSpeed;
  int minSpeed;
  
  float drivenLength;

  float segmentThreshold;
  float curveThreshold;
  
  float diffThreshold;
  
  int numPreviews;
  int previewOffset;

  int slowLoopTime;
  int fastLoopTime;

  float maxCurve;

  int acceleration;
  int deceleration;

  int irl_time;
};

extern struct Config* config;
