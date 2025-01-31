// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/system/virtual_keyboard/virtual_keyboard_tray.h"

#include <algorithm>

#include "ash/keyboard/keyboard_ui.h"
#include "ash/resources/vector_icons/vector_icons.h"
#include "ash/shelf/shelf_constants.h"
#include "ash/shelf/wm_shelf.h"
#include "ash/shell.h"
#include "ash/strings/grit/ash_strings.h"
#include "ash/system/tray/tray_constants.h"
#include "ash/system/tray/tray_container.h"
#include "ash/wm_window.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/display/display.h"
#include "ui/events/event.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/keyboard/keyboard_controller.h"
#include "ui/views/controls/image_view.h"

namespace ash {

VirtualKeyboardTray::VirtualKeyboardTray(WmShelf* wm_shelf)
    : TrayBackgroundView(wm_shelf),
      icon_(new views::ImageView),
      wm_shelf_(wm_shelf) {
  SetInkDropMode(InkDropMode::ON);

  gfx::ImageSkia image =
      gfx::CreateVectorIcon(kShelfKeyboardIcon, kShelfIconColor);
  icon_->SetImage(image);
  const int vertical_padding = (kTrayItemSize - image.height()) / 2;
  const int horizontal_padding = (kTrayItemSize - image.width()) / 2;
  icon_->SetBorder(views::CreateEmptyBorder(
      gfx::Insets(vertical_padding, horizontal_padding)));
  tray_container()->AddChildView(icon_);

  // The Shell may not exist in some unit tests.
  if (Shell::HasInstance())
    Shell::Get()->keyboard_ui()->AddObserver(this);
  // Try observing keyboard controller, in case it is already constructed.
  ObserveKeyboardController();
}

VirtualKeyboardTray::~VirtualKeyboardTray() {
  // Try unobserving keyboard controller, in case it still exists.
  UnobserveKeyboardController();
  // The Shell may not exist in some unit tests.
  if (Shell::HasInstance())
    Shell::Get()->keyboard_ui()->RemoveObserver(this);
}

base::string16 VirtualKeyboardTray::GetAccessibleNameForTray() {
  return l10n_util::GetStringUTF16(
      IDS_ASH_VIRTUAL_KEYBOARD_TRAY_ACCESSIBLE_NAME);
}

void VirtualKeyboardTray::HideBubbleWithView(
    const views::TrayBubbleView* bubble_view) {}

void VirtualKeyboardTray::ClickedOutsideBubble() {}

bool VirtualKeyboardTray::PerformAction(const ui::Event& event) {
  const int64_t display_id =
      wm_shelf_->GetWindow()->GetDisplayNearestWindow().id();
  Shell::Get()->keyboard_ui()->ShowInDisplay(display_id);
  // Normally, active status is set when virtual keyboard is shown/hidden,
  // however, showing virtual keyboard happens asynchronously and, especially
  // the first time, takes some time. We need to set active status here to
  // prevent bad things happening if user clicked the button before keyboard is
  // shown.
  SetIsActive(true);
  return true;
}

void VirtualKeyboardTray::OnKeyboardEnabledStateChanged(bool new_enabled) {
  SetVisible(new_enabled);
  if (new_enabled) {
    // Observe keyboard controller to detect when the virtual keyboard is
    // shown/hidden.
    ObserveKeyboardController();
  } else {
    // Try unobserving keyboard controller, in case it is not yet destroyed.
    UnobserveKeyboardController();
  }
}

void VirtualKeyboardTray::OnKeyboardBoundsChanging(
    const gfx::Rect& new_bounds) {
  SetIsActive(!new_bounds.IsEmpty());
}

void VirtualKeyboardTray::OnKeyboardClosed() {}

void VirtualKeyboardTray::ObserveKeyboardController() {
  keyboard::KeyboardController* keyboard_controller =
      keyboard::KeyboardController::GetInstance();
  if (keyboard_controller)
    keyboard_controller->AddObserver(this);
}

void VirtualKeyboardTray::UnobserveKeyboardController() {
  keyboard::KeyboardController* keyboard_controller =
      keyboard::KeyboardController::GetInstance();
  if (keyboard_controller)
    keyboard_controller->RemoveObserver(this);
}

}  // namespace ash
