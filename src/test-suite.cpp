#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <Simulator.hpp>
using namespace boost::unit_test;
using namespace boost;
using namespace PUMA;
using namespace std;

BOOST_AUTO_TEST_CASE(check_update)
{
    /// Creates test landmap with only land
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);

    /// Applies the update function that is to be tested.
    tested.apply_step();
    shared_array<landscape> current_state = tested.get_current(); 
    shared_array<landscape> temp_state = tested.get_temp(); 

    /// Verifies that the Update method changed something at all
    for (size_t i = 0; i < 100; ++i) {
        BOOST_CHECK(current_state[i].is_land); 
        BOOST_CHECK(abs(current_state[i].hare_density - temp_state[i].hare_density) > 1e-15);
    }

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_landmap)
{
    /// Creates test landmap with only land
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);

    /// Checks whether the landmap was correctly parsed by the constructor
    shared_array<landscape> current_state = tested.get_current(); 
    for (size_t i = 0; i < 100; ++i) 
        BOOST_CHECK(current_state[i].is_land);

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_no_migration_through_water)
{
    /** Creates test landmap that consists of a large island
     *  surrounded by water and a few water points on the island
     **/
    bool* landmap1 = new bool[64];
    for (size_t i = 0; i < 64; ++i)
        landmap1[i] = false;
    for (size_t j = 1; j < 7; ++j)
        for (size_t i = 1; i < 7; ++i)
            landmap1[ i + j * 8] = true;
    landmap1[3+4*8]=false;
    landmap1[2+6*8]=false;
    landmap1[5+1*8]=false;


    TestSimulator tested(8, 8, landmap1, 0.000213);
    
    /// Tests whether pumas or hares migrate through water, which they shouldn't
    for (int step = 0; step < 20; ++step) 
    {
        tested.apply_step();
        shared_array<landscape> current_state = tested.get_current();
        shared_array<landscape> last_state = tested.get_temp();
        for( size_t i = 0; i < 64; ++i)
        {
            BOOST_CHECK(current_state[i].is_land == last_state[i].is_land);
            if (current_state[i].is_land == false)
            {
                BOOST_CHECK(abs(current_state[i].hare_density) < 1e-15);
                BOOST_CHECK(abs(current_state[i].puma_density) < 1e-15);
            }
        }
    }

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_average)
{
    /// Creates test landmap with only land
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    /// Tests whether get_average actually returns the average of the densities
    TestSimulator tested(2, 50, landmap1, 0.000213);
    tested.set_densities_const(3.0, 2.0, 2, 50);
    average_densities av = tested.get_averages();
    BOOST_CHECK(av.first == 3.0);
    BOOST_CHECK(av.second == 2.0);
    tested.set_densities_const(0.0, 0.0, 2, 25);
    av = tested.get_averages();
    BOOST_CHECK(av.first == 1.5);
    BOOST_CHECK(av.second == 1.0);

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_migration_rate)
{
    /// Creates test landmap with only land
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);

    /// Sets all parameters to zero except migration rates
    tested.r = 0.0;
    tested.a = 0.0;
    tested.b = 0.0;
    tested.m = 0.0;

    /** Sets initial densities to 0 for half of the landscape 
     *  and to some constant for the other half.
     **/
    tested.set_densities_const(3.0, 2.0, 2, 50);
    tested.set_densities_const(0.0, 0.0, 2, 25);

    /// Checks whether hares and pumas populate the whole landscape after some time
    for (int i = 0; i < 100; ++i)
        tested.apply_step();

    shared_array<landscape> current_state = tested.get_current();

    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK(current_state[i].hare_density != 0.0);
        BOOST_CHECK(current_state[i].puma_density != 0.0);
    }

    /** Checking if there are any hares or pumas born/dying.
     *  Which shouldn't be the case.
     **/
    average_densities av = tested.get_averages();
    BOOST_CHECK(abs(av.first - 1.5) < 1e-15);
    BOOST_CHECK(abs(av.second - 1.0) < 1e-15);

    delete[] landmap1;
}

test_suite *init_unit_test_suite(int, char *[])
{
    framework::master_test_suite().p_name.value = "PUMAS unit test";
    return 0;
}
