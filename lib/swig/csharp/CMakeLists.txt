#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate CamelCase conversions
FILE(GLOB_RECURSE header_files "${PKMN_SOURCE_DIR}/include/*.hpp")
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py
    DEPENDS ${header_files}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --include-dir=${PKMN_SOURCE_DIR}/include --output=${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMENT "Generating csharp_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    csharp_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
)

# Generate C# docstrings
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating C# XML docs"
)
ADD_CUSTOM_TARGET(
    pkmn_csharp_docs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
)

# Copy over C# sources
SET(csharp_files
    AssemblyInfo.cs
    Equality.cs
)

FOREACH(cs ${csharp_files})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/CSharpSources/${cs}
        ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/${cs}
    @ONLY)
ENDFOREACH(cs ${csharp_files})

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_CSHARP_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_CSHARP_LIBRARIES
    pkmn
)

INCLUDE(SWIGCSharp)
SWIG_CSHARP_INIT()

SWIG_BUILD_CSHARP_MODULE(buildinfo_csharp BuildInfo TRUE)
SWIG_BUILD_CSHARP_MODULE(calculations_csharp Calculations TRUE)
SWIG_BUILD_CSHARP_MODULE(paths_csharp Paths TRUE)
SWIG_BUILD_CSHARP_MODULE(pkmn_csharp PKMN TRUE)
SWIG_BUILD_CSHARP_MODULE(stl_csharp STL TRUE)
SWIG_BUILD_CSHARP_MODULE(database_csharp Database TRUE)

CSHARP_BUILD_DLL("pkmn-cs" "buildinfo_csharp;calculations_csharp;database_csharp;paths_csharp;pkmn_csharp;stl_csharp")