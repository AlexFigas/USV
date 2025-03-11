#include "Settings.h"
#include "FileSystem.h"
#include "Display.h"

#include <Arduino_JSON.h>
#include <esp_system.h>
#include <Utils.h>
#include <tinyxml2.h>
#include <WiFi.h>

using namespace tinyxml2;

const char* settingsFileName = "/settings.xml";

static SettingsWiFi settingsWiFi;
static SettingsRange settingsSliderGloMot;
static SettingsExpandersPWM settingsExpandersPWM;
static SettingsExpandersIO settingsExpandersIO;
static SettingsServos settingsServos;
static SettingsCrossings settingsCrossings;

void SettingsWiFi::show() {
  DebugMessagePrintf( "{SSID: %s, Password: %s, AP Mode: %s, Address: %s, Gateway: %s, Network mask: %s}\n", this->ssid, this->password, this->apMode ? "true" : "false", this->address, this->gateway, this->network );
}

void SettingsRange::show() {
  DebugMessagePrintf( "{Units: %s, Min value: %d, Value: %d, Max value: %d}\n", this->units, this->min, this->value, this->max );
}

void SettingsExpanderPWM::show() {
  DebugMessagePrintf( "{Address: 0x%02X, Description: %s}\n", this->address, this->expanderDesc );
}

void SettingsExpandersPWM::show() {
  int size;
  if ( (size=this->expandersPWM.size())!=0 ) {
    DebugMessagePrintf( "[\n" );
    for( int idx=0; idx<size; ++idx) {
      this->expandersPWM.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsExpanderIO::show() {
  DebugMessagePrintf( "{Address: 0x%02X, IRQ pin: %d, Description: %s}\n", this->address, this->pinIRQ, this->expanderDesc );
}

void SettingsExpandersIO::show() {
  int size;
  if ( (size=this->expandersIO.size())!=0 ) {
    DebugMessagePrintf( "[\n" );
    for( int idx=0; idx<size; ++idx) {
      this->expandersIO.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsServo::show(){
  DebugMessagePrintf( "{\n" );    
  DebugMessagePrintf( "Name: %s\nDescription: %s\nExpander: %d\nIndex: %d\nSmooth movement=%s", this->servoName, this->servoDesc, this->expander, this->index, this->smooth==true ? "true" : "false" );
  DebugMessagePrintf( "Min position: " ); this->minPosition.show();
  DebugMessagePrintf( "Max position: " ); this->minPosition.show();
  DebugMessagePrintf( "}\n" ); 
}

void SettingsServos::show() {
  int size;
  if ( (size=this->servos.size())!=0 ) {
    DebugMessagePrintf( "[\n" );
    for( int idx=0; idx<size; ++idx) {
      this->servos.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsPortIn::show() {
  DebugMessagePrintf( "{IO Expander#: %d, Mask: 0x%02X}\n", this->expander, this->mask );
}

void SettingsPortOut::show() {
  DebugMessagePrintf( "{Servo#: %d}\n", this->servo );
}

void SettingsFence::show() {
  DebugMessagePrintf( "{Fence#: %d}\n", this->servoIndex );
}

void SettingsFences::show() {
  int size;
  if ( (size=this->fences.size())!=0 ) {
    DebugMessagePrintf( "[\n" );
    for( int idx=0; idx<size; ++idx) {
      this->fences.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsCrossing::show() {
  DebugMessagePrintf( "{\n" );
  DebugMessagePrintf( "Name: %s\nDescription: %s\npinDAC: %d\ntimerDAC: %d\nSound file: %s\n", this->crossingName, this->crossingDesc, this->pinDAC, this->timerDAC, this->soundFileName );
  DebugMessagePrintf( "Min position: " ); this->minVolume.show();
  DebugMessagePrintf( "Max position: " ); this->maxVolume.show();
  
  show( this->enter, "Enter ports:" );
  show( this->leave, "Leave ports:" );
  show( this->light, "Light ports:" );
  show( this->fences, "Fences:" );
  
  DebugMessagePrintf( "}\n" );
}

void SettingsCrossing::show(SimpleLinkedList<PtrSettingsPortIn>& ports, char * name) {
  int size;
  if ( (size=ports.size())!=0 ) {
    DebugMessagePrintf( "[\nList of %s\n", name );
    for( int idx=0; idx<size; ++idx) {
      ports.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsCrossing::show(SimpleLinkedList<PtrSettingsPortOut>& ports, char * name) {
  int size;
  if ( (size=ports.size())!=0 ) {
    DebugMessagePrintf( "[\nList of %s\n", name );
    for( int idx=0; idx<size; ++idx) {
      ports.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsCrossing::show(SimpleLinkedList<PtrSettingsFence>& fences, char * name) {
  int size;
  if ( (size=fences.size())!=0 ) {
    DebugMessagePrintf( "[\nList of %s\n", name );
    for( int idx=0; idx<size; ++idx) {
      fences.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

void SettingsCrossings::show() {
  int size;
  if ( (size=this->crossings.size())!=0 ) {
    DebugMessagePrintf( "[\n" );
    for( int idx=0; idx<size; ++idx) {
      this->crossings.get( idx )->show();
      DebugMessagePrintf( "\n" );
    }
    DebugMessagePrintf( "]\n" );
  }
}

// Private functions
static XMLElement* getElementRoot(XMLDocument& doc) {
  DebugMessagePrintf( "Settings::getRootElement(XMLDocument&)\n" );

  XMLDeclaration* declNode = doc.NewDeclaration( "xml version='1.0' encoding='UTF-8' standalone='no'" );
  XMLUnknown* typeNode = doc.NewUnknown( "DOCTYPE Settings SYSTEM 'settings.dtd'" );

  XMLNode* n1 = doc.InsertFirstChild( declNode );
  XMLNode* n2 = doc.InsertAfterChild( n1, typeNode);

  XMLElement* rootNode = doc.NewElement( "Settings" );

  doc.InsertAfterChild( n2, rootNode );

  //XMLElement* rootNode = doc.NewElement( "Settings" );
  //doc.InsertFirstChild( rootNode );

  return rootNode;
}

static XMLElement* getElementWiFi(XMLElement* rootNode, PtrSettingsWiFi settings) {
  DebugMessagePrintf( "Settings::getWiFiElement(%s, PtrSettingsWiFi)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "Wifi" );
  
  child->SetAttribute( "apMode", settings->apMode ? "1" : "0" );
  
  XMLElement * childSSID = child->InsertNewChildElement( "ssid" );
  childSSID->SetText( settings->ssid );

  XMLElement * childPassword = child->InsertNewChildElement( "password" );
  XMLElement * childAddress  = child->InsertNewChildElement( "address" );
  XMLElement * childGateway  = child->InsertNewChildElement( "gateway" );
  XMLElement * childNetwork  = child->InsertNewChildElement( "network" );
  
  if ( settings->apMode==true ) {
    childAddress->SetText( settings->address );
    childGateway->SetText( settings->gateway );
    childNetwork->SetText( settings->network );
  }
  else {
    childPassword->SetText( settings->password );
  }

  return child;
}

static XMLElement* getElementGlobalMotion(XMLElement* rootNode, PtrSettingsRange settings) {
  DebugMessagePrintf( "Settings::getGlobalMotionElement(%s, PtrSettingsRange)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "GlobalMotion" );
  XMLElement* childRange = child->InsertNewChildElement( "Range" );
  childRange->SetAttribute( "units", "ms" );
  
  XMLElement* childRangeMin = childRange->InsertNewChildElement( "min" );
  childRangeMin->SetText( settings->min );

  XMLElement* childRangeValue = childRange->InsertNewChildElement( "value" );
  childRangeValue->SetText( settings->value );

  XMLElement* childRangeMax = childRange->InsertNewChildElement( "max" );
  childRangeMax->SetText( settings->max );

  return child;
}

static XMLElement* getElementExpandersPWM(XMLElement* rootNode, PtrSettingsExpandersPWM settings) {
  DebugMessagePrintf( "Settings::getExpandersPWMElement(%s, PtrSettingsExpandersPWM)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "ExpandersPWM" );

  for(int i=0; i<settings->expandersPWM.size(); ++i ) {

    PtrSettingsExpanderPWM currentExpander = settings->expandersPWM.get( i );

    XMLElement* childExpander = child->InsertNewChildElement( "ExpanderPWM" );
    
    char * aux = format( "0x%02X", currentExpander->address );
    childExpander->SetAttribute( "address", aux );
    delete aux;

    if ( currentExpander->expanderDesc!=NULL ) {
      childExpander->SetAttribute( "desc", currentExpander->expanderDesc );
    }
  }

  return child;
}

static XMLElement* getElementExpandersIO(XMLElement* rootNode, PtrSettingsExpandersIO settings) {
  DebugMessagePrintf( "Settings::getExpandersIOElement(%s, PtrSettingsExpandersIO)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "ExpandersIO" );

  for(int i=0; i<settings->expandersIO.size(); ++i ) {

    PtrSettingsExpanderIO currentExpander = settings->expandersIO.get( i );

    XMLElement* childExpander = child->InsertNewChildElement( "ExpanderIO" );

    char * aux = format( "0x%02X", currentExpander->address );
    childExpander->SetAttribute( "address", aux );
    delete aux;

    childExpander->SetAttribute( "pinIRQ", currentExpander->pinIRQ );

    if ( currentExpander->expanderDesc!=NULL ) {
      childExpander->SetAttribute( "desc", currentExpander->expanderDesc );
    }
  }

  return child;
}

static XMLElement* getElementServos(XMLElement* rootNode, PtrSettingsServos settings) {
  DebugMessagePrintf( "Settings::getServosElement(%s, PtrSettingsServos)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "Servos" );

  for(int i=0; i<settings->servos.size(); ++i ) {

    PtrSettingsServo currentServo = settings->servos.get( i );

    XMLElement* childServo = child->InsertNewChildElement( "Servo" );
    childServo->SetAttribute( "name", currentServo->servoName );
    childServo->SetAttribute( "desc", currentServo->servoDesc );

    childServo->SetAttribute( "expander", currentServo->expander );
    childServo->SetAttribute( "index", currentServo->index );
    childServo->SetAttribute( "smooth", currentServo->smooth==true ? "1" : "0" );

    XMLElement* childRange1 = childServo->InsertNewChildElement( "Range" );
    childRange1->SetAttribute( "units", currentServo->minPosition.units );

    XMLElement* childRange1Min = childRange1->InsertNewChildElement( "min" );
    childRange1Min->SetText( currentServo->minPosition.min );

    XMLElement* childRange1Value = childRange1->InsertNewChildElement( "value" );
    childRange1Value->SetText( currentServo->minPosition.value );

    XMLElement* childRange1Max = childRange1->InsertNewChildElement( "max" );
    childRange1Max->SetText( currentServo->minPosition.max );

    XMLElement* childRange2 = childServo->InsertNewChildElement( "Range" );
    childRange2->SetAttribute( "units", currentServo->maxPosition.units );

    XMLElement* childRange2Min = childRange2->InsertNewChildElement( "min" );
    childRange2Min->SetText( currentServo->maxPosition.min );

    XMLElement* childRange2Value = childRange2->InsertNewChildElement( "value" );
    childRange2Value->SetText( currentServo->maxPosition.value );

    XMLElement* childRange2Max = childRange2->InsertNewChildElement( "max" );
    childRange2Max->SetText( currentServo->maxPosition.max );   
  }

  return child;
}

static XMLElement* getElementCrossings(XMLElement* rootNode, PtrSettingsCrossings settings) {
  DebugMessagePrintf( "Settings::getCrossingsElement(%s, PtrSettingsCrossings)\n", rootNode->Value() ) ;

  XMLElement* child = rootNode->InsertNewChildElement( "Crossings" );

  for(int i=0; i<settings->crossings.size(); ++i ) {
    PtrSettingsCrossing currentCrossing = settings->crossings.get( i );

    XMLElement* childCrossing = child->InsertNewChildElement( "Crossing" );

    childCrossing->SetAttribute( "name", currentCrossing->crossingName );
    childCrossing->SetAttribute( "desc", currentCrossing->crossingDesc );

    XMLElement* childSensors = childCrossing->InsertNewChildElement( "Sensors" );

    XMLElement* childSensorsEnter = childSensors->InsertNewChildElement( "Enter" );

    for(int iS=0; iS<currentCrossing->enter.size(); ++iS ) {
      XMLElement* elem = childSensorsEnter->InsertNewChildElement( "PortIn" );
      elem->SetAttribute( "expander", currentCrossing->enter.get( iS )->expander );

      char * aux = format( "0x%02X", currentCrossing->enter.get( iS )->mask );
      elem->SetAttribute( "mask", aux );
      delete aux;
    }

    XMLElement* childSensorsLeave = childSensors->InsertNewChildElement( "Leave" );
    for(int iS=0; iS<currentCrossing->leave.size(); ++iS ) {

      XMLElement* elem = childSensorsLeave->InsertNewChildElement( "PortIn" );
      elem->SetAttribute( "expander", currentCrossing->leave.get( iS )->expander );

      char * aux = format( "0x%02X", currentCrossing->leave.get( iS )->mask );
      elem->SetAttribute( "mask", aux );
      delete aux;
    }

    XMLElement* childSensorsLight = childSensors->InsertNewChildElement( "Light" );
    for(int iS=0; iS<currentCrossing->light.size(); ++iS ) {
      XMLElement* elem = childSensorsLight->InsertNewChildElement( "PortOut" );
      elem->SetAttribute( "servo", currentCrossing->light.get( iS )->servo );
    }

    XMLElement* childFences = childCrossing->InsertNewChildElement( "Fences" );
    for(int iF=0; iF<currentCrossing->fences.size(); ++iF ) {
      XMLElement* elem = childFences->InsertNewChildElement( "Fence" );
      elem->SetAttribute( "servo", currentCrossing->fences.get( iF )->servoIndex );
    }

    XMLElement* childSound = childCrossing->InsertNewChildElement( "Sound" );
    childSound->SetAttribute( "pinDAC", currentCrossing->pinDAC );
    childSound->SetAttribute( "timerDAC", currentCrossing->timerDAC );
    if ( currentCrossing->soundFileName!=NULL ) {
      childSound->SetAttribute( "soundFileName", currentCrossing->soundFileName );
    }

    XMLElement* childRange1 = childCrossing->InsertNewChildElement( "Range" );
    if ( currentCrossing->minVolume.units!=NULL ) {
      childRange1->SetAttribute( "units", currentCrossing->minVolume.units );
    }

    XMLElement* childRange1Min = childRange1->InsertNewChildElement( "min" );
    childRange1Min->SetText( currentCrossing->minVolume.min );

    XMLElement* childRange1Value = childRange1->InsertNewChildElement( "value" );
    childRange1Value->SetText( currentCrossing->minVolume.value );

    XMLElement* childRange1Max = childRange1->InsertNewChildElement( "max" );
    childRange1Max->SetText( currentCrossing->minVolume.max );

    XMLElement* childRange2 = childCrossing->InsertNewChildElement( "Range" );
    if ( currentCrossing->maxVolume.units!=NULL ) {
      childRange2->SetAttribute( "units", currentCrossing->maxVolume.units );
    }

    XMLElement* childRange2Min = childRange2->InsertNewChildElement( "min" );
    childRange2Min->SetText( currentCrossing->maxVolume.min );

    XMLElement* childRange2Value = childRange2->InsertNewChildElement( "value" );
    childRange2Value->SetText( currentCrossing->maxVolume.value );

    XMLElement* childRange2Max = childRange2->InsertNewChildElement( "max" );
    childRange2Max->SetText( currentCrossing->maxVolume.max );   
  }

  return child;
}

static char* readElement(XMLElement * parent, const char* elemName) {
  DebugMessagePrintf( "Settings::readElement(%s, %s)\n", parent->Value(), elemName);
  
  XMLElement * elem;
  elem = parent->FirstChildElement( elemName );
  //DebugMessagePrintf( "XMLElement(%s) %s present.\n", elemName, elem!=NULL ? "is" : "isn't" );

  const char* auxValue;
  auxValue = elem!=NULL ? elem->GetText() : "" ;
  //DebugMessagePrintf( "XMLElement(%s) value %s present (%s).\n", elemName, auxValue!=NULL ? "is" : "isn't", auxValue!=NULL ? auxValue : "N.A." );

  char* result;
  if ( auxValue!=NULL ) {
    result = new char[ strlen(auxValue) + 1 ];
    strcpy( result, (char*)auxValue );
  }
  else {
    result = new char[ 2 ];
    strcpy( result, "" );
  }

  return result;
}

static char* readAttribute(XMLElement * elem, const char* attributeName) {
  DebugMessagePrintf( "Settings::readAttribute(%s, %s)\n", elem->Value(), attributeName);
  
  const char* auxValue;
  auxValue = elem->Attribute( attributeName );
  //DebugMessagePrintf( "XMLElement(%s) attribute (%s) %s present (%s).\n", elem->Value(), attributeName, auxValue!=NULL ? "is" : "isn't", auxValue!=NULL ? auxValue : "N.A." );

  char* result;
  if ( auxValue!=NULL ) {
    result = new char[ strlen(auxValue) + 1 ];
    strcpy( result, (char*)auxValue );
  }
  else {
    result = new char[ 2 ];
    strcpy( result, "" );
  }

  return result;
}

static void readSettingsWiFi(XMLElement* elem, SettingsWiFi& settings) {
  settings.ssid     = readElement( elem, "ssid" );
  settings.apMode   = (String( elem->Attribute( "apMode" ) )).toInt() == 0 ? false : true;
  settings.password = readElement( elem, "password" );
  settings.address  = readElement( elem, "address" );
  settings.gateway  = readElement( elem, "gateway" );
  settings.network  = readElement( elem, "network" );
}

static void readSettingsRange(XMLElement* elem, SettingsRange& settings) {
  settings.units = readAttribute( elem, "units" );
  settings.min   = strtol( readElement( elem, "min" ), NULL, 10 );
  settings.value = strtol( readElement( elem, "value" ), NULL, 10 );
  settings.max   = strtol( readElement( elem, "max" ), NULL, 10 );
}

static PtrSettingsExpanderPWM readSettingsExpanderPWM(XMLElement* elem) {
  PtrSettingsExpanderPWM settings = new SettingsExpanderPWM();

  settings->expanderDesc = readAttribute( elem, "desc" );
  settings->address      = strtol( readAttribute( elem, "address" ), NULL, 16 );

  return settings;
}

static PtrSettingsExpanderIO readSettingsExpanderIO(XMLElement* elem) {
  PtrSettingsExpanderIO settings = new SettingsExpanderIO();

  settings->expanderDesc = readAttribute( elem, "desc" );
  settings->address      = strtol( readAttribute( elem, "address" ), NULL, 16 );
  settings->pinIRQ       = strtol( readAttribute( elem, "pinIRQ" ), NULL, 10 );

  return settings;
}

static PtrSettingsServo readSettingsServo(XMLElement* elem) {
  PtrSettingsServo settings = new SettingsServo();

  settings->servoName = readAttribute( elem, "name" );
  settings->servoDesc = readAttribute( elem, "desc" );

  settings->expander  = strtol( readAttribute( elem, "expander" ), NULL, 10 );
  settings->index     = strtol( readAttribute( elem, "index" ), NULL, 10 );
  settings->smooth    = (String( elem->Attribute( "smooth" ) )).toInt() == 0 ? false : true;

  XMLElement* elemMinPositionElem = elem->FirstChildElement();
  readSettingsRange( elemMinPositionElem, settings->minPosition );

  XMLElement* elemMaxPosition = elemMinPositionElem->NextSiblingElement();
  readSettingsRange( elemMaxPosition, settings->maxPosition );

  return settings;
}

static PtrSettingsPortIn readSettingsPortIn(XMLElement* elem) {
  PtrSettingsPortIn settings = new SettingsPortIn();

  settings->expander = strtol( readAttribute( elem, "expander" ), NULL, 10 );
  settings->mask     = strtol( readAttribute( elem, "mask" ), NULL, 16 );

  return settings;
}

static PtrSettingsPortOut readSettingsPortOut(XMLElement* elem) {
  PtrSettingsPortOut settings = new SettingsPortOut();

  settings->servo = strtol( readAttribute( elem, "servo" ), NULL, 10 );

  return settings;
}

static void readSettingsPorts(XMLElement* elemPorts, SimpleLinkedList<PtrSettingsPortIn>& portList) {
  for (XMLElement* elemPort = elemPorts->FirstChildElement( "PortIn" ); elemPort!= NULL; elemPort = elemPort->NextSiblingElement() )  {
    if ( elemPort!=NULL ) {
      PtrSettingsPortIn newPort = readSettingsPortIn( elemPort );
      portList.add( newPort );
    }
  }
}

static void readSettingsPorts(XMLElement* elemPorts, SimpleLinkedList<PtrSettingsPortOut>& portList) {
  for (XMLElement* elemPort = elemPorts->FirstChildElement( "PortOut" ); elemPort!= NULL; elemPort = elemPort->NextSiblingElement() )  {
    if ( elemPort!=NULL ) {
      PtrSettingsPortOut newPort = readSettingsPortOut( elemPort );
      portList.add( newPort );
      newPort->show();
    }
  }
}

static PtrSettingsFence readSettingsFence(XMLElement* elem) {
  PtrSettingsFence settings = new SettingsFence();

  settings->servoIndex = strtol( readAttribute( elem, "servo" ), NULL, 10 );

  return settings;
}

static void readSettingsFences(XMLElement* elemFences, SimpleLinkedList<PtrSettingsFence>& fencesList) {
  for (XMLElement* elemFence = elemFences->FirstChildElement( "Fence" ); elemFence!= NULL; elemFence = elemFence->NextSiblingElement() )  {
    if ( elemFence!=NULL ) {
      PtrSettingsFence newFence = readSettingsFence( elemFence );
      fencesList.add( newFence );
    }
  }
}

static PtrSettingsCrossing readSettingsCrossing(XMLElement* elem) {
  PtrSettingsCrossing settings = new SettingsCrossing();

  settings->crossingName = readAttribute( elem, "name" );
  settings->crossingDesc = readAttribute( elem, "desc" );

  XMLElement* sensors = elem->FirstChildElement( "Sensors" );
  readSettingsPorts( sensors->FirstChildElement( "Enter" ), settings->enter );
  readSettingsPorts( sensors->FirstChildElement( "Leave" ), settings->leave );
  readSettingsPorts( sensors->FirstChildElement( "Light" ), settings->light );

  XMLElement* fences = elem->FirstChildElement( "Fences" );
  readSettingsFences( fences, settings->fences );

  XMLElement* sound = elem->FirstChildElement( "Sound" );
  settings->pinDAC         = strtol( readAttribute( sound, "pinDAC" ), NULL, 10 );
  settings->timerDAC       = strtol( readAttribute( sound, "timerDAC" ), NULL, 10 );
  settings->soundFileName  = readAttribute( sound, "soundFileName" );

  XMLElement* elemMinVolume = elem->FirstChildElement( "Range" );
  readSettingsRange( elemMinVolume, settings->minVolume );

  XMLElement* elemMaxVolume = elemMinVolume->NextSiblingElement();
  readSettingsRange( elemMaxVolume, settings->maxVolume );

  return settings;
}

static void writeSettings(
    PtrSettingsWiFi settingsWifi, 
    PtrSettingsRange settingsRange,
    PtrSettingsExpandersPWM settingsExpandersPWM,
    PtrSettingsExpandersIO settingsExpandersIO,
    PtrSettingsServos settingsServos, 
    PtrSettingsCrossings settingsCrossings) {

  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsWiFi, PtrSettingsRange, PtrSettingsExpandersPWM, PtrSettingsExpandersIO, PtrSettingsServos, PtrSettingsCrossings)\n" );

  XMLDocument doc;
  XMLElement* rNode = getElementRoot( doc );
  
  getElementWiFi( rNode, settingsWifi );
  getElementGlobalMotion( rNode, settingsRange );

  XMLElement* expanders = rNode->InsertNewChildElement( "Expanders" );

  getElementExpandersPWM( expanders, settingsExpandersPWM );
  getElementExpandersIO( expanders, settingsExpandersIO );

  getElementServos( rNode, settingsServos );
  getElementCrossings( rNode, settingsCrossings );
  
  doc.Print( NULL );
  XMLPrinter prn;
  doc.Accept( &prn );

  writeFileContents( settingsFileName, String( prn.CStr() ) );
}

// Public functions
PtrSettingsWiFi getSettingsWiFi() {
  DebugMessagePrintf( "Settings::getSettingsWiFi()\n" );

  return &settingsWiFi;
}

PtrSettingsRange getSettingsGlobalMotion() {
  DebugMessagePrintf( "Settings::getSettingsGlobalMotion()\n" );

  return &settingsSliderGloMot;
}

PtrSettingsExpandersPWM getSettingsExpandersPWM() {
  DebugMessagePrintf( "Settings::getSettingsExpandersPWM()\n" );

  return &settingsExpandersPWM;
}

PtrSettingsExpandersIO getSettingsExpandersIO() {
  return &settingsExpandersIO;
}

PtrSettingsServos getSettingsServos() {
  return &settingsServos;
}

PtrSettingsCrossings getSettingsCrossings() {
  return &settingsCrossings;
}

void readSettings() {
  DebugMessagePrintf( "Settings::readSettings()\n" );
  XMLDocument doc;

  if ( doc.Parse( readFileContents( settingsFileName ).c_str() )!= XML_SUCCESS ) {
    DebugMessagePrintf( "Error parsing configuration file (%s).\n", settingsFileName );
    return;
  }

  DebugMessagePrintf( "Configuration file is loaded.\n" );
  doc.Print( NULL );

  /* Read WiFi settings - Begin */
    XMLElement* elemSettingsWiFi = doc.FirstChildElement( "Settings" )->FirstChildElement( "Wifi" );
    readSettingsWiFi( elemSettingsWiFi, settingsWiFi );

    settingsWiFi.show();
  /* Read WiFi settings - End */
  
  /* Read Global Motion settings - Begin */
    XMLElement* elemSettingsGobMot = doc.FirstChildElement( "Settings" )->FirstChildElement( "GlobalMotion" )->FirstChildElement( "Range" );
    readSettingsRange( elemSettingsGobMot, settingsSliderGloMot );

    settingsSliderGloMot.show();
  /* Read Global Motion settings - End */
  
  /* Read Expanders settings - Begin */
    XMLElement* elemSettingsExpanders = doc.FirstChildElement( "Settings" )->FirstChildElement( "Expanders" );

    /* Read PWM Expanders settings - Begin */
    XMLElement* elemSettingsExpandersPWM = elemSettingsExpanders->FirstChildElement( "ExpandersPWM" );

    for (XMLElement* elemExpander = elemSettingsExpandersPWM->FirstChildElement(); elemExpander!= NULL; elemExpander = elemExpander->NextSiblingElement() )  {
      if ( elemExpander!=NULL ) {
        PtrSettingsExpanderPWM expander = readSettingsExpanderPWM( elemExpander );
        settingsExpandersPWM.expandersPWM.add( expander );
      }
    }
    /* Read PWM Expanders settings - End */

    /* Read IO Expanders settings - Begin */
    XMLElement* elemSettingsExpandersIO = elemSettingsExpanders->FirstChildElement( "ExpandersIO" );

    for (XMLElement* elemExpander = elemSettingsExpandersIO->FirstChildElement(); elemExpander!= NULL; elemExpander = elemExpander->NextSiblingElement() )  {
      if ( elemExpander!=NULL ) {
        PtrSettingsExpanderIO expander = readSettingsExpanderIO( elemExpander );
        settingsExpandersIO.expandersIO.add( expander );
      }
    }
    /* Read IO Expanders settings - End */
  /* Read Expanders settings - End */

  /* Read Servos settings - Begin */
    XMLElement* elemSettingsServos = doc.FirstChildElement( "Settings" )->FirstChildElement( "Servos" );
    for (XMLElement* elemServo = elemSettingsServos->FirstChildElement(); elemServo!= NULL; elemServo = elemServo->NextSiblingElement() )  {
      if ( elemServo!=NULL ) {
        PtrSettingsServo newServo = readSettingsServo( elemServo );
        settingsServos.servos.add( newServo );
      }
    }
  /* Read Servos settings - End */

  /* Read Crossings settings - Begin */
    XMLElement* elemSettingsCrossings = doc.FirstChildElement( "Settings" )->FirstChildElement( "Crossings" );
    for (XMLElement* elemCrossing = elemSettingsCrossings->FirstChildElement(); elemCrossing!= NULL; elemCrossing = elemCrossing->NextSiblingElement() )  {
      if ( elemCrossing!=NULL ) {
        PtrSettingsCrossing newCrossing = readSettingsCrossing( elemCrossing );
        settingsCrossings.crossings.add( newCrossing );
      }
    }
  /* Read Crossings settings - End */
}

String getSettingsServosAsJSON() {
  DebugMessagePrintf( "Settings::getSettingsServosAsJSON()\n" );
  JSONVar result;

  for( int idx=0; idx<settingsServos.servos.size(); ++idx) {
    PtrSettingsServo currentServo = settingsServos.servos.get( idx );

    result[ idx ]["servo"][ "name" ]                    = currentServo->servoName;
    result[ idx ]["servo"][ "description" ]             = currentServo->servoDesc;
    result[ idx ]["servo"][ "smooth" ]                  = currentServo->smooth == true ? "true" : "false";

    result[ idx ]["servo"][ "minPosition" ][ "min" ]    = currentServo->minPosition.min;
    result[ idx ]["servo"][ "minPosition" ][ "value" ]  = currentServo->minPosition.value;
    result[ idx ]["servo"][ "minPosition" ][ "max" ]    = currentServo->minPosition.max; 
    result[ idx ]["servo"][ "minPosition" ][ "units" ]  = currentServo->minPosition.units; 

    result[ idx ]["servo"][ "maxPosition" ][ "min" ]    = currentServo->maxPosition.min;
    result[ idx ]["servo"][ "maxPosition" ][ "value" ]  = currentServo->maxPosition.value;
    result[ idx ]["servo"][ "maxPosition" ][ "max" ]    = currentServo->maxPosition.max; 
    result[ idx ]["servo"][ "maxPosition" ][ "units" ]  = currentServo->maxPosition.units; 
  }

  return JSON.stringify( result );
}

String getSettingsCrossingsAsJSON() {
  DebugMessagePrintf( "Settings::getSettingsCrossingsAsJSON()\n" );
  JSONVar result;

  for( int idx=0; idx<settingsCrossings.crossings.size(); ++idx) {
    PtrSettingsCrossing currentCrossing = settingsCrossings.crossings.get( idx );

    result[ idx ]["crossing"][ "name" ]                 = currentCrossing->crossingName;
    result[ idx ]["crossing"][ "description" ]          = currentCrossing->crossingDesc;

    result[ idx ]["crossing"][ "minVolume" ][ "min" ]   = currentCrossing->minVolume.min;
    result[ idx ]["crossing"][ "minVolume" ][ "value" ] = currentCrossing->minVolume.value;
    result[ idx ]["crossing"][ "minVolume" ][ "max" ]   = currentCrossing->minVolume.max; 
    result[ idx ]["crossing"][ "minVolume" ][ "units" ] = currentCrossing->minVolume.units; 

    result[ idx ]["crossing"][ "maxVolume" ][ "min" ]   = currentCrossing->maxVolume.min;
    result[ idx ]["crossing"][ "maxVolume" ][ "value" ] = currentCrossing->maxVolume.value;
    result[ idx ]["crossing"][ "maxVolume" ][ "max" ]   = currentCrossing->maxVolume.max; 
    result[ idx ]["crossing"][ "maxVolume" ][ "units" ] = currentCrossing->maxVolume.units; 
  }

  return JSON.stringify( result );
}

void writeSettings(PtrSettingsWiFi settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsWiFi)\n" );

  writeSettings( 
    settings, 
    getSettingsGlobalMotion(), 
    getSettingsExpandersPWM(), 
    getSettingsExpandersIO(), 
    getSettingsServos(), 
    getSettingsCrossings()
  );
}

void writeSettings(PtrSettingsRange settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsRange)\n" );

  writeSettings( 
    getSettingsWiFi(), 
    settings, 
    getSettingsExpandersPWM(), 
    getSettingsExpandersIO(), 
    getSettingsServos(), 
    getSettingsCrossings()
 );
}

void writeSettings(PtrSettingsExpandersPWM settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsExpanderPWM)\n" );

  writeSettings( 
    getSettingsWiFi(), 
    getSettingsGlobalMotion(), 
    settings, 
    getSettingsExpandersIO(), 
    getSettingsServos(), 
    getSettingsCrossings()
 );
}

void writeSettings(PtrSettingsExpandersIO settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsExpanderIO)\n" );

  writeSettings( 
    getSettingsWiFi(), 
    getSettingsGlobalMotion(), 
    getSettingsExpandersPWM(), 
    settings, 
    getSettingsServos(), 
    getSettingsCrossings()
 );
}

void writeSettings(PtrSettingsServos settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsServos)\n" );

  writeSettings( 
    getSettingsWiFi(), 
    getSettingsGlobalMotion(),
    getSettingsExpandersPWM(), 
    getSettingsExpandersIO(),
    settings, 
    getSettingsCrossings()
 );
}

void writeSettings(PtrSettingsCrossings settings) {
  DebugMessagePrintf( "Settings::writeSettings(PtrSettingsCrossings)\n" );

  writeSettings( 
    getSettingsWiFi(), 
    getSettingsGlobalMotion(),
    getSettingsExpandersPWM(), 
    getSettingsExpandersIO(),
    getSettingsServos(),
    settings
  );
}
