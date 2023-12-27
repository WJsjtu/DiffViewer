cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME server)

set(COMPILER_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMPILER_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build the shared library" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BUILD_TYPE MATCHES DEBUG OR CMAKE_BUILD_TYPE MATCHES RELWITHDEBINFO)
elseif(CMAKE_BUILD_TYPE MATCHES RELEASE OR CMAKE_BUILD_TYPE MATCHES MINSIZEREL)
else()
    set(CMAKE_BUILD_TYPE "MINSIZEREL")
endif()

file(
    GLOB_RECURSE src_files
    LIST_DIRECTORIES false
    "${PROJECT_SOURCE_DIR}/Source/*.cpp"
    "${PROJECT_SOURCE_DIR}/Source/*.h"
)

set(LIBRARY_NAME "server")

add_library(${LIBRARY_NAME} SHARED ${src_files})
target_include_directories(${LIBRARY_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/Source")
target_compile_definitions(${LIBRARY_NAME} PRIVATE SERVER_DLL)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

set(LIBRARY_PUBLIC_HDRS
    ${PROJECT_SOURCE_DIR}/Source/server.h
)

install(FILES ${LIBRARY_PUBLIC_HDRS} DESTINATION "${INSTALL_INC_DIR}")
install(FILES $<TARGET_PDB_FILE:${LIBRARY_NAME}> DESTINATION bin OPTIONAL)

install(TARGETS ${LIBRARY_NAME} ${LIBRARY_NAME}
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}")

# ============ Test ==============
file(
    GLOB_RECURSE main_src
    LIST_DIRECTORIES false
    "${PROJECT_SOURCE_DIR}/Test/*.cpp"
    "${PROJECT_SOURCE_DIR}/Test/*.h"
)

add_executable("${PROJECT_NAME}_test" ${main_src})
target_include_directories("${PROJECT_NAME}_test" PUBLIC "${PROJECT_SOURCE_DIR}/Source")
target_include_directories("${PROJECT_NAME}_test" PUBLIC "${PROJECT_SOURCE_DIR}/Test")
add_custom_command(TARGET "${PROJECT_NAME}_test" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${LIBRARY_NAME}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}_test>"
)
target_link_libraries("${PROJECT_NAME}_test" PRIVATE ${LIBRARY_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)