//
//  TestCppWrapper.m
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/25.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#import "OscCppWrapper.h"
#include "HueOSCReceiver.h"

static OscCppWrapper* oscCppWapper = nil;

void oscCCallBack(int lightNo, int hue, int sat, int value){
    printf("testCFunc called");
    
    [oscCppWapper callback:lightNo hue:hue sat:sat value:value ];
}

@implementation OscCppWrapper

- (id) init {
	if ((self=[super init]) ) {
        oscReceiver = (void*)new HueOSCReceiver();
    }
    
    oscCppWapper = self ;
    
	return self;
}

- (void) dealloc {
    delete (HueOSCReceiver *)oscReceiver ;
}


- (void) setCallback {
    ((HueOSCReceiver *)oscReceiver)->setCallback( oscCCallBack );
}

- (void) callback:(int)lightNo hue:(int)hue sat:(int)sat value:(int)value{
    NSLog(@"obj-c callback called lightNo:%d hue:%d, sat:%d value:%d ", lightNo, hue, sat, value);
    
    // ここでdelegateを呼ぶ
    [_delegate hueOscMessageGet:lightNo hue:hue sat:sat value:value];
}

- (void) setUp:(int)port {
    ((HueOSCReceiver *)oscReceiver)->setup(port);
}

- (void)setDelegate:(id<HueOscDelegate>)delegate {
    _delegate = delegate;
}



@end
