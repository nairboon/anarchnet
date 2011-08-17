/*
 * Copyright (C) 2010 Remo Hertig (nairboon)
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

#include <glog/logging.h>
#include <iostream>
#include <boost/thread.hpp>
#include "version.h"
#include "anarchNet.h"
#include "db_manager.h"
#include <json/json.h>
#include "jsonrpc.h"
#include "config_manager.h"
#include "net_manager.h"

namespace an
{
	
	bool	NetManager::init() {
		_server = new Json::Rpc::TcpServer(std::string("127.0.0.1"), ConfigManager::instance().port());
		
		
		p2p::Hello hs;
		_server->AddMethod(new Json::Rpc::RpcMethod<p2p::Hello>(hs, &p2p::Hello::Hello, std::string("Hello")));
		_server->AddMethod(new Json::Rpc::RpcMethod<p2p::Hello>(hs, &p2p::Hello::GetPeers, std::string("GetPeers")));
		return true;
	}	
	void NetManager::run() 
	{
		
		if(!_server->Bind())
		{
			LOG(ERROR) << "Bind failed";
			exit(EXIT_FAILURE);
		}
		
		if(!_server->Listen())
		{
			LOG(ERROR) << "Listen failed";
			exit(EXIT_FAILURE);
		}
		
		
		LOG(INFO) << "Start p2p networking";
		
		while(running_)
		{
			_server->WaitMessage(1000);
		}
		
		_server->Close();
	}
	
	NetManager::~NetManager() {
		LOG(INFO) << "Stop NetManager";
		delete _server;
	}	
		
}