//
//  TestCppWrapper.h
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/25.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TestCppWrapper : NSObject
{
    void *testCpp ;
}

- (void) setCallback;
- (void) callback:(int)a b:(int)b;
- (void) testCallback;

@end
