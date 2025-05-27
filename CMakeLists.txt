cmake_minimum_required(VERSION 3.16)

project(organizer VERSION 0.1 LANGUAGES CXX C)

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



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

set(PROJECT_C_SOURCES
    script.c
    sqlite3.c
    sqlite3.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(organizer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_C_SOURCES}
        rule_set_container.h rule_set_container.cpp
        .gitignore
        sqlite3.c sqlite3.h
        rulesetcreate.h rulesetcreate.cpp rulesetcreate.ui
        filtercreate.h filtercreate.cpp filtercreate.ui
        editfilterscreen.h editfilterscreen.cpp editfilterscreen.ui
        clickableframe.h clickableframe.cpp
        script.h
    )




# Define target properties for Android with Qt 6 as:
#    set_property(TARGET organizer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(organizer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(organizer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_sources(organizer PRIVATE
    ${PROJECT_SOURCES}
    ${PROJECT_C_SOURCES})
# find_library(SQLITE3_LIB sqlite3 PATHS C:/MinGW/include)
target_link_libraries(organizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


target_link_libraries(organizer PRIVATE nlohmann_json::nlohmann_json)

if(MSVC) # Check if using Microsoft Visual C++ compiler
    set_target_properties(your_target_name PROPERTIES
        RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/organizer.manifest"
    )
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.organizer)
endif()
set_target_properties(organizer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS organizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(organizer)
endif()
