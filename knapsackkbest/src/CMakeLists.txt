#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 11:21

SET(KBEST_INSTALL_PREFIX "${BellavistaKBest_SOURCE_DIR}" CACHE PATH "Prefix prepended to
install directories")
SET(CMAKE_INSTALL_PREFIX "${KBEST_INSTALL_PREFIX}" CACHE INTERNAL "Prefix
prepended to install directories" FORCE)

SET (CMAKE_INSTALL_PREFIX "${BellavistaKBest_SOURCE_DIR}")

OPTION(TESTS "Compile the tests" ON)
OPTION(DEMO "Compile to demo" ON)
OPTION(ENABLE_DEBUG_PRINTS "Compile with the debug prints" OFF)
OPTION(STRIP_DEMO_RPATH "Strip the demo runtime path (to be used for deploy)" OFF)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  MESSAGE (" KB > DEBUG mode")
  set (CMAKE_C_FLAGS "-ggdb")
ELSE()
  MESSAGE (" KB > RELEASE mode")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

MESSAGE (" KB > Install Directory is: \"" ${CMAKE_INSTALL_PREFIX} "\"")
MESSAGE ("      \tTo change this value, set KBEST_INSTALL_PREFIX")

ADD_LIBRARY(kbest SHARED ./kbest.c ./kbest_io_utility.c ./utility/debug.c
  ./utility/utility.c ./kp_algorithm/kp_alg_forward.c
  ./kp_algorithm/kp_alg_backtrack.c ./kp_model/kp_inner_model.c)

INSTALL (FILES ./kbest.h ./kbest_io_utility.h DESTINATION ${BellavistaKBest_INCLUDE_DIR}/kbest/)

INSTALL (TARGETS kbest
  RUNTIME DESTINATION ${BellavistaKBest_BIN_DIR}
  LIBRARY DESTINATION ${BellavistaKBest_LIB_DIR}
  ARCHIVE DESTINATION ${BellavistaKBest_LIB_DIR})

IF(TESTS)
  MESSAGE (" KB > Tests enabled")
  ADD_SUBDIRECTORY(tests)
ENDIF(TESTS)

IF(DEMO)
  MESSAGE (" KB > Demo enabled")
  ADD_SUBDIRECTORY(demo)
ENDIF(DEMO)
