#include <napi.h>

Napi::Value getPrototype(Napi::Value object)
{
    Napi::Env env = object.Env();

    if (object.IsNull() || object.IsUndefined()) {
        return env.Null();
    }

    napi_value prototype;

    if (napi_get_prototype(env, object, &prototype) != napi_ok)
    {
        Napi::Error::New(env).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Value(env, prototype);
}

Napi::Value retrieveFunction(Napi::Value object, const char* utf8name)
{
    Napi::Env env = object.Env();

    if (object.IsObject() && !object.IsFunction())
    {
        Napi::Value p = object.As<Napi::Object>().Get(utf8name);

        if (!p.IsUndefined()) {
            return p.IsFunction() ? p : env.Undefined();
        }
    }

    Napi::Value prototype = getPrototype(object);

    while (!prototype.IsNull())
    {
        Napi::Value p = prototype.As<Napi::Object>().Get(utf8name);

        if (!p.IsUndefined()) {
            return p.IsFunction() ? p : env.Undefined();
        }

        prototype = getPrototype(prototype);
    }

    return env.Undefined();
}

Napi::Value toJSON(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    Napi::Value obj = info[0];

    Napi::Value toJSON = retrieveFunction(obj, "toJSON");

    if (toJSON.IsFunction())
    {
        Napi::String key = Napi::String::New(env, "");

        return toJSON.As<Napi::Function>().Call(obj, {key});
    }
    else
    {
        Napi::Error::New(env, "Do not found toJSON").ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("toJSON", Napi::Function::New(env, toJSON));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);

