//
//  RCore.cpp
//  MSRLWeb
//
//  Raylib Core module intrinsics
//

#include "RaylibIntrinsics.h"
#include "RaylibTypes.h"
#include "raylib.h"
#include "MiniscriptInterpreter.h"
#include "MiniscriptTypes.h"
#include <emscripten.h>

using namespace MiniScript;

#define INTRINSIC_LAMBDA [](Context *context, IntrinsicResult partialResult) -> IntrinsicResult

// Helper: Set window title
EM_JS(void, _SetWindowTitle, (const char *title), {
	const _title = UTF8ToString(title);
	document.title = _title;
	document.querySelector("h1").textContent = _title;
});

// Helper: Set window icon
// We need to free the buffer after we're done with it so this function won't
// return until after it's done.
EM_ASYNC_JS(void, _SetWindowIcon, (unsigned char *data, long size), {
	await new Promise((resolve, reject)=>{
		const _data = new Uint8Array(HEAP8.buffer, data, size);
		const blob = new Blob([_data], {type:"image/png"});
		const reader = new FileReader();
		reader.onloadend = () => {
			const dataURL = reader.result;
			let link = document.querySelector('link[rel="icon"]');
			if (link===null) {
				link = document.createElement("link");
				link.setAttribute("rel", "icon");
				document.head.appendChild(link);
			}
			link.href = dataURL;
			resolve();
		};
		reader.onerror = reject;
		reader.readAsDataURL(blob);
	});
});

void AddRCoreMethods(ValueDict raylibModule) {
	Intrinsic *i;

	// Drawing-related functions

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		BeginDrawing();
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("BeginDrawing", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		EndDrawing();
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("EndDrawing", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("color", ColorToValue(BLACK));
	i->code = INTRINSIC_LAMBDA {
		Value colorVal = context->GetVar(String("color"));
		Color color = ValueToColor(colorVal);
		ClearBackground(color);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ClearBackground", i->GetFunc());

	// Timing functions

	i = Intrinsic::Create("");
	i->AddParam("fps");
	i->code = INTRINSIC_LAMBDA {
		SetTargetFPS(context->GetVar(String("fps")).IntValue());
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetTargetFPS", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetFrameTime());
	};
	raylibModule.SetValue("GetFrameTime", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetTime());
	};
	raylibModule.SetValue("GetTime", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetFPS());
	};
	raylibModule.SetValue("GetFPS", i->GetFunc());

	// Input-related functions: keyboard

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsKeyPressed(context->GetVar(String("key")).IntValue()));
	};
	raylibModule.SetValue("IsKeyPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsKeyPressedRepeat(context->GetVar(String("key")).IntValue()));
	};
	raylibModule.SetValue("IsKeyPressedRepeat", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsKeyDown(context->GetVar(String("key")).IntValue()));
	};
	raylibModule.SetValue("IsKeyDown", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsKeyReleased(context->GetVar(String("key")).IntValue()));
	};
	raylibModule.SetValue("IsKeyReleased", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsKeyUp(context->GetVar(String("key")).IntValue()));
	};
	raylibModule.SetValue("IsKeyUp", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetKeyPressed());
	};
	raylibModule.SetValue("GetKeyPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetCharPressed());
	};
	raylibModule.SetValue("GetCharPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("key");
	i->code = INTRINSIC_LAMBDA {
		SetExitKey(context->GetVar(String("key")).IntValue());
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetExitKey", i->GetFunc());

	// Input-related functions: gamepad

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsGamepadAvailable(context->GetVar(String("gamepad")).IntValue()));
	};
	raylibModule.SetValue("IsGamepadAvailable", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetGamepadName(context->GetVar(String("gamepad")).IntValue()));
	};
	raylibModule.SetValue("GetGamepadName", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsGamepadButtonPressed(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsGamepadButtonPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsGamepadButtonDown(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsGamepadButtonDown", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsGamepadButtonReleased(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsGamepadButtonReleased", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsGamepadButtonUp(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsGamepadButtonUp", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetGamepadButtonPressed());
	};
	raylibModule.SetValue("GetGamepadButtonPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetGamepadAxisCount(context->GetVar(String("gamepad")).IntValue()));
	};
	raylibModule.SetValue("GetGamepadAxisCount", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("axis");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetGamepadAxisMovement(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("axis")).IntValue()));
	};
	raylibModule.SetValue("GetGamepadAxisMovement", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("mappings");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(SetGamepadMappings(context->GetVar(String("mappings")).ToString().c_str()));
	};
	raylibModule.SetValue("SetGamepadMappings", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("gamepad", 0);
	i->AddParam("leftMotor", 0.0);
	i->AddParam("rightMotor", 0.0);
	i->AddParam("duration", 0.0);
	i->code = INTRINSIC_LAMBDA {
		SetGamepadVibration(
			context->GetVar(String("gamepad")).IntValue(),
			context->GetVar(String("leftMotor")).FloatValue(),
			context->GetVar(String("rightMotor")).FloatValue(),
			context->GetVar(String("duration")).FloatValue());
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetGamepadVibration", i->GetFunc());

	// Input-related functions: mouse

	i = Intrinsic::Create("");
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsMouseButtonPressed(context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsMouseButtonPressed", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsMouseButtonDown(context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsMouseButtonDown", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsMouseButtonReleased(context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsMouseButtonReleased", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("button");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsMouseButtonUp(context->GetVar(String("button")).IntValue()));
	};
	raylibModule.SetValue("IsMouseButtonUp", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetMouseX());
	};
	raylibModule.SetValue("GetMouseX", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetMouseY());
	};
	raylibModule.SetValue("GetMouseY", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		Vector2 pos = GetMousePosition();
		ValueDict posMap;
		posMap.SetValue(String("x"), Value(pos.x));
		posMap.SetValue(String("y"), Value(pos.y));
		return IntrinsicResult(posMap);
	};
	raylibModule.SetValue("GetMousePosition", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		Vector2 delta = GetMouseDelta();
		ValueDict deltaMap;
		deltaMap.SetValue(String("x"), Value(delta.x));
		deltaMap.SetValue(String("y"), Value(delta.y));
		return IntrinsicResult(deltaMap);
	};
	raylibModule.SetValue("GetMouseDelta", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(GetMouseWheelMove());
	};
	raylibModule.SetValue("GetMouseWheelMove", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("cursor");
	i->code = INTRINSIC_LAMBDA {
		SetMouseCursor(context->GetVar(String("cursor")).IntValue());
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetMouseCursor", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		ShowCursor();
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("ShowCursor", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		HideCursor();
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("HideCursor", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsCursorHidden());
	};
	raylibModule.SetValue("IsCursorHidden", i->GetFunc());

	i = Intrinsic::Create("");
	i->code = INTRINSIC_LAMBDA {
		return IntrinsicResult(IsCursorOnScreen());
	};
	raylibModule.SetValue("IsCursorOnScreen", i->GetFunc());

	// Set window title/icon
	i = Intrinsic::Create("");
	i->AddParam("caption", "MSRLWeb - MiniScript + Raylib");
	i->code = INTRINSIC_LAMBDA {
		String caption = context->GetVar(String("caption")).GetString();
		_SetWindowTitle(caption.c_str());
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetWindowTitle", i->GetFunc());

	i = Intrinsic::Create("");
	i->AddParam("image");
	i->code = INTRINSIC_LAMBDA {
		Image image = ValueToImage(context->GetVar(String("image")));
		int size;
		unsigned char *data = ExportImageToMemory(image, ".png", &size);
		_SetWindowIcon(data, size);
		free(data);
		return IntrinsicResult::Null;
	};
	raylibModule.SetValue("SetWindowIcon", i->GetFunc());

	// Load text files
	i = Intrinsic::Create("");
	i->AddParam("fileName");
	i->code = INTRINSIC_LAMBDA {
		const char *fileName = context->GetVar("fileName").GetString().c_str();
		char *text = LoadFileText(fileName);
		String ret(text);
		UnloadFileText(text);
		return IntrinsicResult(ret);
	};
	raylibModule.SetValue("LoadFileText", i->GetFunc());
}
