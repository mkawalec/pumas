#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(test1)
{
    BOOST_CHECK_EQUAL(1, 1);
}

test_suite *init_unit_test_suite(int, char *[])
{
    framework::master_test_suite().p_name.value = "PUMAS unit test";

    return 0;
}
