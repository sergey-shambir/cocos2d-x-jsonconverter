//
//  CCJSONData.h
//  cocos2d-x-jc
//
//  Created by LIN BOYU on 11/14/12.
//
//

#ifndef __cocos2d_x_jc__CCJSONData__
#define __cocos2d_x_jc__CCJSONData__

#include "cocos2d.h"

namespace cocos2d {

class CCNull : public CCObject {
public:
    static CCNull * create()
    {
        CCNull * pRet = new CCNull();
        pRet->autorelease();
        return pRet;
    }
};

} // namespace cocos2d

#endif /* defined(__cocos2d_x_jc__CCJSONData__) */
