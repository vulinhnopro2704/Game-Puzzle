#pragma once
#ifndef COMMON_FUNCTION_H_

#define COMMON_FUNCTION_H_

#include <windows.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <queue>
#include <vector>
#include <set>
#include <utility>
#include "GameObject.h"
#include <string>
#include "Gameplay.h"
#include <SDL_ttf.h>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

//Window chung
extern SDL_Window* gWindow;

//Renderer chung
extern SDL_Renderer* gRenderer;

//Sự kiện chung
extern SDL_Event* g_Event;

//Font
extern TTF_Font* gFont;

//Kiểm tra thoát game
extern bool outGame;

//Screen

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int FPS = 60;
const int frameDelay = 1000 / FPS;

const int TOTAL_IMAGE = 4;

//Button constants
const int BUTTON_WIDTH = 470;
const int BUTTON_HEIGHT = 93;
const int TOTAL_BUTTONS = 3;
extern int n;
extern int Mode;
extern int Order;
extern bool isUSE;
extern bool isInit;

//Trạng thái của button
enum LButtonSprite
{
    BUTTON_DEFAULT = -1,
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

enum MenuButtons {
    //BUTTON_RELOAD = ,
    BUTTON_AUTO_RUN = 0,
    BUTTON_SELF_SOLVING,
    /*BUTTON_BACK,*/
    TOTAL_GAMEPLAY_BUTTONS = 2
};


#endif

//typedef union SDL_Event
//{
//    Uint32 type;                    /**< Event type, shared with all events */
//    SDL_CommonEvent common;         /**< Common event data */
//    SDL_WindowEvent window;         /**< Window event data */
//    SDL_KeyboardEvent key;          /**< Keyboard event data */
//    SDL_TextEditingEvent edit;      /**< Text editing event data */
//    SDL_TextInputEvent text;        /**< Text input event data */
//    SDL_MouseMotionEvent motion;    /**< Mouse motion event data */
//    SDL_MouseButtonEvent button;    /**< Mouse button event data */
//    SDL_MouseWheelEvent wheel;      /**< Mouse wheel event data */
//    SDL_JoyAxisEvent jaxis;         /**< Joystick axis event data */
//    SDL_JoyBallEvent jball;         /**< Joystick ball event data */
//    SDL_JoyHatEvent jhat;           /**< Joystick hat event data */
//    SDL_JoyButtonEvent jbutton;     /**< Joystick button event data */
//    SDL_JoyDeviceEvent jdevice;     /**< Joystick device change event data */
//    SDL_ControllerAxisEvent caxis;      /**< Game Controller axis event data */
//    SDL_ControllerButtonEvent cbutton;  /**< Game Controller button event data */
//    SDL_ControllerDeviceEvent cdevice;  /**< Game Controller device event data */
//    SDL_QuitEvent quit;             /**< Quit request event data */
//    SDL_UserEvent user;             /**< Custom event data */
//    SDL_SysWMEvent syswm;           /**< System dependent window event data */
//    SDL_TouchFingerEvent tfinger;   /**< Touch finger event data */
//    SDL_MultiGestureEvent mgesture; /**< Gesture event data */
//    SDL_DollarGestureEvent dgesture; /**< Gesture event data */
//    SDL_DropEvent drop;             /**< Drag and drop event data */
//
//    /* This is necessary for ABI compatibility between Visual C++ and GCC
//       Visual C++ will respect the push pack pragma and use 52 bytes for
//       this structure, and GCC will use the alignment of the largest datatype
//       within the union, which is 8 bytes.
//
//       So... we'll add padding to force the size to be 56 bytes for both.
//    */
//    Uint8 padding[56];
//} SDL_Event;