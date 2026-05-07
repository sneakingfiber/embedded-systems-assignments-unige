set(DEPENDENT_MP_BIN2HEXassignment1_last_default_pUEi2Fcg "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_pUEi2Fcg "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment1_last/default.elf")
set(DEPENDENT_TARGET_DIRassignment1_last_default_pUEi2Fcg "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment1_last")
set(DEPENDENT_BYPRODUCTSassignment1_last_default_pUEi2Fcg ${DEPENDENT_TARGET_DIRassignment1_last_default_pUEi2Fcg}/${sourceFileNameassignment1_last_default_pUEi2Fcg}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment1_last_default_pUEi2Fcg}/${sourceFileNameassignment1_last_default_pUEi2Fcg}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment1_last_default_pUEi2Fcg} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_pUEi2Fcg} --image ${sourceFileNameassignment1_last_default_pUEi2Fcg} ${addressassignment1_last_default_pUEi2Fcg} ${modeassignment1_last_default_pUEi2Fcg} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment1_last_default_pUEi2Fcg}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment1_last_default_pUEi2Fcg})
add_custom_target(
    dependent_produced_source_artifactassignment1_last_default_pUEi2Fcg 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment1_last_default_pUEi2Fcg}/${sourceFileNameassignment1_last_default_pUEi2Fcg}.s
    )
