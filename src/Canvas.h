#pragma once

#include <napi.h>

#include "CanvasRenderingContext2D.h"
#include "include/core/SkSurface.h"

class Canvas {
public:
    static napi_value Init(napi_env env, napi_value exports);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

private:
    explicit Canvas(int width, int height);
    ~Canvas();

    static inline napi_value Constructor(napi_env env);
    static napi_value New(napi_env env, napi_callback_info info);

    static napi_value GetWidth(napi_env env, napi_callback_info info);
    static napi_value SetWidth(napi_env env, napi_callback_info info);
    static napi_value GetHeight(napi_env env, napi_callback_info info);
    static napi_value SetHeight(napi_env env, napi_callback_info info);
    static napi_value GetContext(napi_env env, napi_callback_info info);
    static napi_value ToBuffer(napi_env env, napi_callback_info info);
    

    napi_env env_;
    napi_ref wrapper_;

    int width_;
    int height_;
    sk_sp<SkSurface> rasterSurface_;

    /**
     * `napi_unwrap` will failed with `napi_invalid_arg` in Canvas instance callback
     */
    napi_value ctx_ = nullptr;
    CanvasRenderingContext2D* inner_ctx = nullptr;
};
