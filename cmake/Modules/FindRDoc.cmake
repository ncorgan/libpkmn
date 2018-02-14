#=============================================================================
# Copyright 2018 Nicholas Corgan <n.corgan@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

IF(RDOC_EXECUTABLE)
    SET(RDOC_FIND_QUIETLY TRUE)
ENDIF()

FIND_PROGRAM(RDOC_EXECUTABLE NAMES rdoc)
IF(RDOC_EXECUTABLE)
    SET(RDOC_FOUND TRUE)
ENDIF()

IF(RDOC_FOUND AND NOT RDOC_FIND_QUIETLY)
    MESSAGE(STATUS "Found RDoc: ${RDOC_EXECUTABLE}")
ENDIF()

# Cache variables
SET(RDOC_EXECUTABLE ${RDOC_EXECUTABLE} CACHE FILEPATH "RDoc executable")
