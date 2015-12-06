include_directories(${CML_INCLUDE_DIRS})
include_directories("../gui")

add_library(ams_gui
    ams/gui/window.cpp
)
