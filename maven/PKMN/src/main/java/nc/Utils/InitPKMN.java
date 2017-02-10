/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import java.io.*;
import java.lang.reflect.Field;
import java.nio.file.*;
import java.nio.file.Paths;

class InitPKMN {

    // This will never be instanced.
    private InitPKMN() {}

    // Order matters.
    private static String[] libraryNames = {
        "pksav", "pkmn", "buildinfo_java", "calculations_java",
        "database_java", "env_java", "paths_java", "pkmn_java",
        "stl_java"
    };

    public static Path pkmnDirectory;
    private static boolean initialized = false;

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
        Path outputPath = pkmnDirectory.resolve(fileName);
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
            try {
                System.load(outputPath.toString());
            } catch (UnsatisfiedLinkError e) {
                System.err.println("Failed to load " + outputPath.toString());
            }
        }

        outputPath.toFile().deleteOnExit();
        return outputPath;
    }

    public static void initialize() throws FileNotFoundException, IOException {
        /*
         * Create temporary directory for this process. Resource files
         * will be extracted here either on import or as needed.
         */
        if(pkmnDirectory == null) {
            try {
                pkmnDirectory = Files.createTempDirectory("PKMN_");
                pkmnDirectory.toFile().deleteOnExit();

                // Make directories
                Path imagesDirectory = Files.createDirectory(pkmnDirectory.resolve("images"));
                imagesDirectory.toFile().deleteOnExit();
                Path tempDirectory = Files.createDirectory(pkmnDirectory.resolve("tmp"));
                tempDirectory.toFile().deleteOnExit();

                // http://stackoverflow.com/a/24988095
                System.setProperty("java.library.path", pkmnDirectory.toString());
                Field fieldSysPath = ClassLoader.class.getDeclaredField("sys_paths");
                fieldSysPath.setAccessible(true);
                fieldSysPath.set(null, null);

                for(int i = 0; i < libraryNames.length; ++i) {
                    extractFileFromJAR("/resources/" + getLibraryFileName(libraryNames[i]), true);
                }
                Path databaseOutputFile = extractFileFromJAR("/resources/libpkmn.db", false);

                // With everything extracted, set LibPKMN's environment variables.
                nc.PKMN.Env.setEnv("PKMN_DATABASE_PATH", databaseOutputFile.toString());
                nc.PKMN.Env.setEnv("PKMN_IMAGES_DIR", imagesDirectory.toString());
                nc.PKMN.Env.setEnv("PKMN_TMP_DIR", tempDirectory.toString());
            } catch(Exception e) {
            }
        }
    }

    static {
        try {
            initialize();
        } catch(Exception e) {
        }
    }
}
