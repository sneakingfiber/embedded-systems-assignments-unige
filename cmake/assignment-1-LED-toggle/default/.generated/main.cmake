include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(assignment_1_LED_toggle_default_library_list )


# Main target for this project
add_executable(assignment_1_LED_toggle_default_image_yCBgzTng ${assignment_1_LED_toggle_default_library_list})

set_target_properties(assignment_1_LED_toggle_default_image_yCBgzTng PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${assignment_1_LED_toggle_default_output_dir}")


# Add the link options from the rule file.
assignment_1_LED_toggle_default_link_rule( assignment_1_LED_toggle_default_image_yCBgzTng)

# Call bin2hex function from the rule file
assignment_1_LED_toggle_default_bin2hex_rule(assignment_1_LED_toggle_default_image_yCBgzTng)

