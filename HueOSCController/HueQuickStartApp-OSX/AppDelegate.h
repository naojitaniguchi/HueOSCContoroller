//
//  AppDelegate.h
//  HueQuickStartApp-OSX
//
//  Created by Paul Verhoeven on 11/5/13.
//  Copyright (c) 2013 Philips. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "PHBridgePushLinkViewController.h"
#include "PHBridgeSelectionViewController.h"
#import <HueSDK_OSX/HueSDK.h>

#import "OscCppWrapper.h"

#define NSAppDelegate  ((AppDelegate *)[[NSApplication sharedApplication] delegate])

@class PHHueSDK;

@interface AppDelegate : NSObject <NSApplicationDelegate,PHBridgePushLinkViewControllerDelegate,PHBridgeSelectionViewControllerDelegate, HueOscDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (strong, nonatomic) PHHueSDK *phHueSDK;

#pragma mark - HueSDK

/**
 Starts the local heartbeat
 */
- (void)enableLocalHeartbeat;

/**
 Stops the local heartbeat
 */
- (void)disableLocalHeartbeat;

/**
 Starts a search for a bridge
 */
- (void)searchForBridgeLocal;

- (void)hueOscMessageGet:(int)lihgtId hue:(int)hue sat:(int)sat value:(int)value;

@end
