/*!
 * \project_name EBU Player
 * \file playerWindow.cpp
 * \brief player window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 *
 * This software is published in LGPLv3.0
 *
 */


#include "playerWindow.hpp"

//
void playerWindow::refGladeMenu(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* menubar */
	refGlade->get_widget("menubar",menubar);
	/* menu _File */
	refGlade->get_widget("openMenuItem", openMenuItem);
	refGlade->get_widget("openPlaylistMenuItem",openPlaylistMenuItem);
	refGlade->get_widget("quitMenuItem", quitMenuItem);
	/* menu _View */
	refGlade->get_widget("metadataMenuItem",metadataMenuItem);
	refGlade->get_widget("playlistMenuItem",playlistMenuItem);
	/* menu _Tools */

	/* menu _Settings */

	/* menu _Help */
	refGlade->get_widget("EBUCoreMenuItem",EBUCoreMenuItem);
}

void playerWindow::refGladePlayerFeatures(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* instantiate the display interface */
	refGlade->get_widget("videoDrawingArea", videoDrawingArea);
	/* instantiate the timecode features*/
  refGlade->get_widget("progress_label", progress_label);
  refGlade->get_widget("frame_label", frame_label);
  refGlade->get_widget("progress_total_label", progress_total_label);
  refGlade->get_widget("frame_total_label", frame_total_label);
	refGlade->get_widget("progress_scale", progress_scale);
	/* set some progress_scale properties */
	progress_scale->set_range(0, 1);
	/* instantiate the volume features*/
	refGlade->get_widget("volume_scale", volume_scale);
	refGlade->get_widget("volume_scale_button", volume_scale_button);
	volume_scale->set_range(0.0, 100.0);
	volume_scale->set_value(80.0);
	volume_scale_button->set_value(80.0);
	volume_scale->set_fill_level(80.0);
	playbin->property_volume() = 0.8;
	/* player border */
	refGlade->get_widget("borderleft", borderleft);
	refGlade->get_widget("borderright", borderright);
	/* gtk box for timecode/timeline */
	refGlade->get_widget("timeline_box", timeline_box);
	/* gtk box for player controls */
	refGlade->get_widget("player_control_box", player_control_box);
	/* gtk box for status bar */
	refGlade->get_widget("status_box", status_box);
	
}

void playerWindow::refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	// button for control player
	refGlade->get_widget("play", play);
	refGlade->get_widget("pause", pause);
	refGlade->get_widget("stop", stop);
	refGlade->get_widget("rewind", rewind);
	refGlade->get_widget("forward", forward);
	refGlade->get_widget("previous", previous);
	refGlade->get_widget("next", next);
	refGlade->get_widget("fullscreen", fullscreenButton);
	refGlade->get_widget("unfullscreen", unfullscreenButton);
	refGlade->get_widget("loop", loopplaylist);
	refGlade->get_widget("shuffle", shuffleplaylist);
	// button to open a window
	refGlade->get_widget("trackoption_show", trackoption_show);
	refGlade->get_widget("metadata_show", metadata_show);
	refGlade->get_widget("playlist_show", playlist_show);
}

void playerWindow::loadWindows(void) {
	m_refGlade->get_widget_derived("METADATAVIEW-EBU-MXF-ENCODER", winMetadata);
	m_refGlade->get_widget_derived("PLAYLIST-EBU-MXF-ENCODER", winPlaylist);
}

void playerWindow::connectSignalClicked(void) {
  play->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_play_clicked));
  pause->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_pause_clicked));
  stop->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_stop_clicked));
  rewind->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_rewind_clicked));
  forward->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_forward_clicked));
  previous->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_previous_clicked));
  next->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_next_clicked));
	metadata_show->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_metadata_show_clicked));
	playlist_show->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_playlist_show_clicked));
	fullscreenButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_fullscreen_clicked));
	unfullscreenButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_fullscreen_clicked));
	// connect all signals for timecode	
	progress_label->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_timecode_pressed));
	frame_label->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_timecode_pressed));
	progress_total_label->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_timecode_pressed));
	frame_total_label->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_timecode_pressed));
	// connect playlist signals		
	loopplaylist->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_loop_clicked));
	shuffleplaylist->signal_clicked().connect(sigc::mem_fun(*this,
	                    &playerWindow::on_shuffle_clicked));
}


void playerWindow::connectSignalActivate(void) {
	openMenuItem->signal_activate().connect(sigc::mem_fun(*this,
                      &playerWindow::on_openMenu_activate));
	openPlaylistMenuItem->signal_activate().connect(sigc::mem_fun(*this,
                      &playerWindow::on_openPlaylistMenuItem_activate));
	quitMenuItem->signal_activate().connect(sigc::mem_fun(*this,
                      &playerWindow::on_quitMenu_activate));
	EBUCoreMenuItem->signal_activate().connect(sigc::mem_fun(*this,
											&playerWindow::on_EBUCoreMenu_activate));
	metadataMenuItem->signal_activate().connect(sigc::mem_fun(*this,
											&playerWindow::on_metadataMenuItem_activate));
	playlistMenuItem->signal_activate().connect(sigc::mem_fun(*this,
											&playerWindow::on_playlistMenuItem_activate));
}

void playerWindow::connectSignalChangeValue(void) {
	progress_scale->signal_change_value().connect(
    sigc::mem_fun(*this, &playerWindow::on_progress_scale_value_changed) );
	
	
	volume_scale->signal_change_value().connect(
    sigc::mem_fun(*this, &playerWindow::on_volume_scale_value_changed) );
}

void playerWindow::connectSignalRealize(void) {
  videoDrawingArea->signal_realize().connect(sigc::mem_fun(*this,
                      &playerWindow::on_videoDrawingArea_realize));
}

playerWindow::playerWindow(BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
	m_refGlade(refGlade)
{

	Gst::init();
  // Create the elements:

  // playbin2 plays any media type, choosing an appropriate set of elements
  // and linking them together.
  playbin = Gst::PlayBin2::create("playbin");

  if(!playbin)
  {
    std::cerr << "The playbin could not be created." << std::endl;
    //return 1;
  }

  // Create a video sink where video (if any) will be drawn:
	video_sink = Gst::XImageSink::create("ximagesink");
  if(!video_sink)
  {
    std::cerr << "The ximagesink could not be created." << std::endl;
    //return 1;
  }

  // Set the playbin's video-sink property so that our video sink is used
  // for video display:
  playbin->property_video_sink() = video_sink;
	

	// link the glade references
	refGladeMenu(refGlade); // instantiate glade references for menu
	refGladePlayerFeatures(refGlade);
	refGladeButton(refGlade); // " " " " buttons
	//m_refGlade->get_widget_derived("METADATAVIEW-EBU-MXF-ENCODER", metadataWindow);
	loadWindows(); // instantiate all player windows
	// connect the signals
	connectSignalClicked(); // clicked buttons
	connectSignalActivate(); // activated menu items
	connectSignalChangeValue(); // update the value of a widget
	connectSignalRealize(); // map the gstreamer playbin into the gdk.window
	initUIStates(); // init the ui states
	videoDrawingArea->override_background_color(Gdk::RGBA("black")); 	
	/* new */



	videoDrawingArea->show();
  // Get the bus from the pipeline:
  Glib::RefPtr<Gst::Bus> bus = playbin->get_bus();

  // Enable synchronous message emission to set up video (if any) at the
  // exact appropriate time
  bus->enable_sync_message_emission();

  // Connect to bus's synchronous message signal (this is done so that
  // video_area can be set up for drawing at the exact appropriate time):
  bus->signal_sync_message().connect(
    sigc::mem_fun(*this, &playerWindow::on_bus_message_sync));

  // Add a bus watch to receive messages from the pipeline's bus:
  watch_id = bus->add_watch(
    sigc::mem_fun(*this, &playerWindow::on_bus_message) );

	

}

void playerWindow::on_videoDrawingArea_realize()
{
  // When the video area (the drawing area) is realized, Get its X Window
  // ID and save it for when the Gst::XOverlay is ready to accept an ID in
  // which to draw the video.
  x_window_id = GDK_WINDOW_XID(videoDrawingArea->get_window()->gobj());
}

// This function is used to receive asynchronous messages from mainPipeline's
// bus, specifically to prepare the Gst::XOverlay to draw inside the window
// in which we want it to draw to.
void playerWindow::on_bus_message_sync(
    const Glib::RefPtr<Gst::Message>& message)
{
  // ignore anything but 'prepare-xwindow-id' element messages
  if(message->get_message_type() != Gst::MESSAGE_ELEMENT)
    return;

  if(!message->get_structure().has_name("prepare-xwindow-id"))
     return;

  Glib::RefPtr<Gst::Element> element =
      Glib::RefPtr<Gst::Element>::cast_dynamic(message->get_source());

  Glib::RefPtr< Gst::ElementInterfaced<Gst::XOverlay> > xoverlay =
      Gst::Interface::cast <Gst::XOverlay>(element);

  if(xoverlay)
  {
    xoverlay->set_xwindow_id(x_window_id);
  }
}

// This function is used to receive asynchronous messages from playbin's bus
bool playerWindow::on_bus_message(const Glib::RefPtr<Gst::Bus>& /* bus */,
          const Glib::RefPtr<Gst::Message>& message)
{
  switch(message->get_message_type())
  {
		// end of stream message
    case Gst::MESSAGE_EOS:
    {
			// if it's not the last file in the playlist, play the next file
			// else stop the player
			int playlistpos = winPlaylist->getCurrentPos();
			int playlistsize = winPlaylist->getSize();
			if (playlistpos < playlistsize-1) {
				if (winPlaylist->isShuffled()) {
					winPlaylist->randomTrack(false);
				} else {
					winPlaylist->nextTrack();
				}
				// set the current track from playlist
				setPlayedFile(winPlaylist->getCurrentTrack());
				on_play_clicked();
				winPlaylist->playing();
				return true;
			} else {
				if (playlistsize>0) {
					if (winPlaylist->isShuffled()) {
						winPlaylist->randomTrack(false);
					} else {
						winPlaylist->nextTrack();
					}
					// set the current track from playlist
					setPlayedFile(winPlaylist->getCurrentTrack());
				}				
				if (winPlaylist->isShuffled()) {
					on_play_clicked();
				} else {
					on_stop_clicked();
					winPlaylist->playing();
				}
			}
      break;
    }
    case Gst::MESSAGE_ERROR:
    {
      Glib::RefPtr<Gst::MessageError> msgError = Glib::RefPtr<Gst::MessageError>::cast_dynamic(message);
      if(msgError)
      {
        Glib::Error err;
        err = msgError->parse();
        std::cerr << "Error: " << err.what() << std::endl;
      }
      else
        std::cerr << "Error." << std::endl;

      on_stop_clicked();
      break;
    }
    default:
    {
      //std::cout << "debug: on_bus_message: unhandled message=" << G_OBJECT_TYPE_NAME(message->gobj()) << std::endl;
    }
  }

  return true;
}

bool playerWindow::on_video_pad_got_buffer(const Glib::RefPtr<Gst::Pad>& pad,
    const Glib::RefPtr<Gst::MiniObject>& data)
{
  Glib::RefPtr<Gst::Buffer> buffer = Glib::RefPtr<Gst::Buffer>::cast_dynamic(data);

  if(buffer)
  {
    int width_value;
    int height_value;

    Glib::RefPtr<Gst::Caps> caps = buffer->get_caps();

    const Gst::Structure structure = caps->get_structure(0);
    if(structure)
    {
      structure.get_field("width", width_value);
      structure.get_field("height", height_value);
    }

    videoDrawingArea->set_size_request(-1, 159);

    // Resize to minimum when first playing by making size 
    // smallest then resizing according to video new size:
    resize(1, 1);
    check_resize();
  }

  pad->remove_buffer_probe(pad_probe_id);
  pad_probe_id = 0; // Clear probe id to indicate that it has been removed

  return true; // Keep buffer in pipeline (do not throw away)
}

void playerWindow::on_play_clicked()
{
  //Change the UI appropriately:
  progress_scale->set_sensitive();
  play->set_sensitive(false);
  pause->set_sensitive();
  stop->set_sensitive();
  rewind->set_sensitive();
  forward->set_sensitive();
  //open->set_sensitive(false);

  play->hide();
  pause->show();

  // Call on_timeout function at a 200ms
  // interval to regularly update the position of the stream
  timeout_connection = Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &playerWindow::on_timeout), 200);

  // set the pipeline to play mode:
 // playbin->set_state(Gst::STATE_PLAYING);
	setState(Gst::STATE_PLAYING);
}

void playerWindow::on_loop_clicked(void) {
	winPlaylist->loopCurrentTrack();
}

void playerWindow::on_shuffle_clicked(void) {
	winPlaylist->shufflePlaylist();
}
 
void playerWindow::on_pause_clicked()
{
  play->set_sensitive();
  pause->set_sensitive(false);

  pause->hide();
  play->show();

  // Disconnect the progress callback:
  timeout_connection.disconnect();
  
  // Set the pipeline to pause mode:
//  playbin->set_state(Gst::STATE_PAUSED);
	setState(Gst::STATE_PAUSED);
}
 
void playerWindow::on_stop_clicked()
{
  //Change the UI appropriately:
  progress_scale->set_sensitive(false);
  play->set_sensitive((winPlaylist->getSize()>0)? true : false);
  pause->set_sensitive(false);
  stop->set_sensitive(false);
  rewind->set_sensitive(false);
  forward->set_sensitive(false);
  //open->set_sensitive();
  pause->hide();
  play->show();

  // Disconnect the progress signal handler:
  timeout_connection.disconnect();

  // Set the pipeline to inactive mode:
	setState(Gst::STATE_NULL);
  //playbin->set_state(Gst::STATE_NULL);
	
  // Reset the display:
  display_label_progress(0, duration);
  progress_scale->set_value(0);

  // Remove video sink pad buffer probe if after playing, probe id is
  // not zero (means probe was not removed because media had no video and
  // video_pad_got_buffer method never got a chance to remove probe)

  if(pad_probe_id != 0)
  {
    video_sink->get_static_pad("sink")->remove_buffer_probe(pad_probe_id);
    pad_probe_id  = 0;
  }
}

bool playerWindow::on_volume_scale_value_changed(Gtk::ScrollType /*type_not_used */, double value) {
	//setVolume(volume);
	playbin->property_volume() = value/100.0;
	volume_scale->set_fill_level(value);	
	volume_scale_button->set_value(value);
	return true;
}

bool playerWindow::on_progress_scale_value_changed(Gtk::ScrollType /* type_not_used */, double value)
{
  const gint64 newPos = gint64(value * duration);

  if(playbin->seek(Gst::FORMAT_TIME, Gst::SEEK_FLAG_FLUSH, newPos))
  {
    display_label_progress(newPos, duration);
    return true;
  }
  else
  {
    std::cerr << "Could not seek." << std::endl;
    return false;
  }
}

void playerWindow::on_rewind_clicked()
{
  static const gint64 skipAmount = Gst::SECOND * 2;

  gint64 pos = 0;
  Gst::Format fmt = Gst::FORMAT_TIME;

  if(playbin->query_position(fmt, pos))
  {
    gint64 newPos = (pos > skipAmount) ? (pos - skipAmount) : 0;

    if(playbin->seek(Gst::FORMAT_TIME, Gst::SEEK_FLAG_FLUSH, newPos))
    {
      progress_scale->set_value(double(newPos) / duration);
      display_label_progress(newPos, duration);
    }
    else
      std::cerr << "Could not seek." << std::endl;
  }
}

void playerWindow::on_forward_clicked()
{
  static const gint64 skipAmount = Gst::SECOND * 3;

  Gst::Format fmt = Gst::FORMAT_TIME;

  Glib::RefPtr<Gst::Query> query = Gst::QueryPosition::create(fmt);

  if(playbin->query(query))
  {
    Glib::RefPtr<Gst::QueryPosition> posQuery =
      Glib::RefPtr<Gst::QueryPosition>::cast_dynamic(query);

    gint64 pos = posQuery->parse();

    gint64 newPos = ((pos + skipAmount) < duration) ? (pos + skipAmount) :
      duration;

    Glib::RefPtr<Gst::Event> event = Gst::EventSeek::create(1.0, fmt,
        Gst::SEEK_FLAG_FLUSH, Gst::SEEK_TYPE_SET, newPos,
        Gst::SEEK_TYPE_NONE, -1);

    Glib::RefPtr<Gst::EventSeek> seekEvent =
      Glib::RefPtr<Gst::EventSeek>::cast_dynamic(event);

    if(playbin->send_event(seekEvent))
    {
      progress_scale->set_value(double(newPos) / duration);
      display_label_progress(newPos, duration);
    }
    else
      std::cerr << "Could not seek." << std::endl;
  }
}
void playerWindow::on_previous_clicked()
{
	on_stop_clicked();
	if (winPlaylist->isShuffled()) {
		winPlaylist->randomTrack(true);
	} else {
		winPlaylist->previousTrack();
	}
	setPlayedFile(winPlaylist->getCurrentTrack());
	on_play_clicked();
}

void playerWindow::on_next_clicked()
{
	on_stop_clicked();
	if (winPlaylist->isShuffled()) {
		winPlaylist->randomTrack(true);
	} else {
		winPlaylist->nextTrack();
	}
	setPlayedFile(winPlaylist->getCurrentTrack());
	on_play_clicked();
}

void playerWindow::on_fullscreen_clicked(void) {
	// hide all useless stuff for fullscreen mode
	menubar->set_visible(!menubar->get_visible());
	playlist_show->set_visible(!playlist_show->get_visible());
	metadata_show->set_visible(!metadata_show->get_visible());
	unfullscreenButton->set_visible(!unfullscreenButton->get_visible());
	fullscreenButton->set_visible(!fullscreenButton->get_visible());
	borderleft->set_visible(!borderleft->get_visible());
	borderright->set_visible(!borderright->get_visible());
	set_decorated(!get_decorated());
	(get_decorated()) ? unmaximize () : maximize ();
	(get_decorated()) ? unfullscreen() : fullscreen();
	fullscreenstate = !fullscreenstate;
	// Call timeout function at a 3000ms
  // interval to hide player controls when it is in fullscreen mode
	// disconnect signal when no more in fullscreen mode
	// connect/disconnect motion detector to show player controls
  if (fullscreenstate) { 
		// timer
		timeout_fullscreen =  Glib::signal_timeout().connect(sigc::mem_fun(*this, &playerWindow::on_timeout_fullscreen), 3000);
		mouse_motion_signal_drawingArea = videoDrawingArea->signal_motion_notify_event().connect(sigc::mem_fun(*this, &playerWindow::on_mousemove));
		// get the focus else the motion detection doesn't occurs when a movie is playing
		videoDrawingArea->set_can_focus(!videoDrawingArea->get_can_focus());
		// triggered the next player controls hidding tryout
		nextmotiondetection = time(NULL)+3;
	} else {
		timeout_fullscreen.disconnect() ;
		mouse_motion_signal_drawingArea.disconnect();
		videoDrawingArea->set_can_focus(!videoDrawingArea->get_can_focus());
	}

std::cout<<"I'd like to switch in fullscreen mode"<<std::endl;
}

bool playerWindow::isMXF(std::string str) {
	std::string mxf = "mxf";
	// find last "."
	int lastindex = str.find_last_of(".");
	// copy what comes after last dot
	 
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
//loop through each character and make it lower-case. stop when you hit '\0'.
	for(int i = 0; str2[i] != '\0'; i++){
		str2[i] = tolower(str2[i]);
	}
	
  if (mxf.compare(str2) == 0) {
		    return true;
	} else {
			return false;
	}
}

void playerWindow::filterFiles(Gtk::FileChooserDialog * chooser) {
	// filter for video file
	Glib::RefPtr<Gtk::FileFilter> filterVideo = Gtk::FileFilter::create();
	filterVideo->set_name("Video");
	filterVideo->add_mime_type("video/*");
	// RAW Video types
	filterVideo->add_mime_type("video/x-raw-yuv");//YUV or YCbCr video
	filterVideo->add_mime_type("video/x-raw-rgb");//RGB video
	// Encoded Video types
	filterVideo->add_mime_type("video/x-3ivx"); //3ivx video
	filterVideo->add_mime_type("video/x-divx"); //divx video
	filterVideo->add_mime_type("video/x-dv"); //digital video
	filterVideo->add_mime_type("video/x-ffv");//ffmpeg video
	filterVideo->add_mime_type("video/x-h263");//H.263 video
	filterVideo->add_mime_type("video/x-h264");//H.264 video
	filterVideo->add_mime_type("video/x-huffyuv");//Huffyuv video
	filterVideo->add_mime_type("video/x-indeo");//Indeo video
	filterVideo->add_mime_type("video/x-intel-h263");//H.263 video Intel variant
	filterVideo->add_mime_type("video/x-intel-h263");//Motion JPEG video
	filterVideo->add_mime_type("video/x-jpeg");//H.263 video Intel variant
	filterVideo->add_mime_type("video/mpeg");//MPEG video
	filterVideo->add_mime_type("video/x-msmpeg");//MPEG4 Microsoft variant
	filterVideo->add_mime_type("video/x-msvideocodec");//Microsoft Video 1
	filterVideo->add_mime_type("video/x-pn-realvideo");//Realmedia video
	filterVideo->add_mime_type("video/x-rle");//RLE Animation video
	filterVideo->add_mime_type("video/x-svq");//Sorenson video
	filterVideo->add_mime_type("video/x-tarkin");//Tarkin video
	filterVideo->add_mime_type("video/x-rle");//Theora video
	filterVideo->add_mime_type("video/x-theora");//RLE Animation video
	filterVideo->add_mime_type("video/x-vp3");//VP3 video
	filterVideo->add_mime_type("video/x-wmv");//Windows Media Video
	filterVideo->add_mime_type("video/x-xvid");//Xvid video
	//Multimedia/Video Container types
	filterVideo->add_mime_type("video/x-ms-asf");//Advanced Streaming Format
	filterVideo->add_mime_type("video/x-msvideo");//AVI Container
	filterVideo->add_mime_type("video/x-matroska");//Matroska (MKV)
	filterVideo->add_mime_type("video/ogg");//Ogg container
	filterVideo->add_mime_type("video/quicktime");//Apple container
	filterVideo->add_mime_type("video/vnd.rn-realmedia");//Realmedia container
	filterVideo->add_mime_type("application/mxf");//MXF Container
	chooser->add_filter(filterVideo);

	// filter for audio file 
	Glib::RefPtr<Gtk::FileFilter> filterAudio = Gtk::FileFilter::create();
	filterAudio->set_name("Audio");
	filterAudio->add_mime_type("audio/*");
	filterAudio->add_pattern("*.mp3");
	filterAudio->add_pattern("*.ogg");
	filterAudio->add_pattern("*.wav");
	filterAudio->add_pattern("*.wma");
	chooser->add_filter(filterAudio);
	// no filter -> all files
	Glib::RefPtr<Gtk::FileFilter> filterAll = Gtk::FileFilter::create();
	filterAll->set_name("All files");
	filterAll->add_pattern("*");
	chooser->add_filter(filterAll);

}

void playerWindow::on_openMenu_activate()
{
  static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Select a media file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  
  chooser.set_current_folder(working_dir);
  
	filterFiles(&chooser);
	
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
    working_dir = chooser.get_current_folder();

		// set the playlist element
		winPlaylist->addtoplaylistfromplayer(Glib::filename_display_basename(chooser.get_filename()),chooser.get_filename());
		// if not playing
		if (getStateString(getState()) != "PLAYING" ) {
			// identify the current track from playlist
			playbin->property_uri() = winPlaylist->getCurrentTrack();
		} else {
			on_stop_clicked();
			winPlaylist->playing();
			playbin->property_uri() = winPlaylist->getCurrentTrack();
		}

    // Resize video_area and window to minimum when opening a file
    //videoDrawingArea->set_size_request(0, 0);
    //resize(1, 1);

    // Add buffer probe to video sink pad when file is opened which will
    // be removed after first buffer is received in on_video_pad_got_buffer
    // method (if there's video).  When first buffer arrives, video
    // size can be extracted.  If there's no video, probe will be
    // removed when media stops in on_button_stop method
    pad_probe_id = video_sink->get_static_pad("sink")->add_buffer_probe(
      sigc::mem_fun(*this, &playerWindow::on_video_pad_got_buffer));

    set_title( Glib::filename_display_basename(chooser.get_filename()) );
		
		if (isMXF( Glib::filename_display_basename(chooser.get_filename()) )){
			winMetadata->extractMetadata(chooser.get_filename());
		}
	//streamInformation(chooser.get_filename());
	

    play->set_sensitive();
    display_label_progress(0, 0);
  }
}

void playerWindow::setPlayedFile(std::string path) {
	  // Set the pipeline to inactive mode:
	setState(Gst::STATE_NULL);
	timeout_connection.disconnect();
	playbin->property_uri() = path;
	  // Call on_timeout function at a 200ms
  // interval to regularly update the position of the stream
  timeout_connection = Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &playerWindow::on_timeout), 200);
	
}
bool playerWindow::on_timeout()
{
  Gst::Format fmt = Gst::FORMAT_TIME;
  gint64 pos = 0;

  if(playbin->query_position(fmt, pos)
    && playbin->query_duration(fmt, duration))
  {
    progress_scale->set_value(double(pos) / duration);
		progress_scale->set_fill_level(double(pos) / duration);
    display_label_progress(pos, duration);
  }

   return true;
}

bool playerWindow::on_timeout_playlist() {
	int playlistsize = winPlaylist->getSize();
	int playlistpos = winPlaylist->getCurrentPos();
	play->set_sensitive((playlistsize>0) ? true : false);
	next->set_sensitive((playlistsize>1) ? true : false);
	previous->set_sensitive((playlistsize>1) ? true : false);
	if (playlistsize>1) { shuffleplaylist->show(); } else {shuffleplaylist->hide(); }
	std::cout<< "Pos :: "<< winPlaylist->getCurrentPos() << " --- Size :: "<< winPlaylist->getSize() << std::endl;
	if (playlistsize>0 and !fileLoaded) {
		loopplaylist->show();
		fileLoaded = !fileLoaded;
		setPlayedFile(winPlaylist->getCurrentTrack());
	} else if (playlistsize>0 and fileLoaded and playlistsize != playlistpos
						and playbin->property_uri() != winPlaylist->getCurrentTrack()) {
		on_stop_clicked();
		setPlayedFile(winPlaylist->getCurrentTrack());
		if ((winPlaylist->isLooping() or winPlaylist->isShuffled()) and winPlaylist->isPlaying()) { 
			on_play_clicked();
		}
	} else if (playlistsize == 0 and fileLoaded) {
		loopplaylist->hide();
		fileLoaded = !fileLoaded;		
		setPlayedFile("");
		// Reset display and controls
		display_label_progress(0, 0);
		progress_scale->set_value(0);
		progress_scale->set_fill_level(0);
		play->set_sensitive(false);
		pause->set_sensitive(false);
		play->set_visible(true);
		pause->set_visible(false);
		stop->set_sensitive(false);
		rewind->set_sensitive(false);
		forward->set_sensitive(false);
		shuffleplaylist->set_sensitive(false);
		loopplaylist->set_sensitive(false);
	}
	return true;
}


bool playerWindow::on_timeout_fullscreen()
{
  if (timeline_box->get_visible() and player_control_box->get_visible() and nextmotiondetection <= time(NULL)) {
		timeline_box->set_visible(false);
		player_control_box->set_visible(false);
		status_box->set_visible(false);
	}
	return true;
}

/*void playerWindow::streamInformation(std::string filename) {
	std::string uri = "file://"+filename;
	C_Func_MediaInfo ((char *)uri.c_str());
}*/

void playerWindow::on_timecode_pressed(void) {
		progress_label->set_visible(!progress_label->get_visible());
		progress_label->set_sensitive(!progress_label->get_sensitive());
		progress_total_label->set_visible(!progress_total_label->get_visible());
		progress_total_label->set_sensitive(!progress_total_label->get_sensitive());
		frame_label->set_visible(!frame_label->get_visible());
		frame_label->set_sensitive(!frame_label->get_sensitive());
		frame_total_label->set_visible(!frame_total_label->get_visible());
		frame_total_label->set_sensitive(!frame_total_label->get_sensitive());
		timecodeStatus = !timecodeStatus;
}

void playerWindow::display_label_progress(gint64 pos, gint64 len)
{
	
	std::ostringstream locationStream (std::ostringstream::out);
	std::ostringstream durationStream (std::ostringstream::out);
	if (!timecodeStatus) {
		locationStream << std::right << std::setfill('0') << 
		  std::setw(3) << Gst::get_hours(pos) << ":" <<
		  std::setw(2) << Gst::get_minutes(pos) << ":" <<
		  std::setw(2) << Gst::get_seconds(pos);/*<< "." <<
		  std::setw(9) << std::left << Gst::get_fractional_seconds(pos);*/

		durationStream << std::right << std::setfill('0') <<
		  std::setw(3) << Gst::get_hours(len) << ":" <<
		  std::setw(2) << Gst::get_minutes(len) << ":" <<
		  std::setw(2) << Gst::get_seconds(len); /*<< "." <<
		  std::setw(9) << std::left << Gst::get_fractional_seconds(len);
	*/
		progress_label->set_label(locationStream.str());
		progress_total_label->set_label(durationStream.str());
	} else {
		locationStream << std::right << std::setfill('x')  <<pos;
		durationStream << std::right << std::setfill('x')  <<len;
		frame_label->set_label(locationStream.str());
		frame_total_label->set_label(durationStream.str());
	}
}


playerWindow::~playerWindow()
{
  playbin->get_bus()->remove_watch(watch_id);
	  // Clean up nicely:
	setState(Gst::STATE_NULL);
  //playbin->set_state(Gst::STATE_NULL);
}

void playerWindow::on_metadata_show_clicked()
{
	int rootx;int rooty;
	get_position(rootx,rooty);
	int width;int height;
	get_size(width,height);
	winMetadata->setWindowsPosition(rootx+width+3,rooty+16);
	winMetadata->set_visible(!winMetadata->get_visible());
}

void playerWindow::on_playlist_show_clicked()
{
	int rootx;int rooty;
	get_position(rootx,rooty);
	int width;int height;
	get_size(width,height);
	winPlaylist->setWindowsPosition(rootx,rooty+height+34);
	winPlaylist->set_visible(!winPlaylist->get_visible());
}
void playerWindow::on_quitMenu_activate()
{

  hide();
}

void playerWindow::on_openPlaylistMenuItem_activate(void) {
	winPlaylist->on_openplaylist_clicked();
}

void playerWindow::on_EBUCoreMenu_activate() {
		system(
    "//home/dossantos/Dev/TreeView/Documentation/tech3293v1_3.pdf"
);
                                    
}

void playerWindow::on_metadataMenuItem_activate() {
	winMetadata->set_visible(!winMetadata->get_visible());
}


void playerWindow::on_playlistMenuItem_activate() {
	winPlaylist->set_visible(!winPlaylist->get_visible());
}

std::string playerWindow::getStateString(Gst::State state) {
    switch (state) {
    case Gst::STATE_VOID_PENDING:
        return "VOID_PENDING";
    case Gst::STATE_NULL:
        return "NULL";
    case Gst::STATE_READY:
        return "READY";
    case Gst::STATE_PAUSED:
        return "PAUSED";
    case Gst::STATE_PLAYING:
        return "PLAYING";
    default:
        return "UNKNOWN";
    }
}

Gst::State playerWindow::getState() {
	Gst::State state;
	Gst::State pending;
	Gst::StateChangeReturn ret = playbin->get_state(state, pending, 1 * Gst::SECOND);

	if (ret == Gst::STATE_CHANGE_SUCCESS) {		
		std::cout<<"State is "<<getStateString(state)<<std::endl;
	  //log::debug("State =", getStateString(state));
	  return state;
	} else if (ret == Gst::STATE_CHANGE_ASYNC) {
		std::cout<<"Query state failed, still performing change"<<std::endl;
		//log::debug("Query state failed, still performing change");
	} else {
		std::cout<<"Query state failed, still performing change"<<std::endl;
		//log::debug("Query state failed, hard failure");
	}

	return Gst::STATE_NULL;
}

bool playerWindow::setState(Gst::State state) {
	Gst::StateChangeReturn ret = playbin->set_state(state);

	if (ret == Gst::STATE_CHANGE_SUCCESS) {
		std::cout<<"state is "<<getStateString(state)<<std::endl;
		//log::debug("State =", getStateString(state));
	} else if (ret == Gst::STATE_CHANGE_ASYNC) {
		Gst::State actualState = getState();
		if (state != actualState) {
			std::cout<<"Failed to set state to "<<getStateString(state)<<". The current state is always "<<getStateString(actualState)<<std::endl;
			//log::error("Failed to set state to", getStateString(state), ". Actual state =", getStateString(actualState));
			return false;
		}
	} else if (ret == Gst::STATE_CHANGE_FAILURE || ret == Gst::STATE_CHANGE_NO_PREROLL) {
			std::cout<<"Failed to set state to "<<getStateString(state)<<". The Gstreamer core knows an hard failure "<<std::endl;
	//log::error("Failed to set state to", getStateString(state), ": hard failure");
		return false;
	}

	return true;
}

bool playerWindow::on_mousemove(GdkEventMotion* event) {
	if (!timeline_box->get_visible() and !player_control_box->get_visible()) {
		timeline_box->set_visible(true);
		player_control_box->set_visible(true);
		status_box->set_visible(true);
	}
	// time before hide the player controls
	nextmotiondetection = time(NULL)+3;
	return true; //It has been handled.
}

void playerWindow::initUIStates(void) {
	// play
	play->set_visible(true);
	play->set_sensitive(false);
	play->show();
	// pause
	pause->set_sensitive(true);
	pause->set_sensitive(true);
	pause->hide();
	// stop
	stop->set_visible(true);
	stop->set_sensitive(false);
	stop->show();
	// forward
	forward->set_visible(true);
	forward->set_sensitive(false);
	forward->show();
	// rewind
	rewind->set_visible(true);
	rewind->set_sensitive(false);
	rewind->show();
	// next
	next->set_visible(true);
	next->set_sensitive(false);
	next->show();
	// previous
	previous->set_visible(true);
	previous->set_sensitive(false);
	previous->show();
	// trackoption_show
	trackoption_show->set_visible(true);
	trackoption_show->set_sensitive(false);
	trackoption_show->show();
	// metadata_show
	metadata_show->set_visible(true);
	metadata_show->set_sensitive(true);
	metadata_show->show();
	// playlist_show
	playlist_show->set_visible(true);
	playlist_show->set_sensitive(true);
	playlist_show->show();
	// loop
	loopplaylist->set_visible(true);
	loopplaylist->set_sensitive(true);
	loopplaylist->hide();
	// shuffle
	shuffleplaylist->set_visible(true);
	shuffleplaylist->set_sensitive(true);
	shuffleplaylist->hide();
	// unfullscreen
	unfullscreenButton->set_visible(false);
	unfullscreenButton->set_sensitive(true);
	unfullscreenButton->hide();
	// fullscreen
	fullscreenButton->set_visible(true);
	fullscreenButton->set_sensitive(true);
	fullscreenButton->show();
	// fullscreenstate
	fullscreenstate = false;
	// initial position
	move(0,0);
	// timer for playlist
	timeout_playlist =  Glib::signal_timeout().connect(sigc::mem_fun(*this, &playerWindow::on_timeout_playlist), 1000);
	// file loaded
	fileLoaded = false;
	// timecode status
	timecodeStatus = false;
	// timecode label classique and in frame
	progress_label->set_visible(true);
	progress_total_label->set_visible(true);
	frame_label->set_visible(false);
	frame_total_label->set_visible(false);
	progress_label->set_sensitive(true);
	progress_total_label->set_sensitive(true);
	frame_label->set_sensitive(false);
	frame_total_label->set_sensitive(false);
	
}
