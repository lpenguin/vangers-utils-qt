import qbs.FileInfo

StaticLibrary {
    name: "vox"

    Depends {
        name: "Qt";
        submodules: ["core"];
        versionAtLeast: "5.12"
    }

    Depends {
        name: "vangers-utils-core"
    }

    Depends {
        name: "riff"
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/vox/vox.h",
        "src/vox/voxbinaryreaderext.h",
        "src/vox/voxbinaryreaderext.cpp",
        "src/vox/voxbinarywriterext.h",
        "src/vox/voxbinarywriterext.cpp",
    ]
}
