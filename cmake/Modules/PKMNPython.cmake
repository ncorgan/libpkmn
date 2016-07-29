#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MACRO(PYTHON_CHECK_MODULE desc mod cmd have)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Checking for Python module ${desc}")
    EXECUTE_PROCESS(
        COMMAND ${PYTHON_EXECUTABLE} -c "
#########################################
try: import ${mod}
except: exit(1)
try: assert ${cmd}
except: exit(2)
exit(0)
#########################################"
        RESULT_VARIABLE ${have}
    )
    IF(${have} EQUAL 0)
        MESSAGE(STATUS "Checking for Python module ${desc} - found")
        SET(${have} TRUE)
    ELSEIF(${have} EQUAL 1)
        MESSAGE(STATUS "Checking for Python module ${desc} - \"import ${mod}\" failed")
        SET(${have} FALSE)
    ELSEIF(${have} EQUAL 2)
        MESSAGE(STATUS "Checking for Python module ${desc} - \"assert ${cmd}\" failed")
        SET(${have} FALSE)
    ELSE()
        MESSAGE(STATUS "Checking for Python module ${desc} - unknown error")
        SET(${have} FALSE)
    ENDIF()
ENDMACRO(PYTHON_CHECK_MODULE)
