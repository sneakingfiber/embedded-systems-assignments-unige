set(DEPENDENT_MP_BIN2HEXuart_default_dK_VN_Ai "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFuart_default_dK_VN_Ai "${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart/default.elf")
set(DEPENDENT_TARGET_DIRuart_default_dK_VN_Ai "${CMAKE_CURRENT_LIST_DIR}/../../../../out/uart")
set(DEPENDENT_BYPRODUCTSuart_default_dK_VN_Ai ${DEPENDENT_TARGET_DIRuart_default_dK_VN_Ai}/${sourceFileNameuart_default_dK_VN_Ai}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRuart_default_dK_VN_Ai}/${sourceFileNameuart_default_dK_VN_Ai}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXuart_default_dK_VN_Ai} ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_dK_VN_Ai} --image ${sourceFileNameuart_default_dK_VN_Ai} ${addressuart_default_dK_VN_Ai} ${modeuart_default_dK_VN_Ai} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRuart_default_dK_VN_Ai}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFuart_default_dK_VN_Ai})
add_custom_target(
    dependent_produced_source_artifactuart_default_dK_VN_Ai 
    DEPENDS ${DEPENDENT_TARGET_DIRuart_default_dK_VN_Ai}/${sourceFileNameuart_default_dK_VN_Ai}.s
    )
