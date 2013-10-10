#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: daniele.bellavsista@studio.unibo.it
#:Date: 2013-08-31 10:46

cmake_minimum_required (VERSION 2.6)
project (BellavistaKBest)
# The version number.

set (BellavistaKBest_VERSION_MAJOR 1)
set (BellavistaKBest_VERSION_MINOR 0)

set (BellavistaKBest_BIN_DIR "bin")
set (BellavistaKBest_INCLUDE_DIR "include")
set (BellavistaKBest_SRC_DIR "${BellavistaKBest_SOURCE_DIR}/src")
set (BellavistaKBest_TEST_DIR "${BellavistaKBest_SRC_DIR}/test")

set (CMAKE_INSTALL_PREFIX "${BellavistaKBest_SOURCE_DIR}")

include_directories (${BellavistaKBest_SRC_DIR})

add_subdirectory(src)
