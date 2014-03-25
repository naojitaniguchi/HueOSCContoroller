//
//  TestCpp.h
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/21.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#ifndef __HueOSCController__TestCpp__
#define __HueOSCController__TestCpp__

typedef void (*TESTFUNCPTR)(int,int);

class TestCpp {
public:
    void testCallback();
    void testPrint();
    void setCallback(TESTFUNCPTR funcPtr );
    TESTFUNCPTR funcPtr ;
};


#endif /* defined(__HueOSCController__TestCpp__) */
