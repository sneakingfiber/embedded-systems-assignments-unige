set(DEPENDENT_MP_BIN2HEXassignment_0_LED_default__euk0R2v "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default__euk0R2v "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED/default.elf")
set(DEPENDENT_TARGET_DIRassignment_0_LED_default__euk0R2v "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-0-LED")
set(DEPENDENT_BYPRODUCTSassignment_0_LED_default__euk0R2v ${DEPENDENT_TARGET_DIRassignment_0_LED_default__euk0R2v}/${sourceFileNameassignment_0_LED_default__euk0R2v}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_0_LED_default__euk0R2v}/${sourceFileNameassignment_0_LED_default__euk0R2v}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_0_LED_default__euk0R2v} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default__euk0R2v} --image ${sourceFileNameassignment_0_LED_default__euk0R2v} ${addressassignment_0_LED_default__euk0R2v} ${modeassignment_0_LED_default__euk0R2v} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_0_LED_default__euk0R2v}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_0_LED_default__euk0R2v})
add_custom_target(
    dependent_produced_source_artifactassignment_0_LED_default__euk0R2v 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_0_LED_default__euk0R2v}/${sourceFileNameassignment_0_LED_default__euk0R2v}.s
    )
