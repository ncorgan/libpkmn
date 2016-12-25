/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPathsTest {

    // http://stackoverflow.com/questions/5116977/how-to-check-the-os-version-at-runtime-e-g-windows-or-linux-without-using-a-con
    public static bool IsLinux
    {
        get
        {
            int p = (int) Environment.OSVersion.Platform;
            return (p == 4) || (p == 6) || (p == 128);
        }
    }

    [Test]
    public void AppDataDirTest() {
        string appDataDir;
        if(IsLinux) {
            appDataDir = "/libpkmn/appdata/dir";
        } else {
            appDataDir = "C:\\libpkmn\\appdata\\dir";
        }

        Environment.SetEnvironmentVariable("PKMN_APPDATA_DIR", appDataDir);
        Assert.AreEqual(PKMN.Paths.GetAppdataDir(), appDataDir);
    }

    [Test]
    public void DatabasePathTest() {
        /*
         * When this unit test is run, the PKMN_DATABASE_PATH environment variable
         * is set to a valid value, so just make sure the call doesn't fail.
         */
        PKMN.Paths.GetDatabasePath();

        /*
         * Make sure a call to an existing file fails if it's not a valid SQLite
         * database.
         */
        Environment.SetEnvironmentVariable(
            "PKMN_DATABASE_PATH",
            System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase
        );
        Assert.Throws<ApplicationException>(
            delegate {
                PKMN.Paths.GetDatabasePath();
            }
        );
    }

    [Test]
    public void ImagesDirTest() {
        string imagesDir;
        if(IsLinux) {
            imagesDir = "/libpkmn/images/dir";
        } else {
            imagesDir = "C:\\libpkmn\\images\\dir";
        }

        Environment.SetEnvironmentVariable("PKMN_IMAGES_DIR", imagesDir);
        Assert.AreEqual(PKMN.Paths.GetImagesDir(), imagesDir);
    }

    [Test]
    [Ignore("Test not implemented in others")]
    public void TmpDirTest() {
    }
}
