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


// class constructor
metadataWindow::metadataWindow(BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(refGlade); // " " " " buttons
	refGladeWidgets(refGlade);// " " " " widgets
	connectSignalClicked(); // clicked buttons
}

// class destructor
metadataWindow::~metadataWindow()
{
}

void metadataWindow::setWindowsPosition(int x, int y) {
	move(x,y);
}

// configure the gtk box encapsulated in the gtk expander
void metadataWindow::configureEncapsultedBox(Gtk::Box * b) {
	// define the vertical alignment
	b->set_valign(Gtk::ALIGN_START);
	// define the left margin (treeview indentation)
	b->set_margin_left(b->get_margin_left()+16);
}

// configure the gtk box encapsulated in the gtk expander
void metadataWindow::configureExpander(Gtk::Expander * e) {
		// signal handler to detect what expander has been expanded
	e->property_expanded().signal_changed().connect(sigc::bind<Gtk::Expander *>(sigc::mem_fun(*this, &metadataWindow::on_openExpander_changed), e));
	// define the margin left
	e->set_margin_left(0);
	// use the pango markup language
	e->set_use_markup(true);
	// define the expander label
	e->set_label("<b>"+e->get_label()+"</b>");
}

// create a new box to encapsulte a node text
void metadataWindow::createExpanderTextBox(Gtk::Expander * ed, Gtk::Box *bd, std::string txt) {
					// create a new box to encapsulte the node text
				Gtk::Box * vBoxTagvalue = manage(new Gtk::VBox());
				Gtk::Box * hBoxTagvalue = manage(new Gtk::HBox());
				// define the vertical and horizontal alignment
				vBoxTagvalue->set_valign(Gtk::ALIGN_START);				
				hBoxTagvalue->set_halign(Gtk::ALIGN_START);
				// show the vbox and add in the hbox				
				vBoxTagvalue->show();
				vBoxTagvalue->add(*hBoxTagvalue);				
				// define the indentation
				vBoxTagvalue->set_margin_left(bd->get_margin_left()+16);
				// create labels
				Gtk::Label *ValueLabel  = (manage(new Gtk::Label("Value : ")));
				Gtk::Label *ValueText  =  (manage(new Gtk::Label(txt))); ///////////////////
				// define the styles and markup for labels
				ValueLabel->set_justify(Gtk::JUSTIFY_LEFT);
				ValueLabel->set_halign(Gtk::ALIGN_START);
				ValueLabel->set_markup("<i>"+ValueLabel->get_text()+"</i>");
				// add labels in hbox
				hBoxTagvalue->add(*ValueLabel);
				hBoxTagvalue->add(*ValueText);
				// show label
				ValueLabel->show();
				ValueText->show();
				// show hbox
				hBoxTagvalue->show();
				// add the new vbox to Gtk Expander parent 
				ed->add(*vBoxTagvalue); ///
}

void metadataWindow::finishEncapsulation(Gtk::Box * root, Gtk::Expander * node) {
				node->show();///************
				//root->show();///************
				root->add(*node);
}

void metadataWindow::createAttributesPopup(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Expander * node) {
	Gtk::Menu *Menu = new Gtk::Menu();
	int num= dom_attrs->getLength();
	if (num != 0) {
		for(int i=0; i<num;i++) {
			xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
			Gtk::MenuItem *nameItem = new Gtk::MenuItem((std::string)xercesc::XMLString::transcode(dom_attr->getName())+" : "+(std::string)xercesc::XMLString::transcode(dom_attr->getValue()));
			Menu->add(*nameItem);
			//Gtk::MenuItem *valueItem = new Gtk::MenuItem(xercesc::XMLString::transcode(dom_attr->getValue()));
			//Menu->add(*valueItem);
		}		
	} else {
		Gtk::MenuItem *nameItem = new Gtk::MenuItem("no attributes");
		Menu->add(*nameItem);
	}
	Menu->show_all();
	node->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this,
	                    &metadataWindow::on_extanderpressed_pressed),Menu));
}

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
	viewport1->remove();
	Expander->remove();
	// enable the html tag support
	Expander->set_use_markup(true);
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
	Expander->set_label("<span color='blue'>"+Glib::filename_display_basename(filename.c_str())+"</span>");
	// build the XML tree
	recursiveConstructTreeView(dom_root, Expander,0);
	// add the new expander to the viewport
	viewport1->add(*Expander);
}

void metadataWindow::recursiveConstructTreeView(xercesc::DOMElement * el, Gtk::Expander * seed, int depth) {
	// create a new box to encapsulte the tree levels
	Gtk::Box * root = manage(new Gtk::VBox());
	// configure the new box
	configureEncapsultedBox(root);
	//xercesc::DOMNode * dom_child = el->getFirstChild();
	do {
		// create a new expander to store the node children
		Gtk::Expander * node = manage( new Gtk::Expander(xercesc::XMLString::transcode(el->getTagName())) );///
		std::cout<<xercesc::XMLString::transcode(el->getTagName())<<"youyou"<<std::endl;
		xercesc::DOMNode* dom_child=el;
		// configure the new expander
		configureExpander(node);		
		// visit the first children
		if (el->getChildElementCount() != 0) {
			recursiveConstructTreeView(el->getFirstElementChild(), node ,depth+1);
		}
		// if the node hasn't children, create a text box and
		// a popup menu with the node attributes
		if (el->getChildElementCount() == 0) {
			xercesc::DOMNode * dom_child = el->getFirstChild();
			if (dom_child != 0) {
				createExpanderTextBox(node, root, xercesc::XMLString::transcode(el->getTextContent()));
				createAttributesPopup(el->getAttributes(),node);
			}
		} else {
			// create a popup menu with node attributes
			createAttributesPopup(el->getAttributes(),node);
			xercesc::DOMNamedNodeMap *dom_attrs = dom_child->getAttributes();
			int num= dom_attrs->getLength();
			if (num != 0) {
				for(int i=0; i<num;i++) {
					xercesc::DOMAttr* dom_attr = (xercesc::DOMAttr*) dom_attrs->item(i);
					std::cout<<(std::string)xercesc::XMLString::transcode(dom_attr->getName())<<" : "<<(std::string)xercesc::XMLString::transcode(dom_attr->getValue())<<std::endl;
				}						
			}
		}
		// end encapsultation level
		finishEncapsulation(root,node);
		// next node at this level
		el = el->getNextElementSibling();
	} while (el != 0);
	// add and show...
	seed->add(*root);
	root->show();
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
	viewport1->remove();
	Expander->remove();	
	// enable html tag support
	Expander->set_use_markup(true);
	// colorize the filename
	Expander->set_label("<span color='blue'>"+Glib::filename_display_basename(XMLfile.c_str())+"</span>");
	// build the xml tree
	recursiveConstructTreeView(dom_root, Expander,0);
	// add the expander to the viewport
	viewport1->add(*Expander);

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

void metadataWindow::on_modifyXML_clicked(void)
{
}

void metadataWindow::on_exportXML_clicked(void)
{
}

void metadataWindow::on_saveXML_clicked(void)
{
}

void metadataWindow::on_XMLconformance_clicked(void)
{
}

std::string metadataWindow::removeTags(std::string str) {
	// current tags
	int nbcurrenttags = 8;
	std::string tags[] = {
		"<b>","</b>","<span color='red'>",
		"<span color='red'>","<span color='blue'>","</span>",
		"<i>","</i>"
	};
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

bool metadataWindow::on_extanderpressed_pressed(GdkEventButton* event, Gtk::Menu * Menu) {
	// detect when mouse right button is pressed
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) {
		// launch the popup menu
		Menu->popup(event->button, event->time);
		// signal has been handled.		
    return true;
  }
	// signal hasn't been handled.	
  return false;
}

void metadataWindow::on_openExpander_changed(Gtk::Expander * exp) {
	/*if (previousnode != NULL) {
		previousnode->set_label("<b>"+removeTags(previousnode->get_label())+"</b>");
	}
	exp->set_label("<span color='red'>"+exp->get_label()+"</span>");
	previousnode = exp;
	*/
}
