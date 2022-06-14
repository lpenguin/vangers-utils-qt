QtApplication {
    name: "model2vmap"

    Depends {
        name: "vangers-utils-core"
    }

    Depends {
        name: "vangers-utils-plugin-image"
    }

    Depends {
        name: "vangers-utils-plugin-palette"
    }

    Depends {
        name: "vangers-utils-plugin-vmap"
    }

    Depends {
        name: "vangers-utils-plugin-model"
    }

    Depends {
        name: "obj"
    }

    Depends {
        name: "Qt";
        submodules: ["core", "widgets", "3dinput", "3drender", "3dextras"];
        versionAtLeast: "5.12"
    }

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

    cpp.cxxLanguageVersion: "c++20"

    cpp.includePaths: [
        "src",
    ]

    files: [
        "src/vangers/model2vmap/heightmapfrommesh.cpp",
        "src/vangers/model2vmap/heightmapfrommesh.h",
        "src/vangers/model2vmap/main.cpp",
    ]

    install: true

    Group {
        name: "Desktop file (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/applications"
        files: [ "./com.lpenguin.model2vmap.desktop" ]
    }

    Group {
        name: "Images (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/icons/hicolor/256x256/apps"
        files: [ "./model2vmap.png" ]
    }

}

