//  Copyright (c) 2018-2019
//  Mohammad Ashar Khan
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  The authors gratefully acknowledge the support of
//  Fraunhofer and Google in producing this work
//  which started as a Google Summer of Code project.

#ifndef BOOST_UBLAS_TENSOR_EXPRESSION_TRANSFORM_HPP
#define BOOST_UBLAS_TENSOR_EXPRESSION_TRANSFORM_HPP

#include <boost/yap/yap.hpp>

namespace boost::numeric::ublas
{

template <class element_type, class storage_format, class storage_type>
class tensor;

template <class size_type>
class basic_extents;

namespace detail
{

template <boost::yap::expr_kind Kind, typename Tuple>
struct tensor_expression;

}

}  // namespace boost::numeric::ublas

namespace boost::numeric::ublas::detail::transforms
{

struct at_index {
    template <typename T, typename F, typename A>
    decltype ( auto ) operator() (
        boost::yap::terminal<tensor_expression, tensor_core<T, F, A>> &expr )
    {
        return boost::yap::make_terminal ( boost::yap::value ( expr ) ( index ) );
    }
    size_t index;
};

// todo(@coder3101): Make a constexpr version of this transform once constexpr based extents are ready.

struct get_extents {

    template <class T, class F, class A>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::terminal>, ::boost::numeric::ublas::detail::tensor_core<T,F,A>& terminal )
    {
        return terminal.extents();
    }

    template <class LExpr, class RExpr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::minus>, LExpr &lexpr, RExpr &rexpr )
    {

        auto left = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( lexpr ), *this );
        auto right = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( rexpr ), *this );
        if ( left != right ) throw std::runtime_error ( "Cannot Subtract Tensor of shapes " + left.to_string() + " and " + right.to_string() );
        return left;

    }

    template <class LExpr, class RExpr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::multiplies>, LExpr &lexpr, RExpr &rexpr )
    {

        auto left = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( lexpr ), *this );
        auto right = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( rexpr ), *this );
        if ( left != right ) throw std::runtime_error ( "Cannot Multiply Tensor of shapes " + left.to_string() + " and " + right.to_string() );
        return left;

    }

    template <class LExpr, class RExpr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::divides>, LExpr &lexpr, RExpr &rexpr )
    {

        auto left = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( lexpr ), *this );
        auto right = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( rexpr ), *this );
        if ( left != right ) throw std::runtime_error ( "Cannot Divide Tensor of shapes " + left.to_string() + " and " + right.to_string() );
        return left;

    }

    template <class LExpr, class RExpr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::plus>, LExpr &lexpr, RExpr &rexpr )
    {
        auto left = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( lexpr ), *this );
        auto right = boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression> ( rexpr ), *this );
        if ( left != right ) throw std::runtime_error ( "Cannot Add Tensor of shapes " + left.to_string() + " and " + right.to_string() );
        return left;
    }

    template <class Expr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::negate>, Expr &expr )
    {
        return boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression>(expr), *this );
    }

    template <class Expr>
    decltype ( auto ) operator() ( ::boost::yap::expr_tag<boost::yap::expr_kind::unary_plus>, Expr &expr )
    {
        return boost::yap::transform ( boost::yap::as_expr<detail::tensor_expression>(expr), *this );
    }
};


}  // namespace boost::numeric::ublas::detail::transforms

#endif
