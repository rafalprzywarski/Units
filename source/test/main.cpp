#include <gmock/gmock.h>
#include "ams_test_listener.hpp"

int main(int argc, char **argv)
{
    testing::InitGoogleMock(&argc, argv);
    testing::UnitTest& unitTest = *testing::UnitTest::GetInstance();
    testing::TestEventListeners& listeners = unitTest.listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new ams_test_listener);
    return RUN_ALL_TESTS();
}
