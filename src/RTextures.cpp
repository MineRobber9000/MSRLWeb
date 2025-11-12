//
//  RTextures.cpp
//  MSRLWeb
//
//  Raylib Textures module intrinsics
//

#include "RaylibIntrinsics.h"
#include "RaylibTypes.h"
#include "raylib.h"
#include "MiniscriptInterpreter.h"
#include "MiniscriptTypes.h"

using namespace MiniScript;

#define INTRINSIC_LAMBDA [](Context *context, IntrinsicResult partialResult) -> IntrinsicResult

void AddRTexturesMethods(ValueDict raylibModule) {
	Intrinsic *i;

	// Image loading

	i = Intrinsic::Create("");
	i->AddParam("fileName");
	i->code = INTRINSIC_LAMBDA {
		String path = context->GetVar(String("fileName")).ToString();
		Image img = LoadImage(path.c_str());
		if (!IsImageValid(img)) return IntrinsicResult::Null;
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("LoadImage", i->GetFunc());

	// Image generation

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("direction", Value::zero);
	i->AddParam("start", ColorToValue(BLACK));
	i->AddParam("end", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		int direction = context->GetVar(String("direction")).IntValue();
		Color start = ValueToColor(context->GetVar(String("start")));
		Color end = ValueToColor(context->GetVar(String("end")));
		Image img = GenImageGradientLinear(width, height, direction, start, end);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageGradientLinear", i->GetFunc());

	// Image management

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		UnloadImage(img);
		// Free the heap-allocated Image struct
		ValueDict map = context->GetVar(String("image")).GetDict();
		Value handleVal = map.Lookup(String("_handle"), Value::zero);
		Image* imgPtr = (Image*)(long)handleVal.IntValue();
		delete imgPtr;
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("UnloadImage", i->GetFunc());

	// Texture loading

	i = Intrinsic::Create("");
	i->AddParam("fileName");
	i->code = INTRINSIC_LAMBDA {
		String path = context->GetVar(String("fileName")).ToString();
		Texture tex = LoadTexture(path.c_str());
		if (!IsTextureValid(tex)) return IntrinsicResult::Null;
		return IntrinsicResult(TextureToValue(tex));
	};
	raylibModule.SetValue("LoadTexture", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		Texture tex = LoadTextureFromImage(img);
		return IntrinsicResult(TextureToValue(tex));
	};
	raylibModule.SetValue("LoadTextureFromImage", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		UnloadTexture(tex);
		// Free the heap-allocated Texture struct
		ValueDict map = context->GetVar(String("texture")).GetDict();
		Value handleVal = map.Lookup(String("_handle"), Value::zero);
		Texture* texPtr = (Texture*)(long)handleVal.IntValue();
		delete texPtr;
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("UnloadTexture", i->GetFunc());

	// Texture drawing

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("posX", Value::zero);
	i->AddParam("posY", Value::zero);
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		int posX = context->GetVar(String("posX")).IntValue();
		int posY = context->GetVar(String("posY")).IntValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTexture(tex, posX, posY, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTexture", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextureV(tex, position, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextureV", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("rotation", Value::zero);
	i->AddParam("scale", Value(1.0));
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		float rotation = context->GetVar(String("rotation")).FloatValue();
		float scale = context->GetVar(String("scale")).FloatValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextureEx(tex, position, rotation, scale, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextureEx", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("source");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		Rectangle source = ValueToRectangle(context->GetVar(String("source")));
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextureRec(tex, source, position, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextureRec", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("source");
	i->AddParam("dest");
	i->AddParam("origin", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("rotation", Value::zero);
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		Rectangle source = ValueToRectangle(context->GetVar(String("source")));
		Rectangle dest = ValueToRectangle(context->GetVar(String("dest")));
		Vector2 origin = ValueToVector2(context->GetVar(String("origin")));
		float rotation = context->GetVar(String("rotation")).FloatValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTexturePro(tex, source, dest, origin, rotation, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTexturePro", i->GetFunc());

	// More image generation functions

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		Image img = GenImageColor(width, height, color);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageColor", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("density", Value(0.5));
	i->AddParam("inner", ColorToValue(WHITE));
	i->AddParam("outer", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		float density = context->GetVar(String("density")).FloatValue();
		Color inner = ValueToColor(context->GetVar(String("inner")));
		Color outer = ValueToColor(context->GetVar(String("outer")));
		Image img = GenImageGradientRadial(width, height, density, inner, outer);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageGradientRadial", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("density", Value(0.5));
	i->AddParam("inner", ColorToValue(WHITE));
	i->AddParam("outer", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		float density = context->GetVar(String("density")).FloatValue();
		Color inner = ValueToColor(context->GetVar(String("inner")));
		Color outer = ValueToColor(context->GetVar(String("outer")));
		Image img = GenImageGradientSquare(width, height, density, inner, outer);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageGradientSquare", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("checksX", Value(8));
	i->AddParam("checksY", Value(8));
	i->AddParam("col1", ColorToValue(WHITE));
	i->AddParam("col2", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		int checksX = context->GetVar(String("checksX")).IntValue();
		int checksY = context->GetVar(String("checksY")).IntValue();
		Color col1 = ValueToColor(context->GetVar(String("col1")));
		Color col2 = ValueToColor(context->GetVar(String("col2")));
		Image img = GenImageChecked(width, height, checksX, checksY, col1, col2);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageChecked", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("factor", Value(0.5));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		float factor = context->GetVar(String("factor")).FloatValue();
		Image img = GenImageWhiteNoise(width, height, factor);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageWhiteNoise", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("tileSize", Value(32));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		int tileSize = context->GetVar(String("tileSize")).IntValue();
		Image img = GenImageCellular(width, height, tileSize);
		return IntrinsicResult(ImageToValue(img));
	};
	raylibModule.SetValue("GenImageCellular", i->GetFunc());

	// Image manipulation

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		Image copy = ImageCopy(img);
		return IntrinsicResult(ImageToValue(copy));
	};
	raylibModule.SetValue("ImageCopy", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("crop");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		Rectangle crop = ValueToRectangle(context->GetVar(String("crop")));
		ImageCrop(&img, crop);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageCrop", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("newWidth");
	i->AddParam("newHeight");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		int newWidth = context->GetVar(String("newWidth")).IntValue();
		int newHeight = context->GetVar(String("newHeight")).IntValue();
		ImageResize(&img, newWidth, newHeight);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageResize", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("newWidth");
	i->AddParam("newHeight");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		int newWidth = context->GetVar(String("newWidth")).IntValue();
		int newHeight = context->GetVar(String("newHeight")).IntValue();
		ImageResizeNN(&img, newWidth, newHeight);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageResizeNN", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageFlipVertical(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageFlipVertical", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageFlipHorizontal(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageFlipHorizontal", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageRotateCW(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageRotateCW", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageRotateCCW(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageRotateCCW", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageColorTint(&img, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageColorTint", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageColorInvert(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageColorInvert", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		ImageColorGrayscale(&img);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageColorGrayscale", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("contrast");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		float contrast = context->GetVar(String("contrast")).FloatValue();
		ImageColorContrast(&img, contrast);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageColorContrast", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("brightness");
	i->code = INTRINSIC_LAMBDA {
		Image img = ValueToImage(context->GetVar(String("image")));
		int brightness = context->GetVar(String("brightness")).IntValue();
		ImageColorBrightness(&img, brightness);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageColorBrightness", i->GetFunc());

	// Image drawing functions

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageClearBackground(&dst, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageClearBackground", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("x", Value::zero);
	i->AddParam("y", Value::zero);
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		int x = context->GetVar(String("x")).IntValue();
		int y = context->GetVar(String("y")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawPixel(&dst, x, y, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawPixel", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawPixelV(&dst, position, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawPixelV", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("startPosX", Value::zero);
	i->AddParam("startPosY", Value::zero);
	i->AddParam("endPosX", Value::zero);
	i->AddParam("endPosY", Value::zero);
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		int startPosX = context->GetVar(String("startPosX")).IntValue();
		int startPosY = context->GetVar(String("startPosY")).IntValue();
		int endPosX = context->GetVar(String("endPosX")).IntValue();
		int endPosY = context->GetVar(String("endPosY")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawLine(&dst, startPosX, startPosY, endPosX, endPosY, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawLine", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("start", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("end", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Vector2 start = ValueToVector2(context->GetVar(String("start")));
		Vector2 end = ValueToVector2(context->GetVar(String("end")));
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawLineV(&dst, start, end, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawLineV", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("centerX", Value(100));
	i->AddParam("centerY", Value(100));
	i->AddParam("radius", Value(32));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		int centerX = context->GetVar(String("centerX")).IntValue();
		int centerY = context->GetVar(String("centerY")).IntValue();
		int radius = context->GetVar(String("radius")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawCircle(&dst, centerX, centerY, radius, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawCircle", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("center", Vector2ToValue(Vector2{100, 100}));
	i->AddParam("radius", Value(32));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Vector2 center = ValueToVector2(context->GetVar(String("center")));
		int radius = context->GetVar(String("radius")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawCircleV(&dst, center, radius, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawCircleV", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("posX", Value::zero);
	i->AddParam("posY", Value::zero);
	i->AddParam("width", Value(256));
	i->AddParam("height", Value(256));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		int posX = context->GetVar(String("posX")).IntValue();
		int posY = context->GetVar(String("posY")).IntValue();
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawRectangle(&dst, posX, posY, width, height, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawRectangle", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("rec");
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Rectangle rec = ValueToRectangle(context->GetVar(String("rec")));
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawRectangleRec(&dst, rec, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawRectangleRec", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("rec");
	i->AddParam("thick", Value(1));
	i->AddParam("color", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Rectangle rec = ValueToRectangle(context->GetVar(String("rec")));
		int thick = context->GetVar(String("thick")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawRectangleLines(&dst, rec, thick, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawRectangleLines", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("src");
	i->AddParam("srcRec");
	i->AddParam("dstRec");
	i->AddParam("tint", ColorToValue(WHITE));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		Image src = ValueToImage(context->GetVar(String("src")));
		Rectangle srcRec = ValueToRectangle(context->GetVar(String("srcRec")));
		Rectangle dstRec = ValueToRectangle(context->GetVar(String("dstRec")));
		Color tint = ValueToColor(context->GetVar(String("tint")));
		ImageDraw(&dst, src, srcRec, dstRec, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDraw", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("dst");
	i->AddParam("text");
	i->AddParam("posX", Value::zero);
	i->AddParam("posY", Value::zero);
	i->AddParam("fontSize", Value(20));
	i->AddParam("color", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		Image dst = ValueToImage(context->GetVar(String("dst")));
		String text = context->GetVar(String("text")).ToString();
		int posX = context->GetVar(String("posX")).IntValue();
		int posY = context->GetVar(String("posY")).IntValue();
		int fontSize = context->GetVar(String("fontSize")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		ImageDrawText(&dst, text.c_str(), posX, posY, fontSize, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ImageDrawText", i->GetFunc());

	// Texture configuration

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("filter");
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		int filter = context->GetVar(String("filter")).IntValue();
		SetTextureFilter(tex, filter);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetTextureFilter", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->AddParam("wrap");
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		int wrap = context->GetVar(String("wrap")).IntValue();
		SetTextureWrap(tex, wrap);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetTextureWrap", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("texture");
	i->code = INTRINSIC_LAMBDA {
		Texture tex = ValueToTexture(context->GetVar(String("texture")));
		GenTextureMipmaps(&tex);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("GenTextureMipmaps", i->GetFunc());

	// RenderTexture2D loading/unloading

	i = Intrinsic::Create("");
	i->AddParam("width", Value(960));
	i->AddParam("height", Value(640));
	i->code = INTRINSIC_LAMBDA {
		int width = context->GetVar(String("width")).IntValue();
		int height = context->GetVar(String("height")).IntValue();
		RenderTexture2D renderTexture = LoadRenderTexture(width, height);
		return IntrinsicResult(RenderTextureToValue(renderTexture));
	};
	raylibModule.SetValue("LoadRenderTexture", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("target");
	i->code = INTRINSIC_LAMBDA {
		RenderTexture2D target = ValueToRenderTexture(context->GetVar(String("target")));
		UnloadRenderTexture(target);
		// Free the heap-allocated RenderTexture2D struct
		ValueDict map = context->GetVar(String("target")).GetDict();
		Value handleVal = map.Lookup(String("_handle"), Value::zero);
		RenderTexture2D* rtPtr = (RenderTexture2D*)(long)handleVal.IntValue();
		delete rtPtr;
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("UnloadRenderTexture", i->GetFunc());

	// RenderTexture2D drawing

	i = Intrinsic::Create("");
	i->AddParam("target");
	i->code = INTRINSIC_LAMBDA {
		RenderTexture2D target = ValueToRenderTexture(context->GetVar(String("target")));
		BeginTextureMode(target);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("BeginTextureMode", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		EndTextureMode();
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("EndTextureMode", i->GetFunc());
}
