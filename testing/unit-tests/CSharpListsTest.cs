/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.Collections.Generic;

public class CSharpEqualityTest {
    public static bool AbilityListTest() {
        System.Console.Write("Starting AbilityListTest...");

        try {
            try {
                PKMN.Database.GetAbilityList(0);
            } catch(System.IndexOutOfRangeException) {
                // We want this
            } catch(System.Exception ex) {
                System.Console.WriteLine(ex.GetType().Name);
                System.Console.WriteLine(ex.Message);
                System.Console.WriteLine(ex.StackTrace);
                System.Console.WriteLine(ex.InnerException.GetType().Name);
                System.Console.WriteLine(ex.InnerException.Message);
                System.Console.WriteLine(ex.InnerException.StackTrace);
                throw new System.Exception("This should have thrown an IndexOutOfRangeException.");
            }

            PKMN.StringList abilityList = PKMN.Database.GetAbilityList(6);
            if(abilityList.Count != 191) {
                throw new System.Exception("abilityList.Count != 191");
            }
            if(!abilityList[0].Equals("Adaptability")) {
                throw new System.Exception("!abilityList[0].Equals(\"Adaptability\"), is " + abilityList[0]);
            }
            if(!abilityList[abilityList.Count-1].Equals("Zen Mode")) {
                throw new System.Exception("!abilityList[abilityList.Count-1].Equals(\"Zen Mode\"), is \"" + abilityList[abilityList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static int Main(string[] args) {
        bool successful = AbilityListTest();

        return successful ? 0 : 1;
    }
}
