set(DEPENDENT_MP_BIN2HEXbig_assignment_default_A5mXfegJ "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_A5mXfegJ "${CMAKE_CURRENT_LIST_DIR}/../../../../out/big-assignment/default.elf")
set(DEPENDENT_TARGET_DIRbig_assignment_default_A5mXfegJ "${CMAKE_CURRENT_LIST_DIR}/../../../../out/big-assignment")
set(DEPENDENT_BYPRODUCTSbig_assignment_default_A5mXfegJ ${DEPENDENT_TARGET_DIRbig_assignment_default_A5mXfegJ}/${sourceFileNamebig_assignment_default_A5mXfegJ}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRbig_assignment_default_A5mXfegJ}/${sourceFileNamebig_assignment_default_A5mXfegJ}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXbig_assignment_default_A5mXfegJ} ${DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_A5mXfegJ} --image ${sourceFileNamebig_assignment_default_A5mXfegJ} ${addressbig_assignment_default_A5mXfegJ} ${modebig_assignment_default_A5mXfegJ} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRbig_assignment_default_A5mXfegJ}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_A5mXfegJ})
add_custom_target(
    dependent_produced_source_artifactbig_assignment_default_A5mXfegJ 
    DEPENDS ${DEPENDENT_TARGET_DIRbig_assignment_default_A5mXfegJ}/${sourceFileNamebig_assignment_default_A5mXfegJ}.s
    )
