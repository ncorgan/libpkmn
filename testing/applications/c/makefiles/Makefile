#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

pkmn-c-app: pkmn-c-app.c
	$(CC) -o pkmn-c-app `pkg-config --cflags --libs pkmn-c` pkmn-c-app.c

clean:
	rm -f pkmn-c-app

.phony: clean
