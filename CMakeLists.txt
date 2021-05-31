cmake_minimum_required(VERSION 3.5)

project(vangers-utils-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.ui
    src/palettegrid.cpp
    src/palettegrid.h
    src/palettes.qrc
    src/paletteview.cpp
    src/paletteview.h
    src/paletteview.ui
    src/binaryio.h
    src/image/bmpimage.cpp
    src/image/bmpimage.h
    src/image/image.cpp
    src/image/image.h
    src/image/palette.cpp
    src/image/palette.h
    src/image/pngimage.cpp
    src/image/pngimage.h
    src/image/xbmimage.cpp
    src/image/xbmimage.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/plugins/imageviewer.cpp
    src/plugins/imageviewer.h
    src/plugins/imageviewer.ui
    src/plugins/paletteviewer.cpp
    src/plugins/paletteviewer.h
    src/plugins/paletteviewer.ui
    src/qimagewidget.cpp
    src/qimagewidget.h
    src/resourceviewer.cpp
    src/resourceviewer.h
)

include_directories(src/)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(vangers-utils-qt
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(vangers-utils-qt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(vangers-utils-qt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

IF (WIN32)
  target_link_libraries(vangers-utils-qt -static gcc stdc++ winpthread -dynamic Qt${QT_VERSION_MAJOR}::Widgets)
ELSE()
  target_link_libraries(vangers-utils-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
ENDIF()



install(TARGETS vangers-utils-qt
        RUNTIME DESTINATION usr/bin)

install(FILES com.lpenguin.vangers-utils.desktop
    DESTINATION usr/share/applications
)

install(FILES VangersUtils.png
    DESTINATION usr/share/icons/hicolor/256x256/apps
)