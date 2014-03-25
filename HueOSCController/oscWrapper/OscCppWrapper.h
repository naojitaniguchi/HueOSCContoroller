//
//  TestCppWrapper.h
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/25.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol HueOscDelegate <NSObject>
- (void)hueOscMessageGet:(int)lihgtId hue:(int)hue sat:(int)sat value:(int)value;
@end

@interface OscCppWrapper : NSObject
{
    void *oscReceiver ;

}

@property (weak, nonatomic, readonly) id<HueOscDelegate>delegate;

- (void) setCallback;
- (void) callback:(int)lightNo hue:(int)hue sat:(int)sat value:(int)value ;
- (void) setUp:(int)port;
- (void) setDelegate:(id<HueOscDelegate>)delegate ;

@end
