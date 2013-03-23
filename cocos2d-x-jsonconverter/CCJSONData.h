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

class CCNumber : public CCObject {
private:
    int m_nValue;
    double m_dValue;
public:
    CCNumber(int v)
    : m_nValue(v),m_dValue(v) {}
    
    CCNumber(double v)
    : m_nValue(v),m_dValue(v) {}
    
    static CCNumber * create(int v)
    {
        CCNumber* pRet = new CCNumber(v);
        pRet->autorelease();
        return pRet;
    }
    static CCNumber* create(double v)
    {
        CCNumber* pRet = new CCNumber(v);
        pRet->autorelease();
        return pRet;
    }
    int getIntValue() const {return m_nValue;}
    double getDoubleValue() const {return m_dValue;}
    void setValue(int v)  { m_nValue=v; m_dValue=v;}
    void setValue(double v)  { m_nValue=v; m_dValue=v;}
};

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
