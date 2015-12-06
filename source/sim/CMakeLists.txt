include_directories(${CML_INCLUDE_DIRS})
include_directories("../core" "../gui")

find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})

add_executable(units_box box.cpp)
target_link_libraries(units_box units_core units_gui ${SFML_LIBRARIES})

add_executable(units_walking walking.cpp)
target_link_libraries(units_walking units_core units_gui ${SFML_LIBRARIES})
