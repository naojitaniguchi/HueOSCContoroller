//
//  TestCppWrapper.m
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/25.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#import "TestCppWrapper.h"
#include <TestCpp.h>

static TestCppWrapper* cppWapper = nil;

void testCCallBack(int a, int b){
    printf("testCFunc called");
    
    [cppWapper callback:a b:b];
}

@implementation TestCppWrapper

- (id) init {
	if ((self=[super init]) ) {
        testCpp = (void*)new TestCpp();
    }
    
    cppWapper = self ;
    
	return self;
}

- (void) dealloc {
    delete (TestCpp *)testCpp ;
}

- (void) testCallback {
    ((TestCpp *)testCpp)->testCallback() ;
}

- (void) setCallback {
    ((TestCpp *)testCpp)->setCallback( testCCallBack );
}

- (void) callback:(int)a b:(int)b{
    NSLog(@"obj-c callback called a:%d, b:%d", a, b);
}


// C++ から obj-cを呼べないか？
// C++ から 関数ポインターは呼べる
// 関数ポインター
// ObjCのメソッドの関数ポインターって？
// 普通にCの関数を定義する？
// obj-cのdelegateをしたい


@end
