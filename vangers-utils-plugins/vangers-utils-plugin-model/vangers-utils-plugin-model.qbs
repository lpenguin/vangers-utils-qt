import qbs.FileInfo

DynamicLibrary {
    name: "vangers-utils-plugin-model"

    Depends {
        name: "Qt";
        submodules: ["core", "widgets", "3drender", "3dextras"];
        versionAtLeast: "5.12"
    }

    Depends {
        name: "vangers-utils-core"
    }

    Depends { name: "m3d" }

    Depends { name: "obj" }

    Depends { name: "mtool" }

    Depends { name: "volint" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [product.sourceDirectory + "/src"]
    }


    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    type: singleBinary ? "staticlibrary" : "dynamiclibrary"
    install: !singleBinary

    files: [
        "src/vangers/plugins/model/c3drenderer.cpp",
        "src/vangers/plugins/model/c3drenderer.h",
        "src/vangers/plugins/model/c3dview.cpp",
        "src/vangers/plugins/model/c3dview.h",
        "src/vangers/plugins/model/m3dobjectscontroller.cpp",
        "src/vangers/plugins/model/m3dobjectscontroller.h",
        "src/vangers/plugins/model/gridmesh.cpp",
        "src/vangers/plugins/model/gridmesh.h",


//        "src/vangers/plugins/model/m3djson/m3djsonext.h",
//        "src/vangers/plugins/model/m3djson/m3djsonext.cpp",
//        "src/vangers/plugins/model/m3djson/m3djsonaccess.cpp",
//        "src/vangers/plugins/model/m3djson/m3djsonaccess.h",
        "src/vangers/plugins/model/modelviewer.cpp",
        "src/vangers/plugins/model/modelviewer.h",
        "src/vangers/plugins/model/modelviewer.ui",
        "src/vangers/plugins/model/modelviewerplugin.cpp",
        "src/vangers/plugins/model/modelviewerplugin.h",
        "src/vangers/plugins/model/myorbitcameracontroller.cpp",
        "src/vangers/plugins/model/myorbitcameracontroller.h",
        "src/vangers/plugins/model/objimport/objexporter.cpp",
        "src/vangers/plugins/model/objimport/objexporter.h",
        "src/vangers/plugins/model/objimport/objimportcontroller.cpp",
        "src/vangers/plugins/model/objimport/objimportcontroller.h",
        "src/vangers/plugins/model/objimport/objimporter.cpp",
        "src/vangers/plugins/model/objimport/objimporter.h",
        "src/vangers/plugins/model/objimport/objimportsettings.cpp",
        "src/vangers/plugins/model/objimport/objimportsettings.h",
        "src/vangers/plugins/model/objimport/objimportsettingsaccess.cpp",
        "src/vangers/plugins/model/objimport/objimportsettingsaccess.h",
        "src/vangers/plugins/model/objimport/objimportsettingswidget.cpp",
        "src/vangers/plugins/model/objimport/objimportsettingswidget.h",
        "src/vangers/plugins/model/objimport/objimportsettingswidget.ui",
        "src/vangers/plugins/model/propertytree.cpp",
        "src/vangers/plugins/model/propertytree.h",
        "src/vangers/plugins/model/scenecontroller.cpp",
        "src/vangers/plugins/model/scenecontroller.h",
        "src/vangers/plugins/model/slotview.cpp",
        "src/vangers/plugins/model/slotview.h",
    ]
}
