#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 11:21

ADD_SUBDIRECTORY(tests)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  MESSAGE ("**** Debug mode")
  set (CMAKE_C_FLAGS "-ggdb")
ELSE()
  MESSAGE ("**** Release mode")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

ADD_EXECUTABLE(kbest debug.c io_utility.c kbest.c kp_alg.c kp_alg_data.c
  main.c utility.c)

INSTALL (TARGETS kbest DESTINATION ${BellavistaKBest_BIN_DIR})
