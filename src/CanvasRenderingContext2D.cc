#include <include/core/SkImageInfo.h>
#include <include/core/SkTextBlob.h>
#include <include/effects/SkDashPathEffect.h>

#include "CanvasRenderingContext2D.h"
#include "ImageData.h"
#include "StyleParser.h"
#include "W3CSkColorParser.h"
#include "helpers.h"

using node_skia::StyleParser;

CanvasRenderingContext2D::CanvasRenderingContext2D() {
    init_canvas_state();
}

CanvasRenderingContext2D::~CanvasRenderingContext2D() {
    napi_delete_reference(env_, wrapper_);
}

void CanvasRenderingContext2D::init_canvas_state() {
    CanvasState init_state = CanvasState();
    init_state.paint_for_fill_ = SkPaint();
    init_state.paint_for_fill_.setStyle(SkPaint::kFill_Style);
    init_state.paint_for_fill_.setAntiAlias(true);

    init_state.paint_for_stroke_ = SkPaint();
    init_state.paint_for_stroke_.setStyle(SkPaint::kStroke_Style);
    init_state.paint_for_stroke_.setAntiAlias(true);
    init_state.paint_for_stroke_.setStrokeMiter(10.0);

    init_state.pargf_style_ = ParagraphStyle();
    init_state.text_style_ = TextStyle();
    init_state.text_style_.setFontSize(10);

    sk_sp<SkTypeface> face = SkTypeface::MakeFromName("sans-serif", SkFontStyle::Normal());
    init_state.text_style_.setTypeface(face);
    init_state.text_baseline_ = TextBaseline::Alphabetic;

    states_.push(init_state);
}

napi_status CanvasRenderingContext2D::Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor properties[] = {
        // properties
        DECLARE_NAPI_PROPERTY("fillStyle", GetFillStyle, SetFillStyle),
        DECLARE_NAPI_PROPERTY("globalAlpha", GetGlobalAlpha, SetGlobalAlpha),
        DECLARE_NAPI_PROPERTY("lineCap", GetLineCap, SetLineCap),
        DECLARE_NAPI_PROPERTY("lineDashOffset", GetLineDashOffset, SetLineDashOffset),
        DECLARE_NAPI_PROPERTY("lineJoin", GetLineJoin, SetLineJoin),
        DECLARE_NAPI_PROPERTY("lineWidth", GetLineWidth, SetLineWidth),
        DECLARE_NAPI_PROPERTY("miterLimit", GetMiterLimit, SetMiterLimit),
        DECLARE_NAPI_PROPERTY("strokeStyle", GetStrokeStyle, SetStrokeStyle),
        DECLARE_NAPI_PROPERTY("textAlign", GetTextAlign, SetTextAlign),
        DECLARE_NAPI_PROPERTY("textBaseline", GetTextBaseline, SetTextBaseline),
        // methods
        DECLARE_NAPI_METHOD("arc", Arc),
        DECLARE_NAPI_METHOD("arcTo", ArcTo),
        DECLARE_NAPI_METHOD("beginPath", BeginPath),
        DECLARE_NAPI_METHOD("bezierCurveTo", BezierCurveTo),
        DECLARE_NAPI_METHOD("createImageData", CreateImageData),
        DECLARE_NAPI_METHOD("createPattern", CreatePattern),
        DECLARE_NAPI_METHOD("clearRect", ClearRect),
        DECLARE_NAPI_METHOD("closePath", ClosePath),
        DECLARE_NAPI_METHOD("drawImage", DrawImage),
        DECLARE_NAPI_METHOD("fill", Fill),
        DECLARE_NAPI_METHOD("fillRect", FillRect),
        DECLARE_NAPI_METHOD("fillWithPath2D", FillWithPath2D),
        DECLARE_NAPI_METHOD("fillText", FillText),
        DECLARE_NAPI_METHOD("getImageData", GetImageData),
        DECLARE_NAPI_METHOD("getLineDash", GetLineDash),
        DECLARE_NAPI_METHOD("lineTo", LineTo),
        DECLARE_NAPI_METHOD("moveTo", MoveTo),
        DECLARE_NAPI_METHOD("putImageData", PutImageData),
        DECLARE_NAPI_METHOD("quadraticCurveTo", QuadraticCurveTo),
        DECLARE_NAPI_METHOD("rect", Rect),
        DECLARE_NAPI_METHOD("restore", Restore),
        DECLARE_NAPI_METHOD("save", Save),
        DECLARE_NAPI_METHOD("setLineDash", SetLineDash),
        DECLARE_NAPI_METHOD("stroke", Stroke),
        DECLARE_NAPI_METHOD("strokeRect", StrokeRect),
        DECLARE_NAPI_METHOD("strokeWithPath2D", StrokeWithPath2D),
        DECLARE_NAPI_METHOD("strokeText", StrokeText),
        DECLARE_NAPI_METHOD("measureText", MeasureText),
    };

    napi_value cons;
    status = napi_define_class(env, "CanvasRenderingContext2D", NAPI_AUTO_LENGTH, New, nullptr, 38, properties, &cons);
    assert(status == napi_ok);

    napi_ref* constructor = new napi_ref;
    status = napi_create_reference(env, cons, 1, constructor);
    assert(status == napi_ok);

    status = napi_set_instance_data(
      env,
      constructor,
      [](napi_env env, void* data, void* hint) {
        napi_ref* constructor = static_cast<napi_ref*>(data);
        napi_status status = napi_delete_reference(env, *constructor);
        assert(status == napi_ok);
        delete constructor;
      },
      nullptr
    );
    assert(status == napi_ok);

    return napi_ok;
}

/**
 * 获取构造函数
 */
napi_value CanvasRenderingContext2D::Constructor(napi_env env) {
    void* instance_data = nullptr;
    napi_status status = napi_get_instance_data(env, &instance_data);
    assert(status == napi_ok);
    napi_ref* constructor = static_cast<napi_ref*>(instance_data);

    napi_value cons;
    status = napi_get_reference_value(env, *constructor, &cons);
    assert(status == napi_ok);

    return cons;
}

/**
 * 实例构造逻辑
 */
napi_value CanvasRenderingContext2D::New(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 0;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, nullptr, &jsthis, nullptr);

    CanvasRenderingContext2D* ctx = new CanvasRenderingContext2D();
    ctx->env_ = env;
    status = napi_wrap(env, jsthis, reinterpret_cast<void*>(ctx), CanvasRenderingContext2D::Destructor, nullptr, &ctx->wrapper_);
    assert(status == napi_ok);

    return jsthis;
}

/**
 * 暴露给 JS 的工厂方法
 */
napi_status CanvasRenderingContext2D::NewInstance(napi_env env, napi_value* result) {
    napi_status status;
    status = napi_new_instance(env, Constructor(env), 0, nullptr, result);
    assert(status == napi_ok);

    return napi_ok;
}

void CanvasRenderingContext2D::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    reinterpret_cast<CanvasRenderingContext2D*>(nativeObject)->~CanvasRenderingContext2D();
}

void CanvasRenderingContext2D::SetCanvas(SkCanvas* canvas) {
    canvas_ = canvas;
};

// ================================== Properties ==================================

napi_value CanvasRenderingContext2D::GetFillStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->states_.top().paint_for_fill_.getColor4f();

    napi_value result, r, g, b, a;
    status = napi_create_array(env, &result);
    status = napi_create_double(env, color.fR, &r);
    status = napi_create_double(env, color.fG, &g);
    status = napi_create_double(env, color.fB, &b);
    status = napi_create_double(env, color.fA, &a);

    status = napi_set_element(env, result, 0, r);
    status = napi_set_element(env, result, 1, g);
    status = napi_set_element(env, result, 2, b);
    status = napi_set_element(env, result, 3, a);

    return result;
}

napi_value CanvasRenderingContext2D::SetFillStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string fill_style = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkColor4f fill_style_color = W3CSkColorParser::rgba_from_string(fill_style);

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    W3CSkColorParser::color_mix_with_alpha(fill_style_color, ctx->states_.top().global_alpha_);

    ctx->states_.top().paint_for_fill_.setColor4f(fill_style_color);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetGlobalAlpha(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    status = napi_create_double(env, ctx->states_.top().global_alpha_, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetGlobalAlpha(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double alpha = 1;
    status = napi_get_value_double(env, argv[0], &alpha);
    
    ctx->states_.top().global_alpha_ = alpha;
    ctx->states_.top().paint_for_fill_.setAlphaf(alpha);
    ctx->states_.top().paint_for_stroke_.setAlphaf(alpha);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineCap(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    SkPaint::Cap cap = ctx->states_.top().paint_for_stroke_.getStrokeCap();
    string capStr = StyleParser::fromStrokeCapToStr(cap);
    
    return Napi::String::New(env, capStr);
}

napi_value CanvasRenderingContext2D::SetLineCap(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string cap = node_skia_helpers::get_utf8_string(env, argv[0]);
    ctx->states_.top().paint_for_stroke_.setStrokeCap(StyleParser::fromStrToStrokeCap(cap));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineDashOffset(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    double val = ctx->states_.top().line_dash_offset;
    
    return Napi::Number::New(env, val);
}

napi_value CanvasRenderingContext2D::SetLineDashOffset(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double line_dash_offset;
    status = napi_get_value_double(env, argv[0], &line_dash_offset);
    ctx->states_.top().line_dash_offset = line_dash_offset;

    sk_sp<SkPathEffect> effect = SkDashPathEffect::Make(ctx->states_.top().intervals.data(), 2, line_dash_offset);
    ctx->states_.top().paint_for_stroke_.setPathEffect(effect);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineJoin(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    SkPaint::Join join = ctx->states_.top().paint_for_stroke_.getStrokeJoin();
    string joinStr = StyleParser::fromStrokeJoinToStr(join);
    
    return Napi::String::New(env, joinStr);
}

napi_value CanvasRenderingContext2D::SetLineJoin(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string join = node_skia_helpers::get_utf8_string(env, argv[0]);
    ctx->states_.top().paint_for_stroke_.setStrokeJoin(StyleParser::fromStrToStrokeJoin(join));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetLineWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().paint_for_stroke_.getStrokeWidth();
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetLineWidth(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double line_width;
    status = napi_get_value_double(env, argv[0], &line_width);
    
    ctx->states_.top().paint_for_stroke_.setStrokeWidth(line_width);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetMiterLimit(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    napi_value result;

    double v = ctx->states_.top().paint_for_stroke_.getStrokeMiter();
    status = napi_create_double(env, v, &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetMiterLimit(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double miter;
    status = napi_get_value_double(env, argv[0], &miter);
    
    ctx->states_.top().paint_for_stroke_.setStrokeMiter(miter);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetStrokeStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    SkColor4f color = ctx->states_.top().paint_for_stroke_.getColor4f();

    napi_value result, r, g, b, a;
    status = napi_create_array(env, &result);
    status = napi_create_double(env, color.fR, &r);
    status = napi_create_double(env, color.fG, &g);
    status = napi_create_double(env, color.fB, &b);
    status = napi_create_double(env, color.fA, &a);

    status = napi_set_element(env, result, 0, r);
    status = napi_set_element(env, result, 1, g);
    status = napi_set_element(env, result, 2, b);
    status = napi_set_element(env, result, 3, a);

    return result;
}

napi_value CanvasRenderingContext2D::SetStrokeStyle(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string stroke_style = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkColor4f stroke_style_color = W3CSkColorParser::rgba_from_string(stroke_style);

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    stroke_style_color.fA = ctx->states_.top().global_alpha_ * stroke_style_color.fA;

    ctx->states_.top().paint_for_stroke_.setColor4f(stroke_style_color);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextAlign(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextAlign align = ctx->states_.top().pargf_style_.getTextAlign();
    string align_name = StyleParser::fromTextAlignToStr(align);

    napi_value result;
    napi_create_string_utf8(env, align_name.c_str(), align_name.size(), &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetTextAlign(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string text_align = node_skia_helpers::get_utf8_string(env, argv[0]);
    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    ctx->states_.top().pargf_style_.setTextAlign(StyleParser::fromStrToTextAlign(text_align));

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetTextBaseline(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    TextBaseline baseline = ctx->states_.top().text_baseline_;
    string baseline_name = StyleParser::fromTextBaselineToStr(baseline);

    napi_value result;
    napi_create_string_utf8(env, baseline_name.c_str(), baseline_name.size(), &result);

    return result;
}

napi_value CanvasRenderingContext2D::SetTextBaseline(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    string text_baseline = node_skia_helpers::get_utf8_string(env, argv[0]);
    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    ctx->states_.top().text_baseline_ = StyleParser::fromStrToTextBaseline(text_baseline);

    return nullptr;
}

// ================================== Methods ==================================

napi_value CanvasRenderingContext2D::Arc(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 5)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, radius, start_angle, end_angle;

    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &radius);
    status = napi_get_value_double(env, argv[3], &start_angle);
    status = napi_get_value_double(env, argv[4], &end_angle);

    double ax = x + radius;
    double ay = y - radius;

    SkRect oval = SkRect::MakeXYWH(ax, ay, radius * 2, radius * 2);
    SkPoint move_point = SkPoint::Make(x, y);

    if (ctx->states_.top().last_move_point_ && move_point != *ctx->states_.top().last_move_point_) {
        ctx->states_.top().path_.lineTo(move_point);
    }

    ctx->states_.top().path_.moveTo(move_point);
    ctx->states_.top().last_move_point_ = &move_point;

    ctx->states_.top().path_.arcTo(oval, start_angle, end_angle - start_angle, true);

    return nullptr;
}

napi_value CanvasRenderingContext2D::ArcTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 5)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x1, y1, x2, y2, radius;

    status = napi_get_value_double(env, argv[0], &x1);
    status = napi_get_value_double(env, argv[1], &y1);
    status = napi_get_value_double(env, argv[2], &x2);
    status = napi_get_value_double(env, argv[3], &y2);
    status = napi_get_value_double(env, argv[4], &radius);

    ctx->states_.top().path_.arcTo(SkPoint::Make(x1, y1), SkPoint::Make(x2, y2), radius);

    return nullptr;
}

napi_value CanvasRenderingContext2D::BeginPath(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->states_.top().path_.reset();
    ctx->states_.top().last_move_point_ = nullptr;

    return nullptr;
}

napi_value CanvasRenderingContext2D::BezierCurveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 6)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double cp1x, cp1y, cp2x, cp2y, x, y;

    status = napi_get_value_double(env, argv[0], &cp1x);
    status = napi_get_value_double(env, argv[1], &cp1y);
    status = napi_get_value_double(env, argv[2], &cp2x);
    status = napi_get_value_double(env, argv[3], &cp2y);
    status = napi_get_value_double(env, argv[4], &x);
    status = napi_get_value_double(env, argv[5], &y);

    ctx->states_.top().path_.cubicTo(SkPoint::Make(cp1x, cp1y), SkPoint::Make(cp2x, cp2y), SkPoint::Make(x, y));

    return nullptr;
}

napi_value CanvasRenderingContext2D::CreateImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    int width, height;

    status = napi_get_value_int32(env, argv[0], &width);
    status = napi_get_value_int32(env, argv[1], &height);

    SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(width, height), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    sk_sp<SkData> data(SkData::MakeUninitialized(image_info.minRowBytes() * image_info.height()));
    sk_bzero(data->writable_data(), image_info.minRowBytes() * image_info.height());

    return ImageData::CreateInstance(env, width, height, data->writable_data());
}

napi_value CanvasRenderingContext2D::CreatePattern(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    napi_value result;
    // 判断输入是一个 Image
    

    return result;
}

napi_value CanvasRenderingContext2D::ClearRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);
    SkPaint p = SkPaint();
    p.setBlendMode(SkBlendMode::kClear);

    ctx->canvas_->drawRect(rect, p);

    return nullptr;
}

napi_value CanvasRenderingContext2D::ClosePath(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->states_.top().path_.close();

    return nullptr;
}

napi_value CanvasRenderingContext2D::DrawImage(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 9)
    // img, sx, sy, sW, sH, dx, dy, dW, dH

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double sx, sy, sW, sH, dx, dy, dW, dH;
    void* data;
    size_t len;

    status = napi_get_buffer_info(env, argv[0], &data, &len);
    status = napi_get_value_double(env, argv[1], &sx);
    status = napi_get_value_double(env, argv[2], &sy);
    status = napi_get_value_double(env, argv[3], &sW);
    status = napi_get_value_double(env, argv[4], &sH);
    status = napi_get_value_double(env, argv[5], &dx);
    status = napi_get_value_double(env, argv[6], &dy);
    status = napi_get_value_double(env, argv[7], &dW);
    status = napi_get_value_double(env, argv[8], &dH);

    sk_sp<SkImage> img = SkImage::MakeFromEncoded(
        SkData::MakeWithoutCopy(data, len)
    );
    SkRect src = SkRect();
    src.setXYWH(sx, sy, sW, sH);
    SkRect dist = SkRect();
    dist.setXYWH(dx, dy, dW, dH);

    SkSamplingOptions sample_options = SkSamplingOptions();

    ctx->canvas_->drawImageRect(img, src, dist, sample_options, &ctx->states_.top().paint_for_fill_, SkCanvas::kFast_SrcRectConstraint);

    return nullptr;
}

napi_value CanvasRenderingContext2D::Fill(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->canvas_->drawPath(ctx->states_.top().path_, ctx->states_.top().paint_for_fill_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::FillRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);

    ctx->canvas_->drawRect(rect, ctx->states_.top().paint_for_fill_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::FillWithPath2D(napi_env env, napi_callback_info info) {
    return nullptr;
}

napi_value CanvasRenderingContext2D::FillText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string input = node_skia_helpers::get_utf8_string(env, argv[0]);
    auto text = SkTextBlob::MakeFromString(input.data(), SkFont(nullptr, 18));

    ctx->canvas_->drawTextBlob(text.get(), 50, 25, ctx->states_.top().paint_for_fill_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::GetImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    int sx, sy, sw, sh;
    status = napi_get_value_int32(env, argv[0], &sx);
    status = napi_get_value_int32(env, argv[1], &sy);
    status = napi_get_value_int32(env, argv[2], &sw);
    status = napi_get_value_int32(env, argv[3], &sh);

    SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(sw, sh), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    sk_sp<SkData> data(SkData::MakeUninitialized(image_info.minRowBytes() * image_info.height()));
    sk_bzero(data->writable_data(), image_info.minRowBytes() * image_info.height());
    ctx->canvas_->readPixels(image_info, data->writable_data(), image_info.minRowBytes(), sx, sy);

    return ImageData::CreateInstance(env, image_info.width(), image_info.height(), data->writable_data());
}

napi_value CanvasRenderingContext2D::GetLineDash(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    vector<SkScalar> intervals = ctx->states_.top().intervals;
    Napi::Array array = Napi::Array::New(env, intervals.size());

    for (int i = 0, l = intervals.size(); i < l; i++) {
        array.Set(i, intervals[i]);
    }

    return array;
}

napi_value CanvasRenderingContext2D::LineTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    if (ctx->states_.top().last_move_point_ != nullptr) {
        double x, y;
        status = napi_get_value_double(env, argv[0], &x);
        status = napi_get_value_double(env, argv[1], &y);

        ctx->states_.top().path_.lineTo(SkPoint::Make(x, y));
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::MoveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 2)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);

    SkPoint move_point = SkPoint::Make(x, y);
    ctx->states_.top().last_move_point_ = &move_point;
    ctx->states_.top().path_.moveTo(move_point);

    return nullptr;
}

napi_value CanvasRenderingContext2D::PutImageData(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 3)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    int dx, dy;
    ImageData* imgData;
    
    status = napi_unwrap(env, argv[0], reinterpret_cast<void**>(&imgData));
    status = napi_get_value_int32(env, argv[1], &dx);
    status = napi_get_value_int32(env, argv[2], &dy);

    SkImageInfo image_info = SkImageInfo::Make(SkISize::Make(imgData->getWidth(), imgData->getHeight()), kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    ctx->canvas_->writePixels(image_info, imgData->getData(), image_info.minRowBytes(), dx, dy);

    return nullptr;
}

napi_value CanvasRenderingContext2D::QuadraticCurveTo(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    double cpx, cpy, x, y;
    status = napi_get_value_double(env, argv[0], &cpx);
    status = napi_get_value_double(env, argv[1], &cpy);
    status = napi_get_value_double(env, argv[2], &x);
    status = napi_get_value_double(env, argv[3], &y);

    ctx->states_.top().path_.quadTo(SkPoint::Make(cpx, cpy), SkPoint::Make(x, y));

    return nullptr;
}

napi_value CanvasRenderingContext2D::Rect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkPoint move_point = SkPoint::Make(x, y);

    if (ctx->states_.top().last_move_point_ && move_point != *ctx->states_.top().last_move_point_) {
        ctx->states_.top().path_.lineTo(move_point);
    }

    ctx->states_.top().path_.moveTo(move_point);
    ctx->states_.top().last_move_point_ = &move_point;

    ctx->states_.top().path_.addRect(SkRect::MakeXYWH(x, y, w, h));

    return nullptr;
}

napi_value CanvasRenderingContext2D::Restore(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    
    if (ctx->states_.size() > 1) {
        ctx->states_.pop();
        ctx->canvas_->restore();
    }

    return nullptr;
}

napi_value CanvasRenderingContext2D::Save(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));
    // 所谓的保持状态，即是 Copy 当前状态并压栈
    
    CanvasState new_state = ctx->states_.top();
    ctx->states_.push(new_state);
    ctx->canvas_->save();

    return nullptr;
}

napi_value CanvasRenderingContext2D::SetLineDash(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    Napi::Array array = Napi::Array(env, argv[0]);
    int len = array.Length();
    vector<SkScalar> intervals = {};
    for (int i = 0; i < len; i++) {
        float val = array.Get(i).As<Napi::Number>().FloatValue();
        intervals.push_back(val);
    }

    ctx->states_.top().intervals = intervals;

    sk_sp<SkPathEffect> effect = SkDashPathEffect::Make(
        ctx->states_.top().intervals.data(),
        intervals.size(),
        ctx->states_.top().line_dash_offset
    );
    ctx->states_.top().paint_for_stroke_.setPathEffect(effect);

    return nullptr;
}

napi_value CanvasRenderingContext2D::Stroke(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO_WITHOUT_ARG(env, info, status)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    ctx->canvas_->drawPath(ctx->states_.top().path_, ctx->states_.top().paint_for_stroke_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::StrokeRect(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 4)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    double x, y, w, h;
    status = napi_get_value_double(env, argv[0], &x);
    status = napi_get_value_double(env, argv[1], &y);
    status = napi_get_value_double(env, argv[2], &w);
    status = napi_get_value_double(env, argv[3], &h);

    SkRect rect = SkRect::MakeXYWH(x, y, w, h);

    ctx->canvas_->drawRect(rect, ctx->states_.top().paint_for_stroke_);

    return nullptr;
}

napi_value CanvasRenderingContext2D::StrokeWithPath2D(napi_env env, napi_callback_info info) {
    return nullptr;
}

napi_value CanvasRenderingContext2D::StrokeText(napi_env env, napi_callback_info info) {
    return nullptr;
}

// FIXME: It's really complex, do it later
napi_value CanvasRenderingContext2D::MeasureText(napi_env env, napi_callback_info info) {
    napi_status status;
    GET_CB_INFO(env, info, status, 1)

    CanvasRenderingContext2D* ctx;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&ctx));

    string text = node_skia_helpers::get_utf8_string(env, argv[0]);
    SkFontMetrics font_metrics;
    ctx->states_.top().text_style_.getFontMetrics(&font_metrics);
    // 新基线到原基线的距离
    float offset = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, ctx->states_.top().text_baseline_);
    auto base_hang = TextBaseline::Hanging;
    float hang = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, base_hang) - offset;
    auto base_ideo = TextBaseline::Ideographic;
    float ideo = StyleParser::getBaselineOffsetFromFontMetrics(font_metrics, base_ideo) - offset;
    // 原基线需要偏移的距离
    float norm = 0 - offset;
    float ascent = norm - font_metrics.fAscent;
    float descent = font_metrics.fDescent - norm;

    // FIXME: font
    // sk_sp<SkTextBlob> tb = SkTextBlob::MakeFromString(text.data(), SkFont(nullptr, 10));
    // SkRect bounds = tb->bounds();

    napi_value text_metrics, v_norm, v_ascent, v_descent, v_hang, v_ideo;
    status = napi_create_double(env, norm, &v_norm);
    status = napi_create_double(env, ascent, &v_ascent);
    status = napi_create_double(env, descent, &v_descent);
    status = napi_create_double(env, hang, &v_hang);
    status = napi_create_double(env, ideo, &v_ideo);

    status = napi_create_object(env, &text_metrics);
    // status = napi_set_named_property(env, text_metrics, "actualBoundingBoxAscent", v_ascent);
    // status = napi_set_named_property(env, text_metrics, "actualBoundingBoxDescent", v_descent);
    status = napi_set_named_property(env, text_metrics, "emHeightAscent", v_ascent);
    status = napi_set_named_property(env, text_metrics, "emHeightDescent", v_descent);
    status = napi_set_named_property(env, text_metrics, "hangingBaseline", v_hang);
    status = napi_set_named_property(env, text_metrics, "alphabeticBaseline", v_norm);
    status = napi_set_named_property(env, text_metrics, "ideographicBaseline", v_ideo);

    sk_sp<SkTypeface> face = SkTypeface::MakeFromName("sans-serif", SkFontStyle::Normal());
    auto font = SkFont(face, 10);

    auto txt = SkTextBlob::MakeFromString(text.c_str(), font);
    ctx->canvas_->drawTextBlob(txt, 10, 10, ctx->states_.top().paint_for_fill_);

    return text_metrics;
}
