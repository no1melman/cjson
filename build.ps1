Remove-Item -Recurse -ErrorAction Ignore build
cmake -G Ninja -B build
cmake --build build
Copy-Item -Force ./build/compile_commands.json ./compile_commands.json
