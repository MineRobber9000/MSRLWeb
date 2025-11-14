#!/bin/sh

# Update the demo hosted on GitHub Pages (in the docs directory),
# from the current state of the build and assets folders.


echo "Copying build/msrlweb.*"
cp build/msrlweb.* docs/

echo "Copying build/assets"
rm -rf docs/assets
cp -r assets docs/

git add docs/
echo "docs folder updated!"
