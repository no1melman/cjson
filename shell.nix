{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    llvmPackages.clang # Clang compiler
    llvmPackages.libclang # Required for clangd to work properly
    clang-tools # Provides clangd, clang-tidy, etc.
    cmake # CMake for build configuration
    ninja # Ninja build system
    gnumake # GNU Make (optional)
    gcc # Standard C++ library (libstdc++)
  ];

  shellHook = ''
    echo "C++ Development Environment Ready!"
    echo "Use 'clangd' for autocompletion."

    build_project() {
      echo "Building project..."
      cmake -G Ninja -B build && cmake --build build
      cp ./build/compile_commands.json ./
    }

    run_project() {
      ./build/cjson "compile_commands.json"
    }
  '';
}
