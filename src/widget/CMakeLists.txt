find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(SOURCES
    customwidget.h
    customtitlebarwidget.h
    customtitlebarwidget.cpp
)

if(WIN32)
    list(APPEND SOURCES
        customwidget_win32.cpp
    )
endif()

if(LINUX)
    list(APPEND SOURCES
        customwidget_x11.cpp
    )
endif()

if(DARWIN)
    list(APPEND SOURCES
        customwidget_darwin.cpp
    )
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    enable_language(RC)
    list(APPEND SOURCES widget.rc)
endif()

add_library(CustomWindowWidget ${SOURCES})
add_library(wangwenx190::CustomWindow::Widget ALIAS CustomWindowWidget)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(CustomWindowWidget INTERFACE
        CUSTOMWINDOW_STATIC
    )
endif()

target_compile_definitions(CustomWindowWidget PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_KEYWORDS
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060200
    CUSTOMWINDOW_BUILD_LIBRARY
)

target_link_libraries(CustomWindowWidget PRIVATE
    Qt${QT_VERSION_MAJOR}::WidgetsPrivate
    wangwenx190::CustomWindow::Core
)

target_include_directories(CustomWindowWidget INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
