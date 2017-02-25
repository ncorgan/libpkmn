/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class AAAAA_PathsTest extends TestCase {
    protected void setUp() {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    public AAAAA_PathsTest(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(AAAAA_PathsTest.class);
    }

    public void testPaths() {
        System.out.println(" * Appdata directory: " + nc.PKMN.Paths.getAppdataDir());
        System.out.println(" * Database path:     " + nc.PKMN.Paths.getDatabasePath());
        System.out.println(" * Images directory:  " + nc.PKMN.Paths.getImagesDir());
        System.out.println(" * Temp directory:    " + nc.PKMN.Paths.getTmpDir());
    }
}
