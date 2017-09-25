/*
 * Created by LinWenhai on 17/11/13.
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "scripting/js-bindings/manual/ui/jsb_cocos2dx_ui_manual.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

static bool js_cocos2dx_LayoutParameter_setMargin(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
    ui::LayoutParameter* cobj = (ui::LayoutParameter *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "Invalid Native Object");

    if (argc == 1) {
        JS::RootedObject tmp(cx);
        JS::RootedValue jsleft(cx), jstop(cx),jsright(cx),jsbottom(cx);
        double left, top,right,bottom;
        bool ok = args[0].isObject() &&
            JS_ValueToObject(cx, args[0], &tmp) &&
            JS_GetProperty(cx, tmp, "left", &jsleft) &&
            JS_GetProperty(cx, tmp, "top", &jstop) &&
            JS_GetProperty(cx, tmp, "right", &jsright) &&
            JS_GetProperty(cx, tmp, "bottom", &jsbottom) &&
            jsleft.isNumber() && jstop.isNumber() && jsright.isNumber() && jsbottom.isNumber();
        
        JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");

        left = jsleft.toNumber();
        top = jstop.toNumber();
        right = jsright.toNumber();
        bottom = jsbottom.toNumber();

        cobj->setMargin(ui::Margin(left,top,right,bottom));
        return true;
    }
    else if (argc == 4) {
        double left, top,right,bottom;
        bool ok = args.get(0).isNumber() &&
                  args.get(1).isNumber() &&
                  args.get(2).isNumber() &&
                  args.get(3).isNumber();
        JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
        
        JS::RootedValue jsv(cx, args.get(0));
        left = jsv.toNumber();
        jsv.set(args.get(1));
        top = jsv.toNumber();
        jsv.set(args.get(2));
        right = jsv.toNumber();
        jsv.set(args.get(3));
        bottom = jsv.toNumber();

        cobj->setMargin(ui::Margin(left,top,right,bottom));
        return true;
    }
    JS_ReportErrorUTF8(cx, "Invalid number of arguments");
    return false;
}

static bool js_cocos2dx_LayoutParameter_getMargin(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
    ui::LayoutParameter* cobj = (ui::LayoutParameter *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "Invalid Native Object");

    if (argc == 0) {
        JS::RootedObject tmp(cx, JS_NewPlainObject(cx));
        if (!tmp) return false;
        ui::Margin margin = cobj->getMargin();
        bool ok = JS_DefineProperty(cx, tmp, "left", margin.left, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "top", margin.top, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "right", margin.right, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "bottom", margin.bottom, JSPROP_ENUMERATE | JSPROP_PERMANENT);
        if (ok)
        {
            args.rval().set(JS::ObjectOrNullValue(tmp));
        }
        else
        {
            return false;
        }
        return true;
    }
    JS_ReportErrorUTF8(cx, "Invalid number of arguments");
    return false;
}

class JSB_EditBoxDelegate
: public Ref
, public EditBoxDelegate
{
public:
    JSB_EditBoxDelegate()
    {
        _JSDelegate = JS::NullValue();
    }
    
    virtual void editBoxEditingDidBegin(EditBox* editBox) override
    {
        js_proxy_t * p = jsb_get_native_proxy(editBox);
        if (!p) return;

        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue arg(cx, JS::ObjectOrNullValue(p->obj));
        JS::HandleValueArray args(arg);
        JS::RootedValue delegateVal(cx, _JSDelegate);
        ScriptingCore::getInstance()->executeFunctionWithOwner(delegateVal, "editBoxEditingDidBegin", args);
    }

    virtual void editBoxEditingDidEnd(EditBox* editBox) override
    {
        js_proxy_t * p = jsb_get_native_proxy(editBox);
        if (!p) return;
        
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue arg(cx, JS::ObjectOrNullValue(p->obj));
        JS::HandleValueArray args(arg);
        JS::RootedValue delegateVal(cx, _JSDelegate);
        ScriptingCore::getInstance()->executeFunctionWithOwner(delegateVal, "editBoxEditingDidEnd", args);
    }

    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) override
    {
        js_proxy_t * p = jsb_get_native_proxy(editBox);
        if (!p) return;
        
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        
        JS::AutoValueVector valArr(cx);
        valArr.append(JS::ObjectOrNullValue(p->obj));
        JS::RootedValue argv(cx);
        std_string_to_jsval(cx, text, &argv);
        valArr.append(argv);
        JS::HandleValueArray args(valArr);
        
        JS::RootedValue delegateVal(cx, _JSDelegate);
        ScriptingCore::getInstance()->executeFunctionWithOwner(delegateVal, "editBoxTextChanged", args);
    }

    virtual void editBoxEditingReturn(EditBox* editBox) override
    {
        js_proxy_t * p = jsb_get_native_proxy(editBox);
        if (!p) return;
        
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS::RootedValue delegateVal(cx, _JSDelegate);
        JS::RootedValue arg(cx, JS::ObjectOrNullValue(p->obj));
        JS::HandleValueArray args(arg);
        ScriptingCore::getInstance()->executeFunctionWithOwner(delegateVal, "editBoxEditingReturn", args);
    }

    void setJSDelegate(JS::HandleValue pJSDelegate)
    {
        _JSDelegate = pJSDelegate;
    }
private:
    JS::Heap<JS::Value> _JSDelegate;
};

static bool js_cocos2dx_CCEditBox_setDelegate(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(cx, obj);
    cocos2d::ui::EditBox* cobj = (cocos2d::ui::EditBox *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "Invalid Native Object");

    if (argc == 1)
    {
        // save the delegate
        JSB_EditBoxDelegate* nativeDelegate = new (std::nothrow) JSB_EditBoxDelegate();
        nativeDelegate->setJSDelegate(args.get(0));
        
        JS_SetProperty(cx, obj, "_delegate", args.get(0));
        
        cobj->setUserObject(nativeDelegate);
        cobj->setDelegate(nativeDelegate);

        nativeDelegate->release();

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}

extern JS::PersistentRootedObject* jsb_cocos2d_ui_LayoutParameter_prototype;
extern JS::PersistentRootedObject* jsb_cocos2d_ui_EditBox_prototype;

void register_all_cocos2dx_ui_manual(JSContext* cx, JS::HandleObject global)
{
    JS::RootedObject proto(cx, jsb_cocos2d_ui_LayoutParameter_prototype->get());
    JS_DefineFunction(cx, proto, "setMargin", js_cocos2dx_LayoutParameter_setMargin, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    JS_DefineFunction(cx, proto, "getMargin", js_cocos2dx_LayoutParameter_getMargin, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);

    proto.set(jsb_cocos2d_ui_EditBox_prototype->get());
    JS_DefineFunction(cx, proto, "setDelegate", js_cocos2dx_CCEditBox_setDelegate, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
}
