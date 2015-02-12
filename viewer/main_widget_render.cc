#include <iostream>

#include <QOpenGLShaderProgram>
#include "../base/floorplan.h"
#include "object_renderer.h"
#include "floorplan_renderer.h"
#include "panel_renderer.h"
#include "panorama_renderer.h"
#include "polygon_renderer.h"
#include "main_widget.h"
#include "navigation.h"
#include <QGLFunctions>

#ifdef __linux__
#include <GL/glu.h>
#elif _WIN32
#include <windows.h>
#include <GL/glu.h>
//#ifndef __glew_h__
//#include <GL/glew.h>
//#include <GL/glext.h>
//#endif
#else
#include <OpenGL/glu.h>
#endif

using namespace Eigen;
using namespace std;

namespace structured_indoor_modeling {
  
void MainWidget::RenderFloorplan(const double alpha, const bool emphasize) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);

  // glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
  // glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ZERO);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  floorplan_renderer.Render(alpha, viewport, modelview, projection, emphasize);

  glDisable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  glPopAttrib();
}

void MainWidget::RenderPanorama(const double alpha) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
   
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);

  switch (navigation.GetCameraStatus()) {
  case kPanorama: {
    panorama_renderers[navigation.GetCameraPanorama().start_index].Render(alpha);
    break;
  }
  case kPanoramaToAirTransition:
  case kAirToPanoramaTransition:
  case kPanoramaToFloorplanTransition:
  case kFloorplanToPanoramaTransition: {
    panorama_renderers[navigation.GetCameraInTransition().camera_panorama.start_index].Render(alpha);
    break;
  }
  default: {
  }
  }

  glDisable(GL_TEXTURE_2D);
  glPopAttrib();
}  

void MainWidget::RenderPanoramaTour() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  int index_pair[2];
  int panorama_index_pair[2];
  double weight_pair[2];
  navigation.GetCameraPanoramaTour().GetIndexWeightPairs(1.0 - navigation.ProgressInverse(),
                                                         index_pair,
                                                         panorama_index_pair,
                                                         weight_pair);
  RenderPanoramaTransition(panorama_index_pair[0],
                           panorama_index_pair[1],
                           weight_pair[0]);

  glPopAttrib();
}

void MainWidget::RenderPanoramaTransition(const int start_index,
                                          const int end_index,
                                          const double start_weight) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Render the source pano.
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  panorama_renderers[start_index].Render(1.0);

  // Render the target pano.
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[1]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  panorama_renderers[end_index].Render(1.0);

  // Blend the two.
  // const double weight_end = 1.0 - weight_start;
  const int kDivideByAlpha = 1;
  BlendFrames(start_weight, kDivideByAlpha);

  glPopAttrib();
}

// divide_by_alpha_mode
// 0: Do not divide by alpha.
// 1: Divide by alpha.
// 2: Divide by alpha and overwrite the first.
// 3: Divide by alpha and overwrite the second.
void MainWidget::BlendFrames(const double weight, const int divide_by_alpha_mode) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_TEXTURE_2D);
    
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, width(), 0, height());
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //////////////////////////////////////////////////////////////////////
  if (!program.bind()) {
    cerr << "Cannot bind." << endl;
    exit (1);
  }

  program.setUniformValue("weight", static_cast<float>(weight));
  program.setUniformValue("divide_by_alpha", divide_by_alpha_mode);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texids[0]);
  glEnable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texids[1]);
  glEnable(GL_TEXTURE_2D);

  glDisable(GL_DEPTH_TEST);
  
  program.setUniformValue("tex0", 0);
  program.setUniformValue("tex1", 1);

  glBegin(GL_QUADS);
  glDisable(GL_DEPTH_TEST);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width(), 0.0f, 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width(), height(), 0.0f);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0, height(), 0.0f);
  glEnd();
  
  program.release();

  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();  
}

void MainWidget::RenderObjects(const double alpha) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  object_renderer.RenderAll(alpha);

  glPopAttrib();

  // debug
  /*
  if (object_renderer.render) {
    const int width2 = width();
    const int height2 = height();
    vector<Vector3d> screen(width2 * height2, Vector3d(0, 0, 0));
    vector<double> weights(width2 * height2, 0.0);

    const int kRadius = 3;
    
    for (int i = 0; i < (int)object_renderer.vertices.size(); i+=3) {
      GLdouble u, v, w;
      gluProject(object_renderer.vertices[i],
		 object_renderer.vertices[i + 1],
		 object_renderer.vertices[i + 2],
		 modelview, projection, viewport, &u, &v, &w);
      const int x = static_cast<int>(round(u));
      const int y = static_cast<int>(round(v));

      for (int jj = -kRadius; jj <= kRadius; ++jj) {
	const int ytmp = y + jj;
	if (ytmp < 0 || height2 <= ytmp)
	  continue;
	for (int ii = -kRadius; ii <= kRadius; ++ii) {
	  const int xtmp = x + ii;
	  if (xtmp < 0 || width2 <= xtmp)
	    continue;
	  
	  const int index2 = (height2 - 1 - ytmp) * width2 + xtmp;
	  const double weight = exp(- (ii * ii + jj * jj) / (2 * 0.25 * 0.25));
	  for (int c = 0; c < 3; ++c)
	    screen[index2][c] += object_renderer.colors[i + c] * weight;
	  weights[index2] += weight;
	}
      }
    }
    // Shrink by kRadius - 1.
    for (int t = 0; t < kRadius - 1; ++t) {
      vector<double> weights_tmp = weights;
      for (int y = 1; y < height2 - 1; ++y)
	for (int x = 1; x < width2 - 1; ++x) {
	  if (weights_tmp[y * width2 + x] == 0.0)
	    continue;
	  if (weights_tmp[(y - 1) * width2 + x] == 0 ||
	      weights_tmp[(y + 1) * width2 + x] == 0 ||
	      weights_tmp[y * width2 + (x - 1)] == 0 ||
	      weights_tmp[y * width2 + (x + 1)] == 0)
	    weights[y * width2 + x] = 0.0;
	}
    }

    glReadBuffer(GL_BACK);
    vector<unsigned char> current(width2 * height2 * 3);
    glReadPixels(0, 0, width2, height2, GL_RGB, GL_UNSIGNED_BYTE, &current[0]);
    for (int y = 0; y < height2; ++y) {
      for (int x = 0; x < width2; ++x) {
	const int index = y * width2 + x;
	if (weights[index] == 0.0) {
	  weights[index] = 1.0;
	  for (int i = 0; i < 3; ++i)
	    screen[index][i] = current[3 * ((height2 - 1 - y) * width2 + x) + i] / 255.0;
	}
      } 
    }

    ofstream ofstr;
    ofstr.open("test.ppm");
    ofstr << "P3" << endl
	  << width2 << ' ' << height2 << endl
	  << 255 << endl;
    cv::Mat image(height2, width2, CV_8UC3);
    int index = 0;
    for (int y = 0; y < height2; ++y)
      for (int x = 0; x < width2; ++x, ++index) {
	Vector3d color(0, 0, 0);
	if (weights[index] != 0) {
	  for (int j = 0; j < 3; ++j)
	    color[j] = 255 * screen[index][j] / weights[index];
	  image.at<cv::Vec3b>(y, x) = cv::Vec3b((int)(round(color[0])),
					    (int)(round(color[1])),
					    (int)(round(color[2])));
	}
	ofstr << (int)color[0] << ' ' << (int)color[1] << ' ' << (int)color[2] << ' ';
      }
    
    //cv::imshow("a", image);
  }
  */
}

void MainWidget::RenderPolygon(const int room_not_rendered,
			       const double alpha,
			       const double height_adjustment,
			       const bool depth_order_height_adjustment,
			       const int room_highlighted) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  const bool kNotRenderLabel = false;
  polygon_renderer.RenderWallAll(navigation.GetCenter(),
                                 alpha,
                                 height_adjustment,
                                 depth_order_height_adjustment,
                                 room_not_rendered,
                                 room_highlighted,
                                 kNotRenderLabel);

  //polygon_renderer.RenderWireframeAll(alpha);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  glPopAttrib();
}

void MainWidget::RenderTexturedPolygon(const double alpha) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);

  {
    glCullFace(GL_FRONT);
    glDisable(GL_TEXTURE_2D);
    polygon_renderer.RenderTextureMappedRooms(alpha * 0.5, alpha * 0.2);
  }

  {
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    polygon_renderer.RenderTextureMappedRooms(alpha, alpha);
  }
  
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);

  glPopAttrib();  
}

void MainWidget::RenderPolygonLabels(const int room_not_rendered,
				     const double height_adjustment,
				     const bool depth_order_height_adjustment) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);

  const bool kRenderLabel = true;
  polygon_renderer.RenderWallAll(navigation.GetCenter(),
                                 1.0,
                                 height_adjustment,
                                 depth_order_height_adjustment,
                                 room_not_rendered,
                                 -1,
                                 kRenderLabel);

  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glPopAttrib();
}

void MainWidget::RenderFloorplanLabels() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);

  floorplan_renderer.RenderLabels();

  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glPopAttrib();
}

void MainWidget::RenderThumbnail(const double alpha,
				 const int room_highlighted,
				 QGLWidget* qgl_widget) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  Vector2i render_pos(mouseMovePosition[0], mouseMovePosition[1]);
  // Decide to draw where. Either right or left. At default right. When too close left.
  const double kMarginRatio = 0.05;
  if (render_pos[0] < viewport[2] * 0.85)
    render_pos[0] += static_cast<int>(round(viewport[2] * kMarginRatio));
  else
    render_pos[0] -= static_cast<int>(round(viewport[2] * (kMarginRatio + PanelRenderer::kWidthRatio / 2)));
  const double kScale = 1.0;
  panel_renderer.RenderThumbnail(alpha,
                                 room_highlighted,
                                 render_pos,
                                 Vector3d(0.4, 0.3, 0.3),
                                 kScale,
                                 qgl_widget);

  glPopAttrib();
}

void MainWidget::RenderAllThumbnails(const double alpha,
				     const int room_highlighted,
				     QGLWidget* qgl_widget) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Make thumbnails smaller when rendering everything.
  const double kScale = 0.5;

  const int num_room = floorplan.GetNumRooms();
  vector<Vector2i> room_centers(num_room);
  for (int room = 0; room < num_room; ++room) {
    const Vector3d& center = floorplan.GetRoomCenterFloorGlobal(room);
    GLdouble u, v, w;
    gluProject(center[0], center[1], center[2], modelview, projection, viewport, &u, &v, &w);
    room_centers[room][0] = static_cast<int>(round(u));
    room_centers[room][1] = static_cast<int>(round(v));
  }

  vector<pair<int, int> > render_order;
  for (int room = 0; room < (int)room_centers.size(); ++room)
    render_order.push_back(make_pair(room_centers[room][1], room));
  sort(render_order.rbegin(), render_order.rend());
  
  for (int i = 0; i < (int)render_order.size(); ++i) {
    const int room = render_order[i].second;
    const int offset_x = static_cast<int>(round(kScale * PanelRenderer::kWidthRatio * viewport[2] / 2.0));
    const int offset_y = offset_x * panel_renderer.GetRoomThumbnail(room).height() /
      panel_renderer.GetRoomThumbnail(room).width();
    
    if (room == room_highlighted)
      panel_renderer.RenderThumbnail(alpha,
                                     room,
                                     Vector2i(room_centers[room][0] - offset_x,
                                              viewport[3] - room_centers[room][1] - offset_y),
                                     Vector3d(1, 1, 1),
                                     kScale,
                                     qgl_widget);
    else
      panel_renderer.RenderThumbnail(alpha,
                                     room,
                                     Vector2i(room_centers[room][0] - offset_x,
                                              viewport[3] - room_centers[room][1] - offset_y),
                                     Vector3d(0.4, 0.3, 0.3),
                                     kScale,
                                     qgl_widget);
  }    
  /*
  Vector2i render_pos(mouseMovePosition[0], mouseMovePosition[1]);
  // Decide to draw where. Either right or left. At default right. When too close left.
  const double kMarginRatio = 0.05;
  if (render_pos[0] < viewport[2] * 0.85)
    render_pos[0] += static_cast<int>(round(viewport[2] * kMarginRatio));
  else
    render_pos[0] -= static_cast<int>(round(viewport[2] * (kMarginRatio + PanelRenderer::kWidthRatio / 2)));

  panel_renderer.RenderThumbnail(alpha, room_highlighted, render_pos, qgl_widget);
  */

  glPopAttrib();
}

void MainWidget::RenderAllRoomNames(const double alpha,
                                    QGLWidget* qgl_widget) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Make thumbnails smaller when rendering everything.
  glDisable(GL_TEXTURE_2D);
  glColor4f(0, 0, 0, alpha);
  const int num_room = floorplan.GetNumRooms();

  const QFont font("Times", 14);
  const double kCharacterWidth = 7.25;
  for (int room = 0; room < num_room; ++room) {
    const Vector3d& center = floorplan.GetRoomCenterFloorGlobal(room);
    GLdouble u, v, w;
    gluProject(center[0], center[1], center[2], modelview, projection, viewport, &u, &v, &w);

    const vector<string>& name = floorplan.GetRoomName(room);
    string full_name("");
    for (const auto& word : name) {
      full_name = full_name + string(" ") + word;
    }  
    
    qgl_widget->renderText(u - full_name.length() * kCharacterWidth / 2.0,
                           viewport[3] - v,
                           full_name.c_str(),
                           font);
  }

  glEnable(GL_TEXTURE_2D);
  glPopAttrib();
}  

void MainWidget::RenderPanoramaToAirTransition(const bool flip) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  const double kFullOpacity = 1.0;
  RenderPanorama(kFullOpacity);
  
  // Render the target pano.
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[1]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  RenderTexturedPolygon(kFullOpacity);
  RenderObjects(kFullOpacity);

  // Blend the two.
  // const double weight_end = 1.0 - weight_start;
  double weight = navigation.ProgressInverse();
  if (flip)
    weight = 1.0 - weight;
  weight = 1.0 - cos(weight * M_PI / 2);
  const int kKeepPolygon = 2;
  BlendFrames(weight, kKeepPolygon);

  glPopAttrib();
}

void MainWidget::RenderPanoramaToFloorplanTransition(const bool flip) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  const double kFullOpacity = 1.0;
  RenderPanorama(kFullOpacity);
  
  // Render the target pano.
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[1]);
  ClearDisplayWithWhite();
  glEnable(GL_TEXTURE_2D);
  RenderFloorplan(kFullOpacity, false);

  // Blend the two.
  // const double weight_end = 1.0 - weight_start;
  double weight = navigation.ProgressInverse();
  if (flip)
    weight = 1.0 - weight;
  weight = 1.0 - cos(weight * M_PI / 2);
  const int kKeepPolygon = 2;
  BlendFrames(weight, kKeepPolygon);

  glPopAttrib();
}

void MainWidget::RenderAirToFloorplanTransition(const bool flip) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  const double kFullOpacity = 1.0;
  RenderTexturedPolygon(kFullOpacity);
  RenderObjects(kFullOpacity);
  
  // Render the target pano.
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[1]);
  ClearDisplayWithWhite();
  RenderFloorplan(kFullOpacity, false);

  // Blend the two.
  double weight = navigation.ProgressInverse();
  if (flip)
    weight = 1.0 - weight;
  weight = 1.0 - cos(weight * M_PI / 2);
  const int kDivideByAlpha = 1;
  BlendFrames(weight, kDivideByAlpha);

  glPopAttrib();
}
  
int MainWidget::FindRoomHighlighted(const Eigen::Vector2i& pixel) {
  unsigned char red, green, blue;
  glBindFramebuffer(GL_FRAMEBUFFER, frameids[0]);
  glReadPixels(pixel[0], viewport[3] - pixel[1], 1, 1, GL_RED, GL_UNSIGNED_BYTE, &red);
  glReadPixels(pixel[0], viewport[3] - pixel[1], 1, 1, GL_GREEN, GL_UNSIGNED_BYTE, &green);
  glReadPixels(pixel[0], viewport[3] - pixel[1], 1, 1, GL_BLUE, GL_UNSIGNED_BYTE, &blue);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Background color is not 0 exactly, and this is just for a safety guard.
  if (red != 0 || green != 0)
    return -1;
  return static_cast<int>(blue) - 1;
}

void MainWidget::ClearDisplay() {
  switch (navigation.GetCameraStatus()) {
  case kFloorplan:
  case kFloorplanTransition: {
    ClearDisplayWithWhite();
    break;
  }
  default: {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  }
}

void MainWidget::ClearDisplayWithWhite() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(kBackgroundColor[0], kBackgroundColor[1], kBackgroundColor[2], 0);
}    
  
}  // namespace structured_indoor_modeling