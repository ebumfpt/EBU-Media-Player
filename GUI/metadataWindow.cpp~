/*!
 * \project_name EBU Player
 * \file metadataWindow.cpp
 * \brief EBUCore Metadata window
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 *
 * This software is published in LGPLv3.0
 *
 */


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


metadataWindow::metadataWindow(BaseObjectType* cobject, 
	const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
	m_refGlade(refGlade)
{
	refGladeButton(refGlade); // " " " " buttons
	refGladeWidgets(refGlade);// " " " " widgets
	connectSignalClicked(); // clicked buttons
}

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

	bmx::connect_libmxf_logging();
	//xercesc::XMLPlatformUtils::Initialize();
	std::cout<<"chemin et nom du fichier mxf : "<<filename<<std::endl;
	std::string output = Glib::get_current_dir ()+"/mxfmetadata.xml";
	std::cout<< output<<std::endl;
	EBUCore::ExtractEBUCoreMetadata(filename.c_str(), output.c_str(), &progress_cb);

	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(output.c_str());
	xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
	viewport1->remove();
	Expander->set_use_markup(true);
	Expander->set_label("<span color='blue'>"+Glib::filename_display_basename(filename.c_str())+"</span>");
	
	recursiveConstructTreeView(dom_root, Expander,0);

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

	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(XMLfile.c_str());
	xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
	viewport1->remove();
	Expander->set_use_markup(true);
	Expander->set_label("<span color='blue'>"+Glib::filename_display_basename(XMLfile.c_str())+"</span>");
	
	recursiveConstructTreeView(dom_root, Expander,0);

	viewport1->add(*Expander);

}

bool metadataWindow::isMXF(std::string str) {
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

void metadataWindow::on_importXML_clicked(void)
{
	std::cout<<"import xml pressed"<<std::endl;
	static Glib::ustring working_dir = Glib::get_home_dir();
  
  Gtk::FileChooserDialog chooser(*this,
    "Select a metatag file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  
  chooser.set_current_folder(working_dir);
  
  const int response = chooser.run();
  if(response == Gtk::RESPONSE_OK)
  {
    working_dir = chooser.get_current_folder();

    set_title( Glib::filename_display_basename(chooser.get_filename()));
		
		
		if (isMXF(Glib::filename_display_basename(chooser.get_filename()))){
			std::cout<<"in the pocket"<<std::endl;
		} else {
			std::cout<<"not in the pocket"<<std::endl;
		}

  //	std::string str =   Glib::filename_display_basename(chooser.get_filename());
		
	constructTreeView(chooser.get_filename());
		//extractMetadata("/home/dossantos/Videos/withEBUCore.MXF");
		
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
	int nbcurrenttags = 8;
	std::string tags[] = {
		"<b>","</b>","<span color='red'>",
		"<span color='red'>","<span color='blue'>","</span>",
		"<i>","</i>"
	};

	std::string replacement = "";	
	std::string txt = str;

	for (int i=0;i<nbcurrenttags;i++){
			boost::regex rx(tags[i]);
			txt = boost::regex_replace(txt, rx, replacement);
	}

	return txt;
}

bool metadataWindow::on_extanderpressed_pressed(GdkEventButton* event, Gtk::Menu * Menu) {
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) {

		Menu->popup(event->button, event->time);

    return true; //It has been handled.
  }
  return false;
}

void metadataWindow::on_openExpander_changed(Gtk::Expander * exp) {
	if (previousnode != NULL) {
		previousnode->set_label("<b>"+removeTags(previousnode->get_label())+"</b>");
	}
	exp->set_label("<span color='red'>"+exp->get_label()+"</span>");
	previousnode = exp;
	
}
