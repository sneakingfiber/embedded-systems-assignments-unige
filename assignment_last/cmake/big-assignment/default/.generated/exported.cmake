set(DEPENDENT_MP_BIN2HEXbig_assignment_default_7nO5YNuI "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_7nO5YNuI ${CMAKE_CURRENT_LIST_DIR}/../../../../out/big-assignment/default.elf)
set(DEPENDENT_TARGET_DIRbig_assignment_default_7nO5YNuI ${CMAKE_CURRENT_LIST_DIR}/../../../../out/big-assignment)
set(DEPENDENT_BYPRODUCTSbig_assignment_default_7nO5YNuI ${DEPENDENT_TARGET_DIRbig_assignment_default_7nO5YNuI}/${sourceFileNamebig_assignment_default_7nO5YNuI}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRbig_assignment_default_7nO5YNuI}/${sourceFileNamebig_assignment_default_7nO5YNuI}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXbig_assignment_default_7nO5YNuI} ${DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_7nO5YNuI} --image ${sourceFileNamebig_assignment_default_7nO5YNuI} ${addressbig_assignment_default_7nO5YNuI} ${modebig_assignment_default_7nO5YNuI} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRbig_assignment_default_7nO5YNuI}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFbig_assignment_default_7nO5YNuI})
add_custom_target(
    dependent_produced_source_artifactbig_assignment_default_7nO5YNuI 
    DEPENDS ${DEPENDENT_TARGET_DIRbig_assignment_default_7nO5YNuI}/${sourceFileNamebig_assignment_default_7nO5YNuI}.s
    )
