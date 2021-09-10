find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui Widgets Quick)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Widgets Quick)

if(TARGET Qt${QT_VERSION_MAJOR}::Gui)
    add_subdirectory(window)
endif()

if(TARGET Qt${QT_VERSION_MAJOR}::Widgets)
    add_subdirectory(widget)
endif()

if(TARGET Qt${QT_VERSION_MAJOR}::Quick)
    add_subdirectory(quick)
endif()
