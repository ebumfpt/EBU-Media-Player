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

#include <gtkmm.h>
#include <gdkmm.h>
#include <gstreamermm.h>
#include <gdk/gdkx.h>
#include <iostream>
#include <string.h>
#include <iterator>
#include <sstream>
#include <iomanip>

#include "metadataWindow.hpp"
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
	/**
	 * @fn void multipleWindows(Gtk::Window* win)
   * @brief To include more gtk windows to the player
   * @brief \n \n
	 * @param[in] win : This is the metadataWindow - It's mandatory.
   * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void multipleWindows(Gtk::Window* win);		

	/* new */


protected:
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
	 * @fn void refGladeWindows(const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the Gtk Windows
   * @brief This method loads all required glade's references to connect the multiple Gtk windows. If you add a new Gtk Window into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following windows have been referenced :\n
	 * \li \c \b metadataWindow We use this window to display any XML metatada 
	 * \li \c \b playlistWindow We use this window to display a playlist\n \n
	 * @param[in] refGlade : This is the reference to your glade windows - It's mandatory.
   * @return noting or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void refGladeWindows(const Glib::RefPtr<Gtk::Builder>& refGlade);
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
	 * @fn bool on_progress_scale_value_changed(Gtk::ScrollType type, double value)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] type : Gtk::ScrollType
	 * @param[in] value : double
   * @return a bool if all is right or an error at compilation time.
   */
  bool on_progress_scale_value_changed(Gtk::ScrollType type, double value);
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

	bool isMXF(std::string str);

protected:
	// Glade reference
  Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< Glade references */
	// Gtk Windows
	// Gtk widgets
  Gtk::DrawingArea* videoDrawingArea;/*!< Gtk drawing area */
  Gtk::Label* progress_label;/*!< Timecode label*/
  Gtk::Scale* progress_scale;/*!< Timecode progress scale*/
  Gtk::Button* play; /*!< start the playback button*/
  Gtk::Button* pause;/*!< pause the playback button*/
  Gtk::Button* stop;/*!< stop the playback button*/
  Gtk::Button* rewind;/*!< rewind the playback button*/
  Gtk::Button* forward;/*!< forward the playback button*/
  Gtk::Button* fullscreen;/*!< Fullscreen switch button*/
	Gtk::Button* metadata_show;/*!< Metadata viewer button*/
	Gtk::Button* playlist_show;/*!< Playlist viewer button*/
	// Menu bar
	// Gtk MenuItems	
	// _File
  Gtk::MenuItem* openMenuItem;/*!< open menu item*/
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
	Gtk::Window * metadataWindow; /*!< metadata Window */
	Gtk::Window * playlistWindow; /*!< playlist Window */
};

#endif
