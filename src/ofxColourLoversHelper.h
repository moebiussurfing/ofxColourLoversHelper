
#pragma once

#include "ofMain.h"

//----
//
#define USE_OFX_IM_GUI
// 
// and uncomment this line only if you want to handle the ImGui instance 
// out of-the-addon, ie: into ofApp instead:
////#define USE_OFX_IM_GUI_EXTERNAL //-> comment required to enable the one only / local ImGui

#define USE_FLOATING_SEARCH_WINDOW_WORAROUND
#define MAX_SEARCH_RESULTS 100

//----

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
//#ifdef USE_OFX_IM_GUI
//#include "ofxImGui.h"
//#endif

#include "ofxColourLovers.h"

class ofxColourLoversHelper {

public:

	ofxColourLoversHelper();
	~ofxColourLoversHelper();

	void setup();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	//void draw();
	void exit();
	void windowResized(int w, int h);

	bool isTextInputTyping() {
		return bCheckMouseOverTextInputLovers;
	}

private:
	bool bShowSearch = true;
	// shows advancded panels to tweak layout or workflow behaviour

private:
	ofParameter<bool> bGui_Search{ "API SEARCH", false };

	ofParameter<bool> bFavorites{ "FAVORITES", false };
	ofParameter<bool> bHistory{ "HISTORY", false };
	ofParameter<bool> bSearch{ "SEARCH", false };

	ofEventListener listener_bFavorites;
	ofEventListener listener_bHistory;
	ofEventListener listener_bSearch;

	//--

public:
	ofParameterGroup params;

private:
#ifdef USE_OFX_IM_GUI

	ofxSurfing_ImGui_Manager guiManager;

	void setup_ImGui();
	void draw_ImGui_Search();
	void draw_ImGui_Main();
	void draw_ImGui_Kit();
	void draw_ImGui_Browser();
#endif

	std::string textInput_temp1 = "";
	std::string textInput_temp1_PRE = "-1";

	ofParameter<bool> bGui_KitPalettes{ "Kit Palettes", true };
	ofParameter<bool> AutoScroll{ "AutoScroll", true };

	ofDirectory _files;

private:
	void Changed_ColourLovers(ColourLoveEvent &e);

	//-

	ImVec4 borderLineColor{ 1,1,1,0.5 };
	float borderLineWidth = 2.0;

	bool bfocus = false;

public:
	ofParameter<bool> SHOW_AdvancedLayout{ "Show Advanced", false };

	//--

	// API

public:
	void setup(glm::vec2 _position, glm::vec2 _size);
	void setGrid(glm::vec2 _position, glm::vec2 _size);
	void setPosition(glm::vec2 _position, glm::vec2 _size);

	void setToggleVisible();
	bool isVisible() {
		return bGui;
	}

	void setVisible(bool b);
	void setVisibleSearcher(bool b) {
		bSearcherVisible = b;
	}

private:
	bool bSearcherVisible = true;

	// colour lovers browsing
public:
	void nextPalette(bool cycled = false);
	void prevPalette();

	void refreshPalette();
	void randomPalette();

	//--

	// pointers back to 'communicate externally'
public:
	void setColorPtr(ofColor &c);
	void setPalettePtr(vector<ofColor> &p);
	void setPaletteNamePtr(std::string &n);
	void setPalette_BACK_Refresh(bool &b);
	void setColor_BACK_Refresh(bool &b);

	// pointers back to 'communicate externally'
private:
	ofColor *myColor_BACK;
	vector<ofColor> *myPalette_BACK;
	std::string *myPalette_Name_BACK;
	bool *bUpdated_Palette_BACK;
	bool *bUpdated_Color_BACK;

	//----

public:
	ofParameter<bool> MODE_PickPalette_BACK{ "Pick Palette", true };
	ofParameter<bool> MODE_PickColor_BACK{ "Pick Color", true };
	ofParameter<std::string> lastMenuTab_Str{ "_MenuTabPick", "" };

	void setEnableKeys(bool b) {
		bKeys = b;
		if (b)
		{
			addKeysListeners();
		}
		else
		{
			removeKeysListeners();
		}
	}

private:
	ofParameter<bool> bKeys{ "Enable Keys", true };

private:
	ofParameter<bool> MODE_FixedSize{ "Fixed Width", false };
	ofParameter<bool> MODE_Slim{ "Slim", false };

	ofColor lastColor_clicked;

	int amountResults = (int)MAX_SEARCH_RESULTS;
	//ofParameter<int> amountResults{ "Amount Results", MAX_SEARCH_RESULTS, 10, 300 };

	ofParameter<int> amnt { "Amount", 20, 10, 200 };
	ofParameter<int> page { "Page", 0, 0, 10 };
	
	//-

	void setPalette(int p);
	int palettesX;

	int currPalette; //last picked palette/color
	vector<ColourLovePalette> palettes;
	//vector<ColourLovePalette> palettesSearch;

	bool bUpdateSearchFlag;
	void build_Gui_Lab();

	std::string lastSearch_Str;
	std::string lastSearch_PRE = "-1";

public:
	ofParameter<std::string> lastPaletteName{ "PaletteName", "" };
	//std::string lastPaletteName = "";

private:
	void loadFavorites();
	void loadHistory();
	void clearFavourites();
	void clearHistory();

	std::string path_Global;
	std::string path_AppSettings;

	ofColor colorMarked;

	//---

	ofParameter<bool> bGui{ "COLOUR-LOVERS", true };
	//bool bGui = true;
	bool isKeysEnabled = true;

	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 gridPosition;
	glm::vec2 gridSize;

	void addKeysListeners();
	void removeKeysListeners();

	void mouseDragged(ofMouseEventArgs& eventArgs);
	void mousePressed(ofMouseEventArgs& eventArgs);
	void mouseReleased(ofMouseEventArgs& eventArgs);

	void addMouseListeners();
	void removeMouseListeners();

	vector<std::string> pNames;
	std::string pName;

	bool bCheckMouseOverTextInputLovers = false;
	bool bTextInputActive = false;

	bool bSearching = false;

public:
	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);
};
