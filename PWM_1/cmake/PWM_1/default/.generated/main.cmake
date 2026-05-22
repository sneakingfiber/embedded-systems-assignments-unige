include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(PWM_1_default_library_list )

# Handle files with suffix s, for group default-XC16
if(PWM_1_default_default_XC16_FILE_TYPE_assemble)
add_library(PWM_1_default_default_XC16_assemble OBJECT ${PWM_1_default_default_XC16_FILE_TYPE_assemble})
    PWM_1_default_default_XC16_assemble_rule(PWM_1_default_default_XC16_assemble)
    list(APPEND PWM_1_default_library_list "$<TARGET_OBJECTS:PWM_1_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(PWM_1_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(PWM_1_default_default_XC16_assemblePreproc OBJECT ${PWM_1_default_default_XC16_FILE_TYPE_assemblePreproc})
    PWM_1_default_default_XC16_assemblePreproc_rule(PWM_1_default_default_XC16_assemblePreproc)
    list(APPEND PWM_1_default_library_list "$<TARGET_OBJECTS:PWM_1_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(PWM_1_default_default_XC16_FILE_TYPE_compile)
add_library(PWM_1_default_default_XC16_compile OBJECT ${PWM_1_default_default_XC16_FILE_TYPE_compile})
    PWM_1_default_default_XC16_compile_rule(PWM_1_default_default_XC16_compile)
    list(APPEND PWM_1_default_library_list "$<TARGET_OBJECTS:PWM_1_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(PWM_1_default_default_XC16_FILE_TYPE_dependentObject)
add_library(PWM_1_default_default_XC16_dependentObject OBJECT ${PWM_1_default_default_XC16_FILE_TYPE_dependentObject})
    PWM_1_default_default_XC16_dependentObject_rule(PWM_1_default_default_XC16_dependentObject)
    list(APPEND PWM_1_default_library_list "$<TARGET_OBJECTS:PWM_1_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(PWM_1_default_default_XC16_FILE_TYPE_bin2hex)
add_library(PWM_1_default_default_XC16_bin2hex OBJECT ${PWM_1_default_default_XC16_FILE_TYPE_bin2hex})
    PWM_1_default_default_XC16_bin2hex_rule(PWM_1_default_default_XC16_bin2hex)
    list(APPEND PWM_1_default_library_list "$<TARGET_OBJECTS:PWM_1_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(PWM_1_default_image_o5HExVzU ${PWM_1_default_library_list})

set_target_properties(PWM_1_default_image_o5HExVzU PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${PWM_1_default_output_dir}")
target_link_libraries(PWM_1_default_image_o5HExVzU PRIVATE ${PWM_1_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
PWM_1_default_link_rule( PWM_1_default_image_o5HExVzU)

# Call bin2hex function from the rule file
PWM_1_default_bin2hex_rule(PWM_1_default_image_o5HExVzU)

