/*
 * Copyright (C) 2010, 2011 Remo Hertig (nairboon)
 * https://launchpad.net/anarchNet
 *
 * This file is part of anarchNet.
 *
 * anarchNet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * anarchNet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with anarchNet.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \mainpage AnarchNet Documentation
 *
 * \section intro_sec Introduction
 *
 * AnarchNet is a p2p overlay network, which connects multiple p2p protocols
 * 
 * \section more_sec More
 * Website http://rbose.org/wiki/AnarchNet
 * Launchpad http://launchpad.net/AnarchNet
 */

/*!
 *  \defgroup daemon Daemon
 *  \brief Classes and functions related to the anarchNet daemon
 */

/*!
 *  \defgroup library Library
 *  \brief Classes and functions related to the anarchNet public library
 */

/*!
 *  \defgroup plugins Plugins
 *  \brief standard plugins distributed with anarchNet
 */

#ifndef SRC_DAEMON_ANARCHNET_H_
#define SRC_DAEMON_ANARCHNET_H_
#include "config.h"

/*!
 *  \namespace an
 *  \brief     Namespace of all AnarchNet framework classes and global functions.
 */
namespace an {
#define ANARCHNET_RPC_PORT 23666
#define ANARCHNET_PORT 23555
#define ANARCHNET_THREADS 4
#define ANARCHNET_BLOCKSIZE 1024

#define AN_ID_LENGTH 64 //hash size 512

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define ANARCHNET_PLATFORM_WIN32
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#   define ANARCHNET_PLATFORM_APPLE
#else
#   define ANARCHNET_PLATFORM_LINUX
#endif

}
#endif  // SRC_DAEMON_ANARCHNET_H_
