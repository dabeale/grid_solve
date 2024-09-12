// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef TESTS_TEST_DIMENSIONS_HPP_
#define TESTS_TEST_DIMENSIONS_HPP_

#include "base/dimensions.hpp"
#include "base/tools.hpp"
#include "base/index.hpp"

int test_dimensions_sub2ind() {
    std::cout << "Test dimensions sub2ind" << std::endl;
    gs::dimensions<3> dims({3, 4, 5}, 5);
    int retVal = 0;
    retVal += ASSERT_BOOL(dims.sub2ind({0, 0, 1}, 0) == 1);
    retVal += ASSERT_BOOL(dims.sub2ind({0, 1, 0}, 0) == 5);
    retVal += ASSERT_BOOL(dims.sub2ind({1, 0, 0}, 0) == 20);
    retVal += ASSERT_BOOL(dims.sub2ind({0, 0, 1}, 1) == 1);
    retVal += ASSERT_BOOL(dims.sub2ind({0, 1, 0}, 1) == 9);
    retVal += ASSERT_BOOL(dims.sub2ind({1, 0, 0}, 1) == 63);
    return retVal;
}

int test_dimensions_sub2ind_simple() {
    std::cout << "Test dimensions sub2ind simple" << std::endl;
    gs::dimensions<2> dims({3, 3}, 5);
    int retVal = 0;
    // At level 0 the index {2,2} is 3*2 + 2 = 8
    retVal += ASSERT_BOOL(dims.sub2ind({2, 2}, 0) == 8);
    return retVal;
}

int test_dimensions_sub2ind_inversion() {
    std::cout << "Test dimensions sub2ind inversion" << std::endl;
    gs::dimensions<3> dims({3, 4, 5}, 5);
    int retVal = 0;
    for ( uint32_t i = 0; i < 3*4*5; ++i ) {
        retVal += ASSERT_BOOL(dims.sub2ind(dims.ind2sub(i)) == i);
    }
    return retVal;
}


int test_point_convert_topoints_ind2sub() {
    std::cout << "Test boxes to points conversion ind2sub" << std::endl;
    const size_t level = 3;
    const size_t nDims = 2;
    gs::dimensions<nDims> dims(2, level);
    int retVal = 0;
    {
        auto type = gs::dimensions<nDims>::BOXES_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::BOXES_MODE;
        auto conv = gs::dimensions<nDims>::POINTS_CONV;
        retVal += ASSERT_BOOL(dims.ind2sub(0, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(1, level, type, mode, conv ) == gs::index<nDims>({0, 2}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(8, level, type, mode, conv ) == gs::index<nDims>({2, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(9, level, type, mode, conv ) == gs::index<nDims>({2, 2}, level));
    }
    {
        auto type = gs::dimensions<nDims>::POINTS_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::BOXES_MODE;
        auto conv = gs::dimensions<nDims>::POINTS_CONV;
        retVal += ASSERT_BOOL(dims.ind2sub(0, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(1, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(8, level, type, mode, conv ) == gs::index<nDims>({1, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(9, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
    }
    return retVal;
}

int test_point_convert_toboxes_ind2sub() {
    std::cout << "Test boxes to boxes conversion ind2sub" << std::endl;
    const size_t level = 3;
    const size_t nDims = 2;
    gs::dimensions<nDims> dims(2, level);
    int retVal = 0;
    {
        auto type = gs::dimensions<nDims>::BOXES_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::POINTS_MODE;
        auto conv = gs::dimensions<nDims>::BOXES_CONV;
        {
            retVal += ASSERT_BOOL(dims.ind2sub(0, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(1, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(16, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(17, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
        }
        {
            retVal += ASSERT_BOOL(dims.ind2sub(2, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(3, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(18, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(19, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
        }
        {
            retVal += ASSERT_BOOL(dims.ind2sub(34, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(35, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(50, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
            retVal += ASSERT_BOOL(dims.ind2sub(51, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
        }
    }
    {
        auto type = gs::dimensions<nDims>::POINTS_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::POINTS_MODE;
        auto conv = gs::dimensions<nDims>::BOXES_CONV;
        retVal += ASSERT_BOOL(dims.ind2sub(0, level, type, mode, conv ) == gs::index<nDims>({0, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(1, level, type, mode, conv ) == gs::index<nDims>({0, 1}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(9, level, type, mode, conv ) == gs::index<nDims>({1, 0}, level));
        retVal += ASSERT_BOOL(dims.ind2sub(10, level, type, mode, conv ) == gs::index<nDims>({1, 1}, level));
    }
    return retVal;
}


int test_point_convert_toboxes_sub2ind() {
    std::cout << "Test points to boxes conversion sub2ind" << std::endl;
    const size_t level = 3;
    const size_t nDims = 2;
    gs::dimensions<nDims> dims(2, level);
    int retVal = 0;
    {
        auto type = gs::dimensions<nDims>::BOXES_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::POINTS_MODE;
        auto conv = gs::dimensions<nDims>::BOXES_CONV;
        {
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 0}, level), level, type, mode, conv ) == 0);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 0}, level), level, type, mode, conv ) == 0);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 1}, level), level, type, mode, conv ) == 0);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 1}, level), level, type, mode, conv ) == 0);
        }
        {
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 2}, level), level, type, mode, conv ) == 1);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 3}, level), level, type, mode, conv ) == 1);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 2}, level), level, type, mode, conv ) == 1);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 3}, level), level, type, mode, conv ) == 1);
        }
        {
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({2, 0}, level), level, type, mode, conv ) == 8);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({3, 0}, level), level, type, mode, conv ) == 8);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({2, 1}, level), level, type, mode, conv ) == 8);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({3, 1}, level), level, type, mode, conv ) == 8);
        }
        {
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({2, 2}, level), level, type, mode, conv ) == 9);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({2, 3}, level), level, type, mode, conv ) == 9);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({3, 2}, level), level, type, mode, conv ) == 9);
            retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({3, 3}, level), level, type, mode, conv ) == 9);
        }
    }
    {
        auto type = gs::dimensions<nDims>::POINTS_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::POINTS_MODE;
        auto conv = gs::dimensions<nDims>::BOXES_CONV;
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 0}, level), level, type, mode, conv ) == 0);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 1}, level), level, type, mode, conv ) == 1);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 0}, level), level, type, mode, conv ) == 8);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 1}, level), level, type, mode, conv ) == 9);
    }
    return retVal;
}

int test_point_convert_topoints_sub2ind() {
    std::cout << "Test points to points conversion sub2ind" << std::endl;
    const size_t level = 3;
    const size_t nDims = 2;
    gs::dimensions<nDims> dims(2, level);
    int retVal = 0;
    {
        auto type = gs::dimensions<nDims>::BOXES_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::BOXES_MODE;
        auto conv = gs::dimensions<nDims>::POINTS_CONV;
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 0}, level), level, type, mode, conv ) == 0);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 1}, level), level, type, mode, conv ) == 2);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 0}, level), level, type, mode, conv ) == 32);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 1}, level), level, type, mode, conv ) == 34);
    }
    {
        auto type = gs::dimensions<nDims>::POINTS_SUBDIVISION;
        auto mode = gs::dimensions<nDims>::BOXES_MODE;
        auto conv = gs::dimensions<nDims>::POINTS_CONV;
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 0}, level), level, type, mode, conv ) == 0);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({0, 1}, level), level, type, mode, conv ) == 1);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 0}, level), level, type, mode, conv ) == 9);
        retVal += ASSERT_BOOL(dims.sub2ind(gs::index<nDims>({1, 1}, level), level, type, mode, conv ) == 10);
    }
    return retVal;
}

#endif  // TESTS_TEST_DIMENSIONS_HPP_
