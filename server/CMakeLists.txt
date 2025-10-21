add_executable(minidrive_server
    src/main.cpp
)

target_include_directories(minidrive_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(minidrive_server
    PRIVATE
        minidrive_shared
        minidrive_warnings
)

set_target_properties(minidrive_server PROPERTIES OUTPUT_NAME server)

