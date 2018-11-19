#ifndef _NAVDATA_CLIENT_H_
#define _NAVDATA_CLIENT_H_

#include "navdata_client.h"
#include "config.h"
#include "navdata_common.h"
#include "thread.h"

#include <asio.hpp>
using namespace asio;
using ip::udp;
using ip::address;

typedef ConcurrentQueue<navdata_demo_t> NAVQueue;

class NavDataClient: public Runnable{

    public:
        NavDataClient(NAVQueue* queue);
        ~NavDataClient();

    private:
		void init_communication();
		
	NAVQueue*			queue;

	io_service			ios;
	udp::endpoint		remote_endpoint;
	udp::endpoint		local_endpoint;
	udp::socket			socket;

	uint8_t*			recv_buffer;

	

	void run_service() override;
};

#endif // _NAVDATA_CLIENT_H_
