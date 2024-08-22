
#include <set>

#include "base/box.hpp"
#include "base/tools.hpp"

int test_box(){
    std::cout << "Test box construction" << std::endl;
    int retVal = 0;
    gs::box<3> testBox(gs::dimensions<3>({2,2,2}, 2), 0, gs::dimensions<3>::POINTS_SUBDIVISION);
    retVal += ASSERT_BOOL(testBox[0].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,0,0}, 3));
    retVal += ASSERT_BOOL(testBox[1].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,0,8}, 3));
    retVal += ASSERT_BOOL(testBox[2].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,8,0}, 3));
    retVal += ASSERT_BOOL(testBox[3].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,8,8}, 3));
    retVal += ASSERT_BOOL(testBox[4].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,0,0}, 3));
    retVal += ASSERT_BOOL(testBox[5].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,0,8}, 3));
    retVal += ASSERT_BOOL(testBox[6].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,8,0}, 3));
    retVal += ASSERT_BOOL(testBox[7].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,8,8}, 3));
    {
        gs::box<3> nbox(testBox);
        nbox.to_neighbour(0, gs::box<3>::POSITIVE);
        retVal += ASSERT_BOOL(nbox[0].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,0,0}, 3));
        retVal += ASSERT_BOOL(nbox[1].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,0,8}, 3));
        retVal += ASSERT_BOOL(nbox[2].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,8,0}, 3));
        retVal += ASSERT_BOOL(nbox[3].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({8,8,8}, 3));
        retVal += ASSERT_BOOL(nbox[4].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({16,0,0}, 3));
        retVal += ASSERT_BOOL(nbox[5].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({16,0,8}, 3));
        retVal += ASSERT_BOOL(nbox[6].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({16,8,0}, 3));
        retVal += ASSERT_BOOL(nbox[7].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({16,8,8}, 3));
    }
    return retVal;
}

int test_subbox(){
    std::cout << "Test subbox" << std::endl;
    int retVal = 0;
    gs::box<3> testBox(gs::dimensions<3>({2,2,2}, 3), 0, gs::dimensions<3>::POINTS_SUBDIVISION);
    // Test first level subboxes
    {
        auto subbox = testBox.subbox(0);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,0,0}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,0,4}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,4,0}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,4,4}, 3));
        retVal += ASSERT_BOOL(subbox[4].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,0,0}, 3));
        retVal += ASSERT_BOOL(subbox[5].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,0,4}, 3));
        retVal += ASSERT_BOOL(subbox[6].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,4,0}, 3));
        retVal += ASSERT_BOOL(subbox[7].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,4,4}, 3));
    }
    {
        auto subbox = testBox.subbox(2);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,4,0}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,4,4}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,8,0}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({0,8,4}, 3));
        retVal += ASSERT_BOOL(subbox[4].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,4,0}, 3));
        retVal += ASSERT_BOOL(subbox[5].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,4,4}, 3));
        retVal += ASSERT_BOOL(subbox[6].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,8,0}, 3));
        retVal += ASSERT_BOOL(subbox[7].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION) == gs::index<3>({4,8,4}, 3));
    }
    {
        std::set<size_t> allVals;
        for (size_t i=0; i<8; ++i){
            auto subbox = testBox.subbox(i);
            for(size_t j=0; j<8; ++j){
                for (size_t k=0; k<3; ++k){
                    allVals.insert(subbox[j].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k]);
                    const auto boxStep = std::abs(
                        static_cast<int32_t>(subbox[j].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k])-
                        static_cast<int32_t>(subbox[(j+1)%8].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k])
                    );
                    retVal += ASSERT_BOOL(boxStep == 4 || boxStep == 0); 
                }
            }
        }
        retVal += ASSERT_BOOL(allVals.size() == 3);
        for(size_t i : {0, 4, 8}){
            retVal += ASSERT_BOOL(allVals.contains(i));
        }
    }
    {
        std::set<size_t> allVals;
        for (size_t i=0; i<8; ++i){
            const auto topSubbox = testBox.subbox(i);
            const auto maxTop = topSubbox.max();
            const auto minTop = topSubbox.min();
            std::array<uint32_t, 3> maxInner;
            maxInner.fill(0);
            std::array<uint32_t, 3> minInner;
            minInner.fill(static_cast<uint32_t>(-1));
            for (size_t l=0; l<8; ++l){
                auto subbox = topSubbox.subbox(l);
                maxInner = max(maxInner, maxTop);
                minInner = min(maxInner, minTop);
                for(size_t j=0; j<8; ++j){
                    for (size_t k=0; k<3; ++k){
                        allVals.insert(subbox[j].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k]);
                        const auto boxStep = std::abs(
                            static_cast<int32_t>(subbox[j].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k])-
                            static_cast<int32_t>(subbox[(j+1)%8].at_level(3, gs::dimensions<3>::POINTS_SUBDIVISION)[k])
                        );
                        retVal += ASSERT_BOOL(boxStep == 2 || boxStep == 0); 
                    }
                }
            }
            retVal += ASSERT_BOOL(maxInner == maxTop);
            retVal += ASSERT_BOOL(minInner == minTop);
        }
        retVal += ASSERT_BOOL(allVals.size() == 5);
        for(size_t i : {0, 2, 4, 6, 8}){
            retVal += ASSERT_BOOL(allVals.contains(i));
        }
    }
    return retVal;
}

int test_box_subpoints(){
    std::cout << "Test box subpoints" << std::endl;
    int retVal = 0;

    // Create a box at the second lowest granularity.
    gs::box<3> box(gs::dimensions<3>({2,2,2}, 4), 3, gs::dimensions<3>::POINTS_SUBDIVISION);

    std::set<uint32_t> corners;
    for (size_t i=0; i< box.m_nCorners; ++i){
        // Find the lowest granularity indices
        auto pt = box[i].at_level(4, gs::dimensions<3>::POINTS_SUBDIVISION);
        // They should be in the lowest corner, with
        // a factor of 2.
        for (uint8_t k = 0; k<3; ++k){
            retVal += ASSERT_BOOL(pt[k] == 0 || pt[k] == 2);
            corners.insert(pt[k]);
        }
    }
    ASSERT_BOOL(corners.size() == 2);

    std::set<uint32_t> innerPts;
    for (auto sp : box.subpoints()){
        auto pt = sp.at_level(4, gs::dimensions<3>::POINTS_SUBDIVISION);
        for (uint8_t k = 0; k<3; ++k){
            retVal += ASSERT_BOOL(pt[k] == 0 || pt[k] == 1 || pt[k] == 2);
            innerPts.insert(pt[k]);
        }
        // Ensure there are no corner points in the list
        retVal += ASSERT_BOOL(pt[0] == 1 || pt[1] == 1 || pt[2] == 1);
    }
    ASSERT_BOOL(innerPts.size() == 3);

    return retVal;
}


int test_subbox_duel(){
    std::cout << "Test subbox duel" << std::endl;
    int retVal = 0;
    gs::box<2> testBox(gs::dimensions<2>({2,2}, 3), 0, gs::dimensions<2>::BOXES_SUBDIVISION);
    {
        retVal += ASSERT_BOOL(testBox[0].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({0,0}, 3));
        retVal += ASSERT_BOOL(testBox[1].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({0,15}, 3));
        retVal += ASSERT_BOOL(testBox[2].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,0}, 3));
        retVal += ASSERT_BOOL(testBox[3].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,15}, 3));
    }

    // Test first level subboxes
    {
        auto subbox = testBox.subbox(0);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({0,0}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({0,7}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({7,0}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({7,7}, 3));
    }
    {
        auto subbox = testBox.subbox(2);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({8,0}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({8,7}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,0}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,7}, 3));
    }

    // Test all for overlaps
    {
        std::set<size_t> allIndices;
        for(size_t i=0; i<4; ++i){
            for(size_t j=i+1; j<4; ++j){
                auto subboxi = testBox.subbox(i);
                auto subboxj = testBox.subbox(j);
                for(size_t k=0; k<4; ++k){
                    for(size_t l=0; l<4; ++l){
                        auto ci = subboxi[k].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION);
                        auto cj = subboxj[l].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION);
                        retVal += ASSERT_BOOL(ci[0] != cj[0] || ci[1] != cj[1]);
                        allIndices.insert(ci[0]);
                        allIndices.insert(ci[1]);
                    }
                }
            }
        }
        retVal += ASSERT_BOOL(allIndices.size() == 4);
        for(size_t i : {0, 7, 8, 15}){
            retVal += ASSERT_BOOL(allIndices.contains(i));
        }
    }

    // Test second level subboxes
    {
        auto subbox = testBox.subbox(2).subbox(3);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({12,4}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({12,7}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,4}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({15,7}, 3));
    }
    {
        auto subbox = testBox.subbox(3).subbox(0);
        retVal += ASSERT_BOOL(subbox[0].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({8,8}, 3));
        retVal += ASSERT_BOOL(subbox[1].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({8,11}, 3));
        retVal += ASSERT_BOOL(subbox[2].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({11,8}, 3));
        retVal += ASSERT_BOOL(subbox[3].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION) == gs::index<2>({11,11}, 3));
    }
    {
        std::set<size_t> allIndices;
        for(size_t sbi =0; sbi < 4; ++sbi)
        {
            for(size_t sbj =sbi+1; sbj < 4; ++sbj)
            {
                auto firstLevelSBi = testBox.subbox(sbi);
                auto firstLevelSBj = testBox.subbox(sbj);
                for(size_t i=0; i<4; ++i){
                    for(size_t j=i+1; j<4; ++j){
                        auto subboxi = firstLevelSBi.subbox(i);
                        auto subboxj = firstLevelSBj.subbox(j);
                        for(size_t k=0; k<4; ++k){
                            for(size_t l=0; l<4; ++l){
                                auto ci = subboxi[k].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION);
                                auto cj = subboxj[l].at_level(3, gs::dimensions<2>::BOXES_SUBDIVISION);
                                retVal += ASSERT_BOOL(ci[0] != cj[0] || ci[1] != cj[1]);
                                allIndices.insert(ci[0]);
                                allIndices.insert(ci[1]);
                            }
                        }
                    }
                }
            }
        }
        retVal += ASSERT_BOOL(allIndices.size() == 8);
        for(size_t i : {0, 3, 4, 7, 8, 11, 12, 15}){
            retVal += ASSERT_BOOL(allIndices.contains(i));
        }
    }

    return retVal;
}