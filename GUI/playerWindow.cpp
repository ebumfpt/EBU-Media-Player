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
#include "metadataWindow.hpp"

//
void playerWindow::refGladeMenu(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* menu _File */
	refGlade->get_widget("openMenuItem", openMenuItem);
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
	refGlade->get_widget("progress_scale", progress_scale);
	/* set some progress_scale properties */
	progress_scale->set_range(0, 1);
}

void playerWindow::refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	// button for control player
	refGlade->get_widget("play", play);
	refGlade->get_widget("pause", pause);
	refGlade->get_widget("stop", stop);
	refGlade->get_widget("rewind", rewind);
	refGlade->get_widget("forward", forward);
	refGlade->get_widget("fullscreen", fullscreen);
	// button to open a window
	refGlade->get_widget("metadata_show", metadata_show);
	refGlade->get_widget("playlist_show", playlist_show);
}

void playerWindow::multipleWindows(Gtk::Window * win) {
	metadataWindow = win;
}
void playerWindow::refGladeWindows(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* instantiate the metadata window */	
	refGlade->get_widget("PLAYLIST-EBU-MXF-ENCODER",playlistWindow);
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
	metadata_show->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_metadata_show_clicked));
	playlist_show->signal_clicked().connect(sigc::mem_fun(*this,
                      &playerWindow::on_playlist_show_clicked));
}


void playerWindow::connectSignalActivate(void) {
	openMenuItem->signal_activate().connect(sigc::mem_fun(*this,
                      &playerWindow::on_openMenu_activate));
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
	refGladeWindows(refGlade); // " " " " windows
	// connect the signals
	connectSignalClicked(); // clicked buttons
	connectSignalActivate(); // activated menu items
	connectSignalChangeValue(); // update the value of a widget
	connectSignalRealize(); // map the gstreamer playbin into the gdk.window
	
	
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


  show_all_children();

	std::cout<<"helloworld : "<<playbin->property_uri()<<std::endl;
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
    case Gst::MESSAGE_EOS:
    {
      on_stop_clicked();
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

    videoDrawingArea->set_size_request(width_value, height_value);

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
  playbin->set_state(Gst::STATE_PLAYING);
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
  playbin->set_state(Gst::STATE_PAUSED);
}
 
void playerWindow::on_stop_clicked()
{
  //Change the UI appropriately:
  progress_scale->set_sensitive(false);
  play->set_sensitive();
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
  playbin->set_state(Gst::STATE_NULL);

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


void playerWindow::on_openMenu_activate()
{
  static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Select a media file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  
  chooser.set_current_folder(working_dir);
  
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
    working_dir = chooser.get_current_folder();

    // Set uri property on the playbin.
    playbin->property_uri() = chooser.get_uri();

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
			//metadataWindow->extractMetadata( Glib::filename_display_basename(chooser.get_filename()));
		}

    play->set_sensitive();
    display_label_progress(0, 0);	
//		std::string allo;
//		playbin->get_property("uri",allo);
//		std::cout<<"helloworld : "<<allo<<std::endl;
  }
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

void playerWindow::display_label_progress(gint64 pos, gint64 len)
{
  std::ostringstream locationStream (std::ostringstream::out);
  std::ostringstream durationStream (std::ostringstream::out);

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
  progress_label->set_text(locationStream.str() + " / " + durationStream.str());
}

playerWindow::~playerWindow()
{
  playbin->get_bus()->remove_watch(watch_id);
	  // Clean up nicely:
  playbin->set_state(Gst::STATE_NULL);
}

void playerWindow::on_metadata_show_clicked()
{

  //(!metadata_box->get_visible()) ? this->get_size(width, height) : this->resize(width,height);
	//	metadata_box->set_visible(!metadata_box->get_visible());
	metadataWindow->set_visible(!metadataWindow->get_visible());
}

void playerWindow::on_playlist_show_clicked()
{

  //(!metadata_box->get_visible()) ? this->get_size(width, height) : this->resize(width,height);
	//	metadata_box->set_visible(!metadata_box->get_visible());
	playlistWindow->set_visible(!playlistWindow->get_visible());
}
void playerWindow::on_quitMenu_activate()
{

  hide();
}

void playerWindow::on_EBUCoreMenu_activate() {
		system(
    "//home/dossantos/Dev/TreeView/Documentation/tech3293v1_3.pdf"
);
                                    
}

void playerWindow::on_metadataMenuItem_activate() {
	metadataWindow->set_visible(!metadataWindow->get_visible());
}


void playerWindow::on_playlistMenuItem_activate() {
	playlistWindow->set_visible(!playlistWindow->get_visible());
}

