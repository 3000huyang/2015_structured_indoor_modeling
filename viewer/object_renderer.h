#ifndef OBJECT_RENDERER_H_
#define OBJECT_RENDERER_H_

#include <Eigen/Dense>
#include <QGLFunctions>
#include <fstream>
#include <vector>
#include <string>

typedef std::pair<Eigen::Vector3d, Eigen::Vector3d> ColoredPoint;
typedef std::vector<ColoredPoint> ColoredPointCloud;

class ObjectRenderer : protected QGLFunctions {
 public:
  ObjectRenderer();
  virtual ~ObjectRenderer();

  void Init(const std::string data_directory);

  void RenderAll(const double alpha);
  void RenderRoom(const int room);
  void RenderObject(const int room, const int object);

 private:
  // For each room, for each object, a colored point cloud.
  std::vector<std::vector<ColoredPointCloud> > colored_point_clouds;
};

#endif  // OBJECT_RENDERER_H_