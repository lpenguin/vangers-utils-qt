import qbs.FileInfo

StaticLibrary {
    name: "splay"

    Depends {
        name: "Qt";
        submodules: ["core"];
        versionAtLeast: "5.12"
    }

    Depends {
        name: "vangers-utils-core"
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/splay/splay.h",
        "src/splay/splay.cpp",
    ]
}
