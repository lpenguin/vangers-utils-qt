import qbs.FileInfo;

Library {
    name: "vangers-utils-core"

    Depends {
        name: "Qt";
        submodules: ["core", "widgets"];
        versionAtLeast: "5.12"
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

    type: singleBinary ? "staticlibrary" : "dynamiclibrary"
    install: !singleBinary

    files: [
        "palettes.qrc",
        "src/vangers/core/error/error.h",
        "src/vangers/core/palette/palette.h",
        "src/vangers/core/palette/palette.cpp",
        "src/vangers/core/binaryio.h",
        "src/vangers/core/optional.h",
        "src/vangers/core/ini/iniaccess.cpp",
        "src/vangers/core/ini/iniaccess.h",
        "src/vangers/core/ini/inisettings.cpp",
        "src/vangers/core/ini/inisettings.h",
        "src/vangers/core/ini/inistreamreader.cpp",
        "src/vangers/core/ini/inistreamreader.h",
        "src/vangers/core/ini/inistreamwriter.cpp",
        "src/vangers/core/ini/inistreamwriter.h",
        "src/vangers/core/vector/vector.h",
        "src/vangers/core/vector/vectorjsonext.h",
        "src/vangers/core/vector/vectorstringext.cpp",
        "src/vangers/core/vector/vectorstringext.h",
        "src/vangers/core/plugin/resourceaccess.h",
        "src/vangers/core/plugin/resourceviewer.h",
        "src/vangers/core/plugin/resourceviewer.cpp",
    ]
}
