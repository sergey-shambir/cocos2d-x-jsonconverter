//
//  CCJSONConverter.h
//  cocos2d-x-jc
//
//  Created by LIN BOYU on 11/14/12.
//
//

#ifndef __cocos2d_x_jc__CCJSONConverter__
#define __cocos2d_x_jc__CCJSONConverter__

#include <stddef.h> // actually, should be inside cJSON
#include "cJSON/cJSON.h"
#include "CCJSONData.h"

namespace cocos2d {

class CCJSONConverter : public CCObject
{
public:
    /// Creates unformatted JSON string (it better for network operations)
    static CCString *getJSON(CCDictionary *dictionary);

    /// Creates human-readable (formatted) representation
    static CCString *getFormattedJSON(CCDictionary *dictionary);

    /// Restores CCDictionary from JSON-string
    static CCDictionary *getDictionary(const char *str);
    
private:
    typedef char *(*PrintingFunction)(cJSON *item);
    static CCString *convertInternal(CCDictionary *dictionary, PrintingFunction pFunction);

    static void convertJsonToDictionary(cJSON *json, CCDictionary *dictionary);
    static void convertDictionaryToJson(CCDictionary *dictionary, cJSON *json);
    static void convertJsonToArray(cJSON *json, CCArray *array);
    static void convertArrayToJson(CCArray *array, cJSON *json);
    static cJSON *getObjJson(CCObject *obj);
    static CCObject *getJsonObj(cJSON *json);
};

} // namespace cocos2d

#endif /* defined(__cocos2d_x_jc__CCJSONConverter__) */
