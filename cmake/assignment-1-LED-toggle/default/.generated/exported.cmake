set(DEPENDENT_MP_BIN2HEXassignment_1_LED_toggle_default_vkIBQrSE "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_vkIBQrSE ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-1-LED-toggle/default.elf)
set(DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_vkIBQrSE ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-1-LED-toggle)
set(DEPENDENT_BYPRODUCTSassignment_1_LED_toggle_default_vkIBQrSE ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_vkIBQrSE}/${sourceFileNameassignment_1_LED_toggle_default_vkIBQrSE}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_vkIBQrSE}/${sourceFileNameassignment_1_LED_toggle_default_vkIBQrSE}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_1_LED_toggle_default_vkIBQrSE} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_vkIBQrSE} --image ${sourceFileNameassignment_1_LED_toggle_default_vkIBQrSE} ${addressassignment_1_LED_toggle_default_vkIBQrSE} ${modeassignment_1_LED_toggle_default_vkIBQrSE} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_vkIBQrSE}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_LED_toggle_default_vkIBQrSE})
add_custom_target(
    dependent_produced_source_artifactassignment_1_LED_toggle_default_vkIBQrSE 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_1_LED_toggle_default_vkIBQrSE}/${sourceFileNameassignment_1_LED_toggle_default_vkIBQrSE}.s
    )
