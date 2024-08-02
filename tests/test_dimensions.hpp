
#include "base/dimensions.hpp"
#include "base/tools.hpp"

using namespace gs;

int test_dimensions_sub2ind(){
    std::cout << "Test dimensions sub2ind" << std::endl;
    dimensions<3> dims({3,4,5}, 5);
    int retVal = 0;
    retVal += assert_bool(dims.sub2ind({0,0,1}, 0) == 1, "dims.sub2ind({0,0,1}, 0) == 1");
    retVal += assert_bool(dims.sub2ind({0,1,0}, 0) == 5, "dims.sub2ind({0,1,0}, 0) == 5");
    retVal += assert_bool(dims.sub2ind({1,0,0}, 0) == 20, "dims.sub2ind({1,0,0}, 0) == 20");
    retVal += assert_bool(dims.sub2ind({0,0,1}, 1) == 1, "dims.sub2ind({0,0,1}, 1) == 1");
    retVal += assert_bool(dims.sub2ind({0,1,0}, 1) == 10, "dims.sub2ind({0,1,0}, 1) == 10");
    retVal += assert_bool(dims.sub2ind({1,0,0}, 1) == 80, "dims.sub2ind({1,0,0}, 1) == 80");
    return retVal;
}

int test_dimensions_sub2ind_inversion(){
    std::cout << "Test dimensions sub2ind inversion" << std::endl;
    dimensions<3> dims({3,4,5}, 5);
    int retVal = 0;
    for (uint32_t i=0; i<3*4*5; ++i){
        retVal += assert_bool(dims.sub2ind(dims.ind2sub(i)) == i, "dims.sub2ind(dims.ind2sub(i)) == i");
    }
    return retVal;
}