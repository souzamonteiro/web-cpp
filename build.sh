#!/bin/sh

rm -rf build/*
rm -rf docs/*

# Creates uncompressed versions of the library.
cat src/Shebang.js src/WebCpp.js src/ComplexNumber.js src/WebCppCompiler.js src/Core.js src/CppLib.js src/WebCppVM.js > build/webcpp.js
cat src/WebCpp.js src/ComplexNumber.js src/WebCppCompiler.js src/Core.js src/CppLib.js > build/libcpp.js

cp build/webcpp.js bin/
cp build/webcpp.js js/
cp build/libcpp.js js/

chmod 755 bin/*

jsdoc -d ./docs ./package.json ./src

mkdir docs/grammar
cp -r grammar/WebCpp.xhtml docs/grammar
