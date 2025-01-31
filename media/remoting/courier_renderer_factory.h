// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_REMOTING_COURIER_RENDERER_FACTORY_H_
#define MEDIA_REMOTING_COURIER_RENDERER_FACTORY_H_

#include "media/base/renderer_factory.h"
#include "media/remoting/renderer_controller.h"

namespace media {
namespace remoting {

// Creates Renderers for remote playback.
class CourierRendererFactory : public RendererFactory {
 public:
  explicit CourierRendererFactory(
      std::unique_ptr<RendererController> controller);
  ~CourierRendererFactory() override;

  std::unique_ptr<Renderer> CreateRenderer(
      const scoped_refptr<base::SingleThreadTaskRunner>& media_task_runner,
      const scoped_refptr<base::TaskRunner>& worker_task_runner,
      AudioRendererSink* audio_renderer_sink,
      VideoRendererSink* video_renderer_sink,
      const RequestSurfaceCB& request_surface_cb) override;

  // Returns whether remote rendering has started, based off of |controller_|.
  // Called by RendererFactorySelector to determine when to create a
  // CourierRenderer.
  bool IsRemotingActive();

 private:
  const std::unique_ptr<RendererController> controller_;

  DISALLOW_COPY_AND_ASSIGN(CourierRendererFactory);
};

}  // namespace remoting
}  // namespace media

#endif  // MEDIA_REMOTING_COURIER_RENDERER_FACTORY_H_
