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
        "src/graphicsviewzoom.cpp",
        "src/graphicsviewzoom.h",
        "src/image/mssimageaccess.cpp",
        "src/image/mssimageaccess.h",
        "src/image/qtimageaccess.cpp",
        "src/image/qtimageaccess.h",
        "src/image/svsimageaccess.cpp",
        "src/image/svsimageaccess.h",
        "src/imagegraphicsview.cpp",
        "src/imagegraphicsview.h",
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
        "src/plugins/vmap/layer/abstractimagelayer.cpp",
        "src/plugins/vmap/layer/abstractimagelayer.h",
        "src/plugins/vmap/layer/allimagelayer.cpp",
        "src/plugins/vmap/layer/allimagelayer.h",
        "src/plugins/vmap/layer/deltaimagelayer.cpp",
        "src/plugins/vmap/layer/deltaimagelayer.h",
        "src/plugins/vmap/layer/doublelevelimagelayer.cpp",
        "src/plugins/vmap/layer/doublelevelimagelayer.h",
        "src/plugins/vmap/layer/heightimagelayer.cpp",
        "src/plugins/vmap/layer/heightimagelayer.h",
        "src/plugins/vmap/layer/isshadowedimagelayer.cpp",
        "src/plugins/vmap/layer/isshadowedimagelayer.h",
        "src/plugins/vmap/layer/terrainimagelayer.cpp",
        "src/plugins/vmap/layer/terrainimagelayer.h",
        "src/plugins/vmap/matrix.h",
        "src/plugins/vmap/vmap.cpp",
        "src/plugins/vmap/vmap.h",
        "src/plugins/vmap/vmapmeta.cpp",
        "src/plugins/vmap/vmapmeta.h",
        "src/plugins/vmap/vmapreader.cpp",
        "src/plugins/vmap/vmapreader.h",
        "src/plugins/vmap/vmapviewer.ui",
        "src/plugins/vmap/vmapviewerplugin.cpp",
        "src/plugins/vmap/vmapviewerplugin.h",
        "src/plugins/vmap/vmapwriter.cpp",
        "src/plugins/vmap/vmapwriter.h",
        "src/qimagewidget.cpp",
        "src/qimagewidget.h",
        "src/span.h",
        "src/splay/splay.cpp",
        "src/splay/splay.h",
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
