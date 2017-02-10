package nc.PKMN;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class ListsTest extends TestCase
{
    protected void setUp() {
        try {
            InitPKMN.initialize();
        } catch(Exception e) {
        }
    }

    public ListsTest( String testName )
    {
        super( testName );
    }

    public static Test suite()
    {
        return new TestSuite( ListsTest.class );
    }

    public void testAbilityList()
    {
        StringVector abilityList = nc.PKMN.Database.getAbilityList(6);
        assertEquals(191, abilityList.size());
        assertEquals("Adaptability", abilityList.get(0));
        assertEquals("Zen Mode", abilityList.get(190));
    }
}
