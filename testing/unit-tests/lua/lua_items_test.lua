--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local test_results = {
    require("gen1_items_tests")
}

for i = 1, #test_results
do
    if test_results[i] ~= 0
    then
        os.exit(test_results[i])
    end
end

os.exit(0)
