add_definitions(--pedantic -Wall)
set(SOURCES
    BehaviorGraph.cpp
    )
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} componentnet)

add_executable(bg-import-model import_behavior_graph.cpp)
target_link_libraries(bg-import-model bgraph)
install(TARGETS bg-import-model
RUNTIME DESTINATION bin)

add_executable(bg-export-model export_behavior_graph.cpp)
target_link_libraries(bg-export-model bgraph)
install(TARGETS bg-export-model
RUNTIME DESTINATION bin)

