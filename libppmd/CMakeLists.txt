add_library(libppmd STATIC
    Model.cpp
)

target_include_directories(libppmd
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
