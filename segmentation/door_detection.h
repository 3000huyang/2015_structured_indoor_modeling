#ifndef ROOM_SEGMENTATION_DOOR_DETECTION_H_
#define ROOM_SEGMENTATION_DOOR_DETECTION_H_

#include "../base/ply/points.h"
#include "../submodular/data.h"

namespace room_segmentation {
  void ConvertPointsToSweep(const ply::Points& points, floored::Sweep* sweep);

  void SetRanges(const std::vector<floored::Sweep>& sweeps,
                 const float average_distance,
                 floored::Frame* frame);

  void ComputeFrame(const std::string directory,
                    const std::vector<floored::Sweep>& sweeps,
                    const float average_distance,
                    floored::Frame* frame);

  float ComputeAverageDistance(const std::vector<floored::Sweep>& sweeps);

  void DetectDoors(const std::vector<floored::Sweep>& sweeps,
                   const floored::Frame& frame,
                   const std::string directory,
                   const std::vector<float>& point_evidence,
                   const std::vector<float>& free_space_evidence,
                   std::vector<float>* door_detection);

}  // namespace room_segmentation

#endif // ROOM_SEGMENTATION_DOOR_DETECTION_H_