/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import org.junit.Test;
import static org.junit.Assert.*;

public class AAAAA_PathsTest {

    // Make sure native libraries are loaded.
    static {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    @Test
    public void testPaths() {
        System.out.println(" * Appdata directory: " + nc.PKMN.Paths.getAppdataDir());
        System.out.println(" * Database path:     " + nc.PKMN.Paths.getDatabasePath());
        System.out.println(" * Images directory:  " + nc.PKMN.Paths.getImagesDir());
        System.out.println(" * Temp directory:    " + nc.PKMN.Paths.getTmpDir());
    }
}
