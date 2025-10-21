add_executable(minidrive_integration_smoke
    integration/smoke.cpp
)

target_link_libraries(minidrive_integration_smoke
    PRIVATE
        minidrive_shared
        minidrive_warnings
)

set_target_properties(minidrive_integration_smoke PROPERTIES OUTPUT_NAME integration_smoke)
