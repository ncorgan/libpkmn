/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.Utils;

import java.io.*;
import java.nio.file.*;

public class InitPKMN {

    // This will never be instanced.
    private InitPKMN() {}

    // Order matters.
    private static String[] libraryNames = {
        "pksav", "pkmn", "buildinfo_java", "calculations_java",
        "database_java", "env_java", "paths_java", "pkmn_java",
        "stl_java"
    };

    public static Path tempDirectory;

    public static boolean isWindows() {
        return System.getProperty("os.name").startsWith("Win");
    }

    public static String getLibraryFileName(String libraryName) {
        if(isWindows()) {
            return (libraryName + ".dll");
        } else {
            return ("lib" + libraryName + ".so");
        }
    }

    public static Path extractFileFromJAR (
        String resourcePath,
        boolean loadLibrary
    ) throws FileNotFoundException, IOException {
        InputStream is = InitPKMN.class.getResourceAsStream(resourcePath);
        if(is == null) {
            throw new FileNotFoundException("Could not find resource " + resourcePath);
        }

        Path fileName = Paths.get(resourcePath).getFileName();
        Path outputPath = tempDirectory.resolve(fileName);
        OutputStream os = new FileOutputStream(outputPath.toFile());

        // Read from resource and write to temp file.
        byte buffer[] = new byte[1024];
        int readBytes = 0;
        try {
            while((readBytes = is.read(buffer)) != -1) {
                os.write(buffer, 0, readBytes);
            }
        } finally {
            os.close();
            is.close();
        }

        if(loadLibrary) {
            System.load(outputPath.toString());
        }

        outputPath.toFile().deleteOnExit();
        return outputPath;
    }

    static {
        /*
         * Create temporary directory for this process. Resource files
         * will be extracted here either on import or as needed.
         */
        try {
            tempDirectory = Files.createTempDirectory("PKMN_");
            tempDirectory.toFile().deleteOnExit();

            extractFileFromJAR("/resources/libpkmn.db", false);

            for(int i = 0; i < libraryNames.length; ++i) {
                extractFileFromJAR("/resources/" + getLibraryFileName(libraryNames[i]), true);
            }
        } catch(IOException e) {
            // TODO: what to do here?
        }
    }
}
