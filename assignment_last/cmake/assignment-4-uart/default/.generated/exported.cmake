set(DEPENDENT_MP_BIN2HEXassignment_4_uart_default_I3iIOPPZ "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_4_uart_default_I3iIOPPZ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-4-uart/default.elf)
set(DEPENDENT_TARGET_DIRassignment_4_uart_default_I3iIOPPZ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment-4-uart)
set(DEPENDENT_BYPRODUCTSassignment_4_uart_default_I3iIOPPZ ${DEPENDENT_TARGET_DIRassignment_4_uart_default_I3iIOPPZ}/${sourceFileNameassignment_4_uart_default_I3iIOPPZ}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_4_uart_default_I3iIOPPZ}/${sourceFileNameassignment_4_uart_default_I3iIOPPZ}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_4_uart_default_I3iIOPPZ} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_4_uart_default_I3iIOPPZ} --image ${sourceFileNameassignment_4_uart_default_I3iIOPPZ} ${addressassignment_4_uart_default_I3iIOPPZ} ${modeassignment_4_uart_default_I3iIOPPZ} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_4_uart_default_I3iIOPPZ}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_4_uart_default_I3iIOPPZ})
add_custom_target(
    dependent_produced_source_artifactassignment_4_uart_default_I3iIOPPZ 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_4_uart_default_I3iIOPPZ}/${sourceFileNameassignment_4_uart_default_I3iIOPPZ}.s
    )
