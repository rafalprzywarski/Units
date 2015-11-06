include_directories(${CML_INCLUDE_DIRS})
include_directories(${GoogleMock_INCLUDE_DIRS})
include_directories("../core")

add_executable(units_test
    units/dummy_test.cpp
    units/unit_container_test.cpp
    main.cpp
)

target_link_libraries(units_test units_core ${Boost_LIBRARIES} gmock pthread)
