include_directories(${CML_INCLUDE_DIRS})
include_directories(${GoogleMock_INCLUDE_DIRS})
include_directories("../core")

add_executable(ams_test
    ams/unit_container_test.cpp
    main.cpp
)

target_link_libraries(ams_test ams_core ${Boost_LIBRARIES} gmock pthread)
