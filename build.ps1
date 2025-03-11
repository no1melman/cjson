Remove-Item -Recurse -ErrorAction Ignore build
cmake -G Ninja -B build
#if ($ExitCode -ne 0)
#{
#  exit 1;
#}
cmake --build build
#if ($ExitCode -ne 0)
#{
#  exit 1;
#}
Copy-Item -Force ./build/compile_commands.json ./compile_commands.json
'{ "something": "else", "you": "there" }' > ./build/test.json
