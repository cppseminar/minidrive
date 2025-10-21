add_executable(minidrive_client
    src/main.cpp
)

target_include_directories(minidrive_client
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(minidrive_client
    PRIVATE
        minidrive_shared
        minidrive_warnings
)

set_target_properties(minidrive_client PROPERTIES OUTPUT_NAME client)
