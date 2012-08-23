#ifndef CPP_METADATAWINDOW_CLASS_H
#define CPP_METADATAWINDOW_CLASS_H

/*!
 * \project_name EBU Player
 * \file metadataWindow.hpp
 * \brief metadata window specifications
 * \details This class is used to load the EBUCore Metadata viewer and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */

#include <gtkmm.h>
#include <iostream>
#include <string>
#include <boost/regex.hpp>
//#include <algorithm>   // transform()
//#include <cctype>      // toupper(), tolower()
//#include <functional>
#include <stdexcept>

#include <bmx/mxf_reader/MXFFileReader.h>
#include <bmx/mxf_reader/MXFGroupReader.h>
#include <bmx/mxf_reader/MXFSequenceReader.h>
#include <bmx/mxf_reader/MXFFrameMetadata.h>
#include <bmx/essence_parser/SoundConversion.h>
#include <bmx/MD5.h>
#include <bmx/CRC32.h>
#include <bmx/MXFUtils.h>
#include <bmx/Utils.h>
#include <bmx/Version.h>
#include <bmx/as11/AS11Info.h>
#include <bmx/BMXException.h>
#include <bmx/Logging.h>

#if defined(_WIN32)
#include <mxf/mxf_win32_file.h>
#endif

#include </home/dossantos/App/ebu-bmx/EBUCoreProcessor/include/EBUCoreProcessor.h>

#include <xercesc/dom/DOM.hpp>

/*! \class metadataWindow
 * \brief this class represent the player window
 *
 * This class controls all the player features and its links
 */


class metadataWindow : public Gtk::Window
{
public:
  /*!*
   * @brief Class constructor
   * @brief The metadataWindow class constructor instantiates a new EBUCore Metadata window and all its features.\n\n
	 * @param[in] cobject : GObject is the fundamental type providing the common attributes and methods for all object types in GTK+. In this case, cobject define the instantiate gtk widget 
	 * @param[in] refGlade : This is the reference to your glade design 
   */
  metadataWindow(BaseObjectType* cobject, 
							const Glib::RefPtr<Gtk::Builder>& refGlade);
  /*!*
   * @brief Class destructor
   * @brief The metadataWindow class destructor destroys all variables and instanciate objects of metadataWindow class. This is the garbage collector class. It is always called at the end of class life.
   *
   */
  virtual ~metadataWindow();
	static void progress_cb(float progress, EBUCore::ProgressCallbackLevel level, const char *function, const char *msg_format, ...);

	
	virtual void extractMetadata(const char* filename);
protected:
	/**
	 * @fn void refGladeWidgets(const Glib::RefPtr<Gtk::Builder>& refGlade)
   * @brief To link the glade references to the required player widgets
   * @brief This method loads all required glade's references to connect the player widgets. If you add a new widget to the media player into the Glade design, you must append his reference in this function.\n \n
	 * @brief Currently, the following widgets have been referenced :\n
	 * \li \c \b expander1 We use this widget to encapsulate the multiple levels of XML DOM 
	 * \li \c \b viewport1 We use this widget to encapsulate the expander1\n \n
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
	 * \li \c \b importXML When clicked, this button should import an XML metatada file into the metadata viewer 
	 * \li \c \b exportXML  When clicked, this button should export an XML metatada from metadata viewer into an XML file.
	 * \li \c \b modifyXML When clicked, this button should open a secondary metadata viewer where the user will modify the metadata of the first viewer.
	 * \li \c \b saveXML When clicked, this button will save the XML metadata into the MXF file.
	 * \li \c \b XMLconformance When clicked, this button starts an EBUCore conformance analysis on the metadata. \n \n
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
	 * \li \c \b importXML When connected, this button sends a signal to import an XML metatada file into the metadata first viewer 
	 * \li \c \b exportXML  When connected, this button sends a signal to export an XML metatada from metadata viewer into an XML file.
	 * \li \c \b modifyXML When connected, this button sends a signal to open a secondary metadata viewer where the user will modify the metadata of the first viewer.
	 * \li \c \b saveXML When connected, this button sends a signal to save the XML metadata into the currently loaded MXF file.
	 * \li \c \b XMLconformance When connected, this button sends a signal to run an EBUCore conformance analysis on the metadata.\n \n
	 * @param[in] void : No params required
   * @return  nothing if all is right or an error at compilation time.
	 * @note This function and his documentation must be completed
   */
	void connectSignalClicked(void);
	/**
	 * @fn void recursiveConstructTreeView(xercesc::DOMElement * el, Gtk::Expander * seed,  int depth)
   * @brief walk through the XML tree and create the encapsulted gtk expander
   * @brief 
	 * @note needs more documentation
	 * @param[in] el : xercesc::DOMElement *
	 * @param[in] seed : Gtk::Expander *
	 * @param[in] depth : int
   * @return nothing if all is right or an error at compilation time.
   */
	void recursiveConstructTreeView(xercesc::DOMElement * el, Gtk::Expander * seed, int depth);
	/**
	 * @fn void constructTreeView(Glib::ustring XMLfile)
   * @brief contruct and display a simili treeview compound of encapsulated gtk expander
   * @brief 
	 * @note needs more documentation
	 * @param[in] XMLfile : Glib::ustring
   * @return nothing if all is right or an error at compilation time.
   */
	void constructTreeView(Glib::ustring XMLfile);
	/**
	 * @fn void on_importXML_clicked(void)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_importXML_clicked(void);
	/**
	 * @fn void on_modifyXML_clicked(void)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_modifyXML_clicked(void);
	/**
	 * @fn void on_exportXML_clicked(void)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_exportXML_clicked(void);
	/**
	 * @fn void on_saveXML_clicked(void)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_saveXML_clicked(void);
	/**
	 * @fn void on_XMLconformance_clicked(void)
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	void on_XMLconformance_clicked(void);
	/**
	 * @fn virtual void on_openExpander_changed(Gtk::Expander * exp))
   * @brief 
   * @brief 
	 * @note needs more documentation
	 * @param[in] void : no params
   * @return nothing if all is right or an error at compilation time.
   */
	virtual void on_openExpander_changed(Gtk::Expander * exp);
	/**
	 * @fn std::string removeTags(std::string str)
   * @brief remove some tags
   * @brief 
	 * @note needs more documentation
	 * @param[in] str : a standard string 
   * @return nothing if all is right or an error at compilation time.
   */
	std::string removeTags(std::string str);
	/**
	 * @fn void on_extanderpressed_pressed(GdkEventButton* event,
																		std::vector<std::string> name, 
																		std::vector<std::string> value)
   * @brief callback when the extander is pressed
   * @brief display the attributes of EBUCore tag
	 * @note needs more documentation
	 * @param[in] event : 
	 * @param[in] name :  std::vector<std::string> contains attribue name
	 * @param[in] value :  std::vector<std::string> contains attributes value
   * @return nothing if all is right or an error at compilation time.
   */
	virtual bool on_extanderpressed_pressed(GdkEventButton* event, Gtk::Menu * Menu);
	/**
	 * @fn void configureEncapsultedBox(Gtk::Box * b)
   * @brief Configure the Gtk Box encapsulated in the Gtk Expander
   * @brief this function makes the configuration of the Gtk Box encapsulated in the Gtk Expander
	 * @note needs more documentation
	 * @param[in] b :  Pointer to a Gtk Box
   * @return nothing if all is right or an error at compilation time.
   */
	void configureEncapsultedBox(Gtk::Box * b);
	/**
	 * @fn void configureExpander(Gtk::Expander * e)
   * @brief Configure the Gtk Expander
   * @brief this function makes the configuration of the Gtk Expander encapsulated where will be encapsulated a Gtk Box.
	 * @note needs more documentation
	 * @param[in] b :  Pointer to a Gtk Expander
   * @return nothing if all is right or an error at compilation time.
   */
	void configureExpander(Gtk::Expander * e);
	/**
	 * @fn void createExpanderTextBox(Gtk::Expander * ed, Gtk::Box *bd, std::string txt)
   * @brief Create a text box in a Gtk Expander to display node text
   * @brief 
	 * @note needs more documentation
	 * @param[in] ed : Pointer to the destination Gtk Expander
	 * @param[in] bd :  Pointer to the destination Gtk Box
	 * @param[in] txt : This string contains the node text value
   * @return nothing if all is right or an error at compilation time.
   */
	void createExpanderTextBox(Gtk::Expander * ed, Gtk::Box *bd, std::string txt);	
	/**
	 * @fn void finishEncapsulation(Gtk::Box * root, Gtk::Expander * node)
   * @brief Finish the encapsultation of expanders and boxes.
   * @brief 
	 * @note needs more documentation
	 * @param[in] root : Pointer to the destination Gtk Box
	 * @param[in] node : Pointer to the destination Gtk Expander
   * @return nothing if all is right or an error at compilation time.
   */
	void finishEncapsulation(Gtk::Box * root, Gtk::Expander * node);	
	/**
	 * @fn void finishEncapsulation(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Expander * node)
   * @brief To create the attributes popup attached to an Gtk Expander
   * @brief 
	 * @note needs more documentation
	 * @param[in] root : Pointer to a collection of attributes nodes
	 * @param[in] node : Pointer to the destination Gtk Expander
   * @return nothing if all is right or an error at compilation time.
   */
	void createAttributesPopup(xercesc::DOMNamedNodeMap *dom_attrs, Gtk::Expander * node);
	bool isMXF(std::string str);
	// Glade reference
  Glib::RefPtr<Gtk::Builder> m_refGlade; /*!< Glade references */
	// Gtk Box
	Gtk::Box* metadata_box; /*!< Metadata Box */
	// Metadata Window
	Gtk::Button* modifyXML; /*!< edit an XML file button */
  Gtk::Button* importXML; /*!< import an XML file button */
  Gtk::Button* exportXML; /*!< export an XML file button */
  Gtk::Button* saveXML; /*!< save an XML file button */
  Gtk::Button* XMLconformance; /*!< EBUcore conformance button */
	Gtk::Viewport* viewport1; /*!< viewport1 the Gtk Viewport where is stored the expander */
	Gtk::Expander * Expander; /*!< Expander the Gtk Expander is the EBUCore Metadata xml tree root */
	
	Gtk::Expander * previousnode; /*!< previousnode Pointer to store temporarly the current node selected */
	xercesc::DOMDocument * metadata;

};

#endif
