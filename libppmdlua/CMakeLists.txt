add_subdirectory(lua)

add_library(libppmdlua SHARED
    libppmdlua.cpp
)
target_link_libraries(libppmdlua libppmd lua52)
install(TARGETS libppmdlua RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/libppmdlua)
