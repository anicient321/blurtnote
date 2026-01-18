cmake_minimum_required(VERSION 3.19)
project(BlurtNote2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg)

qt_standard_project_setup()

qt_add_executable(blurtnote
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    themeManager.h
    themeManager.cpp
    resources.qrc
    documentManager.h
    documentManager.cpp
    aboutdialog.h
    aboutdialog.cpp
)

qt_add_resources(app_resources resources.qrc)

target_link_libraries(blurtnote
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
)

include(GNUInstallDirs)

install(TARGETS blurtnote
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET blurtnote
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
