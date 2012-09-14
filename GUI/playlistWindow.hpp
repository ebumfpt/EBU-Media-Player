#ifndef CPP_PLAYLISTWINDOW_CLASS_H
#define CPP_PLAYLISTWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file playlistWindow.hpp
 * \brief playlist window specifications
 * \details This class is used to load the playlist of EBU Player and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 28 august 2012
 * \copyright GNU GPLv3
 *
 */

#ifdef _WIN32
    #define NEWLINE "\r\n"
#elif defined macintosh // OS 9
    #define NEWLINE "\r"
#else
    #define NEWLINE "\n" // Mac OS X uses \n
#endif


#include <gtkmm.h>
#include <gdkmm.h>
#include <gdk/gdkx.h>
#include <iostream>
#include <string>
#include <fstream>

class playlistWindow : public Gtk::Window
{
public:
	playlistWindow(BaseObjectType* cobject, 
							const Glib::RefPtr<Gtk::Builder>& refGlade);
	~playlistWindow();

	/*void clear_list_data();
	Gtk::TreeView * get_tree_view();
	bool is_playlist_empty();
	void play_new_list();
	void restore_playlist();
	void save_playlist();
	void set_list_data(Artist *);
	void set_list_data(Album *);
	void set_list_data(Song *);
	void show_all_songs();*/
	void setWindowsPosition(int x, int y);
	void addtoplaylistfromplayer(std::string file, std::string path);
	void on_openplaylist_clicked(void);
	std::string getCurrentTrack(void);
	int getCurrentPos(void);
	void randomTrack(bool nextprevious);
	void nextTrack(void);
	void previousTrack(void);
	void loopCurrentTrack(void);
	void shufflePlaylist(void);
	void playing(void);
	bool isPlaying(void);
	bool isShuffled(void);
	bool isLooping(void);
	int getSize(void);
protected:
	/**
	 * @fn void refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the required player widgets
   * @brief This method loads all required glade's references to connect the player widgets. If you add a new widget to the media player into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following widgets have been referenced :\n
	 * @param[in] refGlade : This is the reference to your glade widgets - It's mandatory.
   * @return noting or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed or transfert to metadata class.
   */
	void refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade);
	/**
	 * @fn refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the buttons
   * @brief This method loads all required glade's references to connect the metadata viewer buttons. If you add a new button to the metadata viewer into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following buttons have been referenced :\n
	 * @param[in] refGlade : This is the reference to your glade player features - It's mandatory.
   * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
	void refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade);
	/**
	 * @fn void connectSignalClicked (void)
   * @brief To connect the clicked signal to events handler 
   * @brief This method connects each widgets clicked with its own signal. If you add a new clicked event, you must append its signal connection here.\n \n
	 * @brief Currently, the following clicked signals have been connected:\n
	 * @param[in] void : No params required
   * @return  nothing if all is right or an error at compilation time.
	 * @note This function and his documentation must be completed
   */
	void connectSignalClicked(void);
	
	void on_addtoplaylist_clicked(void);
	void on_removefromplaylist_clicked(void);
	void on_saveplaylist_clicked(void);
	bool on_playlist_selection_changed(GdkEventButton *event);
	bool fileExists(std::string filename);
	void writeM3U(std::string filename);
	bool isM3U(std::string str);
	int countSubstring(const std::string& str, const std::string& sub);
	std::string writeM3UPath(const std::string& filename, const std::string& path);
	std::string rewriteM3UPath (const std::string& filename, const std::string& path);
	/**
	 * @fn void initPlaylistStates(void)
   * @brief To initiliaze the playlist interface states
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void initPlaylistStates(void);
	void configureTreeview(void);
	void colorizePlaylist(void);
	Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< Glade references */

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns() { 
			add(playlistidcol);
			add(playlistnamecol); 
			add(playlistlengthcol);
			add(playlistformatcol); 
			add(playlistdateofcreationcol);
			add(playlistcellbgcolorcol);
		}

    Gtk::TreeModelColumn<int> playlistidcol;
    Gtk::TreeModelColumn<Glib::ustring> playlistnamecol;
    Gtk::TreeModelColumn<Glib::ustring> playlistlengthcol;
    Gtk::TreeModelColumn<Glib::ustring> playlistformatcol;
    Gtk::TreeModelColumn<Glib::ustring> playlistdateofcreationcol;
    Gtk::TreeModelColumn<Glib::ustring> playlistcellbgcolorcol;

  };

  ModelColumns playlistColumns;

	Gtk::TreeView * playlistTreeview;
	Glib::RefPtr<Gtk::ListStore> playlistStore;
  Glib::RefPtr<Gtk::TreeSelection> playlistTreeviewSelection;

	std::vector<std::string> pathfilestoplay;
	int tracktoplay;
	int trackselected;
	bool loopTrack;
	bool randomPlaylist;
	bool playingState;

	Gtk::Button * addtoplaylistButton;
	Gtk::Button * removefromplaylistButton;
	Gtk::Button * saveplaylistButton;
	Gtk::Button * openplaylistButton;

	
};

#endif /*CPP_PLAYLISTWINDOW_CLASS_H*/
