#include "Config.h"

Config initialConfig {
  .calibrationSpeed = 65,

  .fullSpeed = 125,
  .minSpeed = 1,

  .drivenLength = 1.0,

  .segmentThreshold = 8,
  .curveThreshold = 1.0,

  .diffThreshold = 0.3,

  .numPreviews = 14,
  .previewOffset = 5,

  .slowLoopTime = 16666,
  .fastLoopTime = 6000,

  .maxCurve = 5, 

 .acceleration = 255 * 4,
 .deceleration = 255
};

// Config initialConfig {
//   .calibrationSpeed = 65,

//   .fullSpeed = 125,
//   .minSpeed = 65,

//   .drivenLength = 1.0,

//   .segmentThreshold = 8,
//   .curveThreshold = 1.0,

//   .diffThreshold = 0.3,

//   .numPreviews = 12,
//   .previewOffset = 5,

//   .slowLoopTime = 16666,
//   .fastLoopTime = 8000,

//   .maxCurve = 5, 

//  .acceleration = 255 * 4,
//  .deceleration = 255
// };

// Config initialConfig {
//   .calibrationSpeed = 65,

//   .fullSpeed = 130,
//   .minSpeed = 65,

//   .drivenLength = 1.0,

//   .segmentThreshold = 8,
//   .curveThreshold = 1.0,

//   .diffThreshold = 0.3,

//   .numPreviews = 25,

//   .slowLoopTime = 16666,
//   .fastLoopTime = 3000,

//   .maxCurve = 4,

//   .acceleration = 255 * 4,
//   .deceleration = 255
// };

Config* config = &initialConfig;
