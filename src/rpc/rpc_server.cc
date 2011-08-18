#include "rpc_server.h"
#include "logger.h"

namespace an
{

	void RpcServer::AddMethod(Json::Rpc::CallbackMethod* method)
    {
      _jsonHandler.AddMethod(method);
    }

    void RpcServer::DeleteMethod(const std::string& method)
    {
      _jsonHandler.DeleteMethod(method);
    }
	RpcServer::~RpcServer()
	{

	}

	
	void session::handle_read(const boost::system::error_code& error,
									 size_t bytes_transferred)
	{
		if (!error)
		{
			LOG(INFO) << "got: " << data_;
			boost::json::Value response;
			_jsonHandler->Process(data_, response);
			if(!response.is_null())
			{
				std::string rep = _jsonHandler->GetString(response);
		

			boost::asio::async_write(socket_,
															 boost::asio::buffer(rep, rep.size()),
															 boost::bind(&session::handle_write, this,
																					 boost::asio::placeholders::error));
			}
		}
		else
		{
			LOG(ERROR) << error;
			delete this;
		}
	}
} 