#!/bin/bash

# MSRLWeb Release Builder
# Creates a distributable package ready for GitHub release

set -e  # Exit on error

VERSION=${1:-dev}
DIST_DIR="dist/msrlweb-${VERSION}"
ZIP_FILE="msrlweb-${VERSION}.zip"

echo "Building MSRLWeb release package (version: ${VERSION})..."
echo ""

# Step 1: Update version number in source code
echo "[1/7] Updating version number in main.cpp..."
# Extract major.minor from version (e.g., 0.2.1 -> 0.2) for numeric hostVersion
HOST_VERSION=$(echo "${VERSION}" | sed 's/^\([0-9]*\.[0-9]*\).*/\1/')
sed -i '' "s/MiniScript::hostVersion = [0-9.]*;/MiniScript::hostVersion = ${HOST_VERSION};/" src/main.cpp
echo "Set hostVersion to ${HOST_VERSION} (from release version ${VERSION})"

# Step 2: Clean and build
echo "[2/7] Clean building project..."
./build.sh clean
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Step 3: Generate API documentation
echo "[3/7] Generating API documentation..."
./generate_api_doc.sh

# Step 4: Create distribution directory
echo "[4/7] Creating distribution directory..."
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"

# Step 5: Copy build artifacts
echo "[5/7] Copying build artifacts..."
cp build/msrlweb.html "$DIST_DIR/"
cp build/msrlweb.js "$DIST_DIR/"
cp build/msrlweb.wasm "$DIST_DIR/"
cp index.html "$DIST_DIR/"

# Step 6: Copy assets and documentation
echo "[6/7] Copying assets and documentation..."
cp -r assets "$DIST_DIR/"
cp RAYLIB_API.md "$DIST_DIR/"

# Create distribution README
cat > "$DIST_DIR/README.md" << 'EOF'
# MSRLWeb - MiniScript + Raylib for Web

MSRLWeb lets you create 2D games and graphics applications using [MiniScript](https://miniscript.org/) with [Raylib](https://www.raylib.com/) graphics, running in your web browser via WebAssembly.

## Quick Start

1. **Start a local web server** in this directory:
   ```bash
   python3 -m http.server 8000
   ```

   Or use any other web server (Live Server in VS Code, `npx serve`, etc.)

2. **Open in browser:**
   ```
   http://localhost:8000
   ```

3. **Edit the game:**
   - Open `assets/main.ms` in your favorite text editor
   - Edit the MiniScript code
   - Reload the browser to see your changes

## What's Included

- `index.html` - Landing page (redirects to msrlweb.html)
- `msrlweb.html` - The main HTML page
- `msrlweb.js` - JavaScript loader
- `msrlweb.wasm` - WebAssembly binary (MiniScript + Raylib)
- `assets/main.ms` - Your MiniScript game code
- `assets/Wumpus.png` - Example sprite
- `assets/wooble.wav` - Example sound effect
- `RAYLIB_API.md` - Complete API reference (174 functions!)

## MiniScript Basics

MiniScript is a simple, elegant scripting language. Here's a taste:

```miniscript
// Variables
x = 100
name = "Player"

// Lists and Maps
inventory = ["sword", "shield", "potion"]
player = {"name": "Hero", "hp": 100, "x": 50, "y": 50}

// Functions
greet = function(name)
    return "Hello, " + name + "!"
end function

// Classes
Enemy = {}
Enemy.hp = 50
Enemy.attack = function()
    print "Enemy attacks!"
end function
```

## Raylib API

MSRLWeb provides 174 Raylib functions organized into modules:
- **rcore** (29 functions) - Window, input, timing
- **rshapes** (35 functions) - Drawing primitives, collision
- **rtextures** (45 functions) - Images and textures
- **rtext** (12 functions) - Fonts and text
- **raudio** (53 functions) - Sound effects and music

See `RAYLIB_API.md` for the complete list.

## Example: Drawing and Input

```miniscript
// Game loop
x = 100
y = 100
while true
    raylib.BeginDrawing
    raylib.ClearBackground raylib.RAYWHITE

    // Handle input
    if raylib.IsKeyDown(raylib.KEY_RIGHT) then x += 5
    if raylib.IsKeyDown(raylib.KEY_LEFT) then x -= 5

    // Draw
    raylib.DrawCircle x, y, 20, raylib.RED
    raylib.DrawText "Use arrow keys!", 10, 10

    raylib.EndDrawing
    yield  // Let the browser breathe
end while
```

## Audio

Audio is initialized automatically. Load and play sounds:

```miniscript
sound = raylib.LoadSound("assets/beep.wav")
raylib.SetSoundPitch sound, 1.5
raylib.PlaySound sound
```

For music (streaming):

```miniscript
music = raylib.LoadMusicStream("assets/song.ogg")
raylib.PlayMusicStream music

// In your main loop:
raylib.UpdateMusicStream music  // Must call each frame!
```

## Resources

- **MiniScript**: https://miniscript.org/
  - [Quick Reference](https://miniscript.org/files/MiniScript-QuickRef.pdf)
  - [Interactive Tutorial](https://miniscript.org/tryit/)
- **Raylib**: https://www.raylib.com/
  - [Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- **MSRLWeb Source**: https://github.com/JoeStrout/MiniScript/tree/main/MSRLWeb

## Tips

1. **Always yield in loops** - Use `yield` in your main loop to let the browser remain responsive
2. **Reload to restart** - Just refresh the browser page to restart your game
3. **Default values** - Most Raylib functions have sensible defaults, e.g., `raylib.DrawRectangle` works without any parameters!

## Troubleshooting

**Nothing appears on screen:**
- Make sure you're calling `raylib.BeginDrawing` and `raylib.EndDrawing`
- Check for errors displayed below the game window

**Audio doesn't play:**
- Make sure your audio files are in the `assets/` folder
- Some browsers require user interaction before playing audio

**Game runs slowly:**
- Make sure you have `yield` in your main loop
- Reduce the number of draw calls per frame
- Use smaller images/textures

---

Happy game making! 🎮
EOF

# Step 7: Create zip file
echo "[7/7] Creating release archive..."
cd dist
rm -f "../${ZIP_FILE}"
zip -r "../${ZIP_FILE}" "msrlweb-${VERSION}" > /dev/null
cd ..

echo ""
echo "✓ Release package created successfully!"
echo ""
echo "  Archive: ${ZIP_FILE}"
echo "  Size: $(du -h ${ZIP_FILE} | cut -f1)"
echo ""
echo "Next steps:"
echo "  1. Test the package:"
echo "     cd ${DIST_DIR}"
echo "     python3 -m http.server 8000"
echo "     (then open a browser to localhost:8000)"
echo ""
echo "  2. Create a GitHub release:"
echo "     - Go to: https://github.com/JoeStrout/MSRLWeb/releases/new"
echo "     - Tag version: v${VERSION}"
echo "     - Title: MSRLWeb ${VERSION}"
echo "     - Upload: ${ZIP_FILE}"
echo ""
