#region License
/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#endregion License

using System.Reflection;
using System.Runtime.CompilerServices;

// Information about this assembly is defined by the following
// attributes.
//
// change them to the information which is associated with the assembly
// you compile.

#if (DEBUG)
[assembly: AssemblyConfiguration("Debug")]
#else
[assembly: AssemblyConfiguration("Release")]
#endif

[assembly: AssemblyTitle("LibPKMN - C# Interface")]
[assembly: AssemblyDescription("LibPKMN - C# Interface")]
[assembly: AssemblyCompany("Nicholas Corgan")]
[assembly: AssemblyProduct("LibPKMN - C# Interface")]
[assembly: AssemblyDefaultAlias("Generated Assembly")]
[assembly: AssemblyCopyright("Copyright © 2016- Nicholas Corgan. All rights reserved.")]
[assembly: AssemblyTrademark("LibPKMN - https://github.com/ncorgan/libpkmn")]
[assembly: AssemblyCulture("")]

// The assembly version has following format :
//
// Major.Minor.Build.Revision
//
// You can specify all values by your own or you can build default build and revision
// numbers with the '*' character (the default):

[assembly: AssemblyVersion("@PKMN_VERSION_MAJOR@.@PKMN_VERSION_MINOR@.*")]
[assembly: AssemblyFileVersion("2")]
[assembly: AssemblyInformationalVersion("1")]

// The following attributes specify the key for the sign of your assembly. See the
// .NET Framework documentation for more information about signing.
// This is not required, if you don't want signing let these attributes like they're.
// http://msdn.microsoft.com/en-us/library/6f05ezxy(VS.80).aspx
[assembly: AssemblyKeyFile("")]
//[assembly: AssemblyKeyName("GDCM Key")]
[assembly: AssemblyKeyName("")]
