/*
 *          Copyright Andrey Semashev 2007 - 2012.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   severity_feature.hpp
 * \author Andrey Semashev
 * \date   08.03.2007
 *
 * The header contains implementation of a severity level support feature.
 */

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif // _MSC_VER > 1000

#ifndef BOOST_LOG_SOURCES_SEVERITY_FEATURE_HPP_INCLUDED_
#define BOOST_LOG_SOURCES_SEVERITY_FEATURE_HPP_INCLUDED_

#include <boost/intrusive_ptr.hpp>
#include <boost/log/detail/prologue.hpp>
#include <boost/log/detail/locks.hpp>
#include <boost/log/detail/default_attribute_names.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/basic_attribute_value.hpp>
#include <boost/log/utility/strictest_lock.hpp>
#include <boost/log/utility/type_dispatch/type_dispatcher.hpp>
#include <boost/log/keywords/severity.hpp>

#ifdef _MSC_VER
#pragma warning(push)
// 'm_A' : class 'A' needs to have dll-interface to be used by clients of class 'B'
#pragma warning(disable: 4251)
// non dll-interface class 'A' used as base for dll-interface class 'B'
#pragma warning(disable: 4275)
#endif // _MSC_VER

namespace boost {

namespace BOOST_LOG_NAMESPACE {

namespace sources {

namespace aux {

    //! The method returns the severity level for the current thread
    BOOST_LOG_EXPORT int get_severity_level();
    //! The method sets the severity level for the current thread
    BOOST_LOG_EXPORT void set_severity_level(int level);

    //! Severity level attribute implementation
    template< typename LevelT >
    class severity_level :
        public attribute
    {
    public:
        //! Stored level type
        typedef LevelT value_type;

    protected:
        //! Factory implementation
        class BOOST_LOG_VISIBLE impl :
            public attribute_value::impl
        {
        public:
            //! The method dispatches the value to the given object
            bool dispatch(type_dispatcher& dispatcher)
            {
                type_dispatcher::callback< value_type > callback =
                    dispatcher.get_callback< value_type >();
                if (callback)
                {
                    callback(static_cast< value_type >(get_severity_level()));
                    return true;
                }
                else
                    return false;
            }

            //! The method is called when the attribute value is passed to another thread
            intrusive_ptr< attribute_value::impl > detach_from_thread()
            {
    #if !defined(BOOST_LOG_NO_THREADS)
                return new attributes::basic_attribute_value< value_type >(
                    static_cast< value_type >(get_severity_level()));
    #else
                // With multithreading disabled we may safely return this here. This method will not be called anyway.
                return this;
    #endif
            }
        };

    public:
        //! Default constructor
        severity_level() : attribute(new impl())
        {
        }
        //! Constructor for casting support
        explicit severity_level(attributes::cast_source const& source) :
            attribute(source.as< impl >())
        {
        }
        //! The method sets the actual level
        void set_value(value_type level)
        {
            set_severity_level(static_cast< int >(level));
        }
    };

} // namespace aux

/*!
 * \brief Severity level feature implementation
 */
template< typename BaseT, typename LevelT = int >
class basic_severity_logger :
    public BaseT
{
    //! Base type
    typedef BaseT base_type;

public:
    //! Character type
    typedef typename base_type::char_type char_type;
    //! Final type
    typedef typename base_type::final_type final_type;
    //! Attribute set type
    typedef typename base_type::attribute_set_type attribute_set_type;
    //! Threading model being used
    typedef typename base_type::threading_model threading_model;
    //! Log record type
    typedef typename base_type::record_type record_type;

    //! Severity level type
    typedef LevelT severity_level;
    //! Severity attribute type
    typedef aux::severity_level< severity_level > severity_attribute;

#if defined(BOOST_LOG_DOXYGEN_PASS)
    //! Lock requirement for the \c open_record_unlocked method
    typedef typename strictest_lock<
        typename base_type::open_record_lock,
        no_lock< threading_model >
    >::type open_record_lock;
#endif // defined(BOOST_LOG_DOXYGEN_PASS)

    //! Lock requirement for the \c swap_unlocked method
    typedef typename strictest_lock<
        typename base_type::swap_lock,
#ifndef BOOST_LOG_NO_THREADS
        boost::log::aux::exclusive_lock_guard< threading_model >
#else
        no_lock< threading_model >
#endif // !defined(BOOST_LOG_NO_THREADS)
    >::type swap_lock;

private:
    //! Default severity
    severity_level m_DefaultSeverity;
    //! Severity attribute
    severity_attribute m_SeverityAttr;

public:
    /*!
     * Default constructor. The constructed logger will have a severity attribute registered.
     * The default level for log records will be 0.
     */
    basic_severity_logger() :
        base_type(),
        m_DefaultSeverity(static_cast< severity_level >(0))
    {
        base_type::add_attribute_unlocked(
            boost::log::aux::default_attribute_names< char_type >::severity(),
            m_SeverityAttr);
    }
    /*!
     * Copy constructor
     */
    basic_severity_logger(basic_severity_logger const& that) :
        base_type(static_cast< base_type const& >(that)),
        m_DefaultSeverity(that.m_DefaultSeverity),
        m_SeverityAttr(that.m_SeverityAttr)
    {
        base_type::attributes()[boost::log::aux::default_attribute_names< char_type >::severity()] = m_SeverityAttr;
    }
    /*!
     * Constructor with named arguments. Allows to setup the default level for log records.
     *
     * \param args A set of named arguments. The following arguments are supported:
     *             \li \c severity - default severity value
     */
    template< typename ArgsT >
    explicit basic_severity_logger(ArgsT const& args) :
        base_type(args),
        m_DefaultSeverity(args[keywords::severity | severity_level()])
    {
        base_type::add_attribute_unlocked(
            boost::log::aux::default_attribute_names< char_type >::severity(),
            m_SeverityAttr);
    }

    /*!
     * Default severity value getter
     */
    severity_level default_severity() const { return m_DefaultSeverity; }

protected:
    /*!
     * Severity attribute accessor
     */
    severity_attribute const& get_severity_attribute() const { return m_SeverityAttr; }

    /*!
     * Unlocked \c open_record
     */
    template< typename ArgsT >
    record_type open_record_unlocked(ArgsT const& args)
    {
        m_SeverityAttr.set_value(args[keywords::severity | m_DefaultSeverity]);
        return base_type::open_record_unlocked(args);
    }

    //! Unlocked \c swap
    void swap_unlocked(basic_severity_logger& that)
    {
        base_type::swap_unlocked(static_cast< base_type& >(that));
        severity_level t = m_DefaultSeverity;
        m_DefaultSeverity = that.m_DefaultSeverity;
        that.m_DefaultSeverity = t;
        m_SeverityAttr.swap(that.m_SeverityAttr);
    }
};

/*!
 * \brief Severity level support feature
 *
 * The logger with this feature registers a special attribute with an integral value type on construction.
 * This attribute will provide severity level for each log record being made through the logger.
 * The severity level can be omitted on logging record construction, in which case the default
 * level will be used. The default level can also be customized by passing it to the logger constructor.
 *
 * The type of the severity level attribute can be specified as a template parameter for the feature
 * template. By default, \c int will be used.
 */
template< typename LevelT = int >
struct severity
{
    template< typename BaseT >
    struct apply
    {
        typedef basic_severity_logger<
            BaseT,
            LevelT
        > type;
    };
};

} // namespace sources

} // namespace log

} // namespace boost

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

//! The macro allows to put a record with a specific severity level into log
#define BOOST_LOG_STREAM_SEV(logger, lvl)\
    BOOST_LOG_STREAM_WITH_PARAMS((logger), (::boost::log::keywords::severity = (lvl)))

#ifndef BOOST_LOG_NO_SHORTHAND_NAMES

//! An equivalent to BOOST_LOG_STREAM_SEV(logger, lvl)
#define BOOST_LOG_SEV(logger, lvl) BOOST_LOG_STREAM_SEV(logger, lvl)

#endif // BOOST_LOG_NO_SHORTHAND_NAMES

#endif // BOOST_LOG_SOURCES_SEVERITY_FEATURE_HPP_INCLUDED_
