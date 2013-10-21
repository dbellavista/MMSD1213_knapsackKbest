#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavista@studio.unibo.it
#:Date: 2013-08-31 11:21

ADD_EXECUTABLE(test_kbest kp_tests.c test_main.c test_suite.c
  ../kbest_io_utility.c ../kbest.c
  ../kp_algorithm/kp_alg_forward.c ../kp_algorithm/kp_alg_backtrack.c
  ../kp_model/kp_inner_model.c ../utility/utility.c)

INSTALL (TARGETS test_kbest DESTINATION ${BellavistaKBest_BIN_DIR})
