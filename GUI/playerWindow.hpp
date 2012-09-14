#ifndef CPP_PLAYERWINDOW_CLASS_H
#define CPP_PLAYERWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file playerWindow.hpp
 * \brief player window specifications
 * \details This class is used to load the EBU Player and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/scalebutton.h>
#include <gdkmm-3.0/gdkmm.h>
#include <gstreamermm.h>
#include <gdk/gdkx.h>
#include <iostream>
#include <string.h>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "mediainfo.h"

#include "metadataWindow.hpp"
#include "playlistWindow.hpp"
/*! \class playerWindow
 * \brief this class represent the player window
 *
 * This class controls all the player features and its links
 */

class playerWindow : public Gtk::Window
{
public:
  /*!*
   * @brief Class constructor
   * @brief The playerWindow class constructor instantiates a new player window and all  its features.\n\n
	 * @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget
	 * @param[in] refGlade : This is the reference to your glade design
   */
  playerWindow(BaseObjectType* cobject,
							const Glib::RefPtr<Gtk::Builder>& refGlade);
  /*!*
   * @brief Class destructor
   * @brief The playerWindow class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
   *
   */
  virtual ~playerWindow();
  void setPlayedFile(std::string path);
protected:

	/**
	 * @fn void multipleWindows(void)
   * @brief To include more gtk windows to the player
   * @brief \n \n
	 * @param[in] void : no params.
   * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void loadWindows(void);
	// link the glade references
	/**
	 * @fn void refGladeMenu (const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the menu items
	 * @brief This method loads all required glade's references to connect the menu items. If you add a new menu item to Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following menu items have been referenced :\n
	 * \li \c \b openMenuItem Open a file (in File menu)
	 * \li \c \b quitMenuItem Quit the application (in File menu)
	 * \li \c \b metadataMenuItem Open the metadata viewer (in View menu)
	 * \li \c \b playlistMenuItem Open the playlist viewer (in View menu)
	 * \li \c \b EBUCoreMenuItem Open the EBUCore documentation (in Help menu)\n \n
	 * @param[in] refGlade : This is the reference to your glade menu - It's mandatory.
	 * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void refGladeMenu(const Glib::RefPtr<Gtk::Builder>& refGlade);
	/**
	 * @fn void refGladePlayerFeatures (const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the player features
   * @brief This method loads all required glade's references to connect the player features. If you add a new player feature to Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following features have been referenced :\n
	 * \li \c \b videoDrawingArea Reference to the Gtk.Gdk.DrawingArea
	 * \li \c \b progress_label Reference to the timecode label
	 * \li \c \b progress_scale Reference to the timecode progressbar \n \n
	 * @param[in] refGlade : This is the reference to your glade player features - It's mandatory.
   * @return noting or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void refGladePlayerFeatures(const Glib::RefPtr<Gtk::Builder>& refGlade);
	/**
	 * @fn void refGladeButton (const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the buttons
   * @brief This method loads all required glade's references to connect the player buttons. If you add a new button to the media player into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following menu items have been referenced :\n
	 * \li \c \b play When clicked, this button starts the playback
	 * \li \c \b pause When clicked, this button pauses the playback
	 * \li \c \b stop When clicked, this button stops the playback
	 * \li \c \b rewind When clicked, this button rewinds the playback
	 * \li \c \b forward When clicked, this button forwards the playback
	 * \li \c \b fullscreen When clicked, this button change the screen mode into full screen mode
	 * \li \c \b metadata_show When clicked, this button shows or hide the metadata viewer
	 * \li \c \b playlist_show When clicked, this button shows or hide the playlist viewer\n \n
   * @param[in] refGlade : This is the reference to your glade buttons - It's mandatory.
   * @return noting or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade);
	/**
	 * @fn void connectSignalClicked (void)
   * @brief To connect the clicked signal to events handler
   * @brief This method connects each widgets clicked with its own signal. If you add a new clicked event, you must append its signal connection here.\n \n
	 * @brief Currently, the following clicked signals have been connected:\n
	 * \li \c \b play When connected, the play button sends a signal to start playback through a callback function.
	 * \li \c \b pause When connected, the pause button sends a signal to abort playback through a callback function.
	 * \li \c \b stop When connected, the stop button sends a signal to stop playback through a callback function.
	 * \li \c \b rewind When connected, the rewind button sends a signal to rewind the playback through a callback function.
	 * \li \c \b forward When connected, the forward button sends a signal to forward the playback through a callback function.
	 * \li \c \b metadata_show When connected, the metadata_show button sends a signal to show or hide the metadata viewer through a callback function.
	 * \li \c \b playlist_show When connected, the playlist_show button sends a signal to show or hide the playlist viewer through a callback function. \n \n
	 * @param[in] void : No params required
   * @return  nothing if all is right or an error at compilation time.
	 * @note This function and his documentation must be completed
   */
	void connectSignalClicked(void);
	/**
	 * @fn void connectSignalActivate(void)
   * @brief To connect the activated signal to events handler
   * @brief Each menu item activated is connected with its own signal. If you add a new activated event, you must append its signal connection here.\n \n
	 * @brief Currently, the following activated signals have been connected:\n
	 * \li \c \b openMenuItem When connected, the selection of option open in the menu file of the menubar sends a signal to lauch a filechooser dialog through a callback function.
	 * \li \c \b quitMenuItem When connected, the selection of option quit in the menu file of the menubar sends a signal to close the application through a callback function.
	 * \li \c \b EBUCoreMenuItem When connected, the selection of option EBU-Core in the menu documentation of the menu help of the menubar sends a signal to lauch the EBU-Core documentation through a callback function.
	 * \note Need to be completed.
	 * \li \c \b metadataMenuItem When connected, the selection of option Metadta in the menu view of the menubar sends a signal to show or hide the metadata viewer through a callback function.
	 * \li \c \b playlistMenuItem When connected, the selection of option Playlist in the menu view of the menubar sends a signal to show or hide the playlist viewer through a callback function. \n \n
	 * @param[in] void : No params required
   * @return nothing if all is right or an error at compilation time.
   */
	void connectSignalActivate(void);
	/**
	 * @fn void connectSignalChangeValue(void)
   * @brief To connect the signal of each widget where the embedded value can change to the event handler
   * @brief This method connects the signal of each widget who is able to change its own value in time or in function of any events. If you add a new changed value event, you must append its signal connection here.\n \n
	 * @brief Currently, the following changed value signals have been connected:\n
	 * \li \c \b progress_scale When connected, the progress_scale_label will be updated through a callback function. \n \n
	 * @param[in] void : No params required
   * @return nothing if all is right or an error at compilation time.
   */
	void connectSignalChangeValue(void);
	/**
	 * @fn void connectSignalRealize(void)
   * @brief To create and connect the resources associated with a widget (signal handlers)
   * @brief This method will create and connect the resources associated to any widget. Normally the realization signal should be implicit because automatically mapped at widget creation. Unfortunately, in some cases we need to instantiate first an extern object before to map it into the widget and only after we can realiaze the association.
	 * @param[in] void : No params required
   * @return nothing if all is right or an error at compilation time.
   */
	void connectSignalRealize(void);


	/**
	 * @fn void on_videoDrawingArea_realize(void)
   * @brief To create and connect the resources associated with the Gtk.Gdk.Window.
   * @brief This method will create and connect the resources associated with the Gtk.Gdk.Window. Normally the realization signal should be implicit because automatically mapped at widget creation. Unfortunately, first we need to instantiate the GStreamer Object to map it into the Gdk.Window and only after we can realiaze the association.
	 * @param[in] void : No params required
   * @return nothing if all is right or an error at compilation time.
   */
  void on_videoDrawingArea_realize(void);
		/**
	 * @fn void on_bus_message_sync(const Glib::RefPtr<Gst::Message>& message)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] message : const Glib::RefPtr<Gst::Message>&
   * @return a bool if all is right or an error at compilation time.
   */
  void on_bus_message_sync(const Glib::RefPtr<Gst::Message>& message);
		/**
	 * @fn bool on_bus_message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] message : const Glib::RefPtr<Gst::Message>&
	 * @param[in] bus : const Glib::RefPtr<Gst::Bus>&
   * @return a bool if all is right or an error at compilation time.
   */
  bool on_bus_message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);
	/**
	 * @fn on_video_pad_got_buffer(const Glib::RefPtr<Gst::Pad>& pad, const Glib::RefPtr<Gst::MiniObject>& buffer)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] pad : const Glib::RefPtr<Gst::Pad>&
	 * @param[in] buffer : const Glib::RefPtr<Gst::MiniObject>&
   * @return a bool if all is right or an error at compilation time.
   */
  bool on_video_pad_got_buffer(const Glib::RefPtr<Gst::Pad>& pad, const Glib::RefPtr<Gst::MiniObject>& buffer);
	/**
	 * @fn void on_play_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_play_clicked(void);
	void on_loop_clicked(void);
	void on_shuffle_clicked(void);
	/**
	 * @fn void on_pause_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_pause_clicked(void);
	/**
	 * @fn void on_stop_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_stop_clicked(void);
	/**
	 * @fn void on_rewind_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_rewind_clicked(void);
	/**
	 * @fn void on_forward_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_forward_clicked(void);
	/**
	 * @fn void on_previous_clicked(void)
   * @brief Action to do when the previous button is pressed
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_previous_clicked(void);
	/**
	 * @fn void on_next_clicked(void)
   * @brief Action to do when the next button is pressed
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_next_clicked(void);
	/**
	 * @fn bool on_progress_scale_value_changed(Gtk::ScrollType type, double value)
   * @brief To update the timecode progress scale
   * @brief
	 * @note needs more documentation
	 * @param[in] type : Gtk::ScrollType
	 * @param[in] value : double
   * @return a bool if all is right or an error at compilation time.
   */
  bool on_progress_scale_value_changed(Gtk::ScrollType type, double value);
	/**
	 * @fn void on_fullscreen_clicked(void)
   * @brief To switch in fullscreen mode
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_fullscreen_clicked(void);
	/**
	 * @fn void on_volume_scale_value_changed(Gtk::ScrollType type, double volume)
   * @brief To update the volume progress scale
   * @brief
	 * @note needs more documentation
	 * @param[in] type : Gtk::ScrollType
	 * @param[in] volume : int32_t
   * @return nothing if all is right or an error at compilation time.
   */
	bool on_volume_scale_value_changed(Gtk::ScrollType type, double value);
	/**
	 * @fn void display_label_progress(gint64 pos, gint64 len)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] pos : gint64
	 * @param[in] len : gint64
   * @return a bool if all is right or an error at compilation time.
   */
  void display_label_progress(gint64 pos, gint64 len);
	/**
	 * @fn bool on_timeout(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return a bool if all is right or an error at compilation time.
   */
	bool on_timeout(void);
	/**
	 * @fn bool on_timeout_playlist(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return a bool if all is right or an error at compilation time.
   */
	bool on_timeout_playlist(void);
	/**
	 * @fn bool on_timeout_fullscreen(void)
   * @brief To hide the player controls when fullscreen mode is enable
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return a bool if all is right or an error at compilation time.
   */
	bool on_timeout_fullscreen(void);
	/**
	 * @fn bool on_mousemove(GdkEventMotion* event)
   * @brief To detect mouse motion when fullscreen mode enable to show the player controls
   * @brief
	 * @note needs more documentation
	 * @param[in] event : a GdkEventMotion pointer
   * @return a bool if all is right or an error at compilation time.
   */
	bool on_mousemove(GdkEventMotion* event);
	/**
	 * @fn void on_metadata_show_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_metadata_show_clicked(void);
	/**
	 * @fn void on_playlist_show_clicked(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_playlist_show_clicked(void);
	/**
	 * @fn void on_openMenu_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
  void on_openMenu_activate(void);
	/**
	 * @fn void on_openPlaylistMenuItem_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_openPlaylistMenuItem_activate(void);
	/**
	 * @fn void on_quitMenu_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_quitMenu_activate(void);
	/**
	 * @fn void on_EBUCoreMenu_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_EBUCoreMenu_activate(void);
	/**
	 * @fn void on_metadataMenuItem_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_metadataMenuItem_activate(void);
	/**
	 * @fn void on_playlistMenuItem_activate(void)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_playlistMenuItem_activate(void);
	/**
	 * @fn void filterFiles(Gtk::FileChooserDialog chooser)
   * @brief
   * @brief
	 * @note needs more documentation
	 * @param[in] chooser : The Gtk File Chooser Dialog where is apply the file filter
   * @return nothing if all is right or an error at compilation time.
   */
	void filterFiles(Gtk::FileChooserDialog * chooser);
	/**
	 * @fn bool isMXF(std::string str)
   * @brief To identify if the selected file is a MXFfile
   * @brief
	 * @note needs more documentation
	 * @param[in] str : This standard string must is the filename of the potential mxf file
   * @return a bool if all is right or an error at compilation time.
   */
	bool isMXF(std::string str);
	/**
	 * @fn std::string getStateString(Gst::State state)
   * @brief This function return the Gstreamer current state as a standard string
   * @brief
	 * @note needs more documentation
	 * @param[in] state : Needs to be a valid Gst::State
   * @return a string if all is right or an error at compilation time.
   */
	std::string getStateString(Gst::State state);
	/**
	 * @fn Gst::State getState(void)
   * @brief To identify the current state of Gstreamer
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return a Gst::State if all is right or an error at compilation time.
   */
	Gst::State getState();
	/**
	 * @fn bool setState(Gst::State state)
   * @brief To set the current state of Gstreamer
   * @brief
	 * @note needs more documentation
	 * @param[in] state : Needs to be a valid Gst::State
   * @return a bool if all is right or an error at compilation time.
   */
	bool setState(Gst::State state);
	/**
	 * @fn void initUIStates(void)
   * @brief To initiliaze user interface states
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void initUIStates(void);
	/**
	 * @fn void streamInformation(std::string filename)
   * @brief To seek information about a stream
   * @brief
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	//void streamInformation(std::string filename);
	void on_timecode_pressed(void);

	// Glade reference
  Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< m_refGlade : Glade references */
	// Gtk Windows
	// Gtk widgets
  Gtk::DrawingArea* videoDrawingArea;/*!< videoDrawingArea : Gtk drawing area */
  Gtk::MenuBar* menubar;/*!< videoDrawingArea : Gtk drawing area */
  Gtk::Button* progress_label;/*!< progress_label Timecode label in hours:minutes:secondes*/
  Gtk::Button* progress_total_label;/*!< progress_label Timecode label in hours:minutes:secondes*/
  Gtk::Button* frame_label;/*!< frame_label Timecode label in frames*/
  Gtk::Button* frame_total_label;/*!< frame_total_label Timecode label in frames*/
  Gtk::Scale* progress_scale;/*!< progress_scale Timecode progress scale*/
  Gtk::Button* play; /*!< start the playback button*/
  Gtk::Button* pause;/*!< pause the playback button*/
  Gtk::Button* stop;/*!< stop the playback button*/
  Gtk::Button* rewind;/*!< rewind the playback button*/
  Gtk::Button* forward;/*!< forward the playback button*/
  Gtk::Button* next;/*!< next track playback button*/
  Gtk::Button* previous;/*!< previous track playback button*/
  Gtk::Button* loopplaylist;/*!< next track playback button*/
  Gtk::Button* shuffleplaylist;/*!< previous track playback button*/
  Gtk::Button* fullscreenButton;/*!< Fullscreen switch button*/
  Gtk::Button* unfullscreenButton;/*!< Fullscreen switch button*/
  Gtk::Button* trackoption_show;/*!< trackoption_show playback option (audio/subtitle/etc) button*/
  Gtk::AspectFrame* borderleft;/*!< borderleft aspect frame left */
  Gtk::AspectFrame* borderright;/*!< borderright aspect frame right*/
  Gtk::Box* timeline_box;/*!< timeline_box gtk box who contains the timeline/timecode */
  Gtk::Box* player_control_box;/*!< player_control_box gtk box who contains player controls */
  Gtk::Box* status_box;/*!< status_box gtk box who contains the status bar */
	Gtk::Button* metadata_show;/*!< Metadata viewer button*/
	Gtk::Button* playlist_show;/*!< Playlist viewer button*/
  Gtk::Scale* volume_scale;/*!< volume_scale : volume levels*/
  Gtk::ScaleButton* volume_scale_button;/*!< volume_scale_button : iconize the volume levels*/
	// Menu bar
	// Gtk MenuItems
	// _File
  Gtk::MenuItem* openMenuItem;/*!< open menu item*/
	Gtk::MenuItem* openPlaylistMenuItem; /*!< open playlist menu item */
	Gtk::MenuItem* quitMenuItem;/*!< quit menu item*/
	// _View
	Gtk::MenuItem* metadataMenuItem;/*!< open metadata menu item*/
	Gtk::MenuItem* playlistMenuItem;/*!< open playlist menu item*/
	// _Help
	Gtk::MenuItem* EBUCoreMenuItem;/*!< open ebucore menu item*/

	// Gstreamer
  Glib::RefPtr<Gst::PlayBin2> playbin;/*!< playbin needs more documentation*/
  Glib::RefPtr<Gst::XImageSink> video_sink;/*!< video_sink needs more documentation*/
  sigc::connection timeout_connection;/*!< timeout_connection needs more documentation*/
  guint watch_id;/*!< watch_id needs more documentation*/
  gint64 duration;/*!< duration needs more documentation*/
  gulong x_window_id;/*!< x_window_id needs more documentation*/
  gulong pad_probe_id;/*!< pad_probe_id needs more documentation*/

	/**/
	int width;/*!< width needs more documentation*/
	int height;/*!< height needs more documentation*/
	/**/
	// Metadata Window
	// Gtk::Window
	metadataWindow * winMetadata; /*!< metadata Window */
	playlistWindow * winPlaylist; /*!< playlist Window */

	bool fileLoaded;
	bool timecodeStatus;

	std::string * mxffilename; /*!< mxffilename Pointer to store temporarly the current played node */
	bool fullscreenstate; /*!< fullscreenstate bolean to know if fullscreen is enable */
	bool motiondetected; /*!< motiondetected bolean to know if a mouse motion occurs */
	time_t  nextmotiondetection; /*!< nextmotiondetection time before next motion detection */
  sigc::connection timeout_fullscreen;/*!< timeout_fullscreen timer to hide player controls */
	sigc::connection mouse_motion_signal_drawingArea;/*!< mouse_motion_signal_drawingArea mouse motion detection signal for the drawing area*/
	sigc::connection timeout_playlist;/*!< timeout_playlist */

};

#endif
