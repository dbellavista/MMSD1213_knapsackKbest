#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 11:21

ADD_EXECUTABLE(test_kbest kp_tests.c test_main.c test_suite.c
  ../debug.c ../io_utility.c ../kbest.c ../kp_alg.c ../kp_alg_data.c ../utility.c)

INSTALL (TARGETS test_kbest DESTINATION ${BellavistaKBest_BIN_DIR})
