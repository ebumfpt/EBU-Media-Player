/*!
 * \project_name EBU Player
 * \file playlistWindow.cpp
 * \brief playlist window function
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 *
 * This software is published in LGPLv3.0
 *
 */

#include "playlistWindow.hpp"

void playlistWindow::refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* instantiate widgets for metadata window*/
	refGlade->get_widget("playlistTreeview",playlistTreeview);
}

void playlistWindow::refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* metadata control buttons */
	refGlade->get_widget("addtoplaylist", addtoplaylistButton);	
	refGlade->get_widget("removefromplaylist", removefromplaylistButton);	
	refGlade->get_widget("openplaylist", openplaylistButton);	
	refGlade->get_widget("saveplaylist", saveplaylistButton);	
}

void playlistWindow::connectSignalClicked(void) {
	addtoplaylistButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playlistWindow::on_addtoplaylist_clicked));
	removefromplaylistButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playlistWindow::on_removefromplaylist_clicked));
	saveplaylistButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playlistWindow::on_saveplaylist_clicked));
	openplaylistButton->signal_clicked().connect(sigc::mem_fun(*this,
                      &playlistWindow::on_openplaylist_clicked));
	
}

void playlistWindow::configureTreeview(void) {
	// create the data model
	playlistStore = Gtk::ListStore::create(playlistColumns);
	// set the data model
  playlistTreeview->set_model(playlistStore);
	//Add the TreeView's view columns
  playlistTreeview->append_column("", playlistColumns.playlistidcol);
  playlistTreeview->append_column("Name", playlistColumns.playlistnamecol);
  playlistTreeview->append_column("Length", playlistColumns.playlistlengthcol);
  playlistTreeview->append_column("Format", playlistColumns.playlistformatcol);
  playlistTreeview->append_column("Date of creation", playlistColumns.playlistdateofcreationcol);
  playlistTreeview->append_column("Cell background color", playlistColumns.playlistcellbgcolorcol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 1; i < 5; i++)
  { 
		pColumn = playlistTreeview->get_column(i);
		Gtk::CellRenderer* cellRenderer = playlistTreeview->get_column_cell_renderer(i);
		pColumn->add_attribute(cellRenderer->property_cell_background(), playlistColumns.playlistcellbgcolorcol);
    pColumn->set_reorderable();
		switch(i) {
			case 1 : pColumn->set_sort_column(playlistColumns.playlistnamecol); break;
			case 2 : pColumn->set_sort_column(playlistColumns.playlistlengthcol); break;
			case 3 : pColumn->set_sort_column(playlistColumns.playlistformatcol); break;
			case 4 : pColumn->set_sort_column(playlistColumns.playlistdateofcreationcol); break;
		}
  }
	pColumn = playlistTreeview->get_column(5);
	pColumn->set_visible(false);
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	playlistTreeview->set_headers_visible(true);
	// Allow the column title buttons to be clicked
	playlistTreeview->set_headers_clickable(true);
	// Resize all columns to their optimal width. 
	playlistTreeview->columns_autosize();
	// all element are reordorable
	playlistTreeview->set_reorderable();
  playlistTreeview->set_rules_hint();
	// grab the treeview selection
	playlistTreeviewSelection = playlistTreeview->get_selection();
	// connect signal to dectect when tree selection change
	playlistTreeview->signal_button_press_event().connect(sigc::mem_fun(*this,
              &playlistWindow::on_playlist_selection_changed),false);

	initPlaylistStates();		

}

playlistWindow::playlistWindow(BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(refGlade); // " " " " buttons
	refGladeWidgets(refGlade);// " " " " widgets
	connectSignalClicked(); // clicked buttons
	configureTreeview(); // configure the treeview
}

playlistWindow::~playlistWindow() {}

void playlistWindow::setWindowsPosition(int x, int y) {
	// move this window until position x,y
	move(x,y);
}

std::string playlistWindow::getCurrentTrack(void) {
	return pathfilestoplay.at(tracktoplay);
}


int playlistWindow::getCurrentPos(void) {
	return tracktoplay;
}

void playlistWindow::nextTrack(void) {
	tracktoplay = (tracktoplay+1)%getSize();
	colorizePlaylist();
}

void playlistWindow::randomTrack(bool nextprevious) {
	int previoustracktoplay = tracktoplay; 
  /* initialize random seed: */
  srand ( time(NULL) );
	tracktoplay = rand()%getSize();
	if (nextprevious and previoustracktoplay == tracktoplay) {
		tracktoplay = (tracktoplay+1)%getSize();
	}
	colorizePlaylist();
}

void playlistWindow::previousTrack(void) {
	tracktoplay = (tracktoplay>0)?(tracktoplay-1):getSize()-1;
	colorizePlaylist();
}

void playlistWindow::loopCurrentTrack(void) {
	loopTrack = !loopTrack;
}

void playlistWindow::shufflePlaylist(void) {
	randomPlaylist = !randomPlaylist;
}

bool playlistWindow::isShuffled(void){
	return randomPlaylist;
}

bool playlistWindow::isLooping(void){
	return loopTrack;
}


void playlistWindow::playing(void){
	playingState = !playingState;
}

bool playlistWindow::isPlaying(void){
	return playingState;
}

int playlistWindow::getSize(void) {
	return pathfilestoplay.size();
}

bool playlistWindow::on_playlist_selection_changed(GdkEventButton *event) {
	
 	if (event->type == GDK_2BUTTON_PRESS && event->button ==1) {
		std::cout<<"I double clicked"<<std::endl;
		// grab the list selection
		Gtk::TreeModel::iterator iter = playlistTreeviewSelection->get_selected();
		removefromplaylistButton->set_sensitive(playlistTreeviewSelection->count_selected_rows() > 0 );
		//play the selection
		tracktoplay = (*iter)[playlistColumns.playlistidcol]-1;
		trackselected = tracktoplay;
		set_title((*iter)[playlistColumns.playlistnamecol]);
		colorizePlaylist();
		return true;
	} else if (event->type == GDK_BUTTON_PRESS && event->button ==1) {
		std::cout<<"I selected"<<std::endl;
		// grab the list selection
		Gtk::TreeModel::iterator iter = playlistTreeviewSelection->get_selected();
		removefromplaylistButton->set_sensitive(playlistTreeviewSelection->count_selected_rows() > 0 );
		trackselected = (*iter)[playlistColumns.playlistidcol]-1;
		colorizePlaylist();
		return true;
	} else {
		std::cout<<"Selected :"<< trackselected<<std::endl;
		removefromplaylistButton->set_sensitive(playlistTreeviewSelection->count_selected_rows() > 0 );
		return true;
	}
	return false;
}

void playlistWindow::addtoplaylistfromplayer(std::string file, std::string path) {
	// create path to the file to play
		std::string filename = "file://"+path;
		// store the path at the begin of playlist
		pathfilestoplay.insert(pathfilestoplay.begin(),filename);
		//pathfilestoplay.push_back(filename);
  	//Fill the TreeView's model from the begin
  	Gtk::TreeModel::Row row = *(playlistStore->prepend());
		// track position
		row[playlistColumns.playlistidcol] = getSize();
		// find last dot in filename "name"
		int lastindex = file.find_last_of(".");
		// copy what comes before last dot	--> file name
		std::string name = file.substr (0,lastindex);
  	row[playlistColumns.playlistnamecol] = name;
  	row[playlistColumns.playlistlengthcol] = "unknown";
		// find last dot in filename
		lastindex = filename.find_last_of(".");
		// copy what comes after last dot	--> file extension
		std::string fileextension = filename.substr (lastindex+1,filename.size()-(lastindex+1));
		//lowerize the character
		for(int i = 0; fileextension[i] != '\0'; i++){
			fileextension[i] = tolower(fileextension[i]);
		}
  	row[playlistColumns.playlistformatcol] = fileextension;
  	row[playlistColumns.playlistdateofcreationcol] = "unknown";
		for (int i=0; i<getSize();i++) {
			row[playlistColumns.playlistidcol]=i+1;
			row++;
		}
		colorizePlaylist();
		
}

void playlistWindow::on_addtoplaylist_clicked(void) {
	  static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Select a media to add in the playlist", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  
  chooser.set_select_multiple(true);
  chooser.set_current_folder(working_dir);
  
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
    working_dir = chooser.get_current_folder();
		int countSelected = chooser.get_filenames().size();
		for (int i=0;i<countSelected;i++) {
			// create path to the file to play
			std::string filename = "file://"+chooser.get_filenames().at(i);
			// store the path
			pathfilestoplay.push_back(filename);
			//Fill the TreeView's model from the end
			Gtk::TreeModel::Row row = *(playlistStore->append());

			// track position
			row[playlistColumns.playlistidcol] = getSize();
			// find last dot in filename "name"
			int lastindex = Glib::filename_display_basename(chooser.get_filenames().at(i)).find_last_of(".");
			// copy what comes before last dot	--> file name
			std::string name = Glib::filename_display_basename(chooser.get_filenames().at(i)).substr (0,lastindex);
			row[playlistColumns.playlistnamecol] = name;
			row[playlistColumns.playlistlengthcol] = "unknown";
			// find last dot in filename
			lastindex = filename.find_last_of(".");
			// copy what comes after last dot	--> file extension
			std::string fileextension = filename.substr (lastindex+1,filename.size()-(lastindex+1));
			//lowerize the character
			for(int j = 0; fileextension[j] != '\0'; j++){
				fileextension[j] = tolower(fileextension[j]);
			}
			row[playlistColumns.playlistformatcol] = fileextension;
			row[playlistColumns.playlistdateofcreationcol] = "unknown";
		}
  }
	colorizePlaylist();
}

void playlistWindow::on_removefromplaylist_clicked(void){
	// grab the list selection
	if (trackselected != -1) {
		for (Gtk::TreeNodeChildren::iterator row = playlistStore->children().begin(); row != playlistStore->children().end();row++) {
			if ((*row)[playlistColumns.playlistidcol] == trackselected) {
				playlistStore->erase(*row);				
				if (tracktoplay == getSize()) {
					tracktoplay = (tracktoplay>0)?(tracktoplay-1):0;
				}
				trackselected = -1;
				break;
			}
		}
	} 
	colorizePlaylist();
}

bool playlistWindow::fileExists(std::string filename) {
  std::ifstream ifile(filename);
  return ifile;
}

int playlistWindow::countSubstring(const std::string& str, const std::string& sub) {
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str.find(sub); offset != std::string::npos;
	 offset = str.find(sub, offset + sub.length()))
    {
        ++count;
    }
    return count;
}

std::string playlistWindow::writeM3UPath(const std::string& filename, const std::string& path) {
		//output relative path
		std::string relativepath;
		// locate beginning of the string
    size_t nSearchOffset = 0;
		// locate the first carriage return
    size_t nSubstringOffset = path.find ("/", 0);
		// locate end of the string
    size_t nFinalOffset = 0;
		bool bFinalOffset = false;
		// if not EOF, loop
    while (nSubstringOffset != std::string::npos)
    {		
				// if next carriage return different of next beginning
				if (nSubstringOffset != nSearchOffset) {
					std::string strf = filename.substr(nSearchOffset,nSubstringOffset-nSearchOffset);
					std::string strp = path.substr(nSearchOffset,nSubstringOffset-nSearchOffset);
					if (strf != strp) {
						if (!bFinalOffset) {
							bFinalOffset=true;
							nFinalOffset=nSearchOffset;
						}
						relativepath = relativepath+"../";
					}
				}
        // Make the 'find' function search the next character onwards
        nSearchOffset = nSubstringOffset + 1;
        nSubstringOffset = path.find ("/", nSearchOffset);
		}
	return relativepath+filename.substr(nFinalOffset,strlen(filename.c_str())-nFinalOffset);
}

std::string playlistWindow::rewriteM3UPath (const std::string& filename, const std::string& path) {
	std::string relativepath = path+"/"+filename;
	size_t previousSearchOffset, nSearchOffset = 0;
  size_t nSubstringOffset = relativepath.find ("../", 0);
	while (nSearchOffset < nSubstringOffset-1) {		
		previousSearchOffset = nSearchOffset+1;
		nSearchOffset = relativepath.find ("/", previousSearchOffset);
	}
	while (nSubstringOffset != std::string::npos) {		
		relativepath = relativepath.replace(previousSearchOffset, nSubstringOffset-previousSearchOffset+3, "");
    // Make the 'find' function search the next character onwards
    nSearchOffset = nSubstringOffset + 1;
    nSubstringOffset = relativepath.find ("../", nSearchOffset);
		nSearchOffset = 0;
		while (nSearchOffset < nSubstringOffset-1) {		
			previousSearchOffset = nSearchOffset+1;
			nSearchOffset = relativepath.find ("/", previousSearchOffset);
		}
	}
	return "file://"+relativepath;
}

void playlistWindow::writeM3U(std::string filename) {
	std::ofstream myfile;
  myfile.open (filename);
	size_t index;
	for (int i=0;i<getSize();i++) {;
			index = 0;
		 /* Locate the substring to replace. */
     	index =  pathfilestoplay.at(i).find("file://", index);
			std::string pathtmp = pathfilestoplay.at(i);
     /* Make the replacement. */
		myfile <<writeM3UPath(pathtmp.replace(index, 7, ""), filename)<< NEWLINE;
	}
  myfile.close();
}

void playlistWindow::on_saveplaylist_clicked(void){
	static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Save your playlist", Gtk::FILE_CHOOSER_ACTION_SAVE);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
  
  chooser.set_current_folder(working_dir);
  
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
		std::string filename = (isM3U(chooser.get_filename()))
				? chooser.get_filename()
				: chooser.get_filename()+".m3u";

		if (fileExists(filename)) {
			Gtk::MessageDialog dialog(*this, "File already exists",
          false /* use_markup */, Gtk::MESSAGE_QUESTION,
          Gtk::BUTTONS_OK_CANCEL);
			dialog.set_secondary_text("A file named \""+Glib::filename_display_basename(chooser.get_filename())+"\"alreay exists." );
			dialog.run();

			int result = dialog.run();

			//Handle the response:
			if (result == Gtk::RESPONSE_OK) {
				writeM3U(filename);
			}
		} else {
			writeM3U(filename);
		}
	}
}

bool playlistWindow::isM3U(std::string str) {
	std::string m3u = "m3u";
	// find last "."
	int lastindex = str.find_last_of(".");
	// copy what comes after last dot
	 
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
//loop through each character and make it lower-case. stop when you hit '\0'.
	for(int i = 0; str2[i] != '\0'; i++){
		str2[i] = tolower(str2[i]);
	}
	
  if (m3u.compare(str2) == 0) {
		    return true;
	} else {
			return false;
	}
}

void playlistWindow::on_openplaylist_clicked(void){
	static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Select a media to add in the playlist", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  chooser.set_current_folder(working_dir);
  
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
    working_dir = chooser.get_current_folder();
		if (isM3U(chooser.get_filename()) ) {
		// open the playlist source file
		std::ifstream playlistFile(chooser.get_filename());
		// read M3Usource
		std::string playlistContent(
							std::istreambuf_iterator<char>(playlistFile), //start
							(std::istreambuf_iterator<char>()) //eof
							);
	
		// locate beginning of the string
    size_t nSearchOffset = 0;
		// locate the first carriage return
    size_t nSubstringOffset = playlistContent.find (NEWLINE, 0);
		// if not EOF, loop
    while (nSubstringOffset != std::string::npos)
    {		
				// if next carriage return different of next beginning
				if (nSubstringOffset != nSearchOffset) {
					std::string strline = playlistContent.substr(nSearchOffset,nSubstringOffset-nSearchOffset);
					std::string str = strline.substr(strline.find_last_of("/")+1,strlen(strline.c_str())-strline.find_last_of("/") );					
					// store the path
					pathfilestoplay.push_back(rewriteM3UPath(strline,working_dir));
					//Fill the TreeView's model from the end
					Gtk::TreeModel::Row row = *(playlistStore->append());
					// track position
					row[playlistColumns.playlistidcol] = getSize();
					// find last dot in filename "name"
					int lastindex = str.find_last_of(".");
					// copy what comes before last dot	--> file name
					std::string name = str.substr (0,lastindex);
					row[playlistColumns.playlistnamecol] = name;
					row[playlistColumns.playlistlengthcol] = "unknown";
					// find last dot in filename
					lastindex = str.find_last_of(".");
					// copy what comes after last dot	--> file extension
					std::string fileextension = str.substr (lastindex+1,str.size()-(lastindex+1));
					//lowerize the character
					for(int i = 0; fileextension[i] != '\0'; i++){
						fileextension[i] = tolower(fileextension[i]);
					}
					row[playlistColumns.playlistformatcol] = fileextension;
					row[playlistColumns.playlistdateofcreationcol] = "unknown";
	
				}
        // Make the 'find' function search the next character onwards
        nSearchOffset = nSubstringOffset + 1;
        nSubstringOffset = playlistContent.find (NEWLINE, nSearchOffset);
    }

  } else {
		Gtk::MessageDialog dialog(*this, "File format error");
		dialog.set_secondary_text("You can only open m3u playlist files.");
		dialog.run();
		}
	}
	colorizePlaylist();
}

void playlistWindow::colorizePlaylist(void) {
	for (Gtk::TreeNodeChildren::iterator row = playlistStore->children().begin(); row != playlistStore->children().end();row++) {
		(*row)[playlistColumns.playlistcellbgcolorcol] = 
			((*row)[playlistColumns.playlistidcol] == tracktoplay+1)
				?
					"#2244FF"
				:
					((*row)[playlistColumns.playlistidcol]%2==0)
						?
							"#F9F9F9"
						:
							"#EEEEEE"
				;
	}
}
void playlistWindow::initPlaylistStates(void) {
// button states to add a file to the playlist
	addtoplaylistButton->set_visible(true);
	addtoplaylistButton->set_sensitive(true);
	addtoplaylistButton->show();
// button states to remove a file from the playlist
	removefromplaylistButton->set_visible(true);
	removefromplaylistButton->set_sensitive(false);
	removefromplaylistButton->show();
// button states to save a playlist
	saveplaylistButton->set_visible(true);
	saveplaylistButton->set_sensitive(false);
	saveplaylistButton->show();
// button states to open a playlist
	openplaylistButton->set_visible(true);
	openplaylistButton->set_sensitive(true);
	openplaylistButton->show();
// Initial states
	tracktoplay = 0;
	trackselected = -1;
	playingState = false;
	loopTrack = false;
	randomPlaylist = false;
}

