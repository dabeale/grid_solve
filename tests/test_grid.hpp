
#include <set>

#include "base/grid.hpp"

int test_grid(){
    std::cout << "Test grid" << std::endl;
    int retVal = 0;
    gs::dimensions<3> dims({2,2,2}, 2);
    gs::grid<3, double, double> grid(dims);
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](gs::box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2, gs::index<3>::POINTS)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 0);
        //
        // There is only 1 box in a 2x2 grid at level 0
        //
        retVal += ASSERT_BOOL(nPts == 1);
        retVal += ASSERT_BOOL(allVals.size() == 2);
        retVal += ASSERT_BOOL(allVals.find(0) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(4) != allVals.end());
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](gs::box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2, gs::index<3>::POINTS)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 1);
        retVal += ASSERT_BOOL(nPts == 8);
        retVal += ASSERT_BOOL(allVals.size() == 3);
        retVal += ASSERT_BOOL(allVals.find(0) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(2) != allVals.end());
        retVal += ASSERT_BOOL(allVals.find(4) != allVals.end());
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](gs::box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2, gs::index<3>::POINTS)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 2);
        retVal += ASSERT_BOOL(nPts == 64);
        retVal += ASSERT_BOOL(allVals.size() == 5);
        for( int i=0; i<=4; ++i){
            retVal += ASSERT_BOOL(allVals.find(i) != allVals.end());
        }
    }
    return retVal;
}