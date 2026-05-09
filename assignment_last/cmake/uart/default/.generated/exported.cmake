set(DEPENDENT_MP_BIN2HEXuart_default_1REwzGSQ "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFuart_default_1REwzGSQ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart/default.elf)
set(DEPENDENT_TARGET_DIRuart_default_1REwzGSQ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart)
set(DEPENDENT_BYPRODUCTSuart_default_1REwzGSQ ${DEPENDENT_TARGET_DIRuart_default_1REwzGSQ}/${sourceFileNameuart_default_1REwzGSQ}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRuart_default_1REwzGSQ}/${sourceFileNameuart_default_1REwzGSQ}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXuart_default_1REwzGSQ} ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_1REwzGSQ} --image ${sourceFileNameuart_default_1REwzGSQ} ${addressuart_default_1REwzGSQ} ${modeuart_default_1REwzGSQ} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRuart_default_1REwzGSQ}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_1REwzGSQ})
add_custom_target(
    dependent_produced_source_artifactuart_default_1REwzGSQ 
    DEPENDS ${DEPENDENT_TARGET_DIRuart_default_1REwzGSQ}/${sourceFileNameuart_default_1REwzGSQ}.s
    )
