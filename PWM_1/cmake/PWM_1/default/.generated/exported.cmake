set(DEPENDENT_MP_BIN2HEXPWM_1_default_o5HExVzU "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_o5HExVzU "${CMAKE_CURRENT_LIST_DIR}/../../../../out/PWM_1/default.elf")
set(DEPENDENT_TARGET_DIRPWM_1_default_o5HExVzU "${CMAKE_CURRENT_LIST_DIR}/../../../../out/PWM_1")
set(DEPENDENT_BYPRODUCTSPWM_1_default_o5HExVzU ${DEPENDENT_TARGET_DIRPWM_1_default_o5HExVzU}/${sourceFileNamePWM_1_default_o5HExVzU}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPWM_1_default_o5HExVzU}/${sourceFileNamePWM_1_default_o5HExVzU}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXPWM_1_default_o5HExVzU} ${DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_o5HExVzU} --image ${sourceFileNamePWM_1_default_o5HExVzU} ${addressPWM_1_default_o5HExVzU} ${modePWM_1_default_o5HExVzU} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPWM_1_default_o5HExVzU}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPWM_1_default_o5HExVzU})
add_custom_target(
    dependent_produced_source_artifactPWM_1_default_o5HExVzU 
    DEPENDS ${DEPENDENT_TARGET_DIRPWM_1_default_o5HExVzU}/${sourceFileNamePWM_1_default_o5HExVzU}.s
    )
