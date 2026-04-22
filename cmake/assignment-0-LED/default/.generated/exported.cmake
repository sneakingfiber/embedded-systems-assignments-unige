set(DEPENDENT_MP_BIN2HEXassignment_0_LED_default_vHmx80al "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_vHmx80al ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED/default.elf)
set(DEPENDENT_TARGET_DIRassignment_0_LED_default_vHmx80al ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED)
set(DEPENDENT_BYPRODUCTSassignment_0_LED_default_vHmx80al ${DEPENDENT_TARGET_DIRassignment_0_LED_default_vHmx80al}/${sourceFileNameassignment_0_LED_default_vHmx80al}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_0_LED_default_vHmx80al}/${sourceFileNameassignment_0_LED_default_vHmx80al}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_0_LED_default_vHmx80al} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_vHmx80al} --image ${sourceFileNameassignment_0_LED_default_vHmx80al} ${addressassignment_0_LED_default_vHmx80al} ${modeassignment_0_LED_default_vHmx80al} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_0_LED_default_vHmx80al}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_vHmx80al})
add_custom_target(
    dependent_produced_source_artifactassignment_0_LED_default_vHmx80al 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_0_LED_default_vHmx80al}/${sourceFileNameassignment_0_LED_default_vHmx80al}.s
    )
