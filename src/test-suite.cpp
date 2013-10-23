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

test_suite *init_unit_test_suite(int, char *[])
{
    framework::master_test_suite().p_name.value = "PUMAS unit test";
    return 0;
}
