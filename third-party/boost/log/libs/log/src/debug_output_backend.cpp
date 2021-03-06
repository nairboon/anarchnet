/*
 *          Copyright Andrey Semashev 2007 - 2012.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   debug_output_backend.cpp
 * \author Andrey Semashev
 * \date   08.11.2008
 *
 * \brief  A logging sink backend that uses debugger output
 */

#include "windows_version.hpp"
#include <string>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <windows.h>

namespace boost {

namespace BOOST_LOG_NAMESPACE {

namespace sinks {

BOOST_LOG_ANONYMOUS_NAMESPACE {

#if defined(BOOST_LOG_USE_CHAR)
    inline void output_debug_string(const char* str)
    {
        OutputDebugStringA(str);
    }
#endif // defined(BOOST_LOG_USE_CHAR)
#if defined(BOOST_LOG_USE_WCHAR_T)
    inline void output_debug_string(const wchar_t* str)
    {
        OutputDebugStringW(str);
    }
#endif // defined(BOOST_LOG_USE_WCHAR_T)

} // namespace

template< typename CharT >
BOOST_LOG_EXPORT bool basic_debug_output_backend< CharT >::debugger_presence_filter::operator() (values_view_type const&) const
{
    return (IsDebuggerPresent() != FALSE);
}

template< typename CharT >
BOOST_LOG_EXPORT basic_debug_output_backend< CharT >::basic_debug_output_backend()
{
}

template< typename CharT >
BOOST_LOG_EXPORT basic_debug_output_backend< CharT >::~basic_debug_output_backend()
{
}

template< typename CharT >
BOOST_LOG_EXPORT typename basic_debug_output_backend< CharT >::debugger_presence_filter
basic_debug_output_backend< CharT >::get_debugger_presence_filter() const
{
    return debugger_presence_filter();
}

//! The method puts the formatted message to the event log
template< typename CharT >
BOOST_LOG_EXPORT void basic_debug_output_backend< CharT >::consume(
    record_type const&, target_string_type const& formatted_message)
{
    output_debug_string(formatted_message.c_str());
}

#ifdef BOOST_LOG_USE_CHAR
template class basic_debug_output_backend< char >;
#endif
#ifdef BOOST_LOG_USE_WCHAR_T
template class basic_debug_output_backend< wchar_t >;
#endif

} // namespace sinks

} // namespace log

} // namespace boost
