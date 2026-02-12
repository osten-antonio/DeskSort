cmake_minimum_required(VERSION 3.16)

project(desksort VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

enable_language(RC)

set(PROJECT_SOURCES
    main.cpp
    components/mainwindow.cpp
    components/clickableframe.cpp
    components/rule_set_container.cpp
    components/rulesetcreate.cpp
    components/filtercreate.cpp
    components/editfilterscreen.cpp

    components/headers/mainwindow.h
    components/headers/clickableframe.h
    components/headers/rule_set_container.h
    components/headers/rulesetcreate.h
    components/headers/filtercreate.h
    components/headers/editfilterscreen.h

    ui/mainwindow.ui
    ui/editfilterscreen.ui
    ui/filtercreate.ui
    ui/rulesetcreate.ui


)

set(PROJECT_C_SOURCES
    script.c
    sqlite3.c
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})


include_directories(
    components/headers
)





if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(desksort
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_C_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS}
        script.h
        sqlite3.h
    )




# Define target properties for Android with Qt 6 as:
#    set_property(TARGET desksort APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(desksort SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(desksort
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_sources(desksort PRIVATE
    ${PROJECT_SOURCES}
    ${PROJECT_C_SOURCES})
# find_library(SQLITE3_LIB sqlite3 PATHS C:/MinGW/include)
target_link_libraries(desksort PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


target_link_libraries(desksort PRIVATE nlohmann_json::nlohmann_json)

if(MSVC) # Check if using Microsoft Visual C++ compiler
    set_target_properties(desksort PROPERTIES
        RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/desksort.manifest"
    )
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.desksort)
endif()
set_target_properties(desksort PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS desksort
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(desksort)
endif()
