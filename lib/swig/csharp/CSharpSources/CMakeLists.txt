#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(csharp_files
    AssemblyInfo.cs
    Equality.cs
)

FOREACH(cs ${csharp_files})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${cs}
        ${CMAKE_CURRENT_BINARY_DIR}/${cs}
    @ONLY)
ENDFOREACH(cs ${csharp_files})
