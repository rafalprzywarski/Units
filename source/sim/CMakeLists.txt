include_directories(${CML_INCLUDE_DIRS})
include_directories("../core" "../gui")

find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})

add_executable(ams_box box.cpp)
target_link_libraries(ams_box ams_core ams_gui ${SFML_LIBRARIES})

add_executable(ams_walking walking.cpp)
target_link_libraries(ams_walking ams_core ams_gui ${SFML_LIBRARIES})
