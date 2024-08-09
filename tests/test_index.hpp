
#include "base/index.hpp"
#include "base/tools.hpp"

int test_index_call(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(3)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(3)[1] == 8);
        retVal += ASSERT_BOOL(ind.at_level(3)[2] == 16);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(2)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(2)[1] == 4);
        retVal += ASSERT_BOOL(ind.at_level(2)[2] == 8);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(1)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(1)[1] == 2);
        retVal += ASSERT_BOOL(ind.at_level(1)[2] == 4);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(0)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(0)[1] == 1);
        retVal += ASSERT_BOOL(ind.at_level(0)[2] == 2);
    }
    return retVal;
}


int test_index_subscript(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        gs::index<3> ind({0,1,2}, 3);
        retVal += ASSERT_BOOL(ind[0] == 0);
        retVal += ASSERT_BOOL(ind[1] == 1);
        retVal += ASSERT_BOOL(ind[2] == 2);
    }
    {
        gs::index<3> ind({0,1,2}, 2);
        retVal += ASSERT_BOOL(ind[0] == 0);
        retVal += ASSERT_BOOL(ind[1] == 1);
        retVal += ASSERT_BOOL(ind[2] == 2);
    }
    return retVal;
}