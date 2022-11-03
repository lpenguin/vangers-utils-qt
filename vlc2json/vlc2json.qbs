Product  {
    name: "vlc2json"
    type: "application"

    Depends {
        name: "Qt";
        submodules: ["core"];
    }

    Depends {
        name: "vangers-utils-core"
    }

    Depends {
        name: "nlohmann_json"
    }


    Depends {
        name: "vlc"
    }

    cpp.defines: [
        "QT_DEPRECATED_WARNINGS",
        /* "QT_DISABLE_DEPRECATED_BEFORE=0x060000" */ // disables all the APIs deprecated before Qt 6.0.0
    ]

    cpp.cxxLanguageVersion: "c++20"

    cpp.includePaths: [
        "src",
    ]

    files: [
        "src/vlc2json/main.cpp",
    ]

    Group {
        name: "Desktop file (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/applications"
        files: [ "./com.lpenguin.vlc2json.desktop" ]
    }

    Group {
        name: "Images (Linux)"
        condition: qbs.targetOS.contains("linux")
        qbs.install: true
        qbs.installDir: "share/icons/hicolor/256x256/apps"
        files: [ "./vlc2json.png" ]
    }

}

