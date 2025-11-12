#!/usr/bin/env python3
"""
API Coverage Checker for MSRLWeb
Scans raylib.h to find all APIs by module, then checks which ones are wrapped in our code.
"""

import re
import sys
from pathlib import Path
from collections import defaultdict

# Raylib modules and their typical function prefixes/patterns
MODULES = {
    'raudio': {
        'file': 'src/RAudio.cpp',
        'patterns': [
            r'\b(InitAudioDevice|CloseAudioDevice|IsAudioDeviceReady|SetMasterVolume)\b',
            r'\b(LoadWave|LoadWaveFromMemory|IsWaveValid|UnloadWave|ExportWave|ExportWaveAsCode|LoadWaveSamples|UnloadWaveSamples)\b',
            r'\b(LoadSound|LoadSoundFromWave|LoadSoundAlias|IsSoundValid|UnloadSound|UpdateSound|PlaySound|StopSound|PauseSound|ResumeSound|IsSoundPlaying)\b',
            r'\b(SetSoundVolume|SetSoundPitch|SetSoundPan)\b',
            r'\b(LoadMusicStream|LoadMusicStreamFromMemory|IsMusicValid|UnloadMusicStream|PlayMusicStream|IsMusicStreamPlaying|UpdateMusicStream|StopMusicStream|PauseMusicStream|ResumeMusicStream)\b',
            r'\b(SeekMusicStream|SetMusicVolume|SetMusicPitch|SetMusicPan|GetMusicTimeLength|GetMusicTimePlayed)\b',
            r'\b(LoadAudioStream|IsAudioStreamValid|UnloadAudioStream|UpdateAudioStream|IsAudioStreamProcessed|PlayAudioStream|PauseAudioStream|ResumeAudioStream|IsAudioStreamPlaying|StopAudioStream)\b',
            r'\b(SetAudioStreamVolume|SetAudioStreamPitch|SetAudioStreamPan|SetAudioStreamBufferSizeDefault)\b',
        ]
    },
    'rcore': {
        'file': 'src/RCore.cpp',
        'patterns': [
            r'\b(InitWindow|CloseWindow|WindowShouldClose|IsWindowReady|IsWindowFullscreen|IsWindowHidden|IsWindowMinimized|IsWindowMaximized|IsWindowFocused|IsWindowResized)\b',
            r'\b(SetWindowState|ClearWindowState|ToggleFullscreen|ToggleBorderlessWindowed|MaximizeWindow|MinimizeWindow|RestoreWindow|SetWindowIcon|SetWindowIcons|SetWindowTitle|SetWindowPosition|SetWindowMonitor|SetWindowMinSize|SetWindowMaxSize|SetWindowSize|SetWindowOpacity|SetWindowFocused)\b',
            r'\b(GetWindowHandle|GetScreenWidth|GetScreenHeight|GetRenderWidth|GetRenderHeight|GetMonitorCount|GetCurrentMonitor|GetMonitorPosition|GetMonitorWidth|GetMonitorHeight|GetMonitorPhysicalWidth|GetMonitorPhysicalHeight|GetMonitorRefreshRate|GetWindowPosition|GetWindowScaleDPI|GetMonitorName)\b',
            r'\b(SetClipboardText|GetClipboardText|GetClipboardImage|EnableEventWaiting|DisableEventWaiting)\b',
            r'\b(ShowCursor|HideCursor|IsCursorHidden|EnableCursor|DisableCursor|IsCursorOnScreen)\b',
            r'\b(ClearBackground|BeginDrawing|EndDrawing|BeginMode2D|EndMode2D|BeginMode3D|EndMode3D|BeginTextureMode|EndTextureMode|BeginShaderMode|EndShaderMode|BeginBlendMode|EndBlendMode|BeginScissorMode|EndScissorMode|BeginVrStereoMode|EndVrStereoMode)\b',
            r'\b(SetTargetFPS|GetFrameTime|GetTime|GetFPS)\b',
            r'\b(SwapScreenBuffer|PollInputEvents|WaitTime)\b',
            r'\b(TakeScreenshot|SetConfigFlags|OpenURL)\b',
            r'\b(IsKeyPressed|IsKeyPressedRepeat|IsKeyDown|IsKeyReleased|IsKeyUp|GetKeyPressed|GetCharPressed|SetExitKey)\b',
            r'\b(IsGamepadAvailable|GetGamepadName|IsGamepadButtonPressed|IsGamepadButtonDown|IsGamepadButtonReleased|IsGamepadButtonUp|GetGamepadButtonPressed|GetGamepadAxisCount|GetGamepadAxisMovement|SetGamepadMappings|SetGamepadVibration)\b',
            r'\b(IsMouseButtonPressed|IsMouseButtonDown|IsMouseButtonReleased|IsMouseButtonUp|GetMouseX|GetMouseY|GetMousePosition|GetMouseDelta|SetMousePosition|SetMouseOffset|SetMouseScale|GetMouseWheelMove|GetMouseWheelMoveV|SetMouseCursor)\b',
            r'\b(GetTouchX|GetTouchY|GetTouchPosition|GetTouchPointId|GetTouchPointCount)\b',
            r'\b(SetGesturesEnabled|IsGestureDetected|GetGestureDetected|GetGestureHoldDuration|GetGestureDragVector|GetGestureDragAngle|GetGesturePinchVector|GetGesturePinchAngle)\b',
            r'\b(LoadFileData|SaveFileData|UnloadFileData|LoadFileText|SaveFileText|UnloadFileText|FileExists|DirectoryExists|IsFileExtension|GetFileLength|GetFileExtension|GetFileName|GetFileNameWithoutExt|GetDirectoryPath|GetPrevDirectoryPath|GetWorkingDirectory|GetApplicationDirectory|MakeDirectory|ChangeDirectory|IsPathFile|IsFileNameValid|LoadDirectoryFiles|LoadDirectoryFilesEx|UnloadDirectoryFiles|IsFileDropped|LoadDroppedFiles|UnloadDroppedFiles|GetFileModTime)\b',
            r'\b(CompressData|DecompressData|EncodeDataBase64|DecodeDataBase64|ComputeCRC32|ComputeMD5|ComputeSHA1)\b',
        ]
    },
    'rshapes': {
        'file': 'src/RShapes.cpp',
        'patterns': [
            r'\bSetShapesTexture\b',
            r'\b(DrawPixel|DrawPixelV|DrawLine|DrawLineV|DrawLineEx|DrawLineStrip|DrawLineBezier|DrawLine3D)\b',
            r'\b(DrawCircle|DrawCircleSector|DrawCircleSectorLines|DrawCircleGradient|DrawCircleV|DrawCircleLines|DrawCircleLinesV|DrawCircle3D)\b',
            r'\b(DrawEllipse|DrawEllipseLines|DrawEllipseV|DrawEllipseLinesV)\b',
            r'\b(DrawRing|DrawRingLines)\b',
            r'\b(DrawRectangle|DrawRectangleV|DrawRectangleRec|DrawRectanglePro|DrawRectangleGradientV|DrawRectangleGradientH|DrawRectangleGradientEx|DrawRectangleLines|DrawRectangleLinesEx|DrawRectangleRounded|DrawRectangleRoundedLines|DrawRectangleRoundedLinesEx)\b',
            r'\b(DrawTriangle|DrawTriangleLines|DrawTriangleFan|DrawTriangleStrip|DrawTriangle3D|DrawTriangleStrip3D)\b',
            r'\b(DrawPoly|DrawPolyLines|DrawPolyLinesEx)\b',
            r'\b(DrawSplineLinear|DrawSplineBasis|DrawSplineCatmullRom|DrawSplineBezierQuadratic|DrawSplineBezierCubic|DrawSplineSegmentLinear|DrawSplineSegmentBasis|DrawSplineSegmentCatmullRom|DrawSplineSegmentBezierQuadratic|DrawSplineSegmentBezierCubic)\b',
            r'\b(CheckCollisionRecs|CheckCollisionCircles|CheckCollisionCircleRec|CheckCollisionCircleLine|CheckCollisionPointRec|CheckCollisionPointCircle|CheckCollisionPointTriangle|CheckCollisionPointLine|CheckCollisionLines|CheckCollisionPointPoly)\b',
            r'\bGetCollisionRec\b',
        ]
    },
    'rtext': {
        'file': 'src/RText.cpp',
        'patterns': [
            r'\b(GetFontDefault|LoadFont|LoadFontEx|LoadFontFromImage|LoadFontFromMemory|IsFontValid|LoadFontData|GenImageFontAtlas|UnloadFontData|UnloadFont|ExportFontAsCode)\b',
            r'\b(DrawFPS|DrawText|DrawTextEx|DrawTextPro|DrawTextCodepoint|DrawTextCodepoints)\b',
            r'\bSetTextLineSpacing\b',
            r'\b(MeasureText|MeasureTextEx|GetGlyphIndex|GetGlyphInfo|GetGlyphAtlasRec)\b',
            r'\b(LoadUTF8|UnloadUTF8|LoadCodepoints|UnloadCodepoints|GetCodepointCount|GetCodepoint|GetCodepointNext|GetCodepointPrevious|CodepointToUTF8)\b',
            r'\b(TextCopy|TextIsEqual|TextLength|TextFormat|TextSubtext|TextReplace|TextInsert|TextJoin|TextSplit|TextAppend|TextFindIndex|TextToUpper|TextToLower|TextToPascal|TextToSnake|TextToCamel|TextToInteger|TextToFloat)\b',
        ]
    },
    'rtextures': {
        'file': 'src/RTextures.cpp',
        'patterns': [
            r'\b(LoadImage|LoadImageRaw|LoadImageSvg|LoadImageAnim|LoadImageAnimFromMemory|LoadImageFromMemory|LoadImageFromTexture|LoadImageFromScreen|IsImageValid|UnloadImage|ExportImage|ExportImageToMemory|ExportImageAsCode)\b',
            r'\b(GenImageColor|GenImageGradientLinear|GenImageGradientRadial|GenImageGradientSquare|GenImageChecked|GenImageWhiteNoise|GenImagePerlinNoise|GenImageCellular|GenImageText)\b',
            r'\b(ImageCopy|ImageFromImage|ImageFromChannel|ImageText|ImageTextEx|ImageFormat|ImageToPOT|ImageCrop|ImageAlphaCrop|ImageAlphaClear|ImageAlphaMask|ImageAlphaPremultiply|ImageBlurGaussian|ImageKernelConvolution|ImageResize|ImageResizeNN|ImageResizeCanvas|ImageMipmaps|ImageDither)\b',
            r'\b(ImageFlipVertical|ImageFlipHorizontal|ImageRotate|ImageRotateCW|ImageRotateCCW|ImageColorTint|ImageColorInvert|ImageColorGrayscale|ImageColorContrast|ImageColorBrightness|ImageColorReplace)\b',
            r'\b(LoadImageColors|LoadImagePalette|UnloadImageColors|UnloadImagePalette|GetImageAlphaBorder|GetImageColor)\b',
            r'\b(ImageClearBackground|ImageDrawPixel|ImageDrawPixelV|ImageDrawLine|ImageDrawLineV|ImageDrawLineEx|ImageDrawCircle|ImageDrawCircleV|ImageDrawCircleLines|ImageDrawCircleLinesV|ImageDrawRectangle|ImageDrawRectangleV|ImageDrawRectangleRec|ImageDrawRectangleLines|ImageDrawTriangle|ImageDrawTriangleEx|ImageDrawTriangleLines|ImageDrawTriangleFan|ImageDrawTriangleStrip|ImageDraw|ImageDrawText|ImageDrawTextEx)\b',
            r'\b(LoadTexture|LoadTextureFromImage|LoadTextureCubemap|LoadRenderTexture|IsTextureValid|UnloadTexture|IsRenderTextureValid|UnloadRenderTexture|UpdateTexture|UpdateTextureRec)\b',
            r'\b(GenTextureMipmaps|SetTextureFilter|SetTextureWrap)\b',
            r'\b(DrawTexture|DrawTextureV|DrawTextureEx|DrawTextureRec|DrawTexturePro|DrawTextureNPatch)\b',
            r'\b(ColorIsEqual|Fade|ColorToInt|ColorNormalize|ColorFromNormalized|ColorToHSV|ColorFromHSV|ColorTint|ColorBrightness|ColorContrast|ColorAlpha|ColorAlphaBlend|ColorLerp|GetColor|GetPixelColor|SetPixelColor|GetPixelDataSize)\b',
        ]
    }
}

def extract_raylib_apis(raylib_h_path):
    """Extract all RLAPI function declarations from raylib.h"""
    apis = defaultdict(set)

    try:
        with open(raylib_h_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Find all RLAPI function declarations
        # Pattern: RLAPI <return_type> <function_name>(<params>);
        rlapi_pattern = r'RLAPI\s+\w+[\s\*]+(\w+)\s*\([^)]*\)\s*;'
        matches = re.finditer(rlapi_pattern, content)

        for match in matches:
            func_name = match.group(1)

            # Categorize by module based on patterns
            for module, info in MODULES.items():
                for pattern in info['patterns']:
                    if re.search(pattern, func_name):
                        apis[module].add(func_name)
                        break

        return apis
    except FileNotFoundError:
        print(f"Error: Could not find raylib.h at {raylib_h_path}")
        sys.exit(1)

def extract_wrapped_apis(module_file_path):
    """Extract all wrapped APIs from a module .cpp file"""
    wrapped = set()

    try:
        with open(module_file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Find all raylibModule.SetValue("FunctionName", ...) calls
        pattern = r'raylibModule\.SetValue\("(\w+)",\s*i->GetFunc\(\)\)'
        matches = re.finditer(pattern, content)

        for match in matches:
            wrapped.add(match.group(1))

        return wrapped
    except FileNotFoundError:
        print(f"Warning: Could not find {module_file_path}")
        return set()

def main():
    # Find raylib.h
    raylib_h = Path("raylib/src/raylib.h")
    if not raylib_h.exists():
        print(f"Error: raylib.h not found at {raylib_h}")
        sys.exit(1)

    print("=" * 80)
    print("MSRLWeb API Coverage Report")
    print("=" * 80)
    print()

    # Extract all Raylib APIs
    raylib_apis = extract_raylib_apis(raylib_h)

    total_raylib = 0
    total_wrapped = 0
    total_missing = 0

    # Check each module
    for module, info in sorted(MODULES.items()):
        module_file = Path(info['file'])

        print(f"Module: {module.upper()}")
        print("-" * 80)

        raylib_funcs = raylib_apis.get(module, set())
        wrapped_funcs = extract_wrapped_apis(module_file) if module_file.exists() else set()
        missing_funcs = raylib_funcs - wrapped_funcs

        total_raylib += len(raylib_funcs)
        total_wrapped += len(wrapped_funcs)
        total_missing += len(missing_funcs)

        print(f"Raylib APIs found: {len(raylib_funcs)}")
        print(f"APIs wrapped: {len(wrapped_funcs)}")
        print(f"APIs missing: {len(missing_funcs)}")

        if missing_funcs:
            print(f"\nMissing APIs in {module}:")
            for func in sorted(missing_funcs):
                print(f"  - {func}")
        else:
            print(f"\n✓ All {module} APIs are wrapped!")

        # Also report wrapped APIs not found in raylib.h (possibly deprecated or custom)
        extra_funcs = wrapped_funcs - raylib_funcs
        if extra_funcs:
            print(f"\nWrapped but not found in raylib.h patterns (may be valid):")
            for func in sorted(extra_funcs):
                print(f"  - {func}")

        print()

    # Summary
    print("=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print(f"Total Raylib APIs identified: {total_raylib}")
    print(f"Total APIs wrapped: {total_wrapped}")
    print(f"Total APIs missing: {total_missing}")

    if total_missing == 0:
        print("\n✓ All identified APIs are wrapped!")
    else:
        coverage = (total_wrapped / total_raylib * 100) if total_raylib > 0 else 0
        print(f"\nCoverage: {coverage:.1f}%")

if __name__ == '__main__':
    main()
