
#ifndef _GS_FMM_
#define _GS_FMM_

#include "base/pattern.hpp"
#include "base/grid.hpp"

namespace gs {

template<int N, typename T> 
class vector_field {
public:
    using grid_val = std::pair<T, std::array<T, N>>;
    using box_val = std::array<T, N>;
    using point_list = std::array<grid_val, pow<2,N>()>;
    using f_box_weight = std::function<void(const point_list&, box_val&)>;
    using f_far_field = std::function<void(const box_val&, grid_val&)>;
    using f_near_field = std::function<void(const point_list&, grid_val&)>;
};

/**
 * The Fast Multipole Method.
 */
template<
    int N,
    typename T=uint32_t,
    class FFarField=vector_field<N, double>::f_far_field,
    class FNearField=vector_field<N, double>::f_near_field,
    class FBoxWeight=vector_field<N, double>::f_box_weight,
    class GridElement=vector_field<N, double>::grid_val,
    class BoxElement=vector_field<N, double>::box_val
>
requires (
    std::is_integral<T>::value &&
    std::invocable<FFarField&, const BoxElement&, GridElement&> &&
    std::invocable<FNearField&, const std::array<GridElement, pow<2,N>()>&, GridElement&> &&
    std::invocable<FBoxWeight&, const std::array<GridElement, pow<2,N>()>&, BoxElement&>
)
class fmm {
    grid<N, GridElement, BoxElement, T> m_grid;
    FFarField m_farFieldFunc;
    FNearField m_nearFieldFunc;
    FBoxWeight m_boxWeightFunc;

public:
    fmm(
        const dimensions<N, T> dimensions,
        FFarField farFieldFunc,
        FNearField nearFieldFunc,
        FBoxWeight boxWeightFunc
    ) :
        m_grid(dimensions),
        m_farFieldFunc(farFieldFunc),
        m_nearFieldFunc(nearFieldFunc),
        m_boxWeightFunc(boxWeightFunc){}

    /**
     * Compute the solution.
     */
    void compute(T nIters=2){
        for (T it; it < nIters; ++it){
            m_grid.iterate(
                [&](box<N, T>& box, BoxElement& element, const PatternComponent pattern) {
                    switch (pattern) {
                        case COARSE_TO_FINE:
                            // Use the far field function for all levels apart
                            // from the most granular.
                            if (box.get_level() < m_grid.get_dimensions().max_level()){
                                for( const auto& corner : box){
                                    m_farFieldFunc(element, m_grid[corner]);
                                }
                            }
                            else {
                                for(const auto& corner : box){
                                    m_nearFieldFunc(m_grid.get_corner_values(box), m_grid[corner]);
                                }
                            }
                            break;
                        case FINE_TO_COARSE:
                            // Compute the box weights
                            m_boxWeightFunc(m_grid.get_corner_values(box), element);
                            break;
                        default:
                            break;
                    }
                },
                v_pattern()
            );
        }
    }

    GridElement& operator[](const index<N, T>& i){return m_grid[i];}
    const GridElement& operator[](const index<N, T>& i) const {return m_grid[i];}
    auto begin() -> decltype(m_grid.begin()){return m_grid.begin();}
    auto end() -> decltype(m_grid.end()){return m_grid.end();}
    auto begin() const -> decltype(m_grid.begin()){return m_grid.begin();}
    auto end() const -> decltype(m_grid.end()){return m_grid.end();}
};
}

#endif