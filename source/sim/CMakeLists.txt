include_directories("../core")

add_executable(units_sim main.cpp)
target_link_libraries(units_sim units_core ${SFML_LIBRARIES})
