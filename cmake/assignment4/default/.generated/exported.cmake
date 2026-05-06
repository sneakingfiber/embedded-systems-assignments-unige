set(DEPENDENT_MP_BIN2HEXassignment4_default_6G8MKC_U "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_6G8MKC_U ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4/default.elf)
set(DEPENDENT_TARGET_DIRassignment4_default_6G8MKC_U ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4)
set(DEPENDENT_BYPRODUCTSassignment4_default_6G8MKC_U ${DEPENDENT_TARGET_DIRassignment4_default_6G8MKC_U}/${sourceFileNameassignment4_default_6G8MKC_U}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment4_default_6G8MKC_U}/${sourceFileNameassignment4_default_6G8MKC_U}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment4_default_6G8MKC_U} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_6G8MKC_U} --image ${sourceFileNameassignment4_default_6G8MKC_U} ${addressassignment4_default_6G8MKC_U} ${modeassignment4_default_6G8MKC_U} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment4_default_6G8MKC_U}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_6G8MKC_U})
add_custom_target(
    dependent_produced_source_artifactassignment4_default_6G8MKC_U 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment4_default_6G8MKC_U}/${sourceFileNameassignment4_default_6G8MKC_U}.s
    )
