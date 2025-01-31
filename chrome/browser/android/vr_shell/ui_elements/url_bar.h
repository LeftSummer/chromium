// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_VR_SHELL_UI_ELEMENTS_URL_BAR_H_
#define CHROME_BROWSER_ANDROID_VR_SHELL_UI_ELEMENTS_URL_BAR_H_

#include <memory>

#include "base/macros.h"
#include "chrome/browser/android/vr_shell/ui_elements/textured_element.h"
#include "url/gurl.h"

namespace vr_shell {

class UrlBarTexture;

class UrlBar : public TexturedElement {
 public:
  explicit UrlBar(int preferred_width);
  ~UrlBar() override;

  void OnHoverEnter() override;
  void OnHoverLeave() override;
  void SetURL(const GURL& gurl);

 private:
  UiTexture* GetTexture() const override;
  std::unique_ptr<UrlBarTexture> texture_;

  DISALLOW_COPY_AND_ASSIGN(UrlBar);
};

}  // namespace vr_shell

#endif  // CHROME_BROWSER_ANDROID_VR_SHELL_UI_ELEMENTS_URL_BAR_H_
