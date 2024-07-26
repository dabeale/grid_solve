
#include "index.hpp"
#include "tools.hpp"

using namespace gs;

int test_construction(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        index<3> ind(
            {0,1,2},
            3
        );
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 8, "ind[1] == 8");
        retVal += assert_bool(ind[2] == 16, "ind[2] == 16");
    }
    {
        index<3> ind(
            {0,1,2},
            2
        );
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 4, "ind[1] == 4");
        retVal += assert_bool(ind[2] == 8, "ind[2] == 8");
    }
    {
        index<3> ind(
            {0,1,2},
            1
        );
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 2, "ind[1] == 2");
        retVal += assert_bool(ind[2] == 4, "ind[2] == 4");
    }
    {
        index<3> ind(
            {0,1,2},
            0
        );
        retVal += assert_bool(ind[0] == 0, "ind[0] == 0");
        retVal += assert_bool(ind[1] == 1, "ind[1] == 2");
        retVal += assert_bool(ind[2] == 2, "ind[2] == 4");
    }
    return retVal;
}

int test_neighbours(){
    std::cout << "Test index neighbours" << std::endl;
    int retVal = 0;
    index<3> ind(
        {0,0,1},
        3
    );
    auto ind2 = ind.neighbour(0, index<3>::POSITIVE);
    retVal += assert_bool(ind2[0] == 8, "ind2[0] == 8");
    retVal += assert_bool(ind2[1] == 0, "ind2[1] == 0");
    retVal += assert_bool(ind2[2] == 8, "ind2[2] == 8");
    return retVal;
}

int test_sub2ind(){
    std::cout << "Test index sub2ind" << std::endl;
    int retVal = 0;
    {
        index<3> ind(
            {0,0,1},
            0
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 1, "ind.sub2ind({3,4,5}) == 1");
    }
    {
        index<3> ind(
            {0,1,0},
            0
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 5, "ind.sub2ind({3,4,5}) == 5");
    }
    {
        index<3> ind(
            {1,0,0},
            0
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 20, "ind.sub2ind({3,4,5}) == 20");
    }
    {
        index<3> ind(
            {0,0,1},
            1
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 2, "ind.sub2ind({3,4,5}) == 2");
    }
    {
        index<3> ind(
            {0,1,0},
            1
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 10, "ind.sub2ind({3,4,5}) == 10");
    }
    {
        index<3> ind(
            {1,0,0},
            1
        );
        retVal += assert_bool(ind.sub2ind({3,4,5}) == 40, "ind.sub2ind({3,4,5}) == 40");
    }
    return retVal;
}