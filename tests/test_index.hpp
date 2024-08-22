
#include "base/index.hpp"
#include "base/tools.hpp"

int test_index_call(){
    std::cout << "Test index construction" << std::endl;
    int retVal = 0;
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[1] == 8);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[2] == 16);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::POINTS_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::POINTS_SUBDIVISION)[1] == 4);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::POINTS_SUBDIVISION)[2] == 8);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::POINTS_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::POINTS_SUBDIVISION)[1] == 2);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::POINTS_SUBDIVISION)[2] == 4);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::POINTS_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::POINTS_SUBDIVISION)[1] == 1);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::POINTS_SUBDIVISION)[2] == 2);
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


int test_index_call_duel(){
    std::cout << "Test index at_level duel" << std::endl;
    int retVal = 0;
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 15);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 16);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 7);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 8);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 3);
        retVal += ASSERT_BOOL(ind.at_level(1, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 4);
    }
    {
        gs::index<3> ind({0,1,2}, 0);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 1);
        retVal += ASSERT_BOOL(ind.at_level(0, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 2);
    }
    {
        gs::index<3> ind({0,1,2}, 2);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 3);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 4);
    }
    {
        gs::index<3> ind({3,4,5}, 2);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 7);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 8);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 11);
    }
    {
        gs::index<3> ind({6,7,8}, 2);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 12);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 15);
        retVal += ASSERT_BOOL(ind.at_level(3, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 16);
    }
    {
        gs::index<3> ind({0,1,2}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 0);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 1);
    }
    {
        gs::index<3> ind({3,4,5}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 1);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 2);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 2);
    }
    {
        gs::index<3> ind({6,7,8}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 4);
    }
    {
        gs::index<3> ind({9,10,11}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 4);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 5);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 5);
    }
    {
        gs::index<3> ind({12,13,14}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 6);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 6);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 7);
    }
    {
        gs::index<3> ind({15,16,17}, 3);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[0] == 7);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[1] == 8);
        retVal += ASSERT_BOOL(ind.at_level(2, gs::dimensions<3>::BOXES_SUBDIVISION)[2] == 8);
    }
    return retVal;
}