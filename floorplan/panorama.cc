#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>

#include "panorama.h"

using namespace Eigen;
using namespace std;

Panorama::Panorama() {
}

bool Panorama::Init(const file_io::FileIO& file_io,
                    const int panorama) {
  rgb_image = cv::imread(file_io.GetPanoramaImage(panorama), 1);
  if (rgb_image.cols == 0 && rgb_image.rows == 0) {
    cerr << "Panorama image cannot be loaded: " << file_io.GetPanoramaImage(panorama) << endl;
    return false;
  }
  width  = rgb_image.cols;
  height = rgb_image.rows;

  InitDepthImage(file_io, panorama);
  InitCameraParameters(file_io, panorama);

  phi_per_pixel = phi_range / height;
  phi_per_depth_pixel = phi_range / depth_height;
  return true;
}

Eigen::Vector2d Panorama::Project(const Eigen::Vector3d& global) const {
  const Vector3d local = GlobalToLocal(global);

  // x coordinate.
  double theta = -atan2(local.y(), local.x());
  if (theta < 0.0)
    theta += 2 * M_PI;
  double theta_ratio = max(0.0, min(1.0, theta / (2 * M_PI)));
  if (theta_ratio == 1.0)
    theta_ratio = 0.0;

  Vector2d uv;
  uv[0] = theta_ratio * width;
  const double depth = sqrt(local.x() * local.x() +
                            local.y() * local.y());
  double phi = atan2(local.z(), depth);
  const double pixel_offset_from_center = phi / phi_per_pixel;
  uv[1] = height / 2.0 - pixel_offset_from_center;

  return uv;
}

Eigen::Vector3d Panorama::Unproject(const Eigen::Vector2d& pixel,
                                    const double distance) const {
  const double theta = -2.0 * M_PI * pixel[0] / width;
  const double phi   = (height / 2.0 - pixel[1]) * phi_per_pixel;

  Vector3d local;
  local[2] = distance * sin(phi);
  local[0] = distance * cos(phi) * cos(theta);
  local[1] = distance * cos(phi) * sin(theta);

  return LocalToGlobal(local);
}

Eigen::Vector3d Panorama::GlobalToLocal(const Eigen::Vector3d& global) const {
  const Vector4d global4(global[0], global[1], global[2], 1.0);
  const Vector4d local4 = global_to_local * global4;
  return Vector3d(local4[0], local4[1], local4[2]); 
}

Eigen::Vector3d Panorama::LocalToGlobal(const Eigen::Vector3d& local) const {
  const Vector4d local4(local[0], local[1], local[2], 1.0);
  const Vector4d global4 = local_to_global * local4;
  return Vector3d(global4[0], global4[1], local4[2]); 
}

Eigen::Vector2d Panorama::RGBToDepth(const Eigen::Vector2d& pixel) const {
  return Vector2d(pixel[0] * depth_width / width,
                  pixel[1] * depth_height / height);
}

Eigen::Vector2d Panorama::DepthToRGB(const Eigen::Vector2d& depth_pixel) const {
  return Vector2d(depth_pixel[0] * width / depth_width,
                  depth_pixel[1] * height / depth_height);
}

Eigen::Vector3f Panorama::GetRGB(const Eigen::Vector2d& pixel) const {
  if (!IsInsideRGB(pixel)) {
    cerr << "Pixel outside." << endl;
    exit (1);
  }
  
  // Bilinear interpolation.
  const int u0 = static_cast<int>(floor(pixel[0]));
  const int v0 = static_cast<int>(floor(pixel[1]));
  const int u1 = u0 + 1;
  const int v1 = v0 + 1;
  
  const double weight00 = (u1 - pixel[0]) * (v1 - pixel[1]);
  const double weight01 = (pixel[0] - u0) * (v1 - pixel[1]);
  const double weight10 = (u1 - pixel[0]) * (pixel[1] - v0);
  const double weight11 = (pixel[0] - u0) * (pixel[1] - v0);
  const int u1_corrected = (u1 % width);
      
  const cv::Vec3b& color00 = rgb_image.at<cv::Vec3b>(v0, u0);
  const cv::Vec3b& color01 = rgb_image.at<cv::Vec3b>(v0, u1_corrected);
  const cv::Vec3b& color10 = rgb_image.at<cv::Vec3b>(v1, u0);
  const cv::Vec3b& color11 = rgb_image.at<cv::Vec3b>(v1, u1_corrected);
      
  return Vector3f((weight00 * color00[0] + weight01 * color01[0] +
                   weight10 * color10[0] + weight11 * color11[0]),
                  (weight00 * color00[1] + weight01 * color01[1] +
                   weight10 * color10[1] + weight11 * color11[1]),
                  (weight00 * color00[2] + weight01 * color01[2] +
                   weight10 * color10[2] + weight11 * color11[2]));
}

double Panorama::GetDepth(const Eigen::Vector2d& depth_pixel) const {
  if (!IsInsideDepth(depth_pixel)) {
    cerr << "Pixel outside." << endl;
    exit (1);
  }
  
  // Bilinear interpolation.
  const int u0 = static_cast<int>(floor(depth_pixel[0]));
  const int v0 = static_cast<int>(floor(depth_pixel[1]));
  const int u1 = u0 + 1;
  const int v1 = v0 + 1;
  
  const double weight00 = (u1 - depth_pixel[0]) * (v1 - depth_pixel[1]);
  const double weight01 = (depth_pixel[0] - u0) * (v1 - depth_pixel[1]);
  const double weight10 = (u1 - depth_pixel[0]) * (depth_pixel[1] - v0);
  const double weight11 = (depth_pixel[0] - u0) * (depth_pixel[1] - v0);
  const int u1_corrected = (u1 % depth_width);
      
  return
    weight00 * depth_image[v0 * depth_width + u0] +
    weight01 * depth_image[v0 * depth_width + u1_corrected] +
    weight10 * depth_image[v1 * depth_width + u0] +
    weight11 * depth_image[v1 * depth_width + u1_corrected];
}

bool Panorama::IsInsideRGB(const Eigen::Vector2d& pixel) const {
  if (pixel[0] < 0.0 || width <= pixel[0] ||
      pixel[1] < 0.0 || height - 1 <= pixel[1]) {
    return false;
  } else {
    return true;
  }
}

bool Panorama::IsInsideDepth(const Eigen::Vector2d& depth_pixel) const {
  if (depth_pixel[0] < 0.0 || depth_width <= depth_pixel[0] ||
      depth_pixel[1] < 0.0 || depth_height - 1 <= depth_pixel[1]) {
    return false;
  } else {
    return true;
  }
}

void Panorama::ResizeRGB(const Eigen::Vector2i& size) {
  cv::Mat resized_rgb_image;
  cv::resize(rgb_image, resized_rgb_image, cv::Size(size[0], size[1]));
  rgb_image = resized_rgb_image;

  width  = size[0];
  height = size[1];

  phi_per_pixel = phi_range / height;
}

void Panorama::InitDepthImage(const file_io::FileIO& file_io,
                              const int panorama) {
  ifstream ifstr;
  ifstr.open(file_io.GetDepthPanorama(panorama));

  string header;
  double min_depth, max_depth;
  ifstr >> header >> depth_width >> depth_height >> min_depth >> max_depth;
    
  depth_image.resize(depth_width * depth_height);
  
  int index = 0;
  average_distance = 0.0;
  for (int y = 0; y < depth_height; ++y) {
    for (int x = 0; x < depth_width; ++x, ++index) {
      ifstr >> depth_image[index];
      average_distance += depth_image[index];
    }
  }
  ifstr.close();

  average_distance /= depth_width * depth_height;
}
  
void Panorama::InitCameraParameters(const file_io::FileIO& file_io,
                                    const int panorama) {
  const string buffer = file_io.GetPanoramaToGlobalTransformation(panorama);

  ifstream ifstr;
  ifstr.open(buffer.c_str());
  string stmp;
  ifstr >> stmp;
  for (int y = 0; y < 4; ++y)
    for (int x = 0; x < 4; ++x)
      local_to_global(y, x) = 0;
  
  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x)
      ifstr >> local_to_global(y, x);
    center(y) = local_to_global(y, 3);
  }

  const Matrix3d rotation = local_to_global.block(0, 0, 3, 3);
  global_to_local.block(0, 0, 3, 3) = rotation.transpose();
  global_to_local.block(0, 3, 3, 1) =
    - rotation.transpose() * local_to_global.block(0, 3, 3, 1);
  global_to_local(3, 0) = 0.0;
  global_to_local(3, 1) = 0.0;
  global_to_local(3, 2) = 0.0;
  global_to_local(3, 3) = 1.0;
    
  ifstr >> phi_range;
  
  ifstr.close();
}  