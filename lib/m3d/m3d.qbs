import qbs.FileInfo

StaticLibrary {
    name: "m3d"

    Depends {
        name: "Qt";
        submodules: ["core"];
        versionAtLeast: "5.12"
    }

    Depends { name: "cpp" }

    Depends { name: "vangers-utils-core" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/m3d/m3d.h",
        "src/m3d/colortable.cpp",
        "src/m3d/colortable.h",
        "src/m3d/modelaccess.cpp",
        "src/m3d/modelaccess.h",
        "src/m3d/modelbinaryreaderext.cpp",
        "src/m3d/modelbinaryreaderext.h",
        "src/m3d/modelbinarywriterext.cpp",
        "src/m3d/modelbinarywriterext.h",
    ]
}
