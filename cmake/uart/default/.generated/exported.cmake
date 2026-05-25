set(DEPENDENT_MP_BIN2HEXuart_default_sOrY30dv "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFuart_default_sOrY30dv ${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart/default.elf)
set(DEPENDENT_TARGET_DIRuart_default_sOrY30dv ${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart)
set(DEPENDENT_BYPRODUCTSuart_default_sOrY30dv ${DEPENDENT_TARGET_DIRuart_default_sOrY30dv}/${sourceFileNameuart_default_sOrY30dv}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRuart_default_sOrY30dv}/${sourceFileNameuart_default_sOrY30dv}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXuart_default_sOrY30dv} ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_sOrY30dv} --image ${sourceFileNameuart_default_sOrY30dv} ${addressuart_default_sOrY30dv} ${modeuart_default_sOrY30dv} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRuart_default_sOrY30dv}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_sOrY30dv})
add_custom_target(
    dependent_produced_source_artifactuart_default_sOrY30dv 
    DEPENDS ${DEPENDENT_TARGET_DIRuart_default_sOrY30dv}/${sourceFileNameuart_default_sOrY30dv}.s
    )
