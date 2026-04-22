set(DEPENDENT_MP_BIN2HEXassignment_0_LED_default_xGjpcz9t "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_xGjpcz9t ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED/production/default-production.elf)
set(DEPENDENT_TARGET_DIRassignment_0_LED_default_xGjpcz9t ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED/production)
set(DEPENDENT_BYPRODUCTSassignment_0_LED_default_xGjpcz9t ${DEPENDENT_TARGET_DIRassignment_0_LED_default_xGjpcz9t}/${sourceFileNameassignment_0_LED_default_xGjpcz9t}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_0_LED_default_xGjpcz9t}/${sourceFileNameassignment_0_LED_default_xGjpcz9t}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_0_LED_default_xGjpcz9t} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_xGjpcz9t} --image ${sourceFileNameassignment_0_LED_default_xGjpcz9t} ${addressassignment_0_LED_default_xGjpcz9t} ${modeassignment_0_LED_default_xGjpcz9t} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_0_LED_default_xGjpcz9t}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default_xGjpcz9t})
add_custom_target(
    dependent_produced_source_artifactassignment_0_LED_default_xGjpcz9t 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_0_LED_default_xGjpcz9t}/${sourceFileNameassignment_0_LED_default_xGjpcz9t}.s
    )
set(loadable_varassignment_0_LED_default_xGjpcz9t ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED/production/default-production.hex)
