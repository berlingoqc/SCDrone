#ifndef VIDEO_STAGING_H
#define VIDEO_STAGING_H

#include "video_common.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libavutil/opt.h>
	#include <libavutil/common.h>
	#include <libavutil/avutil.h>
	#include <libavutil/imgutils.h>
	#include <libswscale/swscale.h>
}
#include <fstream>
#include <iostream>
#include <filesystem>

#include <opencv2/core/mat.hpp>

namespace fs = std::filesystem;


#define H264_INBUF_SIZE 100000 // Grosseur de mon buffer que j'utilise pour les trames que je recoit

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;


struct video_staging_info
{
	int					codec;
	int					bit_rate;
	int					d_width;
	int					d_height;

	int					nbr_frame_missing;
	double				average_decoding_time;
	int					nbr_frame_gap;
	
	bool				is_recording_raw;
	const char*			file_name;

};


template<int Size>
struct PacketBuffer {
	uint8_t		buffer[Size];
	int			max_length = Size;
	int			index_parsing = 0;
	int			index_end = 0;


	// Retourne si j'ai de l'espace pour ajouter le nombre de byte passer
	bool		validSpace(int data_got) {
		return index_parsing+index_end + data_got + AV_INPUT_BUFFER_PADDING_SIZE <= max_length;
	}

	void		reset() {
		printf("Reseting Packet buffer index_end %d \n", index_end);

		index_end = 0;
		index_parsing = 0;
	}

	void		setBegin(int index) {
		index_parsing = index;
	}

	void		append(uint8_t* data, int size) {
		memcpy(buffer + index_end, data, size);
		index_end += size;
	}




};

class VideoStaging : public Runnable
{
	video_encapsulation_t		prev_encapsulation_ = {}; // Dernier header PaVE re�u

	AVPixelFormat				format_in;					// Format pixel du flux video
	AVPixelFormat				format_out;					// Format pixel de l'image de sortit pour OpenCV
	int							display_width;				// Largeur de l'image dans le stream
	int							display_height;				// Hauteur de l'image dans le stream
	int							bit_rate;					// Bit-rate du stream re�u
	int							fps;						// FPS du stream re�u

#ifdef DEBUG_VIDEO_STAGING
	int								frame_lost = 0;
	TimePoint						start_gap;
	TimePoint						end_gap;
	TimePoint						last_start;
	TimePoint						last_end;
	std::vector<long>				times;
#endif
	AVCodec*					codec;						// Contient les informations du codec qu'on utilise (H264)
	AVCodecContext*				codec_ctx;					// Contient le context de notre codec ( information sur comment decoder le stream )
	AVCodecParserContext*		codec_parser;				// Contient le context sur comment parser les frame de notre stream


	AVFrame*					frame;						// Contient la derni�re frame qu'on na re�u a reconstruire de notre stream
	AVFrame*					frame_output;				// Contient la derni�re frame convertit dans le format pour l'envoyer a OpenCV
	AVPacket*					packet;

	//uint8_t**					buffer_array;				// Contient la liste des buffers que nous avons re�u depuis le d�but
	//uint8_t*					buffer;						// Contient le buffer en attendant qu'on n'aille une frame complete
	//int							indice_buffer;				// Contient l'indice courrant dans le buffer
	//int							buffer_size;				// Contient la grosseur du buffer en ce moment

	PacketBuffer<H264_INBUF_SIZE> packet_buffer;			// Structure qui contient mon buffer que je remplit pou avoir une frame


	int							line_size;

	SwsContext*					img_convert_ctx;			// Contient le context pour effectuer la conversion entre notre image YUV420 et BGR pour OpenCV

	int							first_frame;				// Contient le num�ro de la premi�re frame re�u
	int							last_frame;					// Contient le num�ro de la derni�re frame re�u

	bool						have_received;				// Indique si on n'a d�j� commencer a recevoir des trames
	bool						only_idr = true;			// Indique si on souhaite selon parser les frames IDR et de skipper les frames P

	atomic<bool>				record_to_file_raw = false; // Indique si on souhaite sauvegarder le stream dans un fichier

	fs::path					record_folder = fs::path("./recording");		// Indique le chemin du fichier a sauvegarder le stream
	int							stream_index = 0;
	std::ofstream				of;

	bool						thread_mode = false;
	VFQueue*					queue;						// La queue dans laquelle les frames re�u par TCP arrivent

	MQueue*						mqueue;

	std::atomic<video_staging_info> staging_info;
	

public:
	VideoStaging(MQueue* mqueue);
	VideoStaging(VFQueue* queue, MQueue* mqueue);
	//VideoStaging(VFQueue* queue, const char* filepath);
	~VideoStaging();

	int	init() const;

	void run_service() override;

	void set_raw_recording(bool state);

	void onNewVideoFrame(VideoFrame& vf);

	video_staging_info getInfo()
	{
		return staging_info;
	}

private:

	bool have_frame_changed(const VideoFrame& vf);
	bool add_frame_buffer(const VideoFrame& vf);
	void init_or_frame_changed(const VideoFrame& vf,bool init = false);
	bool frame_to_mat(const AVFrame* avframe, cv::Mat& m);
	void append_file(const VideoFrame& vf);
};


#endif