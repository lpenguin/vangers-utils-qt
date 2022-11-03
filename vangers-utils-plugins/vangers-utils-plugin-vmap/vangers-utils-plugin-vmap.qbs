import qbs.FileInfo

DynamicLibrary {
    name: "vangers-utils-plugin-vmap"

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


    Depends {
        name: "vangers-utils-plugin-palette"
    }

    Depends {
        name: "splay"
    }

    Depends {
        name: "vox"
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
        "src/vangers/plugins/vmap/layer/abstractimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/abstractimagelayer.h",
        "src/vangers/plugins/vmap/layer/allimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/allimagelayer.h",
        "src/vangers/plugins/vmap/layer/deltaimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/deltaimagelayer.h",
        "src/vangers/plugins/vmap/layer/doublelevelimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/doublelevelimagelayer.h",
        "src/vangers/plugins/vmap/layer/heightimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/heightimagelayer.h",
        "src/vangers/plugins/vmap/layer/isshadowedimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/isshadowedimagelayer.h",
        "src/vangers/plugins/vmap/layer/terrainimagelayer.cpp",
        "src/vangers/plugins/vmap/layer/terrainimagelayer.h",
        "src/vangers/plugins/vmap/layer/floodlayer.h",
        "src/vangers/plugins/vmap/layer/floodlayer.cpp",
        "src/vangers/plugins/vmap/matrix.h",
        "src/vangers/plugins/vmap/vmap.cpp",
        "src/vangers/plugins/vmap/vmap.h",
        "src/vangers/plugins/vmap/vmapiniwriter.cpp",
        "src/vangers/plugins/vmap/vmapiniwriter.h",
        "src/vangers/plugins/vmap/vmapmeta.cpp",
        "src/vangers/plugins/vmap/vmapmeta.h",
        "src/vangers/plugins/vmap/vmapparams.h",
        "src/vangers/plugins/vmap/vmapparamsreader.cpp",
        "src/vangers/plugins/vmap/vmapparamsreader.h",
        "src/vangers/plugins/vmap/vmapreader.cpp",
        "src/vangers/plugins/vmap/vmapreader.h",
        "src/vangers/plugins/vmap/vmapviewer.ui",
        "src/vangers/plugins/vmap/vmapviewerplugin.cpp",
        "src/vangers/plugins/vmap/vmapviewerplugin.h",
        "src/vangers/plugins/vmap/vmapvmpwriter.cpp",
        "src/vangers/plugins/vmap/vmapvmpwriter.h",
        "src/vangers/plugins/vmap/vmapvoxwriter.cpp",
        "src/vangers/plugins/vmap/vmapvoxwriter.h",
        "src/vangers/plugins/vmap/imagegraphicsview.cpp",
        "src/vangers/plugins/vmap/imagegraphicsview.h",
        "src/vangers/plugins/vmap/voxwritersettingsdialog.cpp",
        "src/vangers/plugins/vmap/voxwritersettingsdialog.h",
        "src/vangers/plugins/vmap/voxwritersettingsdialog.ui",
        "src/vangers/plugins/vmap/vprwriter.cpp",
        "src/vangers/plugins/vmap/vprwriter.h",
    ]
}
