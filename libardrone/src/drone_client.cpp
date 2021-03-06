#include "drone_client.h"

DroneClient::DroneClient() :
	video_staging(&vf_queue, &mat_queue),
	video_client(&vf_queue),
	at_client(&at_queue),
	nd_client(&nav_queue),
	control(&at_queue),
	auto_control(&at_client)
{
}

DroneClient::DroneClient(bool lol) :
	video_staging(&mat_queue),
	video_client(&video_staging),
	at_client(&at_queue),
	nd_client(&nav_queue),
	control(&at_queue),
	auto_control(&at_client) {
	united_video_thread = lol;
}

#include "video_client.h"

int DroneClient::Start()
{
	if(int ret = init(); ret > 0)
		return ret;
	mainLoop();
	return stop();
}

bool DroneClient::isAllThreadRunning()
{
	return video_client.isRunning();
}

int DroneClient::init()
{
	if (int ret = video_staging.init(); ret > 0)
	{
		std::cerr << "Error during video staging init " << ret << endl;
		return 1;
	}
	at_thread = at_client.start();
	vc_thread = video_client.start();
	if(!this->united_video_thread)
		vs_thread = video_staging.start();
	nd_thread = nd_client.start();
	std::cout << "All thread are started" << std::endl;
	return 0;
}

int DroneClient::stop()
{
	at_client.set_ref(LAND_FLAG);
	std::this_thread::sleep_for(100ms);

	std::cout << "Stopping all thread and joining theme" << std::endl;
	video_staging.stop();
	video_client.stop();
	at_client.stop();
	nd_client.stop();

	nd_thread.join();
	if(!united_video_thread)
		vs_thread.join();
	vc_thread.join();
	at_thread.join();
	return 0;
}
