import qbs.FileInfo

StaticLibrary {
    name: "mtool"

    Depends { name: "cpp" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/mtool/ModelClass.h",
        "src/mtool/ModelClass.cpp",
    ]
}
