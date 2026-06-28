set(DEPENDENT_MP_BIN2HEXFinal_project_default_pBWSUJVi "/Applications/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pBWSUJVi ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project/default.elf)
set(DEPENDENT_TARGET_DIRFinal_project_default_pBWSUJVi ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project)
set(DEPENDENT_BYPRODUCTSFinal_project_default_pBWSUJVi ${DEPENDENT_TARGET_DIRFinal_project_default_pBWSUJVi}/${sourceFileNameFinal_project_default_pBWSUJVi}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRFinal_project_default_pBWSUJVi}/${sourceFileNameFinal_project_default_pBWSUJVi}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXFinal_project_default_pBWSUJVi} ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pBWSUJVi} --image ${sourceFileNameFinal_project_default_pBWSUJVi} ${addressFinal_project_default_pBWSUJVi} ${modeFinal_project_default_pBWSUJVi} -mdfp=/Users/fyder9/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRFinal_project_default_pBWSUJVi}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_pBWSUJVi})
add_custom_target(
    dependent_produced_source_artifactFinal_project_default_pBWSUJVi 
    DEPENDS ${DEPENDENT_TARGET_DIRFinal_project_default_pBWSUJVi}/${sourceFileNameFinal_project_default_pBWSUJVi}.s
    )
