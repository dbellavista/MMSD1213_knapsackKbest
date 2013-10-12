#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 11:21

ADD_SUBDIRECTORY(tests)

ADD_EXECUTABLE(kbest debug.c io_utility.c kbest.c kp_alg.c kp_alg_data.c
  main.c utility.c)

INSTALL (TARGETS kbest DESTINATION ${BellavistaKBest_BIN_DIR})
