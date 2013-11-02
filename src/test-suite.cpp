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
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;
   
    TestSimulator tested(2, 50, landmap1, 0.000213);
    tested.apply_step();
    shared_array<landscape> current_state = tested.get_current(); 
    shared_array<landscape> temp_state = tested.get_temp(); 

    for (size_t i = 0; i < 100; ++i) {
        BOOST_CHECK(current_state[i].is_land); 
        BOOST_CHECK(abs(current_state[i].hare_density - temp_state[i].hare_density) > 1e-15);
    }

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_landmap)
{
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);
    BOOST_CHECK(true);

    shared_array<landscape> current_state = tested.get_current(); 
    for (size_t i = 0; i < 100; ++i) 
        BOOST_CHECK(current_state[i].is_land);

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_no_migration_through_water)
{
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
    
    for (int step = 0; step < 2; ++step) 
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
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);
    tested.set_densities_const(3.0, 2.0, 2, 50);
    average_densities av = tested.get_averages();
    BOOST_CHECK(av.hares == 3.0);
    BOOST_CHECK(av.pumas == 2.0);


    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_migration_rate)
{
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);
    tested.r = 0.0;
    tested.a = 0.0;
    tested.b = 0.0;
    tested.m = 0.0;
    tested.set_densities_const(3.0, 2.0, 2, 50);
    tested.set_densities_const(0.0, 0.0, 2, 25);
    for (int i = 0; i < 100; ++i)
        tested.apply_step();

    shared_array<landscape> current_state = tested.get_current();

    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK(current_state[i].hare_density != 0.0);
        BOOST_CHECK(current_state[i].puma_density != 0.0);
    }

    // Checking if there are any hares or pumas born/dying.
    // Which shouldn't be the case.
    average_densities av = tested.get_averages();
    BOOST_CHECK(abs(av.hares - 1.5) < 1e-15);
    BOOST_CHECK(abs(av.pumas - 1.0) < 1e-15);

    delete[] landmap1;
}

BOOST_AUTO_TEST_CASE(check_timestep_change)
{
    bool *landmap1 = new bool[100];
    for (size_t i = 0; i < 100; ++i)
        landmap1[i] = true;

    TestSimulator tested(2, 50, landmap1, 0.000213);
    BOOST_CHECK(true);

    delete[] landmap1;
}

test_suite *init_unit_test_suite(int, char *[])
{
    framework::master_test_suite().p_name.value = "PUMAS unit test";
    return 0;
}
