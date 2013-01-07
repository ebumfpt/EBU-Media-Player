/*!
 * \project_name EBU Player
 * \file ebucoreParser.cpp
 * \brief EBUCore Parser body
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \dateOfCreation 20 august 2012
 * \dateOfUpdate 26 september 2012
 * This software is published in LGPLv3.0
 *
 */

// include files and libraries
#include "ebucoreParser.hpp"


// class constructor
ebucoreParser::ebucoreParser(void)
{
	// EBUcore Schemas folder
	std::string dir = std::string("./EBUCoreSchema");
	// Schemas filenames
	std::vector<std::string> files = std::vector<std::string>();
	cpti=0;
	// Get the schemas filenames
	getSchemas(dir,files);
	// Debug output
	std::cout<<"I have "<<files.size()<<" schemas files in memory."<<std::endl;
	for (unsigned int i = 0;i < files.size();i++) {
		std::cout << files[i] << std::endl;
		std::cout<< "start parsing..."<<std::endl;
		extractSchema(dir+"/"+files[i]);
	}
//	printElements();
std::cout<<"J'ai en mémoire : "<<cpti<<" éléments."<<std::endl;
std::cout<<" number of model : "<<ebucoremodel.size()<<std::endl;
}

// class destructor
ebucoreParser::~ebucoreParser(void)
{
	schemasRef.clear();
	name.clear();
	child.clear();
	attribute.clear();
	type.clear();
	occurency.clear();
}

int ebucoreParser::getSchemas (std::string dir, std::vector<std::string> &files) {
	DIR *dp;
	struct dirent *dirp;
	// if opening folder fail then
	if((dp = opendir(dir.c_str())) == NULL) {
		// display error message
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}
	// while not the end of folder
	while ((dirp = readdir(dp)) != NULL) {
		// if it is am xsd file then
		if (dirp->d_type == DT_REG and isExtension(dirp->d_name, "xsd"))
			// save the schema filename inside the vector
			files.push_back(std::string(dirp->d_name));
	}
	closedir(dp); // close folder
	return 0;
}

bool ebucoreParser::isExtension(std::string str, std::string extension) {
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

bool ebucoreParser::elementExist(std::string str) {
	for (std::vector<std::string>::iterator it = name.begin() ; it != name.end(); ++it) {
		if (*it == str) { return true; }
	}
  return false;
}

int ebucoreParser::findElement(std::string str) {
	int position = 0;
	for (std::vector<std::string>::iterator it = name.begin() ; it != name.end(); ++it) {
		if (*it == str) { return position; }
		position++; 
	}
  return -1;
}

bool ebucoreParser::groupExist(std::string str) {
	for (std::vector<std::string>::iterator it = group.begin() ; it != group.end(); ++it) {
		if (*it == str) { return true; }
	}
  return false;
}

int ebucoreParser::findGroup(std::string str) {
	int position = 0;
	for (std::vector<std::string>::iterator it = group.begin() ; it != group.end(); ++it) {
		if (*it == str) { return position; }
		position++; 
	}
  return -1;
}

std::string ebucoreParser::removePrefix(std::string str, std::string prefix) {
	// find last "." position in a string
	int lastindex = str.find_last_of(prefix);
	// copy what comes after last dot
	std::string str2 = str.substr (lastindex+1,str.size()-(lastindex+1));
	// compare the file extension to the extension pattern
  return str2;
}

void ebucoreParser::extractSchema(std::string pathtofile) {
	// init xercesc
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::DOMImplementation* dom_xsd = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode(""));
	xercesc::DOMLSParser* dom_file = dom_xsd->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	// extract xml DOM document and element
	xercesc::DOMDocument* schema_doc  = dom_file->parseURI(pathtofile.c_str());
	xercesc::DOMElement*  schema_root = schema_doc->getDocumentElement();
	// put the cursor at the beginning
	schema_root = schema_root->getFirstElementChild();
	schema_root = schema_root->getNextElementSibling()->getNextElementSibling();
	// generate and store the ebucore root element
	ebucoreParser::ElementStruct * element;
	// save the root
	std::cout<<"here..."<<std::endl;
	ebucoremodel.push_back(element);	
	std::cout<<"here 2..."<<std::endl;
	// construct the skelleton
	constructSchema(schema_root, element);
	std::cout<<"here 3..."<<std::endl;
}

std::vector<std::string > ebucoreParser::generateChild(std::string father, xercesc::DOMElement * el) {
	std::vector<std::string > children;
	xercesc::DOMElement * tmpEl = el;
	//std::cout<<"J'entre"<<std::endl;
	while ((std::string)(xercesc::XMLString::transcode((tmpEl->getParentNode())->getNodeName())) != "schema") {
		tmpEl = dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode());
	}
tmpEl=(dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode()))->getFirstElementChild();
	//std::cout<<"Je suis à la racine"<<std::endl;
	//std::cout<<"... et je suis actuellement dans le node : "<< (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) <<std::endl;
	//std::cout<<"And my name is : "<<xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))<<std::endl;
	while (tmpEl != 0 and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father)) {
		tmpEl = tmpEl->getNextElementSibling();
	}	
	
	//if (tmpEl == 0) {std::cout<<"aaah ba..."<<father<<"cogito, ergo sum : "<<groupExist(father)<<std::endl;}
	//std::cout<<"J'ai trouvé mon complextype..."<<std::endl;
	tmpEl=tmpEl->getFirstElementChild();	
	//if (tmpEl == 0) {std::cout<<"aaah ba..."<<std::endl;}
	//std::cout<<"... et je suis rentré dedans"<<std::endl;
	
	//		std::cout<<"... et je suis actuellement dans le node : "<< (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) <<std::endl;
	if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexContent") {
		while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice") {
			tmpEl = tmpEl->getNextElementSibling();
		}
		if (tmpEl == 0) { return children; }
		//std::cout<<"J'ai trouvé ma séquence/choix..."<<std::endl;
		tmpEl=tmpEl->getFirstElementChild();	
		//std::cout<<"... et j'y rentre"<<std::endl;
		do {
			if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
				std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
				std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
				//std::cout<<"   "<<((name.size()>0)?name:ref)<<std::endl;
				children.push_back( (name.size()>0)?name:ref);
			}
			tmpEl = tmpEl->getNextElementSibling();
		} while (tmpEl != 0);
	}
	//std::cout<<"I'm here"<<std::endl; 
	return children;
}

std::vector<std::string > ebucoreParser::generateChild(xercesc::DOMElement * el) {
	std::vector<std::string > children;
	xercesc::DOMElement * tmpEl = el;
	std::cout<<"J'entre"<<std::endl;
	while (tmpEl != 0 and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType")) {
		tmpEl = tmpEl->getNextElementSibling();
	}	
	std::cout<<"... et je suis actuellement dans le node : "<< (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) <<std::endl;
	//if (tmpEl == 0) {std::cout<<"aaah ba..."<<father<<"cogito, ergo sum : "<<groupExist(father)<<std::endl;}
	//std::cout<<"J'ai trouvé mon complextype..."<<std::endl;
	while (tmpEl != 0 and ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "extension")) {
		tmpEl=tmpEl->getFirstElementChild();
	}
	//if (tmpEl == 0) {std::cout<<"aaah ba..."<<std::endl;}
	//std::cout<<"... et je suis rentré dedans"<<std::endl;
	std::cout<<"... et je suis actuellement dans le node : "<< (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) <<std::endl;
	//		std::cout<<"... et je suis actuellement dans le node : "<< (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) <<std::endl;
	std::string mytype (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("base"))));

	children = copyLocalChildren(findGroup(mytype));

	std::cout<<"J'ai fini de copier"<<std::endl;
	while (tmpEl != 0 and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence" and (std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "choice") {
		std::cout<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
		tmpEl = tmpEl->getFirstElementChild();
	}
	std::cout<<"Je suis arrivé à la sequence...........<<<"<<std::endl;
	std::cout<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
	if (tmpEl == 0) { return children; }	
	std::cout<<"J'ai encore des enfants"<<std::endl;
	//std::cout<<"... et j'y rentre"<<std::endl;
	do {
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			//std::cout<<"   "<<((name.size()>0)?name:ref)<<std::endl;
			children.push_back( (name.size()>0)?name:ref);
		}
		tmpEl = tmpEl->getNextElementSibling();
	} while (tmpEl != 0);
	std::cout<<"I'm here"<<std::endl; 
	return children;
}

std::list<ebucoreParser::AttributeStruct> * ebucoreParser::generateAttributes(std::string father, xercesc::DOMElement * el) {
	//std::cout<<"I'm trying to generate the element attribute list of : "<<father<<std::endl;
	std::list<ebucoreParser::AttributeStruct> * attributes;
	xercesc::DOMElement * tmpEl = el;
	//std::cout<<"back to the schema root"<<std::endl;
	while ((std::string)(xercesc::XMLString::transcode((tmpEl->getParentNode())->getNodeName())) != "schema") {
		tmpEl = dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode());
	}
	//std::cout<<"go to the first children of the schema"<<std::endl;
	tmpEl=(dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode()))->getFirstElementChild();	
	//std::cout<<"find the complextype who has the name of the father"<<std::endl;
	//std::cout<<"TagName : "<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
	//std::cout<<"AttributeName : "<<(std::string)xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))<<std::endl;
	while ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father ) {
		tmpEl = tmpEl->getNextElementSibling();
		//std::cout<<"TagName : "<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
	//std::cout<<"AttributeName : "<<(std::string)xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))<<std::endl;
	}	
	//std::cout<<"I found my complexType"<<std::endl;

	tmpEl=tmpEl->getFirstElementChild();	

	
//std::cout<< " my attribute list : ";
	do {
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "attribute") {
			ebucoreParser::AttributeStruct attr;
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));	
			std::string byDefault (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("default"))));

			attr.name = ((name.size()>0)?name:ref);
			attr.type = ((type.size()>0)?type:"string");
			attr.hasDefaultValue = ((byDefault.size()>0)?true:false);
			attr.defaultValue = ((byDefault.size()>0)?byDefault:"NULL");
///////////////////////
			attr.hasEnumeration = false;
			std::vector<std::string> attrEnum;
			attr.Enumeration = attrEnum;
////////////////////////
			//std::cout<<((name.size()>0)?name:ref)<<" of type "<<((type.size()>0)?type:"string")<<" with "<<((byDefault.size()>0)?byDefault:"NULL")<<" as default value "<<", ";
			attributes->push_back(attr);
		}
		tmpEl = tmpEl->getNextElementSibling();
	} while (tmpEl != 0);
	
	return attributes;
}


void ebucoreParser::generateAttributes(std::string father, xercesc::DOMElement * el, std::list<ebucoreParser::AttributeStruct> * att) {
	xercesc::DOMElement * tmpEl = el;
	//std::cout<<"back to the schema root"<<std::endl;
	while ((std::string)(xercesc::XMLString::transcode((tmpEl->getParentNode())->getNodeName())) != "schema") {
		tmpEl = dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode());
	}
	//std::cout<<"go to the first children of the schema"<<std::endl;
	tmpEl=(dynamic_cast<xercesc::DOMElement*>(tmpEl->getParentNode()))->getFirstElementChild();	
	//std::cout<<"find the complextype who has the name of the father"<<std::endl;
	//std::cout<<"TagName : "<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
	//std::cout<<"AttributeName : "<<(std::string)xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))<<std::endl;
	while ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father ) {
		tmpEl = tmpEl->getNextElementSibling();
		//std::cout<<"TagName : "<<(std::string)(xercesc::XMLString::transcode(tmpEl->getTagName()))<<std::endl;
	//std::cout<<"AttributeName : "<<(std::string)xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name")))<<std::endl;
	}	
	//std::cout<<"I found my complexType"<<std::endl;

	tmpEl=tmpEl->getFirstElementChild();	

	
//std::cout<< " my attribute list : ";
	do {
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "attribute") {
			ebucoreParser::AttributeStruct attr;
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			std::string ref (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("ref"))));
			std::string type (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("type"))));	
			std::string byDefault (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("default"))));

			attr.name = ((name.size()>0)?name:ref);
			attr.type = ((type.size()>0)?type:"string");
			attr.hasDefaultValue = ((byDefault.size()>0)?true:false);
			attr.defaultValue = ((byDefault.size()>0)?byDefault:"NULL");
///////////////////////
			attr.hasEnumeration = false;
			std::vector<std::string> attrEnum;
			attr.Enumeration = attrEnum;
////////////////////////
			//std::cout<<((name.size()>0)?name:ref)<<" of type "<<((type.size()>0)?type:"string")<<" with "<<((byDefault.size()>0)?byDefault:"NULL")<<" as default value "<<", ";
			att->push_back(attr);
		}
		tmpEl = tmpEl->getNextElementSibling();
	} while (tmpEl != 0);
}

std::list<ebucoreParser::ElementStruct> ebucoreParser::generateChildren(std::string father, xercesc::DOMElement * el) {

	std::list<ebucoreParser::ElementStruct> children;
	xercesc::DOMElement * tmpEl = el;

	while ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "complexType" or xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))) != father ) {
		tmpEl = tmpEl->getNextElementSibling();
	}
	tmpEl=tmpEl->getFirstElementChild();

	while ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) != "sequence") {
		tmpEl = tmpEl->getNextElementSibling();
	}
	tmpEl=tmpEl->getFirstElementChild();

	do {
		if ((std::string)(xercesc::XMLString::transcode(tmpEl->getTagName())) == "element") {
			std::string name (xercesc::XMLString::transcode(tmpEl->getAttribute (xercesc::XMLString::transcode("name"))));
			ebucoreParser::ElementStruct internalChildren;
			internalChildren.name = name;
			children.push_back(internalChildren);
		}	
		tmpEl = tmpEl->getNextElementSibling();
	} while (tmpEl != 0);

	return children;

}

void ebucoreParser::copyChildren(std::string elementname) {
	child.push_back(child.at(findElement(elementname)));
}
void ebucoreParser::copyAttributes(std::string elementname) {
	attribute.push_back(attribute.at(findElement(elementname)));
}
void ebucoreParser::copyTypes(std::string elementname) {
	type.push_back(type.at(findElement(elementname)));
}
void ebucoreParser::copyOccurencies(std::string elementname) {
	occurency.push_back(occurency.at(findElement(elementname)));
}

void ebucoreParser::copyChildren(int position) {
	child.push_back(child.at(position));
}
void ebucoreParser::copyAttributes(int position) {
	attribute.push_back(attribute.at(position));
}
void ebucoreParser::copyTypes(int position) {
	type.push_back(type.at(position));
}
void ebucoreParser::copyOccurencies(int position) {
	occurency.push_back(occurency.at(position));
}

std::vector<std::string > ebucoreParser::copyLocalChildren(int position) {
	return child.at(position);
}
std::vector<std::string > ebucoreParser::copyLocalAttributes(int position) {
	return attribute.at(position);
}
std::vector<std::string > ebucoreParser::copyLocalTypes(int position) {
	return type.at(position);
}
std::vector<int> ebucoreParser::copyLocalOccurencies(int position) {
	return occurency.at(position);
}

std::vector<std::string > ebucoreParser::DCAttribute(void) {
	std::vector<std::string > attr;
	attr.push_back("xml:lang");
	return attr;
}

std::string ebucoreParser::DCType(void) {
	return "Dublin Core";
}

std::vector<std::string> ebucoreParser::listEnumeration(xercesc::DOMElement * el) {
	std::vector<std::string> listEnum;
	while (el != 0) {
		std::cout<<"Enum :: "<<(std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value")))<<std::endl; 
		listEnum.push_back((std::string)xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("value"))));
		el = el->getNextElementSibling();
	}
	return listEnum;
}

std::string ebucoreParser::identifyType(xercesc::DOMElement * el) {
	//std::cout<<"hello world !!! 111"<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
	while (el != 0 and (std::string)(xercesc::XMLString::transcode(el->getTagName())) != "complexType" and (std::string)(xercesc::XMLString::transcode(el->getTagName())) != "simpleType" ) {
		el = el->getNextElementSibling();
	}
	//std::cout<<"hello world !!! 222"<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
	if (el != 0) {
		//std::cout<<"hello world !!! "<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
		std::string ct(xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
		//std::cout<<"hello world !!! "<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
		if (ct.size()> 0) {
			//std::cout<<"hello world !!! "<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
			return ct;
		} else {
			//std::cout<<"hello world qqqq!!! "<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
			while (el != 0 and ((std::string)(xercesc::XMLString::transcode(el->getTagName())) != "extension" and (std::string)(xercesc::XMLString::transcode(el->getTagName())) != "attributeGroup") and (std::string)(xercesc::XMLString::transcode(el->getTagName())) != "restriction")  {
				//std::cout<<"hello world !!! xxx"<<(std::string)(xercesc::XMLString::transcode(el->getTagName()))<<std::endl;
				if ((std::string)(xercesc::XMLString::transcode(el->getTagName())) == "complexType" or (std::string)(xercesc::XMLString::transcode(el->getTagName())) == "complexContent" or (std::string)(xercesc::XMLString::transcode(el->getTagName())) == "simpleContent" or (std::string)(xercesc::XMLString::transcode(el->getTagName())) == "simpleType") {
					el = el->getFirstElementChild();
				} else {
					el = el->getNextElementSibling();
				}
			}
			if (el != 0) {
				std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
				std::string base (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("base"))));
				return ((ref.size()>0)?ref:base);
			} else {
					return "NULL";
			}
		}
		return (std::string)(xercesc::XMLString::transcode(el->getTagName()));
	}
	return "NULL";
}

void ebucoreParser::constructSchema(xercesc::DOMElement * el, ebucoreParser::ElementStruct * root) {
	
		
		std::cout<<"i start constructing..."<<std::endl;
		
		std::string str (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
		std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));
		std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
		std::string min (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("minOccurs"))));
		std::string max (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("maxOccurs"))));


	std::cout<<"I write"<<std::endl;

		root->name = (str.size()>0)?str:ref;
	
	std::cout<<"name..."<<str<<std::endl;
		root->minCardinality = ((min.size()>0)?atoi(min.c_str()):1);

	std::cout<<" min..."<<min<<std::endl;
		root->maxCardinality = ((max.size()>0)?isUnbounded(max):1);

	std::cout<<"max..."<<max<<std::endl;

	std::cout<<"type -> "<<type<<std::endl;
	std::list<ebucoreParser::AttributeStruct> att;
	generateAttributes(removePrefix (type, ":"), el, &root->attribute);


	std::cout<<"attribute..."<<std::endl;
		root->children = generateChildren(removePrefix (type, ":"), el);

	std::cout<<"I add cpt"<<std::endl;
		cpti++;
			
		// verification

	std::cout<<"I verify..."<<std::endl;
		std::cout<<"element : "<<root->name<<std::endl;
		std::cout<<"   cardinality : ["<<root->minCardinality<<", "<<root->maxCardinality<<"]"<<std::endl;
		std::cout<<"   attributes : "<<root->attribute.size()<<std::endl;
		for (std::list<ebucoreParser::AttributeStruct>::iterator it=root->attribute.begin(); it != root->attribute.end(); ++it) {
    	std::cout << "     - " <<it->name<<" : "<<it->type<<" / "<<((it->hasDefaultValue)?it->defaultValue:"undefined")<<" / "<<((it->hasEnumeration)?"yes":"no")<<std::endl;
		}
		std::cout<<"   children : "<<root->children.size()<<std::endl;
		for (std::list<ebucoreParser::ElementStruct>::iterator it=root->children.begin(); it != root->children.end(); ++it) {
    	std::cout << "     - " <<it->name<<std::endl;
		}
}

/*void ebucoreParser::recursiveConstructSchema(xercesc::DOMElement * el) {
	do {
		if (el->hasChildNodes() and el->getChildElementCount() != 0) {
			// create a new expander to store the node children
			if ((std::string)(xercesc::XMLString::transcode(el->getTagName())) == "element") {
				std::string str (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
				std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));
				std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
				if (!elementExist (  (str.size() > 0)? str : ref)){
					name.push_back((str.size() > 0)? str : ref);
					//std::cout<<"1 - my name is : "<<std::endl<<"   "<<((str.size() > 0)? str : ref)<<" and my type is : "<<type<<std::endl;
					//std::cout<<str<<std::endl;
					if (isDCSimpleType((type.size()>0)?type:ref)) {
						std::cout<<"name : "<<str<<" type:"<<type<<" ref:"<<ref<<" IF transcode type : -->"<<isDCSimpleType((type.size()>0)?type:ref)<<std::endl;
						child.push_back(generateChildText());
						attribute.push_back(DCAttribute());
					} else {
						std::cout<<"name : "<<str<<" type:"<<type<<" ref:"<<ref<<" ELSE"<<std::endl;
						//std::cout<<"1 - My children are : "<<std::endl;
						if (type.size()>0) {
							if (!groupExist(type)) {
							child.push_back(generateChild(removePrefix (type, ":"), el) );
							//std::cout<<"1 - My attributes are : "<<std::endl;
							attribute.push_back(generateAttributes(removePrefix (type, ":"), el));
							} else {
								copyChildren(findGroup(type));
								copyAttributes(findGroup(type));
							}
						} else {
							if (ref.size()>0) {
							} else {
								if (el->hasChildNodes() and el->getChildElementCount() != 0) {
									std::cout<<"Fonction principal : entrée"<<std::endl;
									child.push_back(generateChild(el->getFirstElementChild()));
									std::cout<<"Fonction principal : sortie"<<std::endl;
								}
								std::cout<<"Fonction principal : sortie 1"<<std::endl;
							}
							std::cout<<"Fonction principal : sortie 2"<<std::endl;
						}
						std::cout<<"Fonction principal : sortie 3"<<std::endl;
					}

					std::cout<<"Fonction principal : sortie 4 + début de stockage de type : "<<type<<std::endl;
group.push_back(type);

					std::cout<<"Fonction principal : sortie 4 + fin de stockage de type : "<<type<<std::endl;
//					attribute.push_back(generateAttributes(removePrefix (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))), ":"), el));
				} else {
					// copy
					//std::cout<<"I must copy..."<<std::endl;
					copyChildren(findElement((str.size() > 0)? str : ref));
					copyAttributes(findElement((str.size() > 0)? str : ref));
				}
			}
			// visit the first children of the current node
			recursiveConstructSchema(el->getFirstElementChild());
		} else {
			// create a new expander to store the node children
			if ((std::string)(xercesc::XMLString::transcode(el->getTagName())) == "element") {
				std::string str (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("name"))));
				std::string type (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))));
				std::string ref (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("ref"))));
				if (!elementExist (  (str.size() > 0)? str : ref)) {// and !elementExist(str)) {			
					name.push_back((str.size() > 0)? str : ref);
					//std::cout<<"2 - my name is : "<<std::endl<<"   "<<((str.size() > 0)? str : ref)<<" and my type is : "<<type<<std::endl;
//".... and : "<<xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type")))<<" and am I a simple type ? "<<((isSimpleType(xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type")))))? "true":"false")<<std::endl;
					if (isDCSimpleType((type.size()>0)?type:ref)) {
						//std::cout<<"2 - I have no child : "<<str<<std::endl;
						child.push_back(generateChildText());
						//std::cout<<"2 - I have no attribute : "<<str<<std::endl;
						attribute.push_back(DCAttribute());
					} else {
						//std::cout<<"2 - My children are : "<<std::endl;
						if (!groupExist(type)) {
							//std::cout<<"My group  doesnt exist..."<<std::endl;
							//std::cout<<"My group type is : "<<xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type")))<<std::endl;
							//std::cout<<"My group type without prefix is : "<<removePrefix (xercesc::XMLString::transcode(el->getAttribute (xercesc::XMLString::transcode("type"))), ":")<<std::endl;
							child.push_back(generateChild(removePrefix (type, ":"), el) );
						//std::cout<<"2 - My attributes are : "<<std::endl;
						attribute.push_back(generateAttributes(removePrefix (type, ":"), el));
						} else {							
							//std::cout<<"My group exist..."<<std::endl;
							copyChildren(findGroup(type));
							copyAttributes(findGroup(type));
						}
					}
					
					group.push_back(type);
				} else {
					copyChildren(findElement((str.size() > 0)? str : ref));
					copyAttributes(findElement((str.size() > 0)? str : ref));
				}
			}
		}
		// next node at this level
		el = el->getNextElementSibling();
	} while (el != 0);
}
*/
bool ebucoreParser::isSimpleType(std::string str) {
	// compare the file extension to the extension pattern
  return (str.find_first_of(":")==std::string::npos) ? true	: false;
}

bool ebucoreParser::isDCSimpleType(std::string str) {
	// compare the file extension to the extension pattern
  return (str.find_first_of("dc:") == 0) ? true : false;
}

int ebucoreParser::isUnbounded(std::string max) {
	return ((max == "unbounded")?-1:std::atoi(max.c_str()));
}



void ebucoreParser::printElements(void) {
	int i = 0;
	for (std::vector<std::string>::iterator it = name.begin() ; it != name.end(); ++it) {
			std::cout<<"Element : ";
			std::cout<<*it<<std::endl;
	//		printAttributes(i);
		//	std::cout<<std::endl;
	//		printChildren(i++);
			std::cout<<std::endl<<"................................................."<<std::endl;
	}
	std::cout<<"Il y a "<<i<<" elements"<<std::endl;
}

void ebucoreParser::printChildren(int position) {
	std::cout<<" * Children : ";
	int i=0;
	for (std::vector<std::string>::iterator it = child.at(position).begin() ; it != child.at(position).end(); ++it) {			
			if (i++>0) { std::cout<<", "; }
			std::cout<<*it;
	}
	if (i==0) {std::cout<<"no child";}
}

void ebucoreParser::printAttributes(int position) {
	std::cout<<" * Attributes : ";
	int i=0;
	for (std::vector<std::string>::iterator it = attribute.at(position).begin() ; it != attribute.at(position).end(); ++it) {			
			if (i++>0) { std::cout<<", "; }
			std::cout<<*it;
	}
	if (i==0) {std::cout<<"no attribute";}
}
