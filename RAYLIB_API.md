# MSRLWeb Raylib API Reference

This document lists all Raylib functions available in MSRLWeb, organized by module.

Generated from: src/RaylibIntrinsics.cpp

## Summary

- **rcore**: 29 functions
- **rshapes**: 35 functions
- **rtextures**: 45 functions
- **rtext**: 12 functions
- **raudio**: 53 functions
- **Total**: 174 functions

## Functions by Module

| rcore | rshapes | rtextures | rtext | raudio |
|-------|---------|-----------|-------|--------|
| BeginDrawing | CheckCollisionCircleRec | DrawTexture | DrawFPS | CloseAudioDevice |
| ClearBackground | CheckCollisionCircles | DrawTextureEx | DrawText | GetMusicTimeLength |
| EndDrawing | CheckCollisionPointCircle | DrawTexturePro | DrawTextCodepoint | GetMusicTimePlayed |
| GetCharPressed | CheckCollisionPointRec | DrawTextureRec | DrawTextEx | InitAudioDevice |
| GetFPS | CheckCollisionPointTriangle | DrawTextureV | DrawTextPro | IsAudioDeviceReady |
| GetFrameTime | CheckCollisionRecs | GenImageCellular | GetGlyphIndex | IsAudioStreamPlaying |
| GetKeyPressed | DrawCircle | GenImageChecked | LoadFont | IsAudioStreamProcessed |
| GetMouseDelta | DrawCircleLines | GenImageColor | LoadFontEx | IsAudioStreamReady |
| GetMousePosition | DrawCircleV | GenImageGradientLinear | LoadFontFromImage | IsMusicReady |
| GetMouseWheelMove | DrawEllipse | GenImageGradientRadial | MeasureText | IsMusicStreamPlaying |
| GetMouseX | DrawEllipseLines | GenImageGradientSquare | MeasureTextEx | IsSoundPlaying |
| GetMouseY | DrawLine | GenImageWhiteNoise | UnloadFont | IsSoundReady |
| GetTime | DrawLineEx | GenTextureMipmaps |  | IsWaveReady |
| HideCursor | DrawLineV | ImageClearBackground |  | LoadMusicStream |
| IsCursorHidden | DrawPixel | ImageColorBrightness |  | LoadMusicStreamFromMemory |
| IsCursorOnScreen | DrawPixelV | ImageColorContrast |  | LoadSound |
| IsKeyDown | DrawPoly | ImageColorGrayscale |  | LoadSoundAlias |
| IsKeyPressed | DrawPolyLines | ImageColorInvert |  | LoadSoundFromWave |
| IsKeyPressedRepeat | DrawPolyLinesEx | ImageColorTint |  | LoadWave |
| IsKeyReleased | DrawRectangle | ImageCopy |  | LoadWaveFromMemory |
| IsKeyUp | DrawRectangleGradientEx | ImageCrop |  | PauseAudioStream |
| IsMouseButtonDown | DrawRectangleGradientH | ImageDraw |  | PauseMusicStream |
| IsMouseButtonPressed | DrawRectangleGradientV | ImageDrawCircle |  | PauseSound |
| IsMouseButtonReleased | DrawRectangleLines | ImageDrawCircleV |  | PlayAudioStream |
| IsMouseButtonUp | DrawRectangleLinesEx | ImageDrawLine |  | PlayMusicStream |
| SetExitKey | DrawRectanglePro | ImageDrawLineV |  | PlaySound |
| SetMouseCursor | DrawRectangleRec | ImageDrawPixel |  | ResumeAudioStream |
| SetTargetFPS | DrawRectangleRounded | ImageDrawPixelV |  | ResumeMusicStream |
| ShowCursor | DrawRectangleRoundedLines | ImageDrawRectangle |  | ResumeSound |
|  | DrawRectangleV | ImageDrawRectangleLines |  | SeekMusicStream |
|  | DrawRing | ImageDrawRectangleRec |  | SetAudioStreamBufferSizeDefault |
|  | DrawRingLines | ImageDrawText |  | SetAudioStreamPan |
|  | DrawTriangle | ImageFlipHorizontal |  | SetAudioStreamPitch |
|  | DrawTriangleLines | ImageFlipVertical |  | SetAudioStreamVolume |
|  | GetCollisionRec | ImageResize |  | SetMasterVolume |
|  |  | ImageResizeNN |  | SetMusicPan |
|  |  | ImageRotateCCW |  | SetMusicPitch |
|  |  | ImageRotateCW |  | SetMusicVolume |
|  |  | LoadImage |  | SetSoundPan |
|  |  | LoadTexture |  | SetSoundPitch |
|  |  | LoadTextureFromImage |  | SetSoundVolume |
|  |  | SetTextureFilter |  | StopAudioStream |
|  |  | SetTextureWrap |  | StopMusicStream |
|  |  | UnloadImage |  | StopSound |
|  |  | UnloadTexture |  | UnloadAudioStream |
|  |  |  |  | UnloadMusicStream |
|  |  |  |  | UnloadSound |
|  |  |  |  | UnloadSoundAlias |
|  |  |  |  | UnloadWave |
|  |  |  |  | UpdateMusicStream |
|  |  |  |  | WaveCopy |
|  |  |  |  | WaveCrop |
|  |  |  |  | WaveFormat |

---

*Generated on Sat Nov  8 07:58:09 MST 2025*
