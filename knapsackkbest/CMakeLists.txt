#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavsista@studio.unibo.it
#:Date: 2013-08-31 10:46

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (BellavistaKBest)
# The version number.

SET (BellavistaKBest_VERSION_MAJOR 1)
SET (BellavistaKBest_VERSION_MINOR 0)

SET (BellavistaKBest_BIN_DIR "bin")
SET (BellavistaKBest_LIB_DIR "lib")
SET (BellavistaKBest_INCLUDE_DIR "include")
SET (BellavistaKBest_SRC_DIR "${BellavistaKBest_SOURCE_DIR}/src")
SET (BellavistaKBest_TEST_DIR "${BellavistaKBest_SRC_DIR}/test")

INCLUDE_DIRECTORIES (${BellavistaKBest_SRC_DIR})

ADD_SUBDIRECTORY (src)
