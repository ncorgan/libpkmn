#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MACRO(LUA_CHECK_MODULE desc mod have)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Checking for Lua module ${desc}")
    EXECUTE_PROCESS(
        COMMAND ${LUA_INTERPRETER} -l ${mod} -e ""
        RESULT_VARIABLE ${have}
        OUTPUT_QUIET
        ERROR_QUIET
    )
    IF(${have} EQUAL 0)
        MESSAGE(STATUS "Checking for Lua module ${desc} - found")
        SET(${have} TRUE)
    ELSE()
        MESSAGE(STATUS "Checking for Lua module ${desc} - failed")
    ENDIF(${have} EQUAL 0)
ENDMACRO(LUA_CHECK_MODULE)
