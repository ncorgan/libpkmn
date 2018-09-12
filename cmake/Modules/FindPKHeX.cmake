#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_FILE(PKHEX_CORE_DLL PKHeX.Core.dll)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PKHeX DEFAULT_MSG PKHEX_CORE_DLL)
