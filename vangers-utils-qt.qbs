import qbs.FileInfo

Project {
    property bool singleBinary: true

    SubProject {
        filePath: "vangers-utils-core/vangers-utils-core.qbs"
    }

    SubProject {
        filePath: "vangers-utils-gui/vangers-utils-gui.qbs"
    }

    SubProject {
        filePath: "vangers-utils-plugins/vangers-utils-plugins.qbs"
    }

    SubProject {
        filePath: "lib/lib.qbs"
    }

    SubProject {
        filePath: "model2vmap/model2vmap.qbs"
    }
}

