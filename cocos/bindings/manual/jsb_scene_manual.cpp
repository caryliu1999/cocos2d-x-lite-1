/****************************************************************************
 Copyright (c) 2021 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.
 
 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "jsb_scene_manual.h"
#include "bindings/auto/jsb_scene_auto.h"
#include "scene/Model.h"
#include "scene/Node.h"

#ifndef JSB_ALLOC
    #define JSB_ALLOC(kls, ...) new (std::nothrow) kls(__VA_ARGS__)
#endif

#ifndef JSB_FREE
    #define JSB_FREE(ptr) delete ptr
#endif

static bool js_scene_Model_setInstancedBuffer(se::State& s) {
    auto* cobj = static_cast<cc::scene::Model*>(s.nativeThisObject());
    SE_PRECONDITION2(cobj, false, "js_scene_Model_setInstancedBuffer : Invalid Native Object");
    const auto& args = s.args();
    size_t      argc = args.size();

    if (argc == 1) {
        SE_PRECONDITION2(args[0].isObject() && args[0].toObject()->isArrayBuffer(), false, "js_gfx_Device_createBuffer: expected Array Buffer!");

        uint8_t* data{nullptr};
        args[0].toObject()->getArrayBufferData(&data, nullptr);
        cobj->setInstancedBuffer(data);

        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_scene_Model_setInstancedBuffer)

static bool js_scene_Node_initWithData(se::State& s) // constructor_overloaded.c
{
    auto* cobj = static_cast<cc::scene::Node*>(s.nativeThisObject());
    CC_UNUSED bool ok   = true;
    const auto&    args = s.args();
    size_t         argc = args.size();
    do {
        if (argc == 1) {
            SE_PRECONDITION2(args[0].isObject() && args[0].toObject()->isTypedArray(), false, "js_scene_Node_initWithData: expected Typed Array!");
            
            uint8_t* data{nullptr};
            size_t dataBytes = 0;
            args[0].toObject()->getTypedArrayData(&data, &dataBytes);
            cobj->initWithData(data);
            return true;
        }
    } while (false);
    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_scene_Node_initWithData)

bool register_all_scene_manual(se::Object* obj) {
    // Get the ns
    se::Value nsVal;
    if (!obj->getProperty("ns", &nsVal)) {
        se::HandleObject jsobj(se::Object::createPlainObject());
        nsVal.setObject(jsobj);
        obj->setProperty("ns", nsVal);
    }
    se::Object* ns = nsVal.toObject();

    __jsb_cc_scene_Model_proto->defineFunction("setInstancedBuffer", _SE(js_scene_Model_setInstancedBuffer));
    __jsb_cc_scene_Node_proto->defineFunction("initWithData", _SE(js_scene_Node_initWithData));

    return true;
}
