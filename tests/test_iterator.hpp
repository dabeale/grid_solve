// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef TESTS_TEST_ITERATOR_HPP_
#define TESTS_TEST_ITERATOR_HPP_

#include "base/box_stack_iterator.hpp"

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
        bsiIt < gs::box_stack_iterator<1>(
            dims,
            gs::dimensions<1>::POINTS_SUBDIVISION, true);
        ++bsiIt, ++iters) {
        retVal += ASSERT_BOOL(iters < nBoxes);
        if ( iters >= nBoxes ) break;
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 2; ++j ) {
                retVal += ASSERT_BOOL(
                    (*bsiIt)[i][j].at_level(
                        2,
                        gs::dimensions<1>::POINTS_SUBDIVISION)[0]
                    == expected[iters][i][j]);
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
        bsiIt < gs::box_stack_iterator<1>(
            dims, gs::dimensions<1>::BOXES_SUBDIVISION, true);
        ++bsiIt, ++iters) {
        retVal += ASSERT_BOOL(iters < nBoxes);
        if ( iters >= nBoxes ) break;
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 2; ++j ) {
                retVal += ASSERT_BOOL(
                    (*bsiIt)[i][j].at_level(
                        2,
                        gs::dimensions<1>::BOXES_SUBDIVISION)[0]
                    == expected[iters][i][j]);
            }
        }
    }

    return retVal;
}

#endif  // TESTS_TEST_ITERATOR_HPP_
