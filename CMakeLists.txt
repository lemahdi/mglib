cmake_minimum_required(VERSION 3.10)

project(MGlib VERSION 0.1.23 LANGUAGES CXX)

# ── C++ Standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Build type default ────────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ── Dependencies ──────────────────────────────────────────────────────────────
# GSL 2.8 (2023-07-27) is the minimum required version.
# Ubuntu 24.04 ships 2.7.1; the CI builds 2.8 from source — see docs/gsl-migration-plan.md.
find_package(GSL 2.8 REQUIRED)   # provides GSL::gsl GSL::gslcblas
find_package(FLEX  REQUIRED)
find_package(BISON REQUIRED)

# ── Include paths ─────────────────────────────────────────────────────────────
# ${CMAKE_SOURCE_DIR}/src   – resolves "nova/...", "genpricer/...", etc.
# GSL_INCLUDE_DIRS          – resolves "gsl/gsl_rng.h" from system GSL
set(MG_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${GSL_INCLUDE_DIRS}
)

# ── Compiler flags ────────────────────────────────────────────────────────────
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

# ── Parser / Lexer (optional regeneration) ────────────────────────────────────
# Pre-generated files are committed to the repository.  To force regeneration
# when bison/flex inputs change, remove the generated files and re-run cmake.
set(INFRA_DIR ${CMAKE_SOURCE_DIR}/src/genpricer/infra)

set(PARSER_TAB_CPP ${INFRA_DIR}/my_parser.tab.cpp)
set(PARSER_TAB_HPP ${INFRA_DIR}/my_parser.tab.hpp)
set(LEXER_CPP      ${INFRA_DIR}/lex.yy.cpp)

if(NOT EXISTS ${PARSER_TAB_CPP} OR NOT EXISTS ${LEXER_CPP})
  BISON_TARGET(MyParser
    ${INFRA_DIR}/my_parser.ypp
    ${PARSER_TAB_CPP}
    DEFINES_FILE ${PARSER_TAB_HPP}
  )
  FLEX_TARGET(MyLexer
    ${INFRA_DIR}/my_scanner.l
    ${LEXER_CPP}
  )
  ADD_FLEX_BISON_DEPENDENCY(MyLexer MyParser)
endif()

# ── Library source files ──────────────────────────────────────────────────────
# Collect all .cpp files under src/, excluding the Windows-only xlw layer
# and the bundled GSL submodule (system GSL is used instead).
file(GLOB_RECURSE MG_SOURCES
  ${CMAKE_SOURCE_DIR}/src/nova/*.cpp
  ${CMAKE_SOURCE_DIR}/src/mktdata/*.cpp
  ${CMAKE_SOURCE_DIR}/src/vanilla/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/genmod/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/genparam/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/genpricer/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/gensec/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/infra/*.cpp
  ${CMAKE_SOURCE_DIR}/src/genpricer/numerics/*.cpp
)

# ── Static library: mglib ─────────────────────────────────────────────────────
add_library(mglib STATIC ${MG_SOURCES})

target_include_directories(mglib PUBLIC ${MG_INCLUDE_DIRS})

target_link_libraries(mglib PUBLIC GSL::gsl GSL::gslcblas)

# Suppress flex/bison generated code warnings on GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set_source_files_properties(
    ${INFRA_DIR}/lex.yy.cpp
    ${INFRA_DIR}/my_parser.tab.cpp
    PROPERTIES COMPILE_FLAGS "-w"
  )
endif()

# ── Executable: mgapp ─────────────────────────────────────────────────────────
add_executable(mgapp
  ${CMAKE_SOURCE_DIR}/app/mgapp/init.cpp
  ${CMAKE_SOURCE_DIR}/app/mgapp/run.cpp
)

target_include_directories(mgapp PRIVATE
  ${MG_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/app       # for init.h relative include path used in init.cpp
)

target_link_libraries(mgapp PRIVATE mglib GSL::gsl GSL::gslcblas)

# ── Install ───────────────────────────────────────────────────────────────────
install(TARGETS mglib mgapp
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
  DESTINATION include/mglib
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
  PATTERN "gsl/*"  EXCLUDE
  PATTERN "xlw/*"  EXCLUDE
)
