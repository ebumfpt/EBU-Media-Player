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
	refGlade->get_widget("FirstScrolledWindowBox",FirstScrolledWindowBox);
	refGlade->get_widget("SecondScrolledWindowBox",SecondScrolledWindowBox);
	refGlade->get_widget("boxEntries",boxEntries);
	refGlade->get_widget("boxStatus",boxStatus);
}

void metadataWindow::refGladeButton(const Glib::RefPtr<Gtk::Builder>& refGlade) {
	/* metadata control buttons */
	refGlade->get_widget("importXML", importXML);	
	refGlade->get_widget("exportXML", exportXML);
	refGlade->get_widget("enableEdition", enableEdition);	
	refGlade->get_widget("disableEdition", disableEdition);	
	refGlade->get_widget("saveXML", saveXML);	
	refGlade->get_widget("XMLconformance", XMLconformance);
}

void metadataWindow::connectSignalClicked(void) {
	// Connect the button clicked signals
	importXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_importXML_clicked));
	exportXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_exportXML_clicked));
	enableEdition->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_modifyXML_clicked));
	disableEdition->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_modifyXML_clicked));
	saveXML->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_saveXML_clicked));
	XMLconformance->signal_clicked().connect(sigc::mem_fun(*this,
                      &metadataWindow::on_XMLconformance_clicked));
}

void metadataWindow::defineColors(void) {
	black.set("black");
	white.set("white");
	whiteghost.set("whiteghost");
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
	initRightSide();
	set_has_resize_grip();
	editionMode = false;
	schema = new ebucoreParser();
}

// class destructor
metadataWindow::~metadataWindow()
{
	// remove the previous temporary xml file to
	// avoid to reload some previous metadata
	std::remove((Glib::get_current_dir ()+"/mxftmpmetadata.xml").c_str());
}
void metadataWindow::initRightSide(void){
	//boxStatus = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
 	//boxStatus->show();
	status = manage(new Gtk::Label("No EBUCore metadata detected"));
 	lineOne = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
 	source = manage(new Gtk::Label("source : "));
 	sourceLabel = manage(new Gtk::Label("no source"));
	lineTwo = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,0));
 	size = manage(new Gtk::Label("size : "));
 	sizeValue = manage(new Gtk::Label("0 byte"));
	status->show();
	source->show();
	sourceLabel->show();
	size->show();
	sizeValue->show();
	lineOne->show();
	lineTwo->show();
	lineOne->add(*source);
	lineOne->add(*sourceLabel);
	lineTwo->add(*size);
	lineTwo->add(*sizeValue);
	boxStatus->add(*status);
	//boxStatus->show();
	boxStatus->add(*lineOne);
	boxStatus->add(*lineTwo);
	//FirstScrolledWindowBox->hide();
	boxEntries->hide();
}

void metadataWindow::setRightSide(void) {
 	if (getSize("mxftmpmetadata.xml") > 0) {
		status->set_text("EBUCore metadata detected");
 	 	source->set_text("source : ");
 	 	sourceLabel->set_text(mxffilename);
		size->set_text("size : ");
		std::string path (Glib::get_current_dir ()+"/mxftmpmetadata.xml");
		int test = getSize(path.c_str()) ;
		std::string myvalue = intToString(test);
 	 	sizeValue->set_text(myvalue);
 		FirstScrolledWindowBox->show();
 		
	} else {
 	 	status->set_text("No EBUCore metadata detected");
		FirstScrolledWindowBox->hide();
	}
}

void metadataWindow::setWindowsPosition(int x, int y) {
	move(x,y);
	position_x = x;
	position_y = y;
}


  void metadataWindow::on_addNode_assistant_apply(Glib::RefPtr<Gtk::ListStore> metadataStore){

std::cout<< "I pressed apply button" << std::endl;
}

void metadataWindow::on_addAttribute_assistant_apply(Glib::RefPtr<Gtk::ListStore> metadataStore){
	int cpt = 0;
	for (Gtk::TreeNodeChildren::iterator row = metadataStore->children().begin(); row != metadataStore->children().end();row++) {
			cpt++;
		}
	Gtk::TreeModel::Row row = *(metadataStore->prepend());
	row[metadataNodeAttributesColumns.metadataNodeAttributeIdCol] = cpt;
	row[metadataNodeAttributesColumns.metadataNodeAttributeNameCol] = attributeNameEntry->get_text();
	row[metadataNodeAttributesColumns.metadataNodeAttributeValueCol] = attributeValueEntry->get_text();
	row[metadataNodeAttributesColumns.metadataNodeAttributeStateCol] = "new";
	row[metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = "lightgreen";

	elReferences.at(previousnodepos)->setAttribute(xercesc::XMLString::transcode(attributeNameEntry->get_text().c_str()), xercesc::XMLString::transcode(attributeValueEntry->get_text().c_str()));

	delete attributeNameEntry;
	delete attributeValueEntry;
}

void metadataWindow::on_addNodeText_assistant(Gtk::Box * text, Gtk::Box * children) {
	std::cout<< "I'm trying to change the second assistant page"<<std::endl;
	std::cout<< "I have "<<metadataAssistant->get_n_pages()<<" pages"<<std::endl;
	std::cout<< "I'm the page number "<<metadataAssistant->get_current_page()<<std::endl;
	metadataAssistant->set_title((addNode_type)?"add a node text": "add a node children");
	addNode_type = !addNode_type;
	/*if (!addNode_type) {
		metadataAssistant->remove_page(1);
		metadataAssistant->append_page(*text);
	  metadataAssistant->set_page_complete(*text, false);		
		metadataAssistant->set_page_type(*text, Gtk::ASSISTANT_PAGE_CONFIRM);
	} else {		
		metadataAssistant->remove_page(1);
		metadataAssistant->append_page(*children);
		metadataAssistant->set_page_complete(*children, false);		
		metadataAssistant->set_page_type(*children, Gtk::ASSISTANT_PAGE_CONFIRM);
	}*/
}

  void metadataWindow::on_assistant_cancel(){
		metadataAssistant->hide();
		metadataAssistant->remove();
		delete metadataAssistant;
	}

  void metadataWindow::on_assistant_close(){
		std::cout<<"I'm trying to quit the app..."<<std::endl;
		metadataAssistant->hide();
		metadataAssistant->remove();
		delete metadataAssistant;
	}


void metadataWindow::addNode(Glib::RefPtr<Gtk::ListStore> metadataStore) {
	metadataAssistant = new Gtk::Assistant();
	metadataAssistant->set_modal(true);
	metadataAssistant->set_title("Add a node");

	Gtk::Box * page1 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
	Gtk::Box * page2 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
	Gtk::Box * page3 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));

////////////// page 1
	Gtk::Box * page1hbox1 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));	
	Gtk::Box * page1hbox2 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,1));	
	Gtk::Box * page1hbox3 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,5));	

	Gtk::AspectFrame * page1hbox1space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox2space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox3space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox3space2 = manage(new Gtk::AspectFrame());


	Gtk::Label * tagNameTitle = manage(new Gtk::Label("Node name :"));
	Gtk::Label * tagTypeTitle = manage(new Gtk::Label("Type node :")); 

	/*Gtk::Entry **/ tagValueEntry = manage(new Gtk::Entry());
	tagValueEntry->set_placeholder_text("Input the tag's name'");
	Gtk::RadioButton::Group radioGroup;
	Gtk::RadioButton *page1hbox3radiobutton1 = manage(new Gtk::RadioButton(radioGroup, "text", true));
	Gtk::RadioButton *page1hbox3radiobutton2 = manage(new Gtk::RadioButton(radioGroup, "node", false));
	addNode_type=false;

page1hbox3radiobutton1->signal_toggled().connect(sigc::bind<Gtk::Box *, Gtk::Box *>(sigc::mem_fun(*this, &metadataWindow::on_addNodeText_assistant),page2,page3));
	metadataAssistant->signal_apply().connect(sigc::bind<Glib::RefPtr<Gtk::ListStore> >(sigc::mem_fun(*this,
		&metadataWindow::on_addNode_assistant_apply),metadataStore));

	page1hbox1->add(*tagNameTitle);
	page1hbox1->add(*page1hbox1space1);
	page1hbox1->add(*tagValueEntry);
	page1hbox1->show_all_children();
	page1hbox1->show();

	page1hbox2->add(*page1hbox2space1);
	page1hbox2->show_all_children();
	page1hbox2->show();
	
	page1hbox3->add(*tagTypeTitle);
	page1hbox3->add(*page1hbox3space1);
	page1hbox3->add(*page1hbox3radiobutton1);
	page1hbox3->add(*page1hbox3space2);
	page1hbox3->add(*page1hbox3radiobutton2);
	page1hbox3->show_all_children();
	page1hbox3->show();

	page1->add(*page1hbox1);
	page1->add(*page1hbox2);
	page1->add(*page1hbox3);
	page1->show_all_children();

// page 2
	Gtk::Label * nodeTextTitle = manage(new Gtk::Label("Node text :"));
	nodeTextTitle->set_justify(Gtk::JUSTIFY_LEFT);
	nodeTextTitle->set_halign(Gtk::ALIGN_START);

	Gtk::Box * assistantTextViewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,2));
	assistantTextViewBox->set_size_request(120, 150);	
	assistantTextViewBox->add(*nodeTextTitle);

	Gtk::ScrolledWindow * assistantTextViewScrolledWindow = manage(new Gtk::ScrolledWindow());
	assistantTextViewScrolledWindow->set_size_request(120, 150);

	Gtk::TextView * assistantTextView = manage (new Gtk::TextView());
	
	assistantTextView->drag_highlight();
	assistantTextView->set_pixels_above_lines(2);
	assistantTextView->set_left_margin(2);
	assistantTextView->set_right_margin(2);

	assistantTextView->set_editable(true);
  assistantTextView->set_cursor_visible(true);
	assistantTextView->set_wrap_mode(Gtk::WRAP_WORD);

	assistantTextBuffer = Gtk::TextBuffer::create();

	 assistantTextView->set_buffer(assistantTextBuffer);
	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  assistantTextViewScrolledWindow->add(*assistantTextView);
	assistantTextView->show();
  //Only show the scrollbars when they are necessary:
  assistantTextViewScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	assistantTextViewBox->add(*assistantTextViewScrolledWindow);
	assistantTextViewScrolledWindow->show();
	page2->add(*assistantTextViewBox);

/////
// page 3
/////

	Gtk::Label * nodeChildrenTitle = manage(new Gtk::Label("Node children :"));
	nodeChildrenTitle->set_justify(Gtk::JUSTIFY_LEFT);
	nodeChildrenTitle->set_halign(Gtk::ALIGN_START);

	Gtk::Box * assistantNodeChildrenViewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,2));
	assistantNodeChildrenViewBox->set_size_request(120, 150);	
	assistantNodeChildrenViewBox->add(*nodeChildrenTitle);

	Gtk::ScrolledWindow * assistantNodeChildrenViewScrolledWindow = manage(new Gtk::ScrolledWindow());
	assistantNodeChildrenViewScrolledWindow->set_size_request(120, 150);

	Gtk::TextView * assistantNodeChildrenView = manage (new Gtk::TextView());
	
	assistantNodeChildrenView->drag_highlight();
	assistantNodeChildrenView->set_pixels_above_lines(2);
	assistantNodeChildrenView->set_left_margin(2);
	assistantNodeChildrenView->set_right_margin(2);

	assistantNodeChildrenView->set_editable(true);
  assistantNodeChildrenView->set_cursor_visible(true);
	assistantNodeChildrenView->set_wrap_mode(Gtk::WRAP_WORD);

	assistantNodeChildrenBuffer = Gtk::TextBuffer::create();

	 assistantNodeChildrenView->set_buffer(assistantNodeChildrenBuffer);
	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  assistantNodeChildrenViewScrolledWindow->add(*assistantNodeChildrenView);
	assistantNodeChildrenView->show();
  //Only show the scrollbars when they are necessary:
  assistantNodeChildrenViewScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	assistantNodeChildrenViewBox->add(*assistantNodeChildrenViewScrolledWindow);
	assistantNodeChildrenViewScrolledWindow->show();
	page3->add(*assistantNodeChildrenViewBox);
//

	metadataAssistant->insert_page(*page1,0);
	metadataAssistant->set_page_complete(*page1, true);	
	metadataAssistant->set_page_type(*page1, Gtk::ASSISTANT_PAGE_INTRO);
	metadataAssistant->insert_page(*page2,1);
  metadataAssistant->set_page_complete(*page2, true);
	metadataAssistant->insert_page(*page3,2);
  metadataAssistant->set_page_complete(*page3, true);
	metadataAssistant->set_page_type(*page3, Gtk::ASSISTANT_PAGE_CONFIRM);

	metadataAssistant->signal_cancel().connect(sigc::mem_fun(*this,
		&metadataWindow::on_assistant_cancel));

	//metadataAssistant->signal_prepare().connect(sigc::mem_fun(*this,
	//	&metadataWindow::on_addNode_assistant_prepare));

//
// save the change
//

metadataAssistant->signal_close().connect(sigc::mem_fun(*this,
		&metadataWindow::on_assistant_close));

	metadataAssistant->show_all_children();
	metadataAssistant->show();
  	//add to the treeview
/*  	Gtk::TreeModel::Row row = *(metadataStore->prepend());
		row[metadataNodeChildrenColumns.metadataNodeChildrenIdCol] = 0;
  	row[metadataNodeChildrenColumns.metadataNodeChildrenNameCol] = "NodeChildrenName";
  	row[metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol] = "lightgreen";
*/
}


void metadataWindow::addNodeAttribute(Glib::RefPtr<Gtk::ListStore> metadataStore) {	
  	//add to the treeview
	metadataAssistant = new Gtk::Assistant();
	metadataAssistant->set_modal(true);
	metadataAssistant->set_title("Add a node attribute");

	Gtk::Box * page1 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
	//Gtk::Box * page2 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
	//Gtk::Box * page3 = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));

////////////// page 1
	Gtk::Box * page1hbox1 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));	
	Gtk::Box * page1hbox2 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,1));	
	Gtk::Box * page1hbox3 = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));	

	Gtk::AspectFrame * page1hbox1space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox2space1 = manage(new Gtk::AspectFrame());
	Gtk::AspectFrame * page1hbox3space1 = manage(new Gtk::AspectFrame());


	Gtk::Label * attributeNameTitle = manage(new Gtk::Label("Attribute name :"));
	Gtk::Label * attributeValueTitle = manage(new Gtk::Label("Attribute value :")); 

	attributeNameEntry = manage(new Gtk::Entry());
	attributeValueEntry = manage(new Gtk::Entry());
	attributeNameEntry->set_placeholder_text("Input the attribute's name'");
	attributeValueEntry->set_placeholder_text("Input the attribute's value'");

	page1hbox1->add(*attributeNameTitle);
	page1hbox1->add(*page1hbox1space1);
	page1hbox1->add(*attributeNameEntry);
	page1hbox1->show_all_children();
	page1hbox1->show();

	page1hbox2->add(*page1hbox2space1);
	page1hbox2->show_all_children();
	page1hbox2->show();
	
	page1hbox3->add(*attributeValueTitle);
	page1hbox3->add(*page1hbox3space1);
	page1hbox3->add(*attributeValueEntry);
	page1hbox3->show_all_children();
	page1hbox3->show();

	page1->add(*page1hbox1);
	page1->add(*page1hbox2);
	page1->add(*page1hbox3);
	page1->show_all_children();


	metadataAssistant->append_page(*page1);
	metadataAssistant->set_page_complete(*page1, true);	
	metadataAssistant->set_page_type(*page1, Gtk::ASSISTANT_PAGE_CONFIRM);


	metadataAssistant->signal_apply().connect(sigc::bind<Glib::RefPtr<Gtk::ListStore> >(sigc::mem_fun(*this,
		&metadataWindow::on_addAttribute_assistant_apply),metadataStore));
	metadataAssistant->signal_cancel().connect(sigc::mem_fun(*this,
		&metadataWindow::on_assistant_cancel));

///
// save the change
//
metadataAssistant->signal_close().connect(sigc::mem_fun(*this,
		&metadataWindow::on_assistant_close));

//

	metadataAssistant->show_all_children();
	metadataAssistant->show();
  	//add to the treeview
/*  	Gtk::TreeModel::Row row = *(metadataStore->prepend());
		row[metadataNodeChildrenColumns.metadataNodeChildrenIdCol] = 0;
  	row[metadataNodeChildrenColumns.metadataNodeChildrenNameCol] = "NodeChildrenName";
  	row[metadataNodeChildrenColumns.metadataNodeChildrenBgColorCol] = "lightgreen";
*/
}


void metadataWindow::removeNode(Glib::RefPtr<Gtk::ListStore> metadataStore) {
 std::cout<<"I'm removing a node"<<std::endl;
}


void metadataWindow::removeNodeAttribute(Glib::RefPtr<Gtk::ListStore> metadataStore) {
	Gtk::TreeModel::iterator iter = metadataNodeAttributesTreeviewSelection->get_selected();
(elReferences.at(previousnodepos))->removeAttribute(xercesc::XMLString::transcode(((Glib::ustring)((*iter)[metadataNodeAttributesColumns.metadataNodeAttributeNameCol])).c_str()));
	metadataStore->erase(iter);
}

void metadataWindow::configureNodeEditionButtonsTreeview(Gtk::Box * nodebox, Glib::RefPtr<Gtk::ListStore> nodeStore) {
	Gtk::Box * EditButtonBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));
	EditButtonBox->set_hexpand (true);	
	EditButtonBox->set_halign (Gtk::ALIGN_END);	
	
	Gtk::Button *addButton = manage(new Gtk::Button());
	Gtk::Button *removeButton = manage(new Gtk::Button());

	Gtk::Image *addImage = manage(new Gtk::Image(Gtk::Stock::ADD,Gtk::ICON_SIZE_MENU));
	Gtk::Image *removeImage = manage(new Gtk::Image(Gtk::Stock::REMOVE,Gtk::ICON_SIZE_MENU));

	addButton->set_image(*addImage);
	removeButton->set_image(*removeImage);

	addButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
		                  &metadataWindow::addNode),nodeStore));
	removeButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
		                  &metadataWindow::removeNode),nodeStore));

	EditButtonBox->add(*addButton);
	EditButtonBox->add(*removeButton);

	addButton->show();
	removeButton->show();
	EditButtonBox->show();

	nodebox->add(*EditButtonBox);
}

void metadataWindow::configureNodeAttributeEditionButtonsTreeview(Gtk::Box * nodebox, Glib::RefPtr<Gtk::ListStore> attributeStore) {
	Gtk::Box * EditButtonBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,3));
	EditButtonBox->set_hexpand (true);	
	EditButtonBox->set_halign (Gtk::ALIGN_END);	
	
	Gtk::Button *addButton = manage(new Gtk::Button());
	Gtk::Button *removeButton = manage(new Gtk::Button());

	Gtk::Image *addImage = manage(new Gtk::Image(Gtk::Stock::ADD,Gtk::ICON_SIZE_MENU));
	Gtk::Image *removeImage = manage(new Gtk::Image(Gtk::Stock::REMOVE,Gtk::ICON_SIZE_MENU));

	addButton->set_image(*addImage);
	removeButton->set_image(*removeImage);

	addButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
		                  &metadataWindow::addNodeAttribute),attributeStore));
	removeButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
			                &metadataWindow::removeNodeAttribute),attributeStore));

	EditButtonBox->add(*addButton);
	EditButtonBox->add(*removeButton);

	addButton->show();
	removeButton->show();
	EditButtonBox->show();

	nodebox->add(*EditButtonBox);
}

void metadataWindow::configureNodeAttributesTreeview(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Box * nodebox) {

	Gtk::Box * AttributesTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	AttributesTreeviewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * metadataNodeAttributesScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeAttributesScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeAttributesTreeview = manage(new Gtk::TreeView());
	metadataNodeAttributesTreeview->drag_highlight();

//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeAttributesScrolledWindow->add(*metadataNodeAttributesTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeAttributesScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeAttributesStore;
 // Glib::RefPtr<Gtk::TreeSelection> metadataNodeAttributesTreeviewSelection;

	metadataNodeAttributesStore.clear();
	// create the data model
	metadataNodeAttributesStore = Gtk::ListStore::create(metadataNodeAttributesColumns);
	// set the data model
  metadataNodeAttributesTreeview->set_model(metadataNodeAttributesStore);
	//Add the TreeView's view columns
  metadataNodeAttributesTreeview->append_column("", metadataNodeAttributesColumns.metadataNodeAttributeIdCol);
  metadataNodeAttributesTreeview->append_column("Name", metadataNodeAttributesColumns.metadataNodeAttributeNameCol);
  metadataNodeAttributesTreeview->append_column("Value", metadataNodeAttributesColumns.metadataNodeAttributeValueCol);
  metadataNodeAttributesTreeview->append_column("State", metadataNodeAttributesColumns.metadataNodeAttributeStateCol);
  metadataNodeAttributesTreeview->append_column("Bgcolor", metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol);

	Gtk::TreeView::Column* pColumn;
	for(guint i = 0; i < 4; i++)
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
	pColumn = metadataNodeAttributesTreeview->get_column(4);
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
			row[metadataNodeAttributesColumns.metadataNodeAttributeStateCol] = "original";
			row[metadataNodeAttributesColumns.metadataNodeAttributeBgColorCol] = (i%2==0)?"white":"ghostwhite";
		}		
	}

	AttributesTreeviewBox->add(*metadataNodeAttributesScrolledWindow);
	
	AttributesTreeviewBox->show();
	metadataNodeAttributesScrolledWindow->show();
	metadataNodeAttributesTreeview->show();

	nodebox->add(*AttributesTreeviewBox);

	if (editionMode) {
		configureNodeAttributeEditionButtonsTreeview(nodebox, metadataNodeAttributesStore);
	}
}

void metadataWindow::configureNodeChildrenTreeview(xercesc::DOMElement * children, Gtk::Box * nodebox) {

	Gtk::Box * ChildrenTreeviewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	ChildrenTreeviewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * metadataNodeChildrenScrolledWindow = manage(new Gtk::ScrolledWindow());
	metadataNodeChildrenScrolledWindow->set_size_request(120, 150);

	Gtk::TreeView * metadataNodeChildrenTreeview = manage(new Gtk::TreeView());
	metadataNodeChildrenTreeview->drag_highlight();

//Add the TreeView, inside a ScrolledWindow, with the button underneath:
  metadataNodeChildrenScrolledWindow->add(*metadataNodeChildrenTreeview);

  //Only show the scrollbars when they are necessary:
  metadataNodeChildrenScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Glib::RefPtr<Gtk::ListStore> metadataNodeChildrenStore;
  //Glib::RefPtr<Gtk::TreeSelection> metadataNodeChildrenTreeviewSelection;
	
	metadataNodeChildrenStore.clear();
	// create the data model
	metadataNodeChildrenStore = Gtk::ListStore::create(metadataNodeChildrenColumns);
	// set the data model
  metadataNodeChildrenTreeview->set_model(metadataNodeChildrenStore);

	//Add the TreeView's view columns
  metadataNodeChildrenTreeview->append_column("", metadataNodeChildrenColumns.metadataNodeChildrenIdCol);
  metadataNodeChildrenTreeview->append_column("Name", metadataNodeChildrenColumns.metadataNodeChildrenNameCol);
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

	if (editionMode) {
		configureNodeEditionButtonsTreeview(nodebox, metadataNodeChildrenStore);
	}
}

void metadataWindow::configureNodeText(std::string nodetext, Gtk::Box * nodebox) {

	Gtk::Box * nodeTextViewBox = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	nodeTextViewBox->set_size_request(120, 150);

	Gtk::ScrolledWindow * nodeTextViewScrolledWindow = manage(new Gtk::ScrolledWindow());
	nodeTextViewScrolledWindow->set_size_request(120, 150);

	Gtk::TextView * nodeTextView = manage (new Gtk::TextView());
	nodeTextView->override_background_color(((editionMode)? white : whiteghost), Gtk::STATE_FLAG_NORMAL);
	
	nodeTextView->drag_highlight();
	nodeTextView->set_pixels_above_lines(2);
	nodeTextView->set_left_margin(2);
	nodeTextView->set_right_margin(2);

	nodeTextView->set_editable(editionMode);
  nodeTextView->set_cursor_visible(editionMode);
	nodeTextView->set_wrap_mode(Gtk::WRAP_WORD);

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
	// and delete it
	if (childbox.size()>0) {
		boxEntries->remove(*childbox.at(0));
		delete childbox.at(0);
	}
		
	Gtk::Box * entryLevel = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,10));
	entryLevel->set_size_request(120, 150);
	entryLevel->set_border_width(10);
	
	Gtk::Entry * editableEntry = manage(new Gtk::Entry());
	editableEntry->set_text(xercesc::XMLString::transcode(el->getTagName()));
	editableEntry->drag_highlight();
	editableEntry->set_has_frame(false);
	editableEntry->set_editable(editionMode);
	editableEntry->show();

	Gtk::Label * nonEditableEntry = manage(new Gtk::Label(xercesc::XMLString::transcode(el->getTagName())));
	nonEditableEntry->set_alignment (Gtk::ALIGN_START,Gtk::ALIGN_CENTER);	
	nonEditableEntry->set_padding(4,4);
	nonEditableEntry->override_color(black, Gtk::STATE_FLAG_NORMAL);
	nonEditableEntry->drag_highlight();
	nonEditableEntry->show();
	

	Gtk::Box * nodeTypeBox = manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,10));
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
	if (editionMode) {
		entryLevel->add(*editableEntry);
		delete nonEditableEntry;
	} else {
		entryLevel->add(*nonEditableEntry);
		delete editableEntry;
	}
	entryLevel->add(*nodeTypeBox);

	Gtk::Label * nodeAttributeTitle = manage(new Gtk::Label("Node attributes"));
	nodeAttributeTitle->set_justify(Gtk::JUSTIFY_LEFT);
	nodeAttributeTitle->set_halign(Gtk::ALIGN_START);
	nodeAttributeTitle->show();

	entryLevel->add(*nodeAttributeTitle);

	//if (el->getAttributes()->getLength()>0) { 
		configureNodeAttributesTreeview(el->getAttributes(),entryLevel);
		entryLevel->add(*nodeContent);
	//}

	if (nodeContent->get_text() == "Empty node") {
	} else if (nodeContent->get_text() == "Node text") {
		configureNodeText(xercesc::XMLString::transcode(el->getTextContent()),entryLevel);
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
	viewport1->remove(); //viewport2->remove();
	Expander->remove();
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// enable html tag support
	Expander->set_use_markup(true);
	// colorize the filename	
	viewport1minimumwidth = 0;
	Expander->override_color(blue, Gtk::STATE_FLAG_NORMAL);
	Expander->set_label(Glib::filename_display_basename(XMLfile.c_str()));
	Expander->property_expanded().signal_changed().connect(sigc::bind<Gtk::Expander *>(sigc::mem_fun(*this, &metadataWindow::on_openExpander_changed), Expander));
	viewport1minimumwidth = ((Glib::filename_display_basename(XMLfile.c_str())).size()*8 > viewport1minimumwidth)? (Glib::filename_display_basename(XMLfile.c_str())).size()*8 : viewport1minimumwidth;
	Expander->set_margin_left(0);
	// build the xml tree
	elReferences.clear();
	previouslabel = false;
 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
	cptnode = 0;
	xml_dom_root=dom_root;
	recursiveConstructTreeView(xml_dom_root, Expander,0,0);
	//recursiveConstructEditableTreeView(dom_root,Expander2,0, boxEntries, xmlEntries);
	boxEntries->show();
	// add the expander to the viewport
	viewport1->add(*Expander);
	viewport1->set_size_request(viewport1minimumwidth,-1);
//	viewport2->add(*boxEntries);
	SecondScrolledWindowBox->add(*boxEntries);
	// set the new size
	set_size_request(viewport1minimumwidth+410,-1);
	// grab the current window position
	get_position(position_x,position_y);
	// reset window size and position and then reload the window if necessary
	if (get_visible()) {
		reshow_with_initial_size();
		move(position_x,position_y);
	}
	//write metadata buffer
	writeMetadataBuffer(XMLfile.c_str());
	setRightSide();
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
	viewport1->remove(); //viewport2->remove();
	Expander->remove();
	// enable the html tag support
	Expander->set_use_markup(true);
	previousnode = manage(new Gtk::Expander("!###ImAnEmptyExpander###!"));
	// extract the metadata from an mxf file
	// and store it in a temporary xml file
	// with the EBUSDK
	int lastindex = filename.find_last_of("/");
	// copy what comes after last dot
	mxffilename = filename.substr (lastindex+1,filename.size()-(lastindex+1)); 
	EBUSDK::EBUCore::ExtractEBUCoreMetadata(filename.c_str(), xmltmp.c_str(), &progress_cb);
	// init xerces-s and then parse
	// the temporary XML file
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xml = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xml->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument* dom_doc  = dom_file->parseURI(xmltmp.c_str());
	
	std::cout<<"I'm here 4 and I'm "<<((dom_doc != 0)?"not empty":"empty")<<"."<<std::endl;
	if (dom_doc != 0) {
		xercesc::DOMElement*  dom_root = dom_doc->getDocumentElement();
		std::cout<<"I'm here 5"<<std::endl;
		// set font color of the label expander
		viewport1minimumwidth = 0;	
		Expander->override_color(blue, Gtk::STATE_FLAG_NORMAL);
		Expander->set_label(Glib::filename_display_basename(filename.c_str()));
		
	Expander->property_expanded().signal_changed().connect(sigc::bind<Gtk::Expander *>(sigc::mem_fun(*this, &metadataWindow::on_openExpander_changed), Expander));
		Expander->set_margin_left(0);
		viewport1minimumwidth = ((Glib::filename_display_basename(filename.c_str())).size()*8 > viewport1minimumwidth)? (Glib::filename_display_basename(filename.c_str())).size()*8 : viewport1minimumwidth;
		// build the XML tree
		elReferences.clear();
		previouslabel = false;
	 	boxEntries = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL,0));
		cptnode = 0;
		xml_dom_root=dom_root;

		recursiveConstructTreeView(xml_dom_root, Expander,0,0);

		boxEntries->show();
		// add the new expander to the viewport
		viewport1->add(*Expander);
		viewport1->	set_size_request(viewport1minimumwidth,-1);
		//viewport2->add(*boxEntries);
		SecondScrolledWindowBox->add(*boxEntries);
		// set the new size
		set_size_request(viewport1minimumwidth+410,-1);
		// grab the current window position
		get_position(position_x,position_y);
		// reset window size and position and then reload the window if required
		if (get_visible()) {
			reshow_with_initial_size();
			move(position_x,position_y);
		}
		//metadataLoaded();
		//write metadata buffer
		writeMetadataBuffer(xmltmp.c_str());
	}
	setRightSide();
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
			//metadataLoaded(); 
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
	editionMode = !editionMode;  
	enableEdition->set_visible(!enableEdition->get_visible());
	disableEdition->set_visible(!disableEdition->get_visible());
	constructEditableNode(elReferences.at(previousnodepos));
	
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
			boxEntries->show();
			boxStatus->hide();
		} else if (previouslabel) {
			previousnodelabel->override_color(black, Gtk::STATE_FLAG_NORMAL);
		}
		txtLabel->override_color(red,Gtk::STATE_FLAG_NORMAL);
		previousnodepos = atoi(evLabel->get_name().c_str());
		previousnodelabel = txtLabel;
		constructEditableNode(elReferences.at(previousnodepos));
		
		if (removeTags(previousnode->get_label()) == mxffilename) {
			boxStatus->show();boxEntries->hide();
		} else {
			boxStatus->hide();boxEntries->show();
		}
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
	enableEdition->set_sensitive(true);	
	disableEdition->set_sensitive(true);
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
	if (removeTags(previousnode->get_label()) == mxffilename) {
		boxStatus->show();boxEntries->hide();
	} else {
		boxStatus->hide();boxEntries->show();
	}
}

unsigned long int metadataWindow::getSize(const char* file) {
	FILE * pFile = fopen (file,"rb");
	unsigned long int size = 0;

	if (pFile!=NULL) {
		fseek (pFile, 0, SEEK_END);   // non-portable
		size = ftell (pFile);
		fclose (pFile);
  	}
	return size;
}


std::string metadataWindow::intToString(unsigned long int number){
	std::string Units[] = {"bytes","Kb","Mb","Gb", "Tb"};
	short divider = 0;
	for (double i=1.0;((float)(number))/i>1000.0;i*=1000.0) {
		divider++;
	}
	std::stringstream converted;//create a stringstream
	converted << ((float)(number))/pow(1000.0,divider);//add number to the stream
	return converted.str()+" "+Units[divider];//return a string with the contents of the stream
}
