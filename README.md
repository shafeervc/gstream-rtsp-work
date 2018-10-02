# gstream-rtsp-work
Work samples done for Gstreamer RTSP server 

I am new to gstreamer, so i decided to create samples that suits my objectives/requirement

Initial requirement is as follows :
 1. gstreamer server with the following capabilities
     a. Stream a video using RTSP 
     b. The video file has to be from a file source
     c. The video file has to be encoded to h264 and streamed to RTSP 
     
 2. To achieve the above requrirement I have browser some of the links related to gstreamer
   Stackoverflow regarding rtsp streaming:
    https://stackoverflow.com/questions/42498991/how-to-use-gst-launch-to-streaming-mp4-audio-and-video-via-rtp-and-play-it
    https://stackoverflow.com/questions/35843178/streaming-audio-and-video-in-sync-for-mp4-container-using-gstreamer-framework

Pipeline that is created :
  
  // Audio and Video
  //	"filesrc location=\"%s\" ! qtdemux name=d "
	//	"d. ! queue ! rtph264pay pt=96 name=pay0 "
	//	"d. ! queue ! rtpmp4apay pt=97 name=pay1 " ")", argv[1]);

    // Using x264enc 
		// "filesrc location=\"%s\" ! qtdemux"		
		// " ! decodebin ! x264enc ! rtph264pay pt=96 name=pay0 )", argv[1]);
    
   
  // Only Video
	// "filesrc location=\"%s\" ! qtdemux name=d "		
	// "d.video_0  ! decodebin ! x264enc ! rtph264pay pt=96 name=pay0 )", argv[1]);
