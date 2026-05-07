# The following variables contains the files used by the different stages of the build process.
set(assignment4_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${assignment4_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${assignment4_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(assignment4_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${assignment4_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${assignment4_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(assignment4_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment-0-LED.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_2_timers.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_2_timers.X/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_3_timers.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_3_timers.X/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_timers.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment2_timers.X/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment3_1_interrupts.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment3_1_interrupts.X/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment4-1-uart.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment4-2-uart.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment_last/assignment1_last/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment_last/assignment1_last/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../assignment_last/assignment1_last/uart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../big-assignment.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../big-assignment.X/timer.c")
set_source_files_properties(${assignment4_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(assignment4_default_default_XC16_FILE_TYPE_link)
set(assignment4_default_default_XC16_FILE_TYPE_bin2hex)
set(assignment4_default_image_name "default.elf")
set(assignment4_default_image_base_name "default")

# The output directory of the final image.
set(assignment4_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/assignment4")

# The full path to the final image.
set(assignment4_default_full_path_to_image ${assignment4_default_output_dir}/${assignment4_default_image_name})
