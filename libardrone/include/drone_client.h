#ifndef DRONE_CLIENT_H
#define DRONE_CLIENT_H

#include "at_client.h"
#include "video_client.h"
#include "video_staging.h"
#include "navdata_client.h"

/**
 * \brief DroneClient is the mother class to create a application with the ARDrone
 */
class DroneClient {
protected:
	~DroneClient() = default;

	bool				united_video_thread;

	VFQueue				vf_queue;
	MQueue				mat_queue;
	ATQueue				at_queue;
	NAVQueue			nav_queue;

	VideoStaging		video_staging;	
	VideoClient			video_client;
	ATClient			at_client;
	NavDataClient		nd_client;
	DroneControl		control;
	AsyncControl		auto_control;
private:
	std::thread			vs_thread;
	std::thread			vc_thread;
	std::thread			at_thread;
	std::thread			nd_thread;

public:
	DroneClient();
	DroneClient(bool lol = true);

	int Start();

	bool isAllThreadRunning();

	virtual void mainLoop() = 0;
protected:

	int init();
	int stop();

};



#endif // _DRONE_CLIENT_H_
