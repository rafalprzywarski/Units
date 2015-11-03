include_directories(${CML_INCLUDE_DIRS})
include_directories("../core")

find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})

add_executable(units_sim main.cpp)
target_link_libraries(units_sim units_core ${SFML_LIBRARIES})
