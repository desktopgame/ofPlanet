#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include <ofMain.h>

#include <glm/glm.hpp>
#include <iostream>

#include "../world/BlockInfoCollection.hpp"
#include "../world/TextureInfoCollection.hpp"
#include "luaex/luaex.hpp"
#include "luaex/luaimpl.hpp"
#include "ofApp.h"

//========================================================================
int main() {
        ofGLFWWindowSettings settings;
		settings.glVersionMajor = 4;
		settings.glVersionMinor = 1;
        settings.setSize(800, 600);
        settings.windowMode = ofWindowMode::OF_WINDOW;
        settings.title = "ofPlanet";
#ifdef _DEBUG
        ofSetLogLevel(ofLogLevel::OF_LOG_FATAL_ERROR);
#endif
        ofCreateWindow(settings);
        ofRunApp(new planet::ofApp());
}
