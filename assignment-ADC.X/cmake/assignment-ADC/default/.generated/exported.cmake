set(DEPENDENT_MP_BIN2HEXassignment_ADC_default_HlqhWyyt "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_ADC_default_HlqhWyyt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-ADC/default.elf)
set(DEPENDENT_TARGET_DIRassignment_ADC_default_HlqhWyyt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-ADC)
set(DEPENDENT_BYPRODUCTSassignment_ADC_default_HlqhWyyt ${DEPENDENT_TARGET_DIRassignment_ADC_default_HlqhWyyt}/${sourceFileNameassignment_ADC_default_HlqhWyyt}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_ADC_default_HlqhWyyt}/${sourceFileNameassignment_ADC_default_HlqhWyyt}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_ADC_default_HlqhWyyt} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_ADC_default_HlqhWyyt} --image ${sourceFileNameassignment_ADC_default_HlqhWyyt} ${addressassignment_ADC_default_HlqhWyyt} ${modeassignment_ADC_default_HlqhWyyt} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_ADC_default_HlqhWyyt}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_ADC_default_HlqhWyyt})
add_custom_target(
    dependent_produced_source_artifactassignment_ADC_default_HlqhWyyt 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_ADC_default_HlqhWyyt}/${sourceFileNameassignment_ADC_default_HlqhWyyt}.s
    )
