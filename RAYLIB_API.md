# MSRLWeb Raylib API Reference

This document lists all Raylib functions available in MSRLWeb, organized by module.

Generated from: src/RaylibIntrinsics.cpp

## Summary

- **rcore**: 29 functions
- **rshapes**: 35 functions
- **rtextures**: 45 functions
- **rtext**: 12 functions
- **raudio**: 55 functions
- **Total**: 176 functions

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
| GetMouseDelta | DrawCircleLines | GenImageColor | LoadFontEx | IsAudioStreamValid |
| GetMousePosition | DrawCircleV | GenImageGradientLinear | LoadFontFromImage | IsMusicReady |
| GetMouseWheelMove | DrawEllipse | GenImageGradientRadial | MeasureText | IsMusicStreamPlaying |
| GetMouseX | DrawEllipseLines | GenImageGradientSquare | MeasureTextEx | IsSoundPlaying |
| GetMouseY | DrawLine | GenImageWhiteNoise | UnloadFont | IsSoundReady |
| GetTime | DrawLineEx | GenTextureMipmaps |  | IsWaveReady |
| HideCursor | DrawLineV | ImageClearBackground |  | LoadAudioStream |
| IsCursorHidden | DrawPixel | ImageColorBrightness |  | LoadMusicStream |
| IsCursorOnScreen | DrawPixelV | ImageColorContrast |  | LoadMusicStreamFromMemory |
| IsKeyDown | DrawPoly | ImageColorGrayscale |  | LoadSound |
| IsKeyPressed | DrawPolyLines | ImageColorInvert |  | LoadSoundAlias |
| IsKeyPressedRepeat | DrawPolyLinesEx | ImageColorTint |  | LoadSoundFromWave |
| IsKeyReleased | DrawRectangle | ImageCopy |  | LoadWave |
| IsKeyUp | DrawRectangleGradientEx | ImageCrop |  | LoadWaveFromMemory |
| IsMouseButtonDown | DrawRectangleGradientH | ImageDraw |  | PauseAudioStream |
| IsMouseButtonPressed | DrawRectangleGradientV | ImageDrawCircle |  | PauseMusicStream |
| IsMouseButtonReleased | DrawRectangleLines | ImageDrawCircleV |  | PauseSound |
| IsMouseButtonUp | DrawRectangleLinesEx | ImageDrawLine |  | PlayAudioStream |
| SetExitKey | DrawRectanglePro | ImageDrawLineV |  | PlayMusicStream |
| SetMouseCursor | DrawRectangleRec | ImageDrawPixel |  | PlaySound |
| SetTargetFPS | DrawRectangleRounded | ImageDrawPixelV |  | ResumeAudioStream |
| ShowCursor | DrawRectangleRoundedLines | ImageDrawRectangle |  | ResumeMusicStream |
|  | DrawRectangleV | ImageDrawRectangleLines |  | ResumeSound |
|  | DrawRing | ImageDrawRectangleRec |  | SeekMusicStream |
|  | DrawRingLines | ImageDrawText |  | SetAudioStreamBufferSizeDefault |
|  | DrawTriangle | ImageFlipHorizontal |  | SetAudioStreamPan |
|  | DrawTriangleLines | ImageFlipVertical |  | SetAudioStreamPitch |
|  | GetCollisionRec | ImageResize |  | SetAudioStreamVolume |
|  |  | ImageResizeNN |  | SetMasterVolume |
|  |  | ImageRotateCCW |  | SetMusicPan |
|  |  | ImageRotateCW |  | SetMusicPitch |
|  |  | LoadImage |  | SetMusicVolume |
|  |  | LoadTexture |  | SetSoundPan |
|  |  | LoadTextureFromImage |  | SetSoundPitch |
|  |  | SetTextureFilter |  | SetSoundVolume |
|  |  | SetTextureWrap |  | StopAudioStream |
|  |  | UnloadImage |  | StopMusicStream |
|  |  | UnloadTexture |  | StopSound |
|  |  |  |  | UnloadAudioStream |
|  |  |  |  | UnloadMusicStream |
|  |  |  |  | UnloadSound |
|  |  |  |  | UnloadSoundAlias |
|  |  |  |  | UnloadWave |
|  |  |  |  | UpdateAudioStream |
|  |  |  |  | UpdateMusicStream |
|  |  |  |  | WaveCopy |
|  |  |  |  | WaveCrop |
|  |  |  |  | WaveFormat |

---

*Generated on Tue Nov 11 04:15:21 EST 2025*
