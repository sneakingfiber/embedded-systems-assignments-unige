# The following variables contains the files used by the different stages of the build process.
set(uart_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${uart_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${uart_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(uart_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${uart_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${uart_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(uart_default_default_XC16_FILE_TYPE_compile
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c")
set_source_files_properties(${uart_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(uart_default_default_XC16_FILE_TYPE_link)
set(uart_default_default_XC16_FILE_TYPE_bin2hex)
set(uart_default_image_name "default.elf")
set(uart_default_image_base_name "default")

# The output directory of the final image.
set(uart_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/uart")

# The full path to the final image.
set(uart_default_full_path_to_image ${uart_default_output_dir}/${uart_default_image_name})
