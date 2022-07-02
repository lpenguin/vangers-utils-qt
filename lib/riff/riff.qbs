import qbs.FileInfo

StaticLibrary {
    name: "riff"

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
        "src/riff/riff.h",
        "src/riff/riff.cpp",
        "src/riff/riffbinaryreaderext.h",
        "src/riff/riffbinaryreaderext.cpp",
        "src/riff/riffbinarywriterext.h",
        "src/riff/riffbinarywriterext.cpp",
        "src/riff/riffencoding.h",
    ]
}
