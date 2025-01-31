// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_IOS_APP_PIN_ENTRY_VIEW_H_
#define REMOTING_CLIENT_IOS_APP_PIN_ENTRY_VIEW_H_

#import <UIKit/UIKit.h>

@protocol PinEntryDelegate<NSObject>

// Notifies the delegate that a pin has been provided and if we should pair.
@optional
- (void)didProvidePin:(NSString*)pin createPairing:(BOOL)createPairing;

@end

// This view is the container for a PIN entry box, a button to submit, and the
// option box to remember the pairing. All used for entering a PIN based
// passcode.
@interface PinEntryView : UIView

// This delegate will handle interactions on the cells in the collection.
@property(weak, nonatomic) id<PinEntryDelegate> delegate;

@end

#endif  // REMOTING_CLIENT_IOS_APP_PIN_ENTRY_VIEW_H_
