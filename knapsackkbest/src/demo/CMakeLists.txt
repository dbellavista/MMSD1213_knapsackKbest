#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 10:54

LINK_DIRECTORIES (${CMAKE_INSTALL_PREFIX}/${BellavistaKBest_LIB_DIR}/kbest)

ADD_EXECUTABLE (kbest_demo kbest_demo_main.c)

IF (NOT(STRIP_DEMO_RPATH))
  MESSAGE (" KB > Demo > RPATH will be set to \""
    ${CMAKE_INSTALL_PREFIX}/${BellavistaKBest_LIB_DIR} "\"")

  SET_TARGET_PROPERTIES (kbest_demo PROPERTIES
                        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${BellavistaKBest_LIB_DIR}
                        INSTALL_RPATH_USE_LINK_PATH TRUE)
ELSE()
  MESSAGE (" KB > Demo > RPATH will be stripped")
ENDIF(NOT(STRIP_DEMO_RPATH))

TARGET_LINK_LIBRARIES (kbest_demo kbest)
INSTALL (TARGETS kbest_demo DESTINATION ${BellavistaKBest_BIN_DIR})
