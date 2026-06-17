set(DEPENDENT_MP_BIN2HEXFinal_project_default_zSqEGAyl "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_zSqEGAyl "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project/default.elf")
set(DEPENDENT_TARGET_DIRFinal_project_default_zSqEGAyl "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_project")
set(DEPENDENT_BYPRODUCTSFinal_project_default_zSqEGAyl ${DEPENDENT_TARGET_DIRFinal_project_default_zSqEGAyl}/${sourceFileNameFinal_project_default_zSqEGAyl}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRFinal_project_default_zSqEGAyl}/${sourceFileNameFinal_project_default_zSqEGAyl}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXFinal_project_default_zSqEGAyl} ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_zSqEGAyl} --image ${sourceFileNameFinal_project_default_zSqEGAyl} ${addressFinal_project_default_zSqEGAyl} ${modeFinal_project_default_zSqEGAyl} -mdfp=C:/Users/mwahi/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRFinal_project_default_zSqEGAyl}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_project_default_zSqEGAyl})
add_custom_target(
    dependent_produced_source_artifactFinal_project_default_zSqEGAyl 
    DEPENDS ${DEPENDENT_TARGET_DIRFinal_project_default_zSqEGAyl}/${sourceFileNameFinal_project_default_zSqEGAyl}.s
    )
