#ifndef CPP_EBUCOREPARSER_CLASS_H
#define CPP_EBUCOREPARSER_CLASS_H

/*!
 * \project_name EBU Player
 * \file EBUCoreParser.hpp
 * \brief EBUCore Schema Parser specifications
 * \details This class is used to parse and load the EBUCore Schema
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 26 september 2012
 * \copyright GNU GPLv3
 *
 */

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <xercesc/dom/DOM.hpp>

/*! \class ebucoreParser
 * \brief this class represent the ebucore parser
 *
 * This class parses and loads the ebucore elements and their attributes
 */

class ebucoreParser {
	public:
  ebucoreParser(void);
	~ebucoreParser(void);
protected:
	int getSchemas (std::string dir, std::vector<std::string> &files);
	bool isExtension(std::string str, std::string extension);
	bool isSimpleType(std::string str);
	bool isDCSimpleType(std::string str);
	int isUnbounded(std::string max);

	bool elementExist(std::string str);
	int findElement(std::string str);
	bool groupExist(std::string str);
	int findGroup(std::string str);
	std::string removePrefix(std::string str, std::string prefix);
	void extractSchema(std::string pathtofile);
	std::vector<std::string > generateChild(std::string father, xercesc::DOMElement * el);
	std::vector<std::string > generateChild(xercesc::DOMElement * el);
	void copyChildren (std::string elementname);
	void copyAttributes (std::string elementname);
	void copyTypes (std::string elementname);
	void copyOccurencies (std::string elementname);
	void copyChildren (int position);
	void copyAttributes (int position);
	void copyTypes (int position);
	void copyOccurencies (int position);

	std::vector<std::string > copyLocalChildren (int position);
	std::vector<std::string > copyLocalAttributes (int position);
	std::vector<std::string > copyLocalTypes (int position);
	std::vector<int> copyLocalOccurencies (int position);


	struct AttributeStruct {
		std::string name;
		std::string type;
		bool hasDefaultValue;
		std::string defaultValue;
		bool hasEnumeration;
		std::vector<std::string> Enumeration;
	};

	struct ElementStruct {
		std::string name;
		std::list<AttributeStruct> attribute;
		std::list<ElementStruct> children;		
		int minCardinality;
		int maxCardinality;	
	};

	std::vector<std::string > DCAttribute(void);
	std::string DCType(void);
	std::vector<std::string > generateChildText(void);
	std::list<AttributeStruct> * generateAttributes(std::string father, xercesc::DOMElement * el);
	void generateAttributes(std::string father, xercesc::DOMElement * el, std::list<AttributeStruct> * att);
	std::list<ElementStruct> generateChildren(std::string father, xercesc::DOMElement * el);

	std::string identifyType(xercesc::DOMElement * el);
	std::vector<std::string> listEnumeration(xercesc::DOMElement * el);
	void constructSchema(xercesc::DOMElement * el, ElementStruct * root);

	void printElements(void);
	void printChildren(int position);
	void printAttributes(int position);

	std::vector<xercesc::DOMElement *> schemasRef;
	std::vector<std::string> name;
	std::vector<std::string> group;
	std::vector<std::vector<std::string > > child;
	std::vector<std::vector<std::string > > attribute;
	std::vector<std::vector<std::string > > type;
	std::vector<std::vector<int> > occurency;

	std::vector<ElementStruct *> ebucoremodel;

int cpti;
};

#endif
