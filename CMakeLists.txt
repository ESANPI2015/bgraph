cmake_minimum_required(VERSION 2.8)

project(swgraph)

include_directories(src)
include_directories(include)

SET(ENV{PKG_CONFIG_PATH}
    ${CMAKE_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}
    )

if(NOT TARGET domaingraph)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../domaingraph domaingraph)
endif(NOT TARGET domaingraph)
if(NOT TARGET hypergraph)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../hypergraph hypergraph)
endif(NOT TARGET hypergraph)

find_package(PkgConfig)
pkg_check_modules(swgraph_PKGCONFIG REQUIRED
    hypergraph domaingraph yaml-cpp
    )

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 11)
include_directories(${swgraph_PKGCONFIG_INCLUDE_DIRS})
#message(${swgraph_PKGCONFIG_INCLUDE_DIRS})
link_directories(${swgraph_PKGCONFIG_LIBRARY_DIRS})
add_definitions(${swgraph_PKGCONFIG_CFLAGS_OTHER} --pedantic -Wall)

set(SOURCES
    src/BehaviorGraph.cpp
    )
set(HEADERS
    include/BehaviorGraph.hpp
    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(swgraph STATIC ${SOURCES})
target_link_libraries(swgraph
    ${swgraph_PKGCONFIG_STATIC_LIBRARIES}
    )

add_executable(bg-import-model src/import_behavior_graph.cpp)
target_link_libraries(bg-import-model swgraph)
install(TARGETS bg-import-model
RUNTIME DESTINATION bin)

add_executable(bg-export-model src/export_behavior_graph.cpp)
target_link_libraries(bg-export-model swgraph)
install(TARGETS bg-export-model
RUNTIME DESTINATION bin)

#add_executable(gen_cpp_class src/gen_cpp_class.cpp)
#target_link_libraries(gen_cpp_class swgraph)
#install(TARGETS gen_cpp_class
#RUNTIME DESTINATION bin)
#
#add_executable(gen_vhdl_entity src/gen_vhdl_entity.cpp)
#target_link_libraries(gen_vhdl_entity swgraph)
#install(TARGETS gen_vhdl_entity
#RUNTIME DESTINATION bin)

# pkg-config, to be installed:
configure_file(${PROJECT_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
# pkg-config for use inside our buildtree
configure_file(${PROJECT_NAME}-uninstalled.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-uninstalled.pc @ONLY)
