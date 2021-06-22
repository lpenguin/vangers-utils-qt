import qbs.FileInfo

QtApplication {
    name: "vangers-utils-qt"

    Depends { name: "Qt"; submodules: ["core", "widgets"]; versionAtLeast: "5.6" }

    // The following define makes your compiler emit warnings if you use
    // any Qt feature that has been marked deprecated (the exact warnings
    // depend on your compiler). Please consult the documentation of the
    // deprecated API in order to know how to port your code away from it.
    // You can also make your code fail to compile if it uses deprecated APIs.
    // In order to do so, uncomment the second entry in the list.
    // You can also select to disable deprecated APIs only up to a certain version of Qt.
    cpp.defines: [
        "QT_DEPRECATED_WARNINGS",
        /* "QT_DISABLE_DEPRECATED_BEFORE=0x060000" */ // disables all the APIs deprecated before Qt 6.0.0
    ]

    cpp.includePaths: [product.sourceDirectory, "src"]

    files: [
        "Readme.md",
        "src/mainwindow.ui",
        "src/palettegrid.cpp",
        "src/palettegrid.h",
        "src/palettes.qrc",
        "src/paletteview.cpp",
        "src/paletteview.h",
        "src/paletteview.ui",
        "src/binaryio.h",
        "src/image/bmpimage.cpp",
        "src/image/bmpimage.h",
        "src/image/image.cpp",
        "src/image/image.h",
        "src/image/palette.cpp",
        "src/image/palette.h",
        "src/image/pngimage.cpp",
        "src/image/pngimage.h",
        "src/image/xbmimage.cpp",
        "src/image/xbmimage.h",
        "src/main.cpp",
        "src/mainwindow.cpp",
        "src/mainwindow.h",
        "src/plugins/palette/microsoftpalaccess.cpp",
        "src/plugins/palette/microsoftpalaccess.h",
        "src/plugins/palette/pngpaletteaccess.cpp",
        "src/plugins/palette/pngpaletteaccess.h",
        "src/plugins/palette/vangerspaletteaccess.cpp",
        "src/plugins/palette/vangerspaletteaccess.h",
        "src/plugins/resourceaccess.h",
        "src/plugins/resourceviewer.cpp",
        "src/plugins/resourceviewer.h",
        "src/plugins/image/imageviewer.cpp",
        "src/plugins/image/imageviewer.h",
        "src/plugins/image/imageviewer.ui",
        "src/plugins/palette/paletteviewer.cpp",
        "src/plugins/palette/paletteviewer.h",
        "src/plugins/palette/paletteviewer.ui",
        "src/qimagewidget.cpp",
        "src/qimagewidget.h",
    ]

    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base

    Group {
        name: "Desktop file (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/applications"
        files: [ "./com.lpenguin.vangers-utils.desktop" ]
    }

    Group {
        name: "Images (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/icons/hicolor/256x256/apps"
        files: [ "./VangersUtils.png" ]
    }
}
