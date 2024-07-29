
#include "index.hpp"
#include "tools.hpp"

using namespace gs;

int test_index_call(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        index<3> ind({0,1,2}, 3);
        retVal += assert_bool(ind(0) == 0, "ind(0) == 0");
        retVal += assert_bool(ind(1) == 8, "ind(1) == 8");
        retVal += assert_bool(ind(2) == 16, "ind(2) == 16");
    }
    {
        index<3> ind({0,1,2}, 2);
        retVal += assert_bool(ind(0) == 0, "ind(0) == 0");
        retVal += assert_bool(ind(1) == 4, "ind(1) == 4");
        retVal += assert_bool(ind(2) == 8, "ind(2) == 8");
    }
    {
        index<3> ind({0,1,2}, 1);
        retVal += assert_bool(ind(0) == 0, "ind(0) == 0");
        retVal += assert_bool(ind(1) == 2, "ind(1) == 2");
        retVal += assert_bool(ind(2) == 4, "ind(2) == 4");
    }
    {
        index<3> ind({0,1,2}, 0);
        retVal += assert_bool(ind(0) == 0, "ind(0) == 0");
        retVal += assert_bool(ind(1) == 1, "ind(1) == 1");
        retVal += assert_bool(ind(2) == 2, "ind(2) == 2");
    }
    return retVal;
}


int test_index_subscript(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        index<3> ind({0,1,2}, 3);
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 1, "ind[1] == 1");
        retVal += assert_bool(ind[2] == 2, "ind[2] == 2");
    }
    {
        index<3> ind({0,1,2}, 2);
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 1, "ind[1] == 1");
        retVal += assert_bool(ind[2] == 2, "ind[2] == 2");
    }
    return retVal;
}