// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef TESTS_TEST_ITERATOR_HPP_
#define TESTS_TEST_ITERATOR_HPP_

#include <utility>

#include "base/box_stack_iterator.hpp"
#include "base/box_duel_iterator.hpp"
#include "base/box.hpp"

int test_bsi_points_1D() {
    std::cout << "Test box stack iterator points 1d" << std::endl;
    int retVal = 0;

    gs::dimensions<1> dims(2, 3);
    retVal += ASSERT_BOOL(dims.max_level() == 3);
    gs::box_stack_iterator<1> bsi(dims, gs::dimensions<1>::POINTS_SUBDIVISION);

    const size_t nBoxes = 4;

    uint32_t expected[nBoxes][3][2] {
        { {0, 4}, {0, 2}, {0, 1} },
        { {0, 4}, {0, 2}, {1, 2} },
        { {0, 4}, {2, 4}, {2, 3} },
        { {0, 4}, {2, 4}, {3, 4} }
    };

    size_t iters = 0;
    for (
        auto bsiIt = bsi;
        bsiIt < gs::box_stack_iterator<1>(dims, gs::dimensions<1>::POINTS_SUBDIVISION, true);
        ++bsiIt, ++iters
    ) {
        retVal += ASSERT_BOOL(iters < nBoxes);
        if ( iters >= nBoxes ) break;
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 2; ++j ) {
                retVal += ASSERT_BOOL(
                    (*bsiIt)[i][j].at_level(2, gs::dimensions<1>::POINTS_SUBDIVISION)[0] == expected[iters][i][j]
                );
            }
        }
    }

    return retVal;
}


int test_bsi_boxes_1D() {
    std::cout << "Test box stack iterator boxes 1d" << std::endl;
    int retVal = 0;

    gs::dimensions<1> dims(2, 3);
    retVal += ASSERT_BOOL(dims.max_level() == 3);
    gs::box_stack_iterator<1> bsi(dims, gs::dimensions<1>::BOXES_SUBDIVISION);

    const size_t nBoxes = 4;

    uint32_t expected[nBoxes][3][2] {
        { {0, 7}, {0, 3}, {0, 1} },
        { {0, 7}, {0, 3}, {2, 3} },
        { {0, 7}, {4, 7}, {4, 5} },
        { {0, 7}, {4, 7}, {6, 7} }
    };

    size_t iters = 0;
    for (
        auto bsiIt = bsi;
        bsiIt < gs::box_stack_iterator<1>(dims, gs::dimensions<1>::BOXES_SUBDIVISION, true);
        ++bsiIt, ++iters
    ) {
        retVal += ASSERT_BOOL(iters < nBoxes);
        if ( iters >= nBoxes ) break;
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 2; ++j ) {
                retVal += ASSERT_BOOL(
                    (*bsiIt)[i][j].at_level(2, gs::dimensions<1>::BOXES_SUBDIVISION)[0] == expected[iters][i][j]
                );
            }
        }
    }

    return retVal;
}

int test_bdi_boxes_1D() {
    std::cout << "Test box duel iterator boxes 1d" << std::endl;
    int retVal = 0;

    const size_t nLevels = 3;
    const size_t nDims = 1;
    gs::dimensions<nDims> dims(2, nLevels);

    std::array< std::pair<size_t, size_t>, 7 > expected {
        std::pair<size_t, size_t> {1, 2},
        std::pair<size_t, size_t> {3, 4},
        std::pair<size_t, size_t> {5, 6},
        std::pair<size_t, size_t> {7, 8},
        std::pair<size_t, size_t> {9, 10},
        std::pair<size_t, size_t> {11, 12},
        std::pair<size_t, size_t> {13, 14}
    };
    int ind = 0;
    for (
        gs::box_duel_iterator<nDims> it(dims, nLevels);
        it != gs::box_duel_iterator<nDims>(dims, nLevels, true);
        ++it, ++ind
    ) {
        retVal += ASSERT_BOOL( (*it)[0][0] == expected[ind].first );
        retVal += ASSERT_BOOL( (*it)[1][0] == expected[ind].second );
    }
    retVal += ASSERT_BOOL( ind == 7 );

    return retVal;
}

int test_bdi_boxes_2D() {
    std::cout << "Test box duel iterator boxes 2d" << std::endl;
    int retVal = 0;

    const size_t nDims = 2;
    const size_t nLevels = 2;
    gs::dimensions<nDims> dims(2, nLevels);
    size_t expected[9][4][2] {
        {{1, 1}, {1, 2}, {2, 1}, {2, 2}},
        {{3, 1}, {3, 2}, {4, 1}, {4, 2}},
        {{5, 1}, {5, 2}, {6, 1}, {6, 2}},
        {{1, 3}, {1, 4}, {2, 3}, {2, 4}},
        {{3, 3}, {3, 4}, {4, 3}, {4, 4}},
        {{5, 3}, {5, 4}, {6, 3}, {6, 4}},
        {{1, 5}, {1, 6}, {2, 5}, {2, 6}},
        {{3, 5}, {3, 6}, {4, 5}, {4, 6}},
        {{5, 5}, {5, 6}, {6, 5}, {6, 6}}
    };
    int ind = 0;
    for (
        gs::box_duel_iterator<nDims> it(dims, nLevels);
        it != gs::box_duel_iterator<nDims>(dims, nLevels, true);
        ++it, ++ind
    ) {
        for ( size_t i = 0; i < 4u; ++i ) {
            retVal += ASSERT_BOOL( (*it)[i][0] == expected[ind][i][0] );
            retVal += ASSERT_BOOL( (*it)[i][1] == expected[ind][i][1] );
        }
    }
    retVal += ASSERT_BOOL( ind == 9 );

    return retVal;
}

int test_bdi_point_boxes() {
    std::cout << "Test duel point boxes" << std::endl;
    int retVal = 0;

    const size_t nDims = 2;
    const size_t nLevels = 3;
    gs::dimensions<nDims> dims(2, nLevels);

    for (
        gs::box_duel_iterator<nDims> it(dims, nLevels);
        it != gs::box_duel_iterator<nDims>(dims, nLevels, true);
        ++it
    ) {
        std::array<gs::box<nDims>, 4u> allBoxes {
            allBoxes[0] = gs::box<nDims>(dims, (*it)[0], gs::dimensions<nDims>::BOXES_SUBDIVISION),
            allBoxes[1] = gs::box<nDims>(dims, (*it)[1], gs::dimensions<nDims>::BOXES_SUBDIVISION),
            allBoxes[2] = gs::box<nDims>(dims, (*it)[2], gs::dimensions<nDims>::BOXES_SUBDIVISION),
            allBoxes[3] = gs::box<nDims>(dims, (*it)[3], gs::dimensions<nDims>::BOXES_SUBDIVISION)
        };
        for ( size_t i = 0; i < 4u; ++i ) {
            retVal += ASSERT_BOOL( allBoxes[i].is_inside((*it)[i], false) );
            for ( size_t j = i+1; j < 4u; ++j ) {
                allBoxes[i] != allBoxes[j];
            }
        }
    }

    return retVal;
}

#endif  // TESTS_TEST_ITERATOR_HPP_
