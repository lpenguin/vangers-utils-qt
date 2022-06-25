import qbs.FileInfo

DynamicLibrary {
    name: "vangers-utils-plugin-image"

    Depends {
        name: "Qt";
        submodules: ["core", "widgets"];
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

    type: singleBinary ? "staticlibrary" : "dynamiclibrary"
    install: !singleBinary

    files: [
        "src/vangers/plugins/image/graphicsviewzoom.cpp",
        "src/vangers/plugins/image/graphicsviewzoom.h",
        "src/vangers/plugins/image/bmpimage.cpp",
        "src/vangers/plugins/image/bmpimage.h",
        "src/vangers/plugins/image/image.cpp",
        "src/vangers/plugins/image/image.h",
        "src/vangers/plugins/image/xbmimage.cpp",
        "src/vangers/plugins/image/xbmimage.h",
        "src/vangers/plugins/image/mssimageaccess.h",
        "src/vangers/plugins/image/mssimageaccess.cpp",
        "src/vangers/plugins/image/svsimageaccess.h",
        "src/vangers/plugins/image/svsimageaccess.cpp",
        "src/vangers/plugins/image/qtimageaccess.cpp",
        "src/vangers/plugins/image/qtimageaccess.h",
        "src/vangers/plugins/image/imageviewer.cpp",
        "src/vangers/plugins/image/imageviewer.h",
        "src/vangers/plugins/image/imageviewer.ui",
        "src/vangers/plugins/image/palettegrid.h",
        "src/vangers/plugins/image/palettegrid.cpp",
        "src/vangers/plugins/image/paletteview.ui",
        "src/vangers/plugins/image/paletteview.h",
        "src/vangers/plugins/image/paletteview.cpp",
        "src/vangers/plugins/image/qimagewidget.h",
        "src/vangers/plugins/image/qimagewidget.cpp",
        "src/vangers/plugins/image/palettes.qrc",
    ]
}
