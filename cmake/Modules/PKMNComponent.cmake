#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

set(_pkmn_enabled_components "" CACHE INTERNAL "" FORCE)
set(_pkmn_disabled_components "" CACHE INTERNAL "" FORCE)

########################################################################
# Register a component into the system
#  - name the component string name
#  - var the global enable variable
#  - enb the default enable setting
#  - deps a list of dependencies
#  - dis the default disable setting
########################################################################
MACRO(PKMN_REGISTER_COMPONENT name var enb deps dis)
    IF(NOT PKMN_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "")
        MESSAGE(STATUS "Determining support for ${name}.")
        FOREACH(dep ${deps})
            MESSAGE(STATUS " - Dependency ${dep} = ${${dep}}")
        ENDFOREACH(dep)
    ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

    #setup the dependent option for this component
    INCLUDE(CMakeDependentOption)
    CMAKE_DEPENDENT_OPTION(${var} "enable ${name} support" ${enb} "${deps}" ${dis})

    #append the component into one of the lists
    IF(${var})
        IF(NOT PKMN_USED_AS_SUBPROJECT)
            MESSAGE(STATUS " - Enabling ${name}.")
        ENDIF(NOT PKMN_USED_AS_SUBPROJECT)
        LIST(APPEND _pkmn_enabled_components ${name})
    ELSE(${var})
        IF(NOT PKMN_USED_AS_SUBPROJECT)
            MESSAGE(STATUS " - Disabling ${name}.")
        ENDIF(NOT PKMN_USED_AS_SUBPROJECT)
        LIST(APPEND _pkmn_disabled_components ${name})
    ENDIF(${var})
    IF(NOT PKMN_USED_AS_SUBPROJECT)
        MESSAGE(STATUS " - Override with -D${var}=ON/OFF")
    ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

    #make components lists into global variables
    SET(_pkmn_enabled_components ${_pkmn_enabled_components} CACHE INTERNAL "" FORCE)
    SET(_pkmn_disabled_components ${_pkmn_disabled_components} CACHE INTERNAL "" FORCE)
ENDMACRO(PKMN_REGISTER_COMPONENT)

#Print the summary of enabled/disabled components
FUNCTION(PKMN_PRINT_COMPONENT_SUMMARY)
    IF(PKMN_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "Building LibPKMN with the following components:")
    ELSE()
        MESSAGE(STATUS "")
        MESSAGE(STATUS "######################################################")
        MESSAGE(STATUS "# LibPKMN - Enabled Components                         #")
        MESSAGE(STATUS "######################################################")
    ENDIF(PKMN_USED_AS_SUBPROJECT)
    FOREACH(comp ${_pkmn_enabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    IF(NOT PKMN_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "")
        MESSAGE(STATUS "######################################################")
        MESSAGE(STATUS "# LibPKMN - Disabled Components                        #")
        MESSAGE(STATUS "######################################################")
        FOREACH(comp ${_pkmn_disabled_components})
            MESSAGE(STATUS " - ${comp}")
        ENDFOREACH(comp)
    ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

    MESSAGE(STATUS "")
ENDFUNCTION(PKMN_PRINT_COMPONENT_SUMMARY)

