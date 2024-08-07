
#include <set>

#include "base/grid.hpp"

using namespace gs;

int test_grid(){
    std::cout << "Test grid" << std::endl;
    int retVal = 0;
    dimensions<3> dims({2,2,2}, 2);
    grid<3, double, double> grid(dims);
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 0);
        retVal += assert_bool(nPts == 8, "nPts == 8");
        //
        // Note that there are 8 boxes in level 0, each of which have
        // 4 corners. This means that each dimension contains 3 points
        // defining the boundry of the 2 segments, and that we should
        // expect to see 3 values (including the boundary points).
        //
        retVal += ASSERT_BOOL(allVals.size() == 3);
        retVal += ASSERT_BOOL(allVals.find(0) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(4) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(8) != allVals.end());
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 1);
        retVal += ASSERT_BOOL(nPts == 64);
        retVal += ASSERT_BOOL(allVals.size() == 5);
        retVal += ASSERT_BOOL(allVals.find(0) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(2) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(4) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(6) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(8) != allVals.end());
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 2);
        retVal += ASSERT_BOOL(nPts == 512);
        retVal += ASSERT_BOOL(allVals.size() == 9);
        for( int i=0; i<=8; ++i){
            retVal += ASSERT_BOOL(allVals.find(i) != allVals.end());
        }
    }
    return retVal;
}