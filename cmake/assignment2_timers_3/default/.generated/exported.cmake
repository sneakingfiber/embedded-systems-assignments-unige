set(DEPENDENT_MP_BIN2HEXassignment2_timers_3_default_FrKMoIuC "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_FrKMoIuC ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment2_timers/default.elf)
set(DEPENDENT_TARGET_DIRassignment2_timers_3_default_FrKMoIuC ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment2_timers)
set(DEPENDENT_BYPRODUCTSassignment2_timers_3_default_FrKMoIuC ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_FrKMoIuC}/${sourceFileNameassignment2_timers_3_default_FrKMoIuC}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_FrKMoIuC}/${sourceFileNameassignment2_timers_3_default_FrKMoIuC}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment2_timers_3_default_FrKMoIuC} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_FrKMoIuC} --image ${sourceFileNameassignment2_timers_3_default_FrKMoIuC} ${addressassignment2_timers_3_default_FrKMoIuC} ${modeassignment2_timers_3_default_FrKMoIuC} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_FrKMoIuC}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment2_timers_3_default_FrKMoIuC})
add_custom_target(
    dependent_produced_source_artifactassignment2_timers_3_default_FrKMoIuC 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment2_timers_3_default_FrKMoIuC}/${sourceFileNameassignment2_timers_3_default_FrKMoIuC}.s
    )
