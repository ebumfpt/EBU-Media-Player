/*!
 * \project_name EBU Player
 * \file metadataWindow.cpp
 * \brief EBUCore Metadata window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 26 september 2012
 * This software is published in LGPLv3.0
 *
 */

// include files and libraries
#include "metadataWindow.hpp"

void metadataWindow::refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* instantiate widgets for metadata window*/
	refGlade->get_widget("expander1",Expander);
	refGlade->get_widget("viewport1",viewport1);
	refGlade->get_widget("viewport2",viewport2);
	refGlade->get_widget("SecondScrolledWindow",SecondScrolledWindow);
}

void metadataWindow::refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* metadata control buttons */
	refGlade->get_widget("importXML", importXML);	
	refGlade->get_widget("exportXML", exportXML);	
	refGlade->get_widget("modifyXML", modifyXML);	
	refGlade->get_widget("saveXML", saveXML);	
	refGlade->get_widget("XMLconformance", XMLconformance);
}

void metadataWindow::connectSignalClicked(void) {
	// Connect the button clicked signals
	importXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_importXML_clicked));
	exportXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_exportXML_clicked));
	modifyXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_modifyXML_clicked));
	saveXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_saveXML_clicked));
	XMLconformance->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_XMLconformance_clicked));
}

void metadataWindow::defineColors(void) {
	black.set("black");
	white.set("white");
	red.set("red");
	blue.set("blue");
}


// class constructor
metadataWindow::metadataWindow(BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(refGlade); // " " " " buttons
	refGladeWidgets(refGlade);// " " " " widgets
	connectSignalClicked(); // clicked buttons
	defineColors();
	set_has_resize_grip();
}

// class destructor
metadataWindow::~metadataWindow()
{
}

void metadataWindow::setWindowsPosition(int x, int y) {
	move(x,y);
	position_x = x;
	position_y = y;
}

void metadataWindow::configureNodeAttributesTreeview(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Box * nodebox) {

	Gtk::Box * AttributesTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));

	AttributesTreeviewBox->set_size_request(120, 150);

	NodeAttributesModelColumns metadataNodeAttributesColumns;

	Gtk::ScrolledWindow * metadataNodeAttributesScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeAttributesScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeAttributesTreeview = manage(new Gtk::TreeView());

//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeAttributesScrolledWindow->add(*metadataNodeAttributesTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeAttributesScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeAttributesStore;
  Glib::RefPtr<Gtk::TreeSelection> metadataNodeAttributesTreeviewSelection;

	// create the data model
	metadataNodeAttributesStore = Gtk::ListStore::create(metadataNodeAttributesColumns);
	// set the data model
  metadataNodeAttributesTreeview->set_model(metadataNodeAttributesStore);
	//Add the TreeView's view columns
  metadataNodeAttributesTreeview->append_column("", metadataNodeAttributesColumns.metadataNodeAttributeIdCol);
  metadataNodeAttributesTreeview->append_column("Attribute name", metadataNodeAttributesColumns.metadataNodeAttributeNameCol);
  metadataNodeAttributesTreeview->append_column("Attribute value", metadataNodeAttributesColumns.metadataNodeAttributeValueCol);
  metadataNodeAttributesTreeview->append_column("bgcolor", metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 1; i < 3; i++)
  { 
		pColumn = metadataNodeAttributesTreeview->get_column(i);
		Gtk::CellRenderer* cellRenderer = metadataNodeAttributesTreeview->get_column_cell_renderer(i);
		pColumn->add_attribute(cellRenderer->property_cell_background(), metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol);
    pColumn->set_reorderable();
		switch(i) {
			case 1 : pColumn->set_sort_column(metadataNodeAttributesColumns.metadataNodeAttributeNameCol); break;
			case 2 : pColumn->set_sort_column(metadataNodeAttributesColumns.metadataNodeAttributeValueCol); break;
		}
  }
	pColumn = metadataNodeAttributesTreeview->get_column(3);
	pColumn->set_visible(false);
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	metadataNodeAttributesTreeview->set_headers_visible(true);
	// Allow the column title buttons to be clicked
	metadataNodeAttributesTreeview->set_headers_clickable(true);
	// Resize all columns to their optimal width. 
	metadataNodeAttributesTreeview->columns_autosize();
	// all element are reordorable
	metadataNodeAttributesTreeview->set_reorderable();
  metadataNodeAttributesTreeview->set_rules_hint();
	// grab the treeview selection
	metadataNodeAttributesTreeviewSelection = metadataNodeAttributesTreeview->get_selection();
	// connect signal to dectect when tree selection change
	//metadataNodeAttributesTreeview->signal_button_press_event().connect(sigc::mem_fun(*this,
  //            &metadataWindow::on_metadata_selection_changed),false);


	int num= dom_attrs->getLength();
	if (num != 0) {
		for(int i=0; i<num;i++) {
			Gtk::TreeModel::Row row = *(metadataNodeAttributesStore->prepend());
			xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
			row[metadataNodeAttributesColumns.metadataNodeAttributeIdCol] = i;
			row[metadataNodeAttributesColumns.metadataNodeAttributeNameCol] = xercesc::XMLString::transcode(dom_attr->getName());
			row[metadataNodeAttributesColumns.metadataNodeAttributeValueCol] = xercesc::XMLString::transcode(dom_attr->getValue());
			row[metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = (i%2==0)?"white":"ghostwhite";
		}		
	}

	AttributesTreeviewBox->add(*metadataNodeAttributesScrolledWindow);
	
	AttributesTreeviewBox->show();
	metadataNodeAttributesScrolledWindow->show();
	metadataNodeAttributesTreeview->show();

	nodebox->add(*AttributesTreeviewBox);
}

void metadataWindow::configureNodeChildrenTreeview(xercesc::DOMElement * children, Gtk::Box * nodebox) {

	Gtk::Box * ChildrenTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	ChildrenTreeviewBox->set_size_request(120, 150);

	NodeChildrenModelColumns metadataNodeChildrenColumns;

	Gtk::ScrolledWindow * metadataNodeChildrenScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeChildrenScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeChildrenTreeview = manage(new Gtk::TreeView());


//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeChildrenScrolledWindow->add(*metadataNodeChildrenTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeChildrenScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeChildrenStore;
  Glib::RefPtr<Gtk::TreeSelection> metadataNodeChildrenTreeviewSelection;

	// create the data model
	metadataNodeChildrenStore = Gtk::ListStore::create(metadataNodeChildrenColumns);
	// set the data model
  metadataNodeChildrenTreeview->set_model(metadataNodeChildrenStore);

	//Add the TreeView's view columns
  metadataNodeChildrenTreeview->append_column("", metadataNodeChildrenColumns.metadataNodeChildrenIdCol);
  metadataNodeChildrenTreeview->append_column("Children name", metadataNodeChildrenColumns.metadataNodeChildrenNameCol);
  metadataNodeChildrenTreeview->append_column("Bgcolor", metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 1; i < 2; i++)
  { 
		pColumn = metadataNodeChildrenTreeview->get_column(i);
		Gtk::CellRenderer* cellRenderer = metadataNodeChildrenTreeview->get_column_cell_renderer(i);
		pColumn->add_attribute(cellRenderer->property_cell_background(), metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol);
    pColumn->set_reorderable();
		switch(i) {
			case 1 : pColumn->set_sort_column(metadataNodeChildrenColumns.metadataNodeChildrenNameCol); break;
		}
  }
	pColumn = metadataNodeChildrenTreeview->get_column(2);
	pColumn->set_visible(false);
	//All the items to be reordered with drag-and-drop
	// Set the visibility state of the headers. 
	metadataNodeChildrenTreeview->set_headers_visible(true);
	// Allow the column title buttons to be clicked
	metadataNodeChildrenTreeview->set_headers_clickable(true);
	// Resize all columns to their optimal width. 
	metadataNodeChildrenTreeview->columns_autosize();
	// all element are reordorable
	metadataNodeChildrenTreeview->set_reorderable();
  metadataNodeChildrenTreeview->set_rules_hint();
	// grab the treeview selection
	metadataNodeChildrenTreeviewSelection = metadataNodeChildrenTreeview->get_selection();
	// connect signal to dectect when tree selection change
	//metadataNodeChildrenTreeview->signal_button_press_event().connect(sigc::mem_fun(*this,
  //            &metadataWindow::on_metadata_selection_changed),false);
	

	int i=0;
	for (xercesc::DOMElement * nodechildren = children; nodechildren != 0	; nodechildren = nodechildren->getNextElementSibling()) {
  	Gtk::TreeModel::Row row = *(metadataNodeChildrenStore->prepend());
		// track position
		row[metadataNodeChildrenColumns.metadataNodeChildrenIdCol] = i;
		row[metadataNodeChildrenColumns.metadataNodeChildrenNameCol] = xercesc::XMLString::transcode(nodechildren->getTagName());
		row[metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol] = (i%2==0)?"white":"ghostwhite";
		i++;
	}

	ChildrenTreeviewBox->show();
	metadataNodeChildrenScrolledWindow->show();
	metadataNodeChildrenTreeview->show();

	ChildrenTreeviewBox->add(*metadataNodeChildrenScrolledWindow);

	nodebox->add(*ChildrenTreeviewBox);
}

void metadataWindow::configureNodeText(std::string nodetext, Gtk::Box * nodebox) {

	Gtk::Box * nodeTextViewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	nodeTextViewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * nodeTextViewScrolledWindow = manage(new Gtk::ScrolledWindow());
	nodeTextViewScrolledWindow->set_size_request(120, 150);

	Gtk::TextView * nodeTextView = manage (new Gtk::TextView());
	metadataTextBuffer = Gtk::TextBuffer::create();
 metadataTextBuffer->set_text(nodetext);

	 nodeTextView->set_buffer(metadataTextBuffer);
	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  nodeTextViewScrolledWindow->add(*nodeTextView);
	nodeTextView->show();
  //Only show the scrollbars when they are necessary:
  nodeTextViewScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	nodeTextViewBox->add(*nodeTextViewScrolledWindow);
	nodeTextViewScrolledWindow->show();
	nodebox->add(*nodeTextViewBox);
	nodeTextViewBox->show();
}


// configure the gtk box encapsulated in the gtk expander
void metadataWindow::configureEncapsultedBox(Gtk::Box * b) {
	// define the vertical alignment
	b->set_valign(Gtk::ALIGN_START);
	// define the left margin (treeview indentation)
	b->set_margin_left(b->get_margin_left()+16);
}

// configure the gtk expander before to encapsulate it into a Gtk::Box
void metadataWindow::configureExpander(Gtk::Expander * e) {
		// signal handler to detect what expander has been expanded
	e->property_expanded().signal_changed().connect(sigc::bind<Gtk::Expander *>(sigc::mem_fun(*this, &metadataWindow::on_openExpander_changed), e));
	e->set_margin_left(0);
	// override font color
	e->override_color (black, Gtk::STATE_FLAG_NORMAL);
	// use the pango markup language
	e->set_use_markup(true);
	// set the expander label
	e->set_label("<b>"+e->get_label()+"</b>");
}

// configure the gtk expander before to encapsulate it into a Gtk::Box
void metadataWindow::configureEventLabel(Gtk::EventBox * evLabel, Gtk::Label * label) {
	// add the label to its eventbox
			evLabel->add(*label);
			// connect mouse detection events
			evLabel->signal_enter_notify_event().connect(sigc::bind(sigc::mem_fun(*this, &metadataWindow::on_enter_label),evLabel));
			evLabel->signal_leave_notify_event().connect(sigc::bind(sigc::mem_fun(*this, &metadataWindow::on_leave_label),evLabel));
			evLabel->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &metadataWindow::on_press_label),evLabel,label));
			// initialize eventBox background color
			evLabel->override_background_color (white, Gtk::STATE_FLAG_NORMAL);
}

// configure the gtk label before to encapsulate it into a Gtk::Box
void metadataWindow::configureLabel(Gtk::Label * l) {
	l->set_justify(Gtk::JUSTIFY_LEFT);
	l->set_halign(Gtk::ALIGN_START);
	l->override_color(black, Gtk::STATE_FLAG_NORMAL);
	l->set_markup("<b>"+l->get_text()+"</b>");
	l->set_margin_left(l->get_margin_left()+18);
}

void metadataWindow::finishEncapsulation(Gtk::Box * root, Gtk::Expander * node) {
				node->show();///*************
				root->add(*node);
}

void metadataWindow::finishEncapsulation(Gtk::Box * root, Gtk::EventBox * evLabel) {
				root->add(*evLabel);
				evLabel->show_all();
}

/*void metadataWindow::createAttributesPopup(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Expander * node) {
	// create the menu
	Gtk::Menu *Menu = manage(new Gtk::Menu());
	int num= dom_attrs->getLength();
	// if the node has attributes
	if (num != 0) {
		for(int i=0; i<num;i++) {
			xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
			// append item to the menu
			Gtk::MenuItem *nameItem = manage(new Gtk::MenuItem((std::string)xercesc::XMLString::transcode(dom_attr->getName())+" : "+(std::string)xercesc::XMLString::transcode(dom_attr->getValue())));
			Menu->add(*nameItem);
		}		
	} else {
		
		Gtk::MenuItem *nameItem = manage(new Gtk::MenuItem("no attributes"));
		Menu->add(*nameItem);
	}
	Menu->show_all();
	node->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this,
	                    &metadataWindow::on_extanderpressed_pressed),Menu));
}

void metadataWindow::createAttributesPopup(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::EventBox * evLabel) {
	Gtk::Menu *Menu = manage(new Gtk::Menu());
	int num= dom_attrs->getLength();
	if (num != 0) {
		for(int i=0; i<num;i++) {
			xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
			Gtk::MenuItem *nameItem = manage(new Gtk::MenuItem((std::string)xercesc::XMLString::transcode(dom_attr->getName())+" -> "+(std::string)xercesc::XMLString::transcode(dom_attr->getValue())));
			Menu->add(*nameItem);
		}
	} else {
		Gtk::MenuItem *nameItem = manage(new Gtk::MenuItem("no attributes"));
		Menu->add(*nameItem);
	}
	Menu->show_all();
	evLabel->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this,
	                    &metadataWindow::on_extanderpressed_pressed),Menu));
}
*/

void metadataWindow::progress_cb(float progress, EBUCore::ProgressCallbackLevel level, const char *function, const char *msg_format, ...) {
	// append a newline to the msg_format string
	int len = strlen(msg_format);
	char* newline_msg_format =(char*)malloc((len+2) * sizeof(char));
	strncpy(newline_msg_format, msg_format, len); newline_msg_format[len] = 0;
	strncat(newline_msg_format, "\n", 1);

	va_list p_arg;
    va_start(p_arg, msg_format);
	switch (level) {
	case EBUCore::INFO:
		bmx::log_info(newline_msg_format, p_arg); return;
	case EBUCore::DEBUG:
	case EBUCore::TRACE:
		bmx::log_debug(newline_msg_format, p_arg); return;
	case EBUCore::ERROR:
		bmx::log_error(newline_msg_format, p_arg); return;
	case EBUCore::WARN:
		bmx::log_warn(newline_msg_format, p_arg); return;
	};
	va_end(p_arg);

	free(newline_msg_format);
}

void metadataWindow::writeMetadataBuffer(std::string filename) {
	// open
	std::ifstream file(filename);
	// read
	std::string xmlViewport1(std::istreambuf_iterator<char>(file),
	(std::istreambuf_iterator<char>()));
	// copy
	xmlViewportFirst = xmlViewport1.c_str();
}

void metadataWindow::recursiveConstructTreeView(xercesc::DOMElement * el, Gtk::Expander * seed, int depth, int idbox) {
	// create a new box to encapsulte the tree levels
	Gtk::Box * root = manage(new Gtk::VBox());

	// configure the new box
	configureEncapsultedBox(root);
	do {
		viewport1minimumwidth = (((xercesc::XMLString::stringLen(el->getTagName())*7)+(depth*16)) > viewport1minimumwidth) ? ((xercesc::XMLString::stringLen(el->getTagName())*7)+(depth*16)) : viewport1minimumwidth;
		if (el->hasChildNodes() and el->getChildElementCount() != 0) {
			// create a new expander to store the node children
			Gtk::Expander * node = manage( new Gtk::Expander(xercesc::XMLString::transcode(el->getTagName())) );
			// set a name/id to the node
			node->set_name(std::to_string(cptnode++));
			// store the DOM element adress
			elReferences.push_back(el);
			// configure the new expander
			configureExpander(node);
			// end encapsultation level
			finishEncapsulation(root,node);
			// visit the first children of the current node
			recursiveConstructTreeView(el->getFirstElementChild(), node ,depth+1,cptnode);
		} else {
			Gtk::EventBox *eventLabel = manage(new Gtk::EventBox);
			eventLabel->set_name(std::to_string(cptnode++));
			// store the DOM element adress
			elReferences.push_back(el);
			// create a new label to store the node children
			Gtk::Label * label = manage( new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())) );
			/// configure the new label
			configureLabel(label);
			/// configure the new eventbox and encapsulate the label
			configureEventLabel(eventLabel,label);
			// end encapsultation level
			finishEncapsulation(root,eventLabel);
		}
		// next node at this level
		el = el->getNextElementSibling();
	} while (el != 0);
	// add and show...
	seed->add(*root);
	root->show();
}

void metadataWindow::constructEditableNode(xercesc::DOMElement * el) {
	// grab the boxEntry child
	std::vector< Widget* > childbox = boxEntries->get_children();
	// create a new box to encapsulte the tree levels
	if (childbox.size()>0) {
		boxEntries->remove(*childbox.at(0));
		delete childbox.at(0);
	}
		
	Gtk::Box * entryLevel = manage(new Gtk::VBox());

	Gtk::Entry * editableEntry = manage(new Gtk::Entry());

	editableEntry->set_text(xercesc::XMLString::transcode(el->getTagName()));

	editableEntry->show();

	Gtk::Box * nodeTypeBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,100));
	Gtk::Label * nodeTypeLabel = manage(new Gtk::Label("Type"));
	Gtk::Label * nodeTypeValue = manage(new Gtk::Label());
	nodeTypeLabel->show();
	nodeTypeValue->show();
	nodeTypeBox->add(*nodeTypeLabel);
	nodeTypeBox->add(*nodeTypeValue);
	nodeTypeBox->show();

	Gtk::Label * tagTitle = manage(new Gtk::Label("Tag name"));
	tagTitle->set_justify(Gtk::JUSTIFY_LEFT);
	tagTitle->set_halign(Gtk::ALIGN_START);
	tagTitle->show();		
	Gtk::Label * nodeContent = manage(new Gtk::Label());
	nodeContent->set_justify(Gtk::JUSTIFY_LEFT);
	nodeContent->set_halign(Gtk::ALIGN_START);
	nodeContent->show();


	if (el->hasChildNodes()) {
		
		// visit the first children of the current node
		if (el->getChildElementCount() != 0) {
			nodeTypeValue->set_text("Node");
			nodeContent->set_text("Node children");
		// if the node hasn't children then create a text box
		} else {
			// if it's a text node
			if (el->getFirstChild() != 0) {
			nodeTypeValue->set_text("Leaf");
			nodeContent->set_text("Node text");
			}
		}
	} else {
		nodeTypeValue->set_text("Lonely node");
		nodeContent->set_text("Empty node");
		Gtk::EventBox *eventLabel = manage(new Gtk::EventBox);
		// create a new label to store the node children
		Gtk::Label * label = manage( new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())) );
		/// configure the new label
		configureLabel(label);
		/// configure the new eventbox and encapsulate the label
		configureEventLabel(eventLabel,label);
		entryLevel->add(*eventLabel);
	}
	entryLevel->add(*tagTitle);
	entryLevel->add(*editableEntry);
	entryLevel->add(*nodeTypeBox);
	// generate the popup menu with the attributes
	configureNodeAttributesTreeview(el->getAttributes(),entryLevel);
	entryLevel->add(*nodeContent);
	if (nodeContent->get_text() == "Empty node") {
	} else if (nodeContent->get_text() == "Node text") {
		configureNodeText(xercesc::XMLString::transcode(el->getTextContent()),entryLevel);

		std::cout<<"I'm a node text"<<std::endl;
	} else if (nodeContent->get_text() == "Node children") {
			configureNodeChildrenTreeview(el->getFirstElementChild(),entryLevel);
	}
	entryLevel->show();
	boxEntries->add(*entryLevel);
}


void metadataWindow::constructTreeView(Glib::ustring XMLfile) {
	// Initialize xerces-c to read an xml file
	// and then parse it
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(XMLfile.c_str());
	xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
	// refresh the viewport and the first expander
	// to display properly the new bunch of metadata
	viewport1->remove(); viewport2->remove();
	Expander->remove();
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// enable html tag support
	Expander->set_use_markup(true);
	// colorize the filename	
	viewport1minimumwidth = 0;
	Expander->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	Expander->set_label(Glib::filename_display_basename(XMLfile.c_str()));
	viewport1minimumwidth = ((Glib::filename_display_basename(XMLfile.c_str())).size()*7 > viewport1minimumwidth)? (Glib::filename_display_basename(XMLfile.c_str())).size()*7 : viewport1minimumwidth;
	Expander->set_margin_left(0);
	// build the xml tree
	elReferences.clear();
	previouslabel = false;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	cptnode = 0;
	recursiveConstructTreeView(dom_root, Expander,0,0);
	//recursiveConstructEditableTreeView(dom_root,Expander2,0, boxEntries, xmlEntries);
	boxEntries->show();
	// add the expander to the viewport
	viewport1->add(*Expander);
	viewport2->add(*boxEntries);
	// set the new size
	set_size_request(viewport1minimumwidth+370,-1);
	// grab the current window position
	get_position(position_x,position_y);
	// reset window size and position and then reload the window if necessary
	if (get_visible()) {
		reshow_with_initial_size();
		move(position_x,position_y);
	}
	//write metadata buffer
	writeMetadataBuffer(XMLfile.c_str());
}

void metadataWindow::extractMetadata(std::string filename) {

	// init bmx and xerces-c
	bmx::connect_libmxf_logging();
	// path to the temporary xml files
	std::string xmltmp = Glib::get_current_dir ()+"/mxftmpmetadata.xml";
	// remove the previous temporary xml file to
	// avoid to reload some previous metadata
	std::remove(xmltmp.c_str());
	// prepare the viewport and the expander
	// to receive a new bunch of metadatas
	viewport1->remove(); viewport2->remove();
	Expander->remove();
	// enable the html tag support
	Expander->set_use_markup(true);
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// extract the metadata from an mxf file
	// and store it in a temporary xml file
	// with the EBUSDK
	EBUSDK::EBUCore::ExtractEBUCoreMetadata(filename.c_str(), xmltmp.c_str(), &progress_cb);
	// init xerces-s and then parse
	// the temporary XML file
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(xmltmp.c_str());
	xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
	// set font color of the label expander
	viewport1minimumwidth = 0;	
	Expander->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	Expander->set_label(Glib::filename_display_basename(filename.c_str()));
	Expander->set_margin_left(0);
	viewport1minimumwidth = ((Glib::filename_display_basename(filename.c_str())).size()*7 > viewport1minimumwidth)? (Glib::filename_display_basename(filename.c_str())).size()*7 : viewport1minimumwidth;
	// build the XML tree
	elReferences.clear();
	previouslabel = false;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	cptnode = 0;
	recursiveConstructTreeView(dom_root, Expander,0,0);
	//recursiveConstructEditableTreeView(dom_root, Expander2,0,boxEntries,xmlEntries);
	boxEntries->show();
	// add the new expander to the viewport
	viewport1->add(*Expander);
	viewport2->add(*boxEntries);
	// set the new size
	set_size_request(viewport1minimumwidth+370,-1);
	// grab the current window position
	get_position(position_x,position_y);
	// reset window size and position and then reload the window if required
	if (get_visible()) {
		reshow_with_initial_size();
		move(position_x,position_y);
	}
	metadataLoaded();
	//write metadata buffer
	writeMetadataBuffer(xmltmp.c_str());
}


bool metadataWindow::isExtension(std::string str, std::string extension) {
	// find last "." position in a string
	int lastindex = str.find_last_of(".");
	// copy what comes after last dot
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
	//loop through each character and make it lower-case. stop when you hit '\0'.
	for(int i = 0; str2[i] != '\0'; i++){
		str2[i] = tolower(str2[i]);
	}
	// compare the file extension to the extension pattern
  return (extension.compare(str2) == 0) ? true : false;
}


void metadataWindow::on_importXML_clicked(void)
{
	// get the local directory (application directory)
	static Glib::ustring working_dir = Glib::get_home_dir();
  // define the file chooser action and features
  Gtk::FileChooserDialog chooser(*this,
    "Select a metatag file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  // set the local directory as current directory
  chooser.set_current_folder(working_dir);
  // run the chooser
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
		// get the selected directory by user
    working_dir = chooser.get_current_folder();
		// set the filename as windows title
    set_title( Glib::filename_display_basename(chooser.get_filename()));
		// verify the extension to choose
		// the appropriate action 
		if (isExtension(Glib::filename_display_basename(chooser.get_filename()), "mxf")){
			// if it is an MXF file
			extractMetadata(chooser.get_filename());
		} else {
			// if it is an XML file
			if (isExtension(Glib::filename_display_basename(chooser.get_filename()), "xml")){
				constructTreeView(chooser.get_filename());
			metadataLoaded(); 
			} else {
				// if it is a file format error then
				// display an error alert
				Gtk::MessageDialog dialog(*this, "File format error");
				dialog.set_secondary_text("You can only open mxf or xml files.");
				dialog.run();
			}
		}
  }
}

void metadataWindow::on_modifyXML_clicked(void){
	//Expander2->set_visible(!Expander2->get_visible());
	//viewport2->set_visible(!Expander2->get_visible());
	//SecondScrolledWindow->set_visible(!Expander2->get_visible());
	
}

void metadataWindow::on_exportXML_clicked(void){
	std::string filename;
	// get the local directory (application directory)
	static Glib::ustring working_dir = Glib::get_home_dir();
  // define the file chooser action and features
  Gtk::FileChooserDialog chooser(*this,
    "Save the metadata file in xml", Gtk::FILE_CHOOSER_ACTION_SAVE);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  // set the local directory as current directory
  chooser.set_current_folder(working_dir);
  // run the chooser
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
		// write the file
		filename = chooser.get_filename();
		std::ofstream myfile;
		myfile.open (((isExtension(Glib::filename_display_basename(chooser.get_filename()), "xml"))?filename:filename+".xml"));
		myfile << xmlViewportFirst;
		myfile.close();
  }
}

void metadataWindow::on_saveXML_clicked(void)
{
}

void metadataWindow::on_XMLconformance_clicked(void)
{
}

std::string metadataWindow::removeTags(std::string str) {
	// current tags
	int nbcurrenttags = 4;
	std::string tags[] = {"<b>","</b>","<i>","</i>"};
	// temporary string
	std::string replacement = "";	
	std::string txt = str;
	// update the string
	for (int i=0;i<nbcurrenttags;i++){
			boost::regex rx(tags[i]);
			txt = boost::regex_replace(txt, rx, replacement);
	}
	return txt;
}

/*bool metadataWindow::on_extanderpressed_pressed(GdkEventButton* event, Gtk::Menu * Menu) {
	// detect when mouse right button is pressed
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) {
		std::cout<<"right button pressed"<<std::endl;
		// launch the popup menu
		Menu->popup(event->button, event->time);
		// signal has been handled.		
    return true;
  }
	// signal hasn't been handled.	
  return false;
}*/


bool metadataWindow::on_enter_label(GdkEventCrossing*, Gtk::EventBox *evLabel) {
		// reset the event box background color
		evLabel->unset_background_color(Gtk::STATE_FLAG_NORMAL);
		return true;
}


bool metadataWindow::on_leave_label(GdkEventCrossing*, Gtk::EventBox *evLabel) {
	// set the event box background color as white
	evLabel->override_background_color (white, Gtk::STATE_FLAG_NORMAL);
	return true;
}


bool metadataWindow::on_press_label(GdkEventButton* event, Gtk::EventBox *evLabel, Gtk::Label * txtLabel) {
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) {
		if (previousnode->get_label() != "!###ImAnEmptyExpander###!" and !previouslabel) {
			previousnode->override_color(black, Gtk::STATE_FLAG_NORMAL);
			previousnode->set_label("<b>"+removeTags(previousnode->get_label())+"</b>");
			previouslabel = !previouslabel;
		} else if (previouslabel) {
			previousnodelabel->override_color(black, Gtk::STATE_FLAG_NORMAL);
		}
		txtLabel->override_color(red,Gtk::STATE_FLAG_NORMAL);
		previousnodepos = atoi(evLabel->get_name().c_str());
		previousnodelabel = txtLabel;
		constructEditableNode(elReferences.at(previousnodepos));
		// signal has been handled.		
    return true;
  }
	// signal hasn't been handled.	
  return false;
}

void metadataWindow::metadataLoaded(void) {
	/* metadata control buttons */
	//importXML
	exportXML->set_sensitive(true);	
	modifyXML->set_sensitive(true);
	saveXML->set_sensitive(true);

}

void metadataWindow::on_openExpander_changed(Gtk::Expander * exp) {
	// update the previous textual breadcrumb if required
	if (previousnode->get_label() != "!###ImAnEmptyExpander###!" and !previouslabel) {		
		previousnode->override_color(black, Gtk::STATE_FLAG_NORMAL);
		previousnode->set_label("<b>"+removeTags(previousnode->get_label())+"</b>");
	} else if (previouslabel) {
		previousnodelabel->override_color(black, Gtk::STATE_FLAG_NORMAL);
		previouslabel = !previouslabel;
	}
	
	exp->override_color(red, Gtk::STATE_FLAG_NORMAL);
	exp->set_label("<b>"+removeTags(exp->get_label())+"</b>");
	previousnode = exp;
	previousnodepos = atoi(exp->get_name().c_str());
	constructEditableNode(elReferences.at(previousnodepos));
}
