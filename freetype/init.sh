rm -fr deps
mkdir deps
pushd deps

git clone https://github.com/freetype/freetype.git
pushd freetype
cmake -B build
popd

popd

cmake -B build

echo ""
echo "Done."
echo "Firstly, You need to open deps/freetype/build/freetype.sln and build freetype project."
echo "Happy Hacking!"
