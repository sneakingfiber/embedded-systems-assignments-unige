set(DEPENDENT_MP_BIN2HEXassignment4_default_249ESi7u "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_249ESi7u "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4/default.elf")
set(DEPENDENT_TARGET_DIRassignment4_default_249ESi7u "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment4")
set(DEPENDENT_BYPRODUCTSassignment4_default_249ESi7u ${DEPENDENT_TARGET_DIRassignment4_default_249ESi7u}/${sourceFileNameassignment4_default_249ESi7u}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment4_default_249ESi7u}/${sourceFileNameassignment4_default_249ESi7u}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment4_default_249ESi7u} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_249ESi7u} --image ${sourceFileNameassignment4_default_249ESi7u} ${addressassignment4_default_249ESi7u} ${modeassignment4_default_249ESi7u} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment4_default_249ESi7u}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment4_default_249ESi7u})
add_custom_target(
    dependent_produced_source_artifactassignment4_default_249ESi7u 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment4_default_249ESi7u}/${sourceFileNameassignment4_default_249ESi7u}.s
    )
