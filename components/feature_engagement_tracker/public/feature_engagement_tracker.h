// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_FEATURE_ENGAGEMENT_TRACKER_PUBLIC_FEATURE_ENGAGEMENT_TRACKER_H_
#define COMPONENTS_FEATURE_ENGAGEMENT_TRACKER_PUBLIC_FEATURE_ENGAGEMENT_TRACKER_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/feature_list.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner.h"
#include "components/keyed_service/core/keyed_service.h"

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#endif  // defined(OS_ANDROID)

namespace feature_engagement_tracker {

// The FeatureEngagementTracker provides a backend for displaying feature
// enlightenment or in-product help (IPH) with a clean and easy to use API to be
// consumed by the UI frontend. The backend behaves as a black box and takes
// input about user behavior. Whenever the frontend gives a trigger signal that
// IPH could be displayed, the backend will provide an answer to whether it is
// appropriate to show it or not.
class FeatureEngagementTracker : public KeyedService {
 public:
#if defined(OS_ANDROID)
  // Returns a Java object of the type FeatureEngagementTracker for the given
  // FeatureEngagementTracker.
  static base::android::ScopedJavaLocalRef<jobject> GetJavaObject(
      FeatureEngagementTracker* feature_engagement_tracker);
#endif  // defined(OS_ANDROID)

  // Invoked when the tracker has been initialized. The |success| parameter
  // indicates that the initialization was a success and the tracker is ready to
  // receive calls.
  using OnInitializedCallback = base::Callback<void(bool success)>;

  // The |storage_dir| is the path to where all local storage will be.
  // The |bakground_task_runner| will be used for all disk reads and writes.
  static FeatureEngagementTracker* Create(
      const base::FilePath& storage_dir,
      const scoped_refptr<base::SequencedTaskRunner>& background_task_runner);

  // Must be called whenever an event happens.
  virtual void NotifyEvent(const std::string& event) = 0;

  // This function must be called whenever the triggering condition for a
  // specific feature happens. Returns true iff the display of the in-product
  // help must happen.
  // If |true| is returned, the caller *must* call Dismissed() when display
  // of feature enlightenment ends.
  virtual bool ShouldTriggerHelpUI(const base::Feature& feature)
      WARN_UNUSED_RESULT = 0;

  // Must be called after display of feature enlightenment finishes.
  virtual void Dismissed() = 0;

  // Returns whether the tracker has been successfully initialized. During
  // startup, this will be false until the internal model has been loaded at
  // which point it is set to true if the initialization was successful. The
  // state will never change from initialized to uninitialized.
  // Callers can invoke AddOnInitializedCallback(...) to be notified when the
  // result of the initialization is ready.
  virtual bool IsInitialized() = 0;

  // For features that trigger on startup, they can register a callback to
  // ensure that they are informed when the tracker has finished the
  // initialization. If the tracker has already been initialized, the callback
  // will still be invoked with the result. The callback is guaranteed to be
  // invoked exactly one time.
  virtual void AddOnInitializedCallback(OnInitializedCallback callback) = 0;

 protected:
  FeatureEngagementTracker() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(FeatureEngagementTracker);
};

}  // namespace feature_engagement_tracker

#endif  // COMPONENTS_FEATURE_ENGAGEMENT_TRACKER_PUBLIC_FEATURE_ENGAGEMENT_TRACKER_H_
