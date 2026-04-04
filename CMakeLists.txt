cmake_minimum_required(VERSION 3.10)
project(VanishingPersona)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(PACKFOLDER_EXE "${CMAKE_CURRENT_SOURCE_DIR}/deps/sciter-js-sdk/bin/windows/packfolder.exe")
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.cpp
    COMMAND ${PACKFOLDER_EXE} "${CMAKE_CURRENT_SOURCE_DIR}/ui" "${CMAKE_CURRENT_SOURCE_DIR}/src/resources.cpp" -v resources
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ui/index.htm ${CMAKE_CURRENT_SOURCE_DIR}/ui/style.css
)

add_custom_target(PackUI ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.cpp)

add_executable(vanishing_persona 
    src/main.cpp 
    src/job_queue.cpp 
    src/storage.cpp
    src/resources.cpp
)

add_dependencies(vanishing_persona PackUI)

target_include_directories(vanishing_persona PRIVATE deps/sciter-js-sdk/include)
target_include_directories(vanishing_persona PRIVATE src)

if (WIN32)
    target_link_libraries(vanishing_persona crypt32 bcrypt)
endif()
