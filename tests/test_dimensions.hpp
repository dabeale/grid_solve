
#include "base/dimensions.hpp"
#include "base/tools.hpp"

int test_dimensions_sub2ind(){
    std::cout << "Test dimensions sub2ind" << std::endl;
    gs::dimensions<3> dims({3,4,5}, 5);
    int retVal = 0;
    retVal += ASSERT_BOOL(dims.sub2ind({0,0,1}, 0) == 1);
    retVal += ASSERT_BOOL(dims.sub2ind({0,1,0}, 0) == 5);
    retVal += ASSERT_BOOL(dims.sub2ind({1,0,0}, 0) == 20);
    retVal += ASSERT_BOOL(dims.sub2ind({0,0,1}, 1) == 1);
    retVal += ASSERT_BOOL(dims.sub2ind({0,1,0}, 1) == 10);
    retVal += ASSERT_BOOL(dims.sub2ind({1,0,0}, 1) == 80);
    return retVal;
}

int test_dimensions_sub2ind_inversion(){
    std::cout << "Test dimensions sub2ind inversion" << std::endl;
    gs::dimensions<3> dims({3,4,5}, 5);
    int retVal = 0;
    for (uint32_t i=0; i<3*4*5; ++i){
        retVal += ASSERT_BOOL(dims.sub2ind(dims.ind2sub(i)) == i);
    }
    return retVal;
}