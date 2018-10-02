/* GStreamer
* Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#include "stdafx.h"
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "554"

static char *port = (char *)DEFAULT_RTSP_PORT;

static char *server_addr = "192.168.128.47";

static GOptionEntry entries[] = {
	{ "port", 'p', 0, G_OPTION_ARG_STRING, &port,
	"Port to listen on (default: " DEFAULT_RTSP_PORT ")", "PORT" },
	{ NULL }
};

/* called when a stream has received an RTCP packet from the client */
static void
on_ssrc_active(GObject * session, GObject * source, GstRTSPMedia * media)
{
	GstStructure *stats;
	GST_INFO("source %p in session %p is active", source, session);
	g_object_get(source, "stats", &stats, NULL);
	if (stats) {
		gchar *sstr;
		sstr = gst_structure_to_string(stats);
		g_print("structure: %s\n", sstr);
		g_free(sstr);
		gst_structure_free(stats);
	}
}

static void
on_sender_ssrc_active(GObject * session, GObject * source,
GstRTSPMedia * media)
{
	GstStructure *stats;
	GST_INFO("source %p in session %p is active", source, session);
	g_object_get(source, "stats", &stats, NULL);
	if (stats) {
		gchar *sstr;
		sstr = gst_structure_to_string(stats);
		g_print("Sender stats:\nstructure: %s\n", sstr);
		g_free(sstr);
		gst_structure_free(stats);
	}
}

/* signal callback when the media is prepared for streaming. We can get the
* session manager for each of the streams and connect to some signals. */
static void
media_prepared_cb(GstRTSPMedia * media)
{
	guint i, n_streams;
	n_streams = gst_rtsp_media_n_streams(media);
	GST_INFO("media %p is prepared and has %u streams", media, n_streams);
	for (i = 0; i < n_streams; i++) {
		GstRTSPStream *stream;
		GObject *session;
		stream = gst_rtsp_media_get_stream(media, i);
		if (stream == NULL)
			continue;
		session = gst_rtsp_stream_get_rtpsession(stream);
		GST_INFO("watching session %p on stream %u", session, i);
		g_signal_connect(session, "on-ssrc-active",
			(GCallback)on_ssrc_active, media);
		g_signal_connect(session, "on-sender-ssrc-active",
			(GCallback)on_sender_ssrc_active, media);
	}
}

static void
media_configure_cb(GstRTSPMediaFactory * factory, GstRTSPMedia * media)
{
	/* connect our prepared signal so that we can see when this media is
	* prepared for streaming */
	g_signal_connect(media, "prepared", (GCallback)media_prepared_cb, factory);
}

int
main(int argc, char *argv[]) {
	GMainLoop *loop;
	GstRTSPServer *server;
	GstRTSPMountPoints *mounts;
	GstRTSPMediaFactory *factory;
	GOptionContext *optctx;
	GError *error = NULL;
	gchar *str, *filesrc;



	optctx = g_option_context_new("<filename.mp4> - Test RTSP Server, MP4");
	g_option_context_add_main_entries(optctx, entries, NULL);
	g_option_context_add_group(optctx, gst_init_get_option_group());
	if (!g_option_context_parse(optctx, &argc, &argv, &error)) {
		g_printerr("Error parsing options: %s\n", error->message);
		g_option_context_free(optctx);
		g_clear_error(&error);
		return -1;
	}
	
	g_print("Streaming Mp4 video %s in RSTP port  %s",argv[1],port);

	if (argc < 2) {
		g_print("%s\n", g_option_context_get_help(optctx, TRUE, NULL));
		return 1;
	}
	g_option_context_free(optctx);

	loop = g_main_loop_new(NULL, FALSE);

	/* create a server instance */
	server = gst_rtsp_server_new();
	gst_rtsp_server_set_address(server, server_addr);


	g_object_set(server, "service", port, NULL);

	/* get the mount points for this server, every server has a default object
	* that be used to map uri mount points to media factories */
	mounts = gst_rtsp_server_get_mount_points(server);

	//str = g_strdup_printf("( "
	//	"filesrc location=\"%s\" ! qtdemux name=d "
	//	"d. ! queue ! rtph264pay pt=96 name=pay0 "
	//	"d. ! queue ! rtpmp4apay pt=97 name=pay1 " ")", argv[1]);

	str = g_strdup_printf("( "
		"filesrc location=\"%s\" ! qtdemux"		
		" ! decodebin ! x264enc ! rtph264pay pt=96 name=pay0 )", argv[1]);
	    

	// working 
	// "filesrc location=\"%s\" ! qtdemux name=d "		
	// "d.video_0  ! decodebin ! x264enc ! rtph264pay pt=96 name=pay0 )", argv[1]);

	//! queue
	//"d.video_0 ! queue ! rtpmp4apay pt=97 name=pay1 )", argv[1]);
	// "d.video_0 ! queue ! h264parse ! avdec_h264 ! videoconvert ! x264enc ! rtph264pay !)", argv[1]);

	
	//str = g_strdup_printf("( ""filesrc location=\"%s\" ! qtdemux "
	//	"! h264parse ! rtph264pay pt=96 name=pay0 "
	//	")", argv[1]);

	/* make a media factory for a test stream. The default media factory can use
	* gst-launch syntax to create pipelines.
	* any launch line works as long as it contains elements named pay%d. Each
	* element with pay%d names will be a stream */
	factory = gst_rtsp_media_factory_new();
	
	/* *************************** To Remove ***************************************************************** */
		GstElement *pipeline;
		GstElement *videosrc, *demuxer, *video_encode, *video_convert, *rtppay, *sink;
		GstBus *bus;

		pipeline = gst_pipeline_new("rtsp_pipeline");
	
		videosrc = gst_element_factory_make("filesrc", "filesrc");
		demuxer = gst_element_factory_make("qtdemux", "qtdemux");
		video_convert = gst_element_factory_make("decodebin", "decodebin");
		video_encode = gst_element_factory_make("x264enc", "x264enc");
		rtppay = gst_element_factory_make("rtph264pay", "pay0");
		sink = gst_element_factory_make("udpsink", "sink");

		g_object_set(videosrc, "location", argv[1], NULL);		
		g_object_set(G_OBJECT(rtppay), "pt", 96, "config-interval", 1, NULL);
		g_object_set(G_OBJECT(sink), "host", server_addr, "port", 5000, "sync", FALSE, "async", FALSE, NULL);
		 // filesrc location=\"%s\" ! qtdemux name=d "
		 // "d.video_0  ! decodebin ! x264enc ! rtph264pay pt=96 name=pay0 )", argv[1]); */
		gst_bin_add_many(GST_BIN(pipeline), videosrc, demuxer, video_convert, video_encode, rtppay,sink, NULL);

		if (gst_element_link_many(videosrc, demuxer, video_convert, video_encode, rtppay,sink, NULL) != TRUE) {
			g_printerr("Elements could not be linked.\n");
			gst_object_unref(pipeline);
			//return -1;
		}
	/* *************************** To Remove ***************************************************************** */


	gst_rtsp_media_factory_set_launch(factory, str);
	g_signal_connect(factory, "media-configure", (GCallback)media_configure_cb,
		factory);

	g_free(str);

	/* attach the test factory to the /test url */
	gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

	/* don't need the ref to the mapper anymore */
	g_object_unref(mounts);

	/* attach the server to the default maincontext */
	gst_rtsp_server_attach(server, NULL);

	/* start serving */
	g_print("stream ready at rtsp://%s:%s/test\n", server_addr,port);
	g_main_loop_run(loop);

	g_print("Ending Stream");

	return 0;
}