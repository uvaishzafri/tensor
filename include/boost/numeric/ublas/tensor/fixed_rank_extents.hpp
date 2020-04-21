//
//  Copyright (c) 2018-2020, Cem Bassoy, cem.bassoy@gmail.com
//  Copyright (c) 2019-2020, Amit Singh, amitsingh19975@gmail.com
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  The authors gratefully acknowledge the support of
//  Google and Fraunhofer IOSB, Ettlingen, Germany
//


#ifndef _BOOST_NUMERIC_UBLAS_TENSOR_FIXED_RANK_EXTENTS_HPP_
#define _BOOST_NUMERIC_UBLAS_TENSOR_FIXED_RANK_EXTENTS_HPP_

#include <algorithm>
#include <string>
#include <initializer_list>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>
#include <array>
#include <boost/numeric/ublas/tensor/detail/type_traits.hpp>

#include <cassert>

namespace boost {
namespace numeric {
namespace ublas {

/** @brief Template class for storing tensor extents for compile time.
 *
 * @code basic_static_extents<1,2,3,4> t @endcode
 * @tparam E parameter pack of extents
 *
 */
template <class ExtentsType, std::size_t N>
struct basic_fixed_rank_extents
{
  

    static constexpr std::size_t const _size = N;


    using base_type       = std::array<ExtentsType,_size>;
    using value_type      = typename base_type::value_type;
    using const_reference = typename base_type::const_reference;
    using reference       = typename base_type::reference;
    using const_pointer   = typename base_type::const_pointer;
    using const_iterator  = typename base_type::const_iterator;
    using size_type       = typename base_type::size_type;

    static_assert( std::numeric_limits<value_type>::is_integer, "Static error in basic_fixed_rank_extents: type must be of type integer.");
    static_assert(!std::numeric_limits<value_type>::is_signed,  "Static error in basic_fixed_rank_extents: type must be of type unsigned integer.");

    //@returns the rank of basic_static_extents
    [[nodiscard]] 
    static constexpr size_type size() noexcept { return _size; }

    [[nodiscard]] inline
    constexpr const_reference at(size_type k) const{ 
        return _base.at(k);
    }

    [[nodiscard]] inline
    constexpr reference at(size_type k){ 
        return _base.at(k);
    }

    [[nodiscard]] inline
    constexpr const_reference operator[](size_type k) const noexcept{ 
        return _base[k];
    }

    [[nodiscard]] inline
    constexpr reference operator[](size_type k) noexcept{ 
        return _base[k];
    }

    // default constructor
    constexpr basic_fixed_rank_extents() = default;
    
    constexpr basic_fixed_rank_extents(std::initializer_list<value_type> li){
        if( li.size() > _size ){
            throw std::out_of_range("boost::numeric::ublas::basic_fixed_rank_extents(): initializer list size is greater than _size");
        }
        
        std::copy(li.begin(), li.end(), _base.begin());

        if ( !valid(*this) ){
            throw std::length_error("Error in basic_fixed_rank_extents::basic_fixed_rank_extents() : shape tuple is not a valid permutation: has zero elements.");
        }
    }
    
    constexpr basic_fixed_rank_extents(const_iterator begin, const_iterator end){
        if( std::distance(begin,end) > _size ){
            throw std::out_of_range("boost::numeric::ublas::basic_fixed_rank_extents(): initializer list size is greater than _size");
        }
        
        std::copy(begin, end, _base.begin());

        if ( !valid(*this) ){
            throw std::length_error("Error in basic_fixed_rank_extents::basic_fixed_rank_extents() : shape tuple is not a valid permutation: has zero elements.");
        }
    }
    
    void fill( value_type value ){
        _base.fill(value);
    }
    
    template<typename OtherExtentsType,
        std::enable_if_t< 
            detail::is_extents<OtherExtentsType>::value
            && ( !std::is_same_v<basic_fixed_rank_extents, OtherExtentsType> )
            ,int > = 0
    >
    constexpr basic_fixed_rank_extents(OtherExtentsType const& e){
        for(auto i = size_type(0); i < size(); ++i){
            _base[i] = e[i];
        }
    }
    
    constexpr basic_fixed_rank_extents(base_type const& data)
        : _base(data)
    {
        if ( !valid(*this) ){
            throw std::length_error("Error in basic_fixed_rank_extents::basic_fixed_rank_extents() : shape tuple is not a valid permutation: has zero elements.");
        }
    }
    
    constexpr basic_fixed_rank_extents(base_type&& data)
        : _base(std::move(data))
    {
        if ( !valid(*this) ){
            throw std::length_error("Error in basic_fixed_rank_extents::basic_fixed_rank_extents() : shape tuple is not a valid permutation: has zero elements.");
        }
    }

    /** @brief Returns the std::vector containing extents */
    [[nodiscard]] inline
    auto to_vector() const {
        return std::vector<value_type>( _base.begin(), _base.end() );
    }

    /** @brief Returns the std::vector containing extents */
    [[nodiscard]] inline
    constexpr base_type const& base() const {
        return _base;
    }

    /** @brief Returns the std::vector containing extents */
    [[nodiscard]] inline
    constexpr base_type& base() {
        return _base;
    }

    /** @brief Returns the basic_extents containing extents */
    [[nodiscard]] inline
    auto to_dynamic_extents() const {
        return basic_extents<value_type>(this->to_vector());
    }

    /** @brief Checks if extents is empty or not
     *
     * @returns true if rank is 0 else false
     *
     */
    [[nodiscard]] inline
    constexpr bool empty() const noexcept { return _size == size_type{0}; }

    friend void swap(basic_fixed_rank_extents& lhs, basic_fixed_rank_extents& rhs) {
        std::swap(lhs._base   , rhs._base   );
    }

    [[nodiscard]] inline
    const_pointer data() const
    {
        return _base.data();
    }

    [[nodiscard]] inline
    const_iterator
    begin() const
    {
        return _base.begin();
    }

    [[nodiscard]] inline
    const_iterator
    end() const
    {
        return _base.end();
    }

    const_reference back () const
    {
        return _base.back();
    }

    /** @brief Returns true if both extents are equal else false */
    [[nodiscard]] inline
    constexpr bool operator==(basic_fixed_rank_extents const &other) const {
        return _base == other._base;
    }

    /** @brief Returns false if both extents are equal else true */
    [[nodiscard]] inline
    constexpr bool operator!=(basic_fixed_rank_extents const &other) const {
        return !(*this == other);
    }

    ~basic_fixed_rank_extents() = default;

private:
    base_type _base{};
};

} // namespace ublas
} // namespace numeric
} // namespace boost


namespace boost::numeric::ublas::detail{
    
template <class T, std::size_t R>
struct is_extents< basic_fixed_rank_extents<T, R> > : std::true_type {};

template <class T, std::size_t R>
struct is_dynamic< basic_fixed_rank_extents<T,R> > : std::true_type {};

template <class T, std::size_t R>
struct is_static_rank< basic_fixed_rank_extents<T,R> > : std::true_type {};

} // namespace boost::numeric::ublas

#endif
