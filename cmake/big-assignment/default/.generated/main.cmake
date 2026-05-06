include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(big_assignment_default_library_list )

# Handle files with suffix s, for group default-XC16
if(big_assignment_default_default_XC16_FILE_TYPE_assemble)
add_library(big_assignment_default_default_XC16_assemble OBJECT ${big_assignment_default_default_XC16_FILE_TYPE_assemble})
    big_assignment_default_default_XC16_assemble_rule(big_assignment_default_default_XC16_assemble)
    list(APPEND big_assignment_default_library_list "$<TARGET_OBJECTS:big_assignment_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(big_assignment_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(big_assignment_default_default_XC16_assemblePreproc OBJECT ${big_assignment_default_default_XC16_FILE_TYPE_assemblePreproc})
    big_assignment_default_default_XC16_assemblePreproc_rule(big_assignment_default_default_XC16_assemblePreproc)
    list(APPEND big_assignment_default_library_list "$<TARGET_OBJECTS:big_assignment_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(big_assignment_default_default_XC16_FILE_TYPE_compile)
add_library(big_assignment_default_default_XC16_compile OBJECT ${big_assignment_default_default_XC16_FILE_TYPE_compile})
    big_assignment_default_default_XC16_compile_rule(big_assignment_default_default_XC16_compile)
    list(APPEND big_assignment_default_library_list "$<TARGET_OBJECTS:big_assignment_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(big_assignment_default_default_XC16_FILE_TYPE_dependentObject)
add_library(big_assignment_default_default_XC16_dependentObject OBJECT ${big_assignment_default_default_XC16_FILE_TYPE_dependentObject})
    big_assignment_default_default_XC16_dependentObject_rule(big_assignment_default_default_XC16_dependentObject)
    list(APPEND big_assignment_default_library_list "$<TARGET_OBJECTS:big_assignment_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(big_assignment_default_default_XC16_FILE_TYPE_bin2hex)
add_library(big_assignment_default_default_XC16_bin2hex OBJECT ${big_assignment_default_default_XC16_FILE_TYPE_bin2hex})
    big_assignment_default_default_XC16_bin2hex_rule(big_assignment_default_default_XC16_bin2hex)
    list(APPEND big_assignment_default_library_list "$<TARGET_OBJECTS:big_assignment_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(big_assignment_default_image_tdIG9E1M ${big_assignment_default_library_list})

set_target_properties(big_assignment_default_image_tdIG9E1M PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${big_assignment_default_output_dir}")
target_link_libraries(big_assignment_default_image_tdIG9E1M PRIVATE ${big_assignment_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
big_assignment_default_link_rule( big_assignment_default_image_tdIG9E1M)

# Call bin2hex function from the rule file
big_assignment_default_bin2hex_rule(big_assignment_default_image_tdIG9E1M)

