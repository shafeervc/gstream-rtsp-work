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

3. To start only mp4 file format support is analyzed and implemented.

Issues faced :
1. Visual studio setup GTsreamer library : new to Visual studio, so it took me some time to set up the environment for Gstreamer library usage.
    Some links followed :
        Linking the gstreamer libraries :
	https://stackoverflow.com/questions/21265582/gstreamer-c-on-visual-studio-2010/50756228
		(1) C/C++ -> Additional Include Directories -> define your include paths such as
		C:\gstreamer\1.0\x86\lib\glib-2.0\include;C:\gstreamer\1.0\x86\include\gstreamer-1.0;C:\gstreamer\1.0\x86\include\glib-			2.0\;C:\gstreamer\1.0\x86\include\glib-2.0\glib;%(AdditionalIncludeDirectories)

		(2) Linker -> General -> Adding Library Directories -> write your lib directory path such as
		C:\gstreamer\1.0\x86\lib;%(AdditionalLibraryDirectories)

		(3) Linker -> Input -> Additional Dependencies -> Write your .lib files you want to use such as
		gstrtspserver-1.0.lib;gobject-2.0.lib;glib-2.0.lib;gstreamer-1.0.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

Issue faced for gstrptserver is because of not including this .dll
Path : C:\gstreamer\1.0\x86\lib
dll --> gstrtspserver-1.0.lib


Plans :
1. To read multiple video files and stream it as a queue
2. GUI for user inputs and configuration
3. multiple video format support
