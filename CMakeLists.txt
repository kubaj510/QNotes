cmake_minimum_required(VERSION 3.16)
project(QNotes VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/UI)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)


set(SOURCES
    main.cpp
    src/Views/MainWindow.cpp
    src/Views/MainWindow.h
    src/Views/PreferencesWindow.cpp
    src/Views/PreferencesWindow.h
    src/Views/HelpWindow.h
    src/Views/HelpWindow.cpp

    src/Config/AppSettings.h
    src/Config/SettingsManager.h
    src/Config/SettingsManager.cpp
    
    src/Model/Document.cpp
    src/Model/Document.h
    
    src/Utils/FileHandler.cpp
    src/Utils/FileHandler.h

    src/Translator/TranslationManager.h
    src/Translator/TranslationManager.cpp

    src/UI/MainWindowUI.ui
    src/UI/preferences.ui
    resources.qrc
)

set(TS_FILES
    translations/notepad_pl.ts
)

add_executable(QNotes ${SOURCES} ${TS_FILES})

qt_add_translations(QNotes TS_FILES ${TS_FILES})

target_include_directories(QNotes PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Views
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Model
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI/Style
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Translator
    ${CMAKE_CURRENT_SOURCE_DIR}/help
    ${CMAKE_CURRENT_BINARY_DIR}
)


target_link_libraries(QNotes PRIVATE 
    Qt6::Widgets 
)

