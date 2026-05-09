set(DEPENDENT_MP_BIN2HEXassignment1_last_default_hKhhFjps "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_hKhhFjps ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment1_last/default.elf)
set(DEPENDENT_TARGET_DIRassignment1_last_default_hKhhFjps ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment1_last)
set(DEPENDENT_BYPRODUCTSassignment1_last_default_hKhhFjps ${DEPENDENT_TARGET_DIRassignment1_last_default_hKhhFjps}/${sourceFileNameassignment1_last_default_hKhhFjps}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment1_last_default_hKhhFjps}/${sourceFileNameassignment1_last_default_hKhhFjps}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment1_last_default_hKhhFjps} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_hKhhFjps} --image ${sourceFileNameassignment1_last_default_hKhhFjps} ${addressassignment1_last_default_hKhhFjps} ${modeassignment1_last_default_hKhhFjps} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment1_last_default_hKhhFjps}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_hKhhFjps})
add_custom_target(
    dependent_produced_source_artifactassignment1_last_default_hKhhFjps 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment1_last_default_hKhhFjps}/${sourceFileNameassignment1_last_default_hKhhFjps}.s
    )
