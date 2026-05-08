set(DEPENDENT_MP_BIN2HEXassignment_1_LED_toggle_default_yCBgzTng "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_yCBgzTng "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-1-LED-toggle/default.elf")
set(DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_yCBgzTng "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-1-LED-toggle")
set(DEPENDENT_BYPRODUCTSassignment_1_LED_toggle_default_yCBgzTng ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_yCBgzTng}/${sourceFileNameassignment_1_LED_toggle_default_yCBgzTng}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_yCBgzTng}/${sourceFileNameassignment_1_LED_toggle_default_yCBgzTng}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_1_LED_toggle_default_yCBgzTng} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_yCBgzTng} --image ${sourceFileNameassignment_1_LED_toggle_default_yCBgzTng} ${addressassignment_1_LED_toggle_default_yCBgzTng} ${modeassignment_1_LED_toggle_default_yCBgzTng} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_yCBgzTng}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_yCBgzTng})
add_custom_target(
    dependent_produced_source_artifactassignment_1_LED_toggle_default_yCBgzTng 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_yCBgzTng}/${sourceFileNameassignment_1_LED_toggle_default_yCBgzTng}.s
    )
