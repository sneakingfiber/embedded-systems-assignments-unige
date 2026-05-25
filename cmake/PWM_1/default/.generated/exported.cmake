set(DEPENDENT_MP_BIN2HEXPWM_1_default_Mem1MNkX "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_Mem1MNkX ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PWM_1/default.elf)
set(DEPENDENT_TARGET_DIRPWM_1_default_Mem1MNkX ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PWM_1)
set(DEPENDENT_BYPRODUCTSPWM_1_default_Mem1MNkX ${DEPENDENT_TARGET_DIRPWM_1_default_Mem1MNkX}/${sourceFileNamePWM_1_default_Mem1MNkX}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPWM_1_default_Mem1MNkX}/${sourceFileNamePWM_1_default_Mem1MNkX}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXPWM_1_default_Mem1MNkX} ${DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_Mem1MNkX} --image ${sourceFileNamePWM_1_default_Mem1MNkX} ${addressPWM_1_default_Mem1MNkX} ${modePWM_1_default_Mem1MNkX} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPWM_1_default_Mem1MNkX}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_Mem1MNkX})
add_custom_target(
    dependent_produced_source_artifactPWM_1_default_Mem1MNkX 
    DEPENDS ${DEPENDENT_TARGET_DIRPWM_1_default_Mem1MNkX}/${sourceFileNamePWM_1_default_Mem1MNkX}.s
    )
