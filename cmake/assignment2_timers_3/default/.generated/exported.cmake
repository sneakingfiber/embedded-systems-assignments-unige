set(DEPENDENT_MP_BIN2HEXassignment2_timers_3_default_phHv02o7 "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_phHv02o7 "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment2_timers/default.elf")
set(DEPENDENT_TARGET_DIRassignment2_timers_3_default_phHv02o7 "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment2_timers")
set(DEPENDENT_BYPRODUCTSassignment2_timers_3_default_phHv02o7 ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_phHv02o7}/${sourceFileNameassignment2_timers_3_default_phHv02o7}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_phHv02o7}/${sourceFileNameassignment2_timers_3_default_phHv02o7}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment2_timers_3_default_phHv02o7} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_phHv02o7} --image ${sourceFileNameassignment2_timers_3_default_phHv02o7} ${addressassignment2_timers_3_default_phHv02o7} ${modeassignment2_timers_3_default_phHv02o7} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_phHv02o7}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_phHv02o7})
add_custom_target(
    dependent_produced_source_artifactassignment2_timers_3_default_phHv02o7 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_phHv02o7}/${sourceFileNameassignment2_timers_3_default_phHv02o7}.s
    )
