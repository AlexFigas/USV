#ifndef _Settings_h_
#define _Settings_h_

#include "Utils.h"
#include <Arduino.h>
#include <SimpleLinkedList.h>

/* WiFi settings */
typedef struct
{
    char* ssid;
    char* password;
    bool apMode;
    char* address;
    char* gateway;
    char* network;

    void show();
} SettingsWiFi, *PtrSettingsWiFi;

/* Range/Slider settings */
typedef struct
{
    char* units;
    int min;
    int value;
    int max;

    void show();
} SettingsRange, *PtrSettingsRange;

/* PWM Expander settings */
typedef struct
{
    int address;
    char* expanderDesc;

    void show();
} SettingsExpanderPWM, *PtrSettingsExpanderPWM;

/* Set of PWM Expander settings */
typedef struct
{
    SimpleLinkedList<PtrSettingsExpanderPWM> expandersPWM;

    void show();
} SettingsExpandersPWM, *PtrSettingsExpandersPWM;

/* IO Expander settings */
typedef struct
{
    int address;
    int pinIRQ;
    char* expanderDesc;

    void show();
} SettingsExpanderIO, *PtrSettingsExpanderIO;

/* Set of IO Expander settings */
typedef struct
{
    SimpleLinkedList<PtrSettingsExpanderIO> expandersIO;

    void show();
} SettingsExpandersIO, *PtrSettingsExpandersIO;

/* Servo settings */
typedef struct
{
    char* servoName;
    char* servoDesc;
    int expander;
    int index;
    bool smooth;
    SettingsRange minPosition;
    SettingsRange maxPosition;

    void show();
} SettingsServo, *PtrSettingsServo;

/* Set of Servo settings */
typedef struct
{
    SimpleLinkedList<PtrSettingsServo> servos;

    void show();
} SettingsServos, *PtrSettingsServos;

/* Input Port settings */
typedef struct
{
    int expander;
    int mask;

    void show();
} SettingsPortIn, *PtrSettingsPortIn;

/* Output Port settings */
typedef struct
{
    int servo;

    void show();
} SettingsPortOut, *PtrSettingsPortOut;

/* Crossing Fence (servo number ) settings */
typedef struct
{
    int servoIndex;

    void show();
} SettingsFence, *PtrSettingsFence;

/* Set of Crossing Fence (servo number ) settings */
typedef struct
{
    SimpleLinkedList<PtrSettingsFence> fences;

    void show();
} SettingsFences, *PtrSettingsFences;

/* Crossing settings */
typedef struct
{
    char* crossingName;
    char* crossingDesc;
    int pinDAC;
    int timerDAC;
    char* soundFileName;
    SimpleLinkedList<PtrSettingsPortIn> enter;
    SimpleLinkedList<PtrSettingsPortIn> leave;
    SimpleLinkedList<PtrSettingsPortOut> light;
    SimpleLinkedList<PtrSettingsFence> fences;
    SettingsRange minVolume;
    SettingsRange maxVolume;

    void show();

  private:
    void show(SimpleLinkedList<PtrSettingsPortIn>& ports, char* name);

    void show(SimpleLinkedList<PtrSettingsPortOut>& ports, char* name);

    void show(SimpleLinkedList<PtrSettingsFence>& fences, char* name);
} SettingsCrossing, *PtrSettingsCrossing;

/* Set of Crossing settings */
typedef struct
{
    SimpleLinkedList<PtrSettingsCrossing> crossings;

    void show();
} SettingsCrossings, *PtrSettingsCrossings;

void readSettings();

PtrSettingsWiFi getSettingsWiFi();

PtrSettingsRange getSettingsGlobalMotion();

PtrSettingsExpandersPWM getSettingsExpandersPWM();

PtrSettingsExpandersIO getSettingsExpandersIO();

PtrSettingsServos getSettingsServos();

PtrSettingsCrossings getSettingsCrossings();

String getSettingsServosAsJSON();

String getSettingsCrossingsAsJSON();

void writeSettings(PtrSettingsWiFi settings);

void writeSettings(PtrSettingsRange settings);

void writeSettings(PtrSettingsExpandersPWM settings);

void writeSettings(PtrSettingsExpandersIO settings);

void writeSettings(PtrSettingsServos settings);

void writeSettings(PtrSettingsCrossings settings);

#endif  // _Settings_h_
