/*
 *          Copyright Andrey Semashev 2007 - 2012.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   singleton.hpp
 * \author Andrey Semashev
 * \date   20.04.2008
 *
 * \brief  This header is the Boost.Log library implementation, see the library documentation
 *         at http://www.boost.org/libs/log/doc/log.html.
 */

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif // _MSC_VER > 1000

#ifndef BOOST_LOG_DETAIL_SINGLETON_HPP_INCLUDED_
#define BOOST_LOG_DETAIL_SINGLETON_HPP_INCLUDED_

#include <boost/log/detail/prologue.hpp>
#include <boost/log/utility/once_block.hpp>

namespace boost {

namespace BOOST_LOG_NAMESPACE {

namespace aux {

//! A base class for singletons, constructed on-demand
template< typename DerivedT, typename StorageT = DerivedT >
class lazy_singleton
{
public:
    BOOST_LOG_DEFAULTED_FUNCTION(lazy_singleton(), {})

    //! Returns the singleton instance
    static StorageT& get()
    {
        BOOST_LOG_ONCE_BLOCK()
        {
            DerivedT::init_instance();
        }
        return get_instance();
    }

    //! Initializes the singleton instance
    static void init_instance()
    {
        get_instance();
    }

    BOOST_LOG_DELETED_FUNCTION(lazy_singleton(lazy_singleton const&))
    BOOST_LOG_DELETED_FUNCTION(lazy_singleton& operator= (lazy_singleton const&))

protected:
    //! Returns the singleton instance (not thread-safe)
    static StorageT& get_instance()
    {
        static StorageT instance;
        return instance;
    }
};

//! A base class for singletons, constructed on namespace scope initialization stage
template< typename DerivedT, typename StorageT = DerivedT >
class singleton :
    public lazy_singleton< DerivedT, StorageT >
{
public:
    static StorageT& instance;
};

template< typename DerivedT, typename StorageT >
StorageT& singleton< DerivedT, StorageT >::instance =
    lazy_singleton< DerivedT, StorageT >::get();

} // namespace aux

} // namespace log

} // namespace boost

#endif // BOOST_LOG_DETAIL_SINGLETON_HPP_INCLUDED_
