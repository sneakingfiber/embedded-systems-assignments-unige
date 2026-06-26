set(DEPENDENT_MP_BIN2HEXFinal_project_default_pLY_ccVJ "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pLY_ccVJ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project/production/default-production.elf)
set(DEPENDENT_TARGET_DIRFinal_project_default_pLY_ccVJ ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project/production)
set(DEPENDENT_BYPRODUCTSFinal_project_default_pLY_ccVJ ${DEPENDENT_TARGET_DIRFinal_project_default_pLY_ccVJ}/${sourceFileNameFinal_project_default_pLY_ccVJ}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRFinal_project_default_pLY_ccVJ}/${sourceFileNameFinal_project_default_pLY_ccVJ}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXFinal_project_default_pLY_ccVJ} ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pLY_ccVJ} --image ${sourceFileNameFinal_project_default_pLY_ccVJ} ${addressFinal_project_default_pLY_ccVJ} ${modeFinal_project_default_pLY_ccVJ} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRFinal_project_default_pLY_ccVJ}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pLY_ccVJ})
add_custom_target(
    dependent_produced_source_artifactFinal_project_default_pLY_ccVJ 
    DEPENDS ${DEPENDENT_TARGET_DIRFinal_project_default_pLY_ccVJ}/${sourceFileNameFinal_project_default_pLY_ccVJ}.s
    )
