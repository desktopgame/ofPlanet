#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <glm/glm.hpp>
#include <ofMain.h>
#include <iostream>

#include "Screen.hpp"
#include "ofApp.h"

#include "../world/TextureInfoCollection.hpp"
#include "../world/BlockInfoCollection.hpp"

#include "luaex/luaex.hpp"
#include "luaex/luaimpl.hpp"

//========================================================================
int main() {
        ofGLFWWindowSettings settings;
        settings.resizable = false;
        settings.glVersionMajor = 4;
        settings.glVersionMinor = 4;
        settings.setSize(planet::Screen::widthi(), planet::Screen::heighti());
        settings.windowMode = ofWindowMode::OF_WINDOW;
		settings.title = "ofPlanet";
        planet::Screen::fullScreenMode = false;
#ifdef _DEBUG
		ofSetLogLevel(ofLogLevel::OF_LOG_FATAL_ERROR);
#endif
        ofCreateWindow(settings);
        ofRunApp(new planet::ofApp());
}
