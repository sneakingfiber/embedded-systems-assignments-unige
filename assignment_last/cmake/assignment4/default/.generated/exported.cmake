set(DEPENDENT_MP_BIN2HEXassignment4_default_bkdZ_2Ql "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_bkdZ_2Ql ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4/default.elf)
set(DEPENDENT_TARGET_DIRassignment4_default_bkdZ_2Ql ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4)
set(DEPENDENT_BYPRODUCTSassignment4_default_bkdZ_2Ql ${DEPENDENT_TARGET_DIRassignment4_default_bkdZ_2Ql}/${sourceFileNameassignment4_default_bkdZ_2Ql}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment4_default_bkdZ_2Ql}/${sourceFileNameassignment4_default_bkdZ_2Ql}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment4_default_bkdZ_2Ql} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_bkdZ_2Ql} --image ${sourceFileNameassignment4_default_bkdZ_2Ql} ${addressassignment4_default_bkdZ_2Ql} ${modeassignment4_default_bkdZ_2Ql} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment4_default_bkdZ_2Ql}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_bkdZ_2Ql})
add_custom_target(
    dependent_produced_source_artifactassignment4_default_bkdZ_2Ql 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment4_default_bkdZ_2Ql}/${sourceFileNameassignment4_default_bkdZ_2Ql}.s
    )
