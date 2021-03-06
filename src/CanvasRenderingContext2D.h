#pragma once

#include <stack>
#include <vector>
#include <napi.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPath.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "StyleParser.h"

using std::stack;
using std::vector;
using skia::textlayout::ParagraphStyle;
using skia::textlayout::TextStyle;
using node_skia::TextBaseline;


struct CanvasState
{
    // state for path
    SkPath path_;
    SkPoint* last_move_point_ = nullptr;
    
    vector<SkScalar> intervals = {};
    double line_dash_offset = 0.0;

    // state for fill & stroke
    SkPaint paint_for_fill_;
    SkPaint paint_for_stroke_;
    double global_alpha_ = 1.0;

    // state for text
    ParagraphStyle pargf_style_;
    TextStyle text_style_;
    TextBaseline text_baseline_;
};


class CanvasRenderingContext2D {
public:
    static napi_status Init(napi_env env, napi_value exports);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status NewInstance(napi_env env, napi_value* result);
    static napi_value Constructor(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

    void SetCanvas(SkCanvas* canvas);

private:
    explicit CanvasRenderingContext2D();
    ~CanvasRenderingContext2D();

    napi_env env_;
    napi_ref wrapper_;

    SkCanvas* canvas_;

    stack<CanvasState> states_;

    void init_canvas_state();

    // ================================== Properties ==================================

    static napi_value GetFillStyle(napi_env env, napi_callback_info info);
    static napi_value SetFillStyle(napi_env env, napi_callback_info info);
    static napi_value GetGlobalAlpha(napi_env env, napi_callback_info info);
    static napi_value SetGlobalAlpha(napi_env env, napi_callback_info info);
    static napi_value GetLineCap(napi_env env, napi_callback_info info);
    static napi_value SetLineCap(napi_env env, napi_callback_info info);
    static napi_value GetLineDashOffset(napi_env env, napi_callback_info info);
    static napi_value SetLineDashOffset(napi_env env, napi_callback_info info);
    static napi_value GetLineJoin(napi_env env, napi_callback_info info);
    static napi_value SetLineJoin(napi_env env, napi_callback_info info);
    static napi_value GetLineWidth(napi_env env, napi_callback_info info);
    static napi_value SetLineWidth(napi_env env, napi_callback_info info);
    static napi_value GetMiterLimit(napi_env env, napi_callback_info info);
    static napi_value SetMiterLimit(napi_env env, napi_callback_info info);
    static napi_value GetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value SetStrokeStyle(napi_env env, napi_callback_info info);
    static napi_value GetTextAlign(napi_env env, napi_callback_info info);
    static napi_value SetTextAlign(napi_env env, napi_callback_info info);
    static napi_value GetTextBaseline(napi_env env, napi_callback_info info);
    static napi_value SetTextBaseline(napi_env env, napi_callback_info info);

    // ================================== Methods ==================================

    static napi_value Arc(napi_env env, napi_callback_info info);
    static napi_value ArcTo(napi_env env, napi_callback_info info);
    static napi_value BeginPath(napi_env env, napi_callback_info info);
    static napi_value BezierCurveTo(napi_env env, napi_callback_info info);
    static napi_value CreateImageData(napi_env env, napi_callback_info info);
    static napi_value CreatePattern(napi_env env, napi_callback_info info);
    static napi_value ClearRect(napi_env env, napi_callback_info info);
    static napi_value ClosePath(napi_env env, napi_callback_info info);
    static napi_value DrawImage(napi_env env, napi_callback_info info);
    static napi_value Fill(napi_env env, napi_callback_info info);
    static napi_value FillRect(napi_env env, napi_callback_info info);
    static napi_value FillWithPath2D(napi_env env, napi_callback_info info); // work for `ctx.fill()`
    static napi_value FillText(napi_env env, napi_callback_info info);
    static napi_value GetImageData(napi_env env, napi_callback_info info);
    static napi_value GetLineDash(napi_env env, napi_callback_info info);
    static napi_value LineTo(napi_env env, napi_callback_info info);
    static napi_value MoveTo(napi_env env, napi_callback_info info);
    static napi_value PutImageData(napi_env env, napi_callback_info info);
    static napi_value QuadraticCurveTo(napi_env env, napi_callback_info info);
    static napi_value Rect(napi_env env, napi_callback_info info);
    static napi_value Restore(napi_env env, napi_callback_info info);
    static napi_value Save(napi_env env, napi_callback_info info);
    static napi_value SetLineDash(napi_env env, napi_callback_info info);
    static napi_value Stroke(napi_env env, napi_callback_info info);
    static napi_value StrokeRect(napi_env env, napi_callback_info info);
    static napi_value StrokeWithPath2D(napi_env env, napi_callback_info info); // work for `ctx.stroke()`
    static napi_value StrokeText(napi_env env, napi_callback_info info);

    static napi_value MeasureText(napi_env env, napi_callback_info info);
};

