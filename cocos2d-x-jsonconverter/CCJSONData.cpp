#include "CCJSONData.h"

NS_CC_BEGIN

CCNull *CCNull::create()
{
    CCNull * pRet = new CCNull();
    pRet->autorelease();
    return pRet;
}

NS_CC_END
