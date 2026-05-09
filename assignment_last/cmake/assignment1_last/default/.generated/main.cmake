include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(assignment1_last_default_library_list )

# Handle files with suffix s, for group default-XC16
if(assignment1_last_default_default_XC16_FILE_TYPE_assemble)
add_library(assignment1_last_default_default_XC16_assemble OBJECT ${assignment1_last_default_default_XC16_FILE_TYPE_assemble})
    assignment1_last_default_default_XC16_assemble_rule(assignment1_last_default_default_XC16_assemble)
    list(APPEND assignment1_last_default_library_list "$<TARGET_OBJECTS:assignment1_last_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(assignment1_last_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(assignment1_last_default_default_XC16_assemblePreproc OBJECT ${assignment1_last_default_default_XC16_FILE_TYPE_assemblePreproc})
    assignment1_last_default_default_XC16_assemblePreproc_rule(assignment1_last_default_default_XC16_assemblePreproc)
    list(APPEND assignment1_last_default_library_list "$<TARGET_OBJECTS:assignment1_last_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(assignment1_last_default_default_XC16_FILE_TYPE_compile)
add_library(assignment1_last_default_default_XC16_compile OBJECT ${assignment1_last_default_default_XC16_FILE_TYPE_compile})
    assignment1_last_default_default_XC16_compile_rule(assignment1_last_default_default_XC16_compile)
    list(APPEND assignment1_last_default_library_list "$<TARGET_OBJECTS:assignment1_last_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(assignment1_last_default_default_XC16_FILE_TYPE_dependentObject)
add_library(assignment1_last_default_default_XC16_dependentObject OBJECT ${assignment1_last_default_default_XC16_FILE_TYPE_dependentObject})
    assignment1_last_default_default_XC16_dependentObject_rule(assignment1_last_default_default_XC16_dependentObject)
    list(APPEND assignment1_last_default_library_list "$<TARGET_OBJECTS:assignment1_last_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(assignment1_last_default_default_XC16_FILE_TYPE_bin2hex)
add_library(assignment1_last_default_default_XC16_bin2hex OBJECT ${assignment1_last_default_default_XC16_FILE_TYPE_bin2hex})
    assignment1_last_default_default_XC16_bin2hex_rule(assignment1_last_default_default_XC16_bin2hex)
    list(APPEND assignment1_last_default_library_list "$<TARGET_OBJECTS:assignment1_last_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(assignment1_last_default_image_hKhhFjps ${assignment1_last_default_library_list})

set_target_properties(assignment1_last_default_image_hKhhFjps PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${assignment1_last_default_output_dir}")
target_link_libraries(assignment1_last_default_image_hKhhFjps PRIVATE ${assignment1_last_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
assignment1_last_default_link_rule( assignment1_last_default_image_hKhhFjps)

# Call bin2hex function from the rule file
assignment1_last_default_bin2hex_rule(assignment1_last_default_image_hKhhFjps)

