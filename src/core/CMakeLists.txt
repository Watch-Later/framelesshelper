find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)

set(SOURCES
    core.qrc
    customwindow_global.h
    systemeventhandler.h
    settings.h
    settings.cpp
    draw.h
    draw.cpp
    utils.h
    utils.cpp
)

if(WIN32)
    list(APPEND SOURCES
        core_windows.h
        systemeventhandler_win32.cpp
        utils_win32.cpp
    )
endif()

if(LINUX)
    list(APPEND SOURCES
        systemeventhandler_x11.cpp
        utils_x11.cpp
    )
endif()

if(DARWIN)
    list(APPEND SOURCES
        systemeventhandler_darwin.cpp
        utils_darwin.cpp
    )
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    enable_language(RC)
    list(APPEND SOURCES core.rc)
endif()

add_library(CustomWindowCore ${SOURCES})
add_library(wangwenx190::CustomWindow::Core ALIAS CustomWindowCore)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(CustomWindowCore INTERFACE
        CUSTOMWINDOW_STATIC
    )
endif()

target_compile_definitions(CustomWindowCore PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_KEYWORDS
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060200
    CUSTOMWINDOW_BUILD_LIBRARY
)

target_link_libraries(CustomWindowCore PRIVATE
    Qt${QT_VERSION_MAJOR}::GuiPrivate
)

target_include_directories(CustomWindowCore INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
