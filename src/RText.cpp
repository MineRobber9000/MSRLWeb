//
//  RText.cpp
//  MSRLWeb
//
//  Raylib Text module intrinsics
//

#include "RaylibIntrinsics.h"
#include "RaylibTypes.h"
#include "raylib.h"
#include "MiniscriptInterpreter.h"
#include "MiniscriptTypes.h"

using namespace MiniScript;

#define INTRINSIC_LAMBDA [](Context *context, IntrinsicResult partialResult) -> IntrinsicResult

void AddRTextMethods(ValueDict raylibModule) {
	Intrinsic *i;

	// Font loading

	i = Intrinsic::Create("");
	i->AddParam("fileName");
	i->code = INTRINSIC_LAMBDA {
		String path = context->GetVar(String("fileName")).ToString();
		Font font = LoadFont(path.c_str());
		if (!IsFontValid(font)) return IntrinsicResult::Null;
		return IntrinsicResult(FontToValue(font));
	};
	raylibModule.SetValue("LoadFont", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("fileName");
	i->AddParam("fontSize", Value(20));
	i->AddParam("codepoints", Value::null);
	i->AddParam("codepointCount", Value::zero);
	i->code = INTRINSIC_LAMBDA {
		String path = context->GetVar(String("fileName")).ToString();
		int fontSize = context->GetVar(String("fontSize")).IntValue();
		// For now, ignore codepoints parameter and load all
		Font font = LoadFontEx(path.c_str(), fontSize, nullptr, 0);
		if (!IsFontValid(font)) return IntrinsicResult::Null;
		return IntrinsicResult(FontToValue(font));
	};
	raylibModule.SetValue("LoadFontEx", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->AddParam("key", ColorToValue(Color{255, 0, 255, 255}));
	i->AddParam("firstChar", Value(32));
	i->code = INTRINSIC_LAMBDA {
		Image image = ValueToImage(context->GetVar(String("image")));
		Color key = ValueToColor(context->GetVar(String("key")));
		Value firstCharVal = context->GetVar(String("firstChar"));
		int firstChar;
		if (firstCharVal.type == ValueType::String) {
			String s = firstCharVal.ToString();
			firstChar = s.empty() ? 32 : (int)s[0];
		} else {
			firstChar = firstCharVal.IntValue();
		}
		Font font = LoadFontFromImage(image, key, firstChar);
		return IntrinsicResult(FontToValue(font));
	};
	raylibModule.SetValue("LoadFontFromImage", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		return IntrinsicResult(IsFontValid(font));
	};
	raylibModule.SetValue("IsFontValid", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		UnloadFont(font);
		// Free the heap-allocated Font struct
		ValueDict map = context->GetVar(String("font")).GetDict();
		Value handleVal = map.Lookup(String("_handle"), Value::zero);
		Font* fontPtr = (Font*)(long)handleVal.IntValue();
		delete fontPtr;
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("UnloadFont", i->GetFunc());

	// Text drawing

	i = Intrinsic::Create("");
	i->AddParam("posX", Value::zero);
	i->AddParam("posY", Value::zero);
	i->code = INTRINSIC_LAMBDA {
		int posX = context->GetVar(String("posX")).IntValue();
		int posY = context->GetVar(String("posY")).IntValue();
		DrawFPS(posX, posY);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawFPS", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("text");
	i->AddParam("posX", Value::zero);
	i->AddParam("posY", Value::zero);
	i->AddParam("fontSize", Value(20));
	i->AddParam("color", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		String text = context->GetVar(String("text")).ToString();
		int posX = context->GetVar(String("posX")).IntValue();
		int posY = context->GetVar(String("posY")).IntValue();
		int fontSize = context->GetVar(String("fontSize")).IntValue();
		Color color = ValueToColor(context->GetVar(String("color")));
		DrawText(text.c_str(), posX, posY, fontSize, color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawText", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->AddParam("text");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("fontSize", Value(20));
	i->AddParam("spacing", Value::zero);
	i->AddParam("tint", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		String text = context->GetVar(String("text")).ToString();
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		float fontSize = context->GetVar(String("fontSize")).FloatValue();
		float spacing = context->GetVar(String("spacing")).FloatValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextEx(font, text.c_str(), position, fontSize, spacing, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextEx", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->AddParam("text");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("origin", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("rotation", Value::zero);
	i->AddParam("fontSize", Value(20));
	i->AddParam("spacing", Value::zero);
	i->AddParam("tint", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		String text = context->GetVar(String("text")).ToString();
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		Vector2 origin = ValueToVector2(context->GetVar(String("origin")));
		float rotation = context->GetVar(String("rotation")).FloatValue();
		float fontSize = context->GetVar(String("fontSize")).FloatValue();
		float spacing = context->GetVar(String("spacing")).FloatValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextPro(font, text.c_str(), position, origin, rotation, fontSize, spacing, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextPro", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->AddParam("codepoint");
	i->AddParam("position", Vector2ToValue(Vector2{0, 0}));
	i->AddParam("fontSize", Value(20));
	i->AddParam("tint", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		int codepoint = context->GetVar(String("codepoint")).IntValue();
		Vector2 position = ValueToVector2(context->GetVar(String("position")));
		float fontSize = context->GetVar(String("fontSize")).FloatValue();
		Color tint = ValueToColor(context->GetVar(String("tint")));
		DrawTextCodepoint(font, codepoint, position, fontSize, tint);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("DrawTextCodepoint", i->GetFunc());

	// Text measurement

	i = Intrinsic::Create("");
	i->AddParam("text");
	i->AddParam("fontSize", Value(20));
	i->code = INTRINSIC_LAMBDA {
		String text = context->GetVar(String("text")).ToString();
		int fontSize = context->GetVar(String("fontSize")).IntValue();
		int width = MeasureText(text.c_str(), fontSize);
		return IntrinsicResult(Value(width));
	};
	raylibModule.SetValue("MeasureText", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->AddParam("text");
	i->AddParam("fontSize", Value(20));
	i->AddParam("spacing", Value::zero);
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		String text = context->GetVar(String("text")).ToString();
		float fontSize = context->GetVar(String("fontSize")).FloatValue();
		float spacing = context->GetVar(String("spacing")).FloatValue();
		Vector2 size = MeasureTextEx(font, text.c_str(), fontSize, spacing);
		ValueDict result;
		result.SetValue(String("x"), Value(size.x));
		result.SetValue(String("y"), Value(size.y));
		return IntrinsicResult(Value(result));
	};
	raylibModule.SetValue("MeasureTextEx", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("font");
	i->AddParam("codepoint");
	i->code = INTRINSIC_LAMBDA {
		Font font = ValueToFont(context->GetVar(String("font")));
		int codepoint = context->GetVar(String("codepoint")).IntValue();
		int index = GetGlyphIndex(font, codepoint);
		return IntrinsicResult(Value(index));
	};
	raylibModule.SetValue("GetGlyphIndex", i->GetFunc());
}
