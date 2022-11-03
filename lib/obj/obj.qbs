import qbs.FileInfo

StaticLibrary {
    name: "obj"

    Depends {
        name: "Qt";
        submodules: ["core"];
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

    files: [
        "src/obj/obj.h",
        "src/obj/obj.cpp",
        "src/obj/objreader.h",
        "src/obj/objreader.cpp",
        "src/obj/objwriter.h",
        "src/obj/objwriter.cpp",
    ]
}
