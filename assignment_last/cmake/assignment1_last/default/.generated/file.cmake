# The following variables contains the files used by the different stages of the build process.
set(assignment1_last_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${assignment1_last_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${assignment1_last_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(assignment1_last_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${assignment1_last_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${assignment1_last_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(assignment1_last_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment1_last/acc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment1_last/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment1_last/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment1_last/uart.c")
set_source_files_properties(${assignment1_last_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(assignment1_last_default_default_XC16_FILE_TYPE_link)
set(assignment1_last_default_default_XC16_FILE_TYPE_bin2hex)
set(assignment1_last_default_image_name "default.elf")
set(assignment1_last_default_image_base_name "default")

# The output directory of the final image.
set(assignment1_last_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/assignment1_last")

# The full path to the final image.
set(assignment1_last_default_full_path_to_image ${assignment1_last_default_output_dir}/${assignment1_last_default_image_name})
