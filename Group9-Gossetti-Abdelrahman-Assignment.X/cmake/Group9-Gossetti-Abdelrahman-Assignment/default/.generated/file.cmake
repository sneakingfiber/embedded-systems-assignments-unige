# The following variables contains the files used by the different stages of the build process.
set(Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../acc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../uart.c")
set_source_files_properties(${Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_link)
set(Group9_Gossetti_Abdelrahman_Assignment_default_default_XC16_FILE_TYPE_bin2hex)
set(Group9_Gossetti_Abdelrahman_Assignment_default_image_name "default.elf")
set(Group9_Gossetti_Abdelrahman_Assignment_default_image_base_name "default")

# The output directory of the final image.
set(Group9_Gossetti_Abdelrahman_Assignment_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Group9-Gossetti-Abdelrahman-Assignment")

# The full path to the final image.
set(Group9_Gossetti_Abdelrahman_Assignment_default_full_path_to_image ${Group9_Gossetti_Abdelrahman_Assignment_default_output_dir}/${Group9_Gossetti_Abdelrahman_Assignment_default_image_name})
