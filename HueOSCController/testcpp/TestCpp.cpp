//
//  TestCpp.cpp
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/21.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#include "TestCpp.h"
#include <iostream>

void TestCpp::testPrint(){
    printf("hello from test cpp\n");
}

void TestCpp::setCallback(TESTFUNCPTR _funcPtr ){
    funcPtr = _funcPtr ;
}

void TestCpp::testCallback(){
    (*funcPtr)( 1, 2 );
}