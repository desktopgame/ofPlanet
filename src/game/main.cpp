#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <glm/glm.hpp>
#include <ofMain.h>
#include <iostream>

#include "ofApp.h"

#include "../world/TextureInfoCollection.hpp"
#include "../world/BlockInfoCollection.hpp"

#include "luaex/luaex.hpp"
#include "luaex/luaimpl.hpp"

//========================================================================
int main() {
        ofGLFWWindowSettings settings;
        settings.glVersionMajor = 4;
        settings.glVersionMinor = 4;
        settings.setSize(1280,720);
        settings.windowMode = ofWindowMode::OF_WINDOW;
		settings.title = "ofPlanet";
#ifdef _DEBUG
		ofSetLogLevel(ofLogLevel::OF_LOG_FATAL_ERROR);
#endif
        ofCreateWindow(settings);
        ofRunApp(new planet::ofApp());
}
