include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(assignment_ADC_default_library_list )

# Handle files with suffix s, for group default-XC16
if(assignment_ADC_default_default_XC16_FILE_TYPE_assemble)
add_library(assignment_ADC_default_default_XC16_assemble OBJECT ${assignment_ADC_default_default_XC16_FILE_TYPE_assemble})
    assignment_ADC_default_default_XC16_assemble_rule(assignment_ADC_default_default_XC16_assemble)
    list(APPEND assignment_ADC_default_library_list "$<TARGET_OBJECTS:assignment_ADC_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(assignment_ADC_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(assignment_ADC_default_default_XC16_assemblePreproc OBJECT ${assignment_ADC_default_default_XC16_FILE_TYPE_assemblePreproc})
    assignment_ADC_default_default_XC16_assemblePreproc_rule(assignment_ADC_default_default_XC16_assemblePreproc)
    list(APPEND assignment_ADC_default_library_list "$<TARGET_OBJECTS:assignment_ADC_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(assignment_ADC_default_default_XC16_FILE_TYPE_compile)
add_library(assignment_ADC_default_default_XC16_compile OBJECT ${assignment_ADC_default_default_XC16_FILE_TYPE_compile})
    assignment_ADC_default_default_XC16_compile_rule(assignment_ADC_default_default_XC16_compile)
    list(APPEND assignment_ADC_default_library_list "$<TARGET_OBJECTS:assignment_ADC_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(assignment_ADC_default_default_XC16_FILE_TYPE_dependentObject)
add_library(assignment_ADC_default_default_XC16_dependentObject OBJECT ${assignment_ADC_default_default_XC16_FILE_TYPE_dependentObject})
    assignment_ADC_default_default_XC16_dependentObject_rule(assignment_ADC_default_default_XC16_dependentObject)
    list(APPEND assignment_ADC_default_library_list "$<TARGET_OBJECTS:assignment_ADC_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(assignment_ADC_default_default_XC16_FILE_TYPE_bin2hex)
add_library(assignment_ADC_default_default_XC16_bin2hex OBJECT ${assignment_ADC_default_default_XC16_FILE_TYPE_bin2hex})
    assignment_ADC_default_default_XC16_bin2hex_rule(assignment_ADC_default_default_XC16_bin2hex)
    list(APPEND assignment_ADC_default_library_list "$<TARGET_OBJECTS:assignment_ADC_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(assignment_ADC_default_image_HlqhWyyt ${assignment_ADC_default_library_list})

set_target_properties(assignment_ADC_default_image_HlqhWyyt PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${assignment_ADC_default_output_dir}")
target_link_libraries(assignment_ADC_default_image_HlqhWyyt PRIVATE ${assignment_ADC_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
assignment_ADC_default_link_rule( assignment_ADC_default_image_HlqhWyyt)

# Call bin2hex function from the rule file
assignment_ADC_default_bin2hex_rule(assignment_ADC_default_image_HlqhWyyt)

