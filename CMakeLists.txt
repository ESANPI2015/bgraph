cmake_minimum_required(VERSION 2.8)

project(swgraph)

include_directories(src)
include_directories(include)

SET(ENV{PKG_CONFIG_PATH}
    ${CMAKE_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}
    )

# Yes, we have to add the directory at least
if(NOT TARGET hypergraph)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../hypergraph hypergraph)
endif(NOT TARGET hypergraph)

find_package(PkgConfig)
#pkg_check_modules(YAML REQUIRED yaml-cpp)
#include_directories(${YAML_INCLUDE_DIRS})
#link_directories(${YAML_LIBRARY_DIRS})
#add_definitions(-DYAML_SUPPORT ${YAML_CFLAGS_OTHER})
#set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} yaml-cpp)

pkg_check_modules(swgraph_PKGCONFIG REQUIRED
    hypergraph yaml-cpp
    )

include_directories(${swgraph_PKGCONFIG_INCLUDE_DIRS})
#message(${swgraph_PKGCONFIG_INCLUDE_DIRS})
link_directories(${swgraph_PKGCONFIG_LIBRARY_DIRS})
add_definitions(${swgraph_PKGCONFIG_CFLAGS_OTHER} -ggdb --pedantic -Wall -std=c++11)

set(SOURCES
    src/SoftwareGraph.cpp
    )
set(HEADERS
    include/SoftwareGraph.hpp
    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(swgraph STATIC ${SOURCES})
target_link_libraries(swgraph
    ${swgraph_PKGCONFIG_STATIC_LIBRARIES}
    )

add_executable(testSWGraph test/testSWGraph.cpp)
target_link_libraries(testSWGraph swgraph)
install(TARGETS testSWGraph
RUNTIME DESTINATION bin)

add_executable(def_bg_components test/def_bg_components.cpp)
target_link_libraries(def_bg_components swgraph)
install(TARGETS def_bg_components
RUNTIME DESTINATION bin)

add_executable(gen_cpp_class tools/gen_cpp_class.cpp)
target_link_libraries(gen_cpp_class swgraph)
install(TARGETS gen_cpp_class
RUNTIME DESTINATION bin)

# pkg-config, to be installed:
configure_file(${PROJECT_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
# pkg-config for use inside our buildtree
configure_file(${PROJECT_NAME}-uninstalled.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-uninstalled.pc @ONLY)
