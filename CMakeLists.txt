cmake_minimum_required(VERSION 3.0)

project(nana_player
    DESCRIPTION "nana player - simple audio player using raudio2"
    LANGUAGES CXX
)

find_package(unofficial-nana CONFIG REQUIRED)
find_package(raudio2 REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(WIN32)
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
            LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
            LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        )
    elseif(MINGW)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_RELEASE "-mwindows"
            LINK_FLAGS_RELWITHDEBINFO "-mwindows"
            LINK_FLAGS_MINSIZEREL "-mwindows"
        )
    endif()
    target_link_options(${PROJECT_NAME} PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::nana::nana raudio2::raudio2)
