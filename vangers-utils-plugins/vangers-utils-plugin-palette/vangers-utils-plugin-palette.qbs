import qbs.FileInfo

DynamicLibrary {
    name: "vangers-utils-plugin-palette"

    Depends {
        name: "Qt";
        submodules: ["core", "widgets"];
        versionAtLeast: "5.12"
    }

    Depends {
        name: "vangers-utils-core"
    }

    Depends {
        name: "vangers-utils-plugin-image"
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }


    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    type: singleBinary ? "staticlibrary" : "dynamiclibrary"
    install: !singleBinary

    files: [
        "src/vangers/plugins/palette/microsoftpalaccess.cpp",
        "src/vangers/plugins/palette/microsoftpalaccess.h",
        "src/vangers/plugins/palette/pngpaletteaccess.cpp",
        "src/vangers/plugins/palette/pngpaletteaccess.h",
        "src/vangers/plugins/palette/vangerspaletteaccess.cpp",
        "src/vangers/plugins/palette/vangerspaletteaccess.h",
        "src/vangers/plugins/palette/paletteviewer.cpp",
        "src/vangers/plugins/palette/paletteviewer.h",
        "src/vangers/plugins/palette/paletteviewer.ui",
    ]
}
