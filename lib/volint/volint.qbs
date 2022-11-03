import qbs.FileInfo

StaticLibrary {
    name: "volint"

    Depends { name: "cpp"; }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/volint/volint.h",
        "src/volint/volint.cpp",
    ]
}
