//
//  CCJSONConverter.cpp
//  cocos2d-x-jc
//
//  Created by LIN BOYU on 11/14/12.
//
//

#include "CCJSON.h"
#include <stddef.h> // actually, should be inside cJSON
#include "cJSON/cJSON.h"

NS_CC_BEGIN

namespace {

    static CCJSON *_sharedConverter;

    typedef char *(*PrintingFunction)(cJSON *item);

    class PrivateJSONConverter : public CCDataVisitor
    {
    public:
        cJSON *convert(CCDictionary *dict)
        {
            dict->acceptVisitor(*this);
            return m_json;
        }

    protected:
        virtual void visitObject(const CCObject *p)
        {
            if (CCNull *p = dynamic_cast<CCNull *>(p)) {
                m_json = cJSON_CreateNull();
            } else {
                CCLOGERROR("Object of type '%s' must not be converted to JSON.", typeid(*p).name());
                m_json = NULL;
            }
        }

        virtual void visit(const CCBool *p)
        {
            m_json = p->getValue() ? cJSON_CreateTrue() :cJSON_CreateFalse();
        }

        virtual void visit(const CCInteger *p)
        {
            m_json = cJSON_CreateNumber(p->getValue());
        }

        virtual void visit(const CCFloat *p)
        {
            m_json = cJSON_CreateNumber(p->getValue());
        }

        virtual void visit(const CCDouble *p)
        {
            m_json = cJSON_CreateNumber(p->getValue());
        }

        virtual void visit(const CCString *p)
        {
            m_json = cJSON_CreateString(p->getCString());
        }

        virtual void visit(const CCArray *p)
        {
            cJSON *json = cJSON_CreateArray();
            CCObject *obj = NULL;
            CCARRAY_FOREACH(p, obj){
                obj->acceptVisitor(*this);
                if (m_json)
                    cJSON_AddItemToArray(json, m_json);
            }
            m_json = json;
        }

        virtual void visit(const CCDictionary *p)
        {
            cJSON *json = cJSON_CreateObject();
            CCDictElement * pElement = NULL;
            CCDICT_FOREACH(p, pElement){
                pElement->getObject()->acceptVisitor(*this);
                if (m_json)
                    cJSON_AddItemToObject(json, pElement->getStrKey(), m_json);
            }
            m_json = json;
        }

    private:
        cJSON *m_json;
    };

    class PrivateJSONRecognizer : public CCDataVisitor
    {
    public:
        kCCJSONType getJSONType(CCObject *object)
        {
            object->acceptVisitor(*this);
            return m_result;
        }

    protected:
        virtual void visitObject(const CCObject *p)
        {
            if (CCNull *p = dynamic_cast<CCNull *>(p)) {
                m_result = kCCJSON_Null;
            } else {
                m_result = kCCJSON_Unknown;
            }
        }

        virtual void visit(const CCBool *p)
        {
            m_result = kCCJSON_Bool;
        }

        virtual void visit(const CCDouble *p)
        {
            m_result = kCCJSON_Number;
        }

        virtual void visit(const CCString *p)
        {
            m_result = kCCJSON_String;
        }

        virtual void visit(const CCArray *p)
        {
            m_result = kCCJSON_Array;
        }

        virtual void visit(const CCDictionary *p)
        {
            m_result = kCCJSON_Object;
        }

    private:
        kCCJSONType m_result;
    };

}

static CCString *convertInternal(CCDictionary *dictionary, PrintingFunction pFunction);
static void convertJsonToDictionary(cJSON *json, CCDictionary *dictionary);
static void convertJsonToArray(cJSON *json, CCArray *array);
static CCObject *getJsonObj(cJSON *json);

kCCJSONType ccGetJSONKind(CCObject *object)
{
    CCAssert(object, "Cannot get JSON type for NULL object.");
    PrivateJSONRecognizer recognizer;
    return recognizer.getJSONType(object);
}

CCJSON *CCJSON::sharedConverter()
{
    if (NULL == _sharedConverter)
        _sharedConverter = new CCJSON;
    return _sharedConverter;
}

bool CCJSON::isFormattingEnabled() const
{
    return m_isFormattingEnabled;
}

void CCJSON::setFormattingEnabled(bool enabled)
{
    m_isFormattingEnabled = enabled;
}

CCString *CCJSON::getJSON(CCDictionary *dictionary) const
{
    if (m_isFormattingEnabled)
        return convertInternal(dictionary, &cJSON_Print);
    return convertInternal(dictionary, &cJSON_PrintUnformatted);
}

CCDictionary *CCJSON::getDictionary(const char *str) const
{
    cJSON *json = cJSON_Parse(str);
    if (!json && json->type != cJSON_Object) {
        CCLOG("CCJSON: cannot parse JSON correctly.");
        return CCDictionary::create();
    }

    CCDictionary *dictionary = CCDictionary::create();
    convertJsonToDictionary(json, dictionary);
	cJSON_Delete(json);
    return dictionary;
}

CCJSON::CCJSON()
    : m_isFormattingEnabled(false)
{
}

CCString *convertInternal(CCDictionary *dictionary, PrintingFunction pFunction)
{
    CCAssert(dictionary, "CCJSON: cannot convert a null pointer");

    PrivateJSONConverter converter;
    cJSON *json = converter.convert(dictionary);

    char *result = pFunction(json);
    cJSON_Delete(json);
    CCString *pRet = CCString::create(result);
    ::free(result);
    return pRet;
}

void convertJsonToDictionary(cJSON *json, CCDictionary *dictionary)
{
    dictionary->removeAllObjects();
    cJSON *pair = json->child;
    while (pair) {
        CCObject *obj = getJsonObj(pair);
        dictionary->setObject(obj, pair->string);
        pair = pair->next;
    }
}

void convertJsonToArray(cJSON *json, CCArray *array)
{
    array->removeAllObjects();
    const int size = cJSON_GetArraySize(json);
    for (int i = 0; i < size; ++i) {
        cJSON *jsonItem = cJSON_GetArrayItem(json, i);
        CCObject *objItem = getJsonObj(jsonItem);
        array->addObject(objItem);
    }
}

CCObject *getJsonObj(cJSON *json)
{
    switch (json->type) {
        case cJSON_Object:
        {
            CCDictionary *dictionary = CCDictionary::create();
            convertJsonToDictionary(json, dictionary);
            return dictionary;
        }
        case cJSON_Array:
        {
            CCArray *array = CCArray::create();
            convertJsonToArray(json, array);
            return array;
        }
        case cJSON_String:
        {
            CCString *string = CCString::create(json->valuestring);
            return string;
        }
        case cJSON_Number:
        {
            CCDouble *number = CCDouble::create(json->valuedouble);
            return number;
        }
        case cJSON_True:
        {
            CCBool *boolean = CCBool::create(true);
            return boolean;
        }
        case cJSON_False:
        {
            CCBool *boolean = CCBool::create(false);
            return boolean;
        }
        case cJSON_NULL:
        {
            CCNull *null = CCNull::create();
            return null;
        }
        default:
        {
            CCLog("CCJSONConverter encountered an unrecognized type");
            return NULL;
        }
    }
}

NS_CC_END
