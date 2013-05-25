//
//  CCJSONConverter.h
//  cocos2d-x-jc
//
//  Created by LIN BOYU on 11/14/12.
//
//

#ifndef __cocos2d_x__extensions__CCJSON__
#define __cocos2d_x__extensions__CCJSON__

#include "CCJSONData.h"

NS_CC_BEGIN

enum kCCJSONType {
    kCCJSON_Unknown,
    kCCJSON_Null,   ///< CCNull.
    kCCJSON_Bool,   ///< CCBool.
    kCCJSON_Number, ///< CCDouble.
    kCCJSON_String, ///< CCString.
    kCCJSON_Array,  ///< CCArray.
    kCCJSON_Object  ///< CCDictionary.
};

/// Use it to read parsed JSON safely. Optimized to determine kind fast.
static kCCJSONType ccGetJSONKind(CCObject *object);

/// Data mapping:
/// \li object      <->  CCDictionary
/// \li array       <->  CCArray
/// \li string      <->  CCString
/// \li number      <->  CCDouble
/// \li true/false  <->  CCBool
/// \li null        <->  CCNull
/// \li CCFloat and CCInteger converted to number when serializing to JSON.
class CCJSON : public CCObject
{
    CC_DISABLE_COPY(CCJSON)
public:
    static CCJSON *sharedConverter();

    /// Formatting disabled by default.
    /// Compressed JSON string is better for network operations,
    /// when formatted can help in debug.
    bool isFormattingEnabled() const;
    void setFormattingEnabled(bool enabled);

    /// Creates unformatted JSON string (it better for network operations)
    CCString *getJSON(CCDictionary *dictionary) const;

    /// Restores CCDictionary from JSON-string
    CCDictionary *getDictionary(const char *str) const;

private:
    CCJSON();

    bool m_isFormattingEnabled;
};

NS_CC_END

#endif /* defined(__cocos2d_x__extensions__CCJSON__) */
