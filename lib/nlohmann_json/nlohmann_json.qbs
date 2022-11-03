import qbs.FileInfo

StaticLibrary {
    name: "nlohmann_json"

    Depends { name: "cpp" }
    
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src"]
    }

    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: ["src"]

    files: [
        "src/nlohmann/json/json.hpp",
    ]
}
