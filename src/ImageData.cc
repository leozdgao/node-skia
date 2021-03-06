#include "ImageData.h"

ImageData::ImageData(const Napi::CallbackInfo& info) : Napi::ObjectWrap<ImageData>(info) {
  Napi::Number width = info[0].As<Napi::Number>();
  Napi::Number height = info[1].As<Napi::Number>();
  Napi::Buffer<unsigned char> data = info[2].As<Napi::Buffer<unsigned char>>();

  width_ = width.Int32Value();
  height_ = height.Int32Value();
  data_ = data.Data();
}

Napi::FunctionReference ImageData::constructor;

// ==================== Methods for ImageData ====================

size_t ImageData::getWidth() {
  return width_;
}

size_t ImageData::getHeight() {
  return height_;
}

void* ImageData::getData() {
  return data_;
}

// ==================== Methods for NAPI ====================

Napi::Object ImageData::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "ImageData", {
      InstanceAccessor<&ImageData::GetWidth>("width"),
      InstanceAccessor<&ImageData::GetHeight>("height"),
      InstanceAccessor<&ImageData::GetData>("data"),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("ImageData", func);

  return exports;
}

Napi::Value ImageData::CreateInstance(Napi::Env env, size_t width, size_t height, void* data) {
    return constructor.New({
      Napi::Number::New(env, width),
      Napi::Number::New(env, height),
      Napi::Buffer<unsigned char>::Copy(env, (unsigned char *)data, width * height * 4)
    });
}

Napi::Value ImageData::GetWidth(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->width_);
}

Napi::Value ImageData::GetHeight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->height_);
}

Napi::Value ImageData::GetData(const Napi::CallbackInfo& info) {
  return Napi::Buffer<unsigned char>::Copy(info.Env(), (unsigned char *)this->data_, this->width_ * this->height_ * 4);
}
