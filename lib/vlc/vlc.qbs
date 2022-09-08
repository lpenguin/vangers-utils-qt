import qbs.FileInfo

StaticLibrary {
    name: "vlc"

    Depends {
        name: "Qt";
        submodules: ["core"];
        versionAtLeast: "5.12"
    }

    Depends {
        name: "vangers-utils-core"
    }

    Depends {
        name: "nlohmann_json"
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/vlc/vlc.h",
        "src/vlc/vlcbinaryreaderext.cpp",
        "src/vlc/vlcbinaryreaderext.h",
        "src/vlc/vlcjsonext.cpp",
        "src/vlc/vlcjsonext.h",
    ]
}
