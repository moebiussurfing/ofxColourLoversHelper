#include "ofxColourLoversHelper.h"

//--------------------------------------------------------------
ofxColourLoversHelper::ofxColourLoversHelper()
{
	// default settings
	position = glm::vec2(1000, 10);
	size = glm::vec2(200, 400);
	gridPosition = glm::vec2(1210, 10);
	gridSize = glm::vec2(200, ofGetHeight());

	//-

	//// set positions and panel sizes
	//glm::vec2 sizeGui(150, 400);
	//glm::vec2 sizeGrid(150, ofGetHeight());
	//glm::vec2 posGui(ofGetWidth() - (sizeGui.x + sizeGrid.x + 4), 0);
	//glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);

	////must be called before setup() to overwrite default settings
	//setGrid(posGrid, sizeGrid);
	//setup(posGui, sizeGui);

	//-

	colorMarked = (ofColor::white);

	//setVisible(true);
	addMouseListeners();

	//setKeysEnabled(true);
	addKeysListeners();
	isKeysEnabled = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setToggleVisible()
{
	bIsVisible = !bIsVisible;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setVisible(bool b)
{
	bIsVisible = b;

	//-

#ifdef USE_OFX_UI
	if (gui_Lab) gui_Lab->setVisible(bIsVisible);
	//gui->setVisible(bIsVisible);
	if (bIsVisible && gui)
		setVisibleSearcher(bSearcherVisible);
#endif

	//-

	if (bIsVisible)
	{
		addMouseListeners();

		addKeysListeners();
		isKeysEnabled = true;
		//setKeysEnabled(true);
	}
	else
	{
		removeMouseListeners();

		removeKeysListeners();
		isKeysEnabled = false;
		//setKeysEnabled(false);
	}
}
#ifdef USE_OFX_IM_GUI
//--------------------------------------------------------------
void ofxColourLoversHelper::drawImGui()
{
	auto mainSettings = ofxImGui::Settings();

#ifndef USE_OFX_IM_GUI_EXTERNAL
	gui_ImGui.begin();
#endif

	//-

	////ImGui::SetNextWindowSize(ofVec2f(200, 100), ImGuiCond_FirstUseEver);
	//mainSettings.windowPos = ofVec2f(100, 10);
	//mainSettings.windowSize = ofVec2f(300, 500);
	//mainSettings.lockPosition = false;
	//mainSettings.windowBlock = false;

	//widgets size
	float h = 20;
	float w = 170;
	//float w = ImGui::GetWindowWidth();

	//-

	//if (ofxImGui::BeginWindow("COLOUR-LOVERS", mainSettings, false))
	{
		if (ofxImGui::BeginWindow("SEARCH", mainSettings, false))
		{
			//ImGui::Text("COLOUR LOVERS");

			//-

			//search
			ImGui::Text("Keyword:");
			//std::string textInput_temp1 = "";

			char tab1[32];
			strncpy(tab1, textInput_temp1.c_str(), sizeof(tab1));
			tab1[sizeof(tab1) - 1] = 0;

			if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1)))
			{
				textInput_temp1 = ofToString(tab1);
				ofLogNotice(__FUNCTION__) << "input: " << textInput_temp1;

				//ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab1);
				//lastSearch_PRE = lastSearch;

				if (textInput_temp1 != textInput_temp1_PRE) {
					textInput_temp1_PRE = textInput_temp1;
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("SEARCH"))
			{
				ofLogNotice(__FUNCTION__) << "searchPalettes: " << textInput_temp1_PRE;
				ofxColourLovers::searchPalettes(textInput_temp1_PRE, amountResults);
				lastSearch = "'" + textInput_temp1_PRE + "'";

				//textInput_temp1_PRE = textInput_temp1 = "";//clear
			}

			//-

			//hide to debug and simplify
	//#define EXTEND_SEARCH
#ifdef EXTEND_SEARCH
		//lover
			ImGui::Text("Lover Id:");
			std::string textInput_temp2 = "";

			char tab2[32];
			strncpy(tab2, textInput_temp2.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;

			if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab2);
				textInput_temp2 = ofToString(tab2);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp2;

				lastSearch = textInput_temp2;
				std::string s = textInput_temp2;
				ofStringReplace(s, " ", "%20");
				ofxColourLovers::getTopPalettesForLover(s, amountResults);
			}

			//-

			//palette
			ImGui::Text("Palette Id:");
			std::string textInput_temp3 = "";

			char tab3[32];
			strncpy(tab3, textInput_temp3.c_str(), sizeof(tab3));
			tab3[sizeof(tab3) - 1] = 0;

			if (ImGui::InputText("", tab3, IM_ARRAYSIZE(tab3)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab3);
				textInput_temp3 = ofToString(tab3);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp3;

				lastSearch = textInput_temp3;
				ofxColourLovers::getPalette(lastSearch);
			}
#endif

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ImGui::Button("FAVS", ImVec2(w * 0.5, h)))
			{
				loadFavourites();
			}
			ImGui::SameLine();
			if (ImGui::Button("HISTORY", ImVec2(w * 0.5, h)))
			{
				loadHistory();
			}

			//bool MODE_fixedSize_PRE = MODE_fixedSize;
			//if (ImGui::Checkbox("FIXED WIDTH", &MODE_fixedSize))
			//{
			//	if (MODE_fixedSize != MODE_fixedSize_PRE)
			//	{
			//		build_Gui_Lab();
			//	}
			//}

			ImGui::Dummy(ImVec2(0.0f, 5));

			ImGui::Text("Name:");
			ImGui::Text(lastPaletteName.c_str());

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ImGui::Button("FAV +", ImVec2(w, h)))
			{
				ofxSurfingHelpers::CheckFolder(path + "favourites/");

				std::string str = path + "favourites/" + palettes[currPalette].id + ".xml";

				palettes[currPalette].save(str);
				ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
			}
			if (ImGui::Button("FAV -", ImVec2(w, h)))
			{
				clearFavourites();

				//workflow
				loadFavourites();
			}
			if (ImGui::Button("CLEAR HISTORY", ImVec2(w, h)))
			{
				clearHistory();

				//workflow
				loadHistory();
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ImGui::Checkbox("PICK PALETTE", &MODE_PickPalette_BACK))
			{
			}
			if (ImGui::Checkbox("PICK COLOR", &MODE_PickColor_BACK))
			{
			}

			ImGui::SliderInt("Amnt Max", &amountResults, 5, 100);
		}
		ofxImGui::EndWindow(mainSettings);


		//----

		//ImGui::SetNextWindowSize(ofVec2f(200, 100), ImGuiCond_FirstUseEver);
		//mainSettings.windowPos = ofVec2f(500, 10);
		//mainSettings.windowSize = ofVec2f(300, 500);
		//mainSettings.lockPosition = false;
		//mainSettings.windowBlock = false;

		ImGuiColorEditFlags colorEdiFlags = false;
		int ww = w / 5;

		//if (ofxImGui::BeginWindow(lastSearch.c_str(), mainSettings, false))
		if (ofxImGui::BeginWindow("BROWSE", mainSettings, false))
		{
			ImGui::Dummy(ImVec2(0.0f, 5));
			ImGui::Text(lastSearch.c_str());

			ImGui::Dummy(ImVec2(0.0f, 5));
			
			//ImGui::Text("Name:"); //ImGui::SameLine();
			//ImGui::Dummy(ImVec2(0.0f, 5));
			
			ImGui::Text(lastPaletteName.c_str());
			
			ImGui::Dummy(ImVec2(0.0f, 5));

			bool MODE_fixedSize_PRE = MODE_fixedSize;

			//if (ImGui::Checkbox("Fixed Width", &MODE_fixedSize))
			if (ofxImGui::AddParameter(MODE_fixedSize))
			{
				if (MODE_fixedSize != MODE_fixedSize_PRE)
				{
					build_Gui_Lab();
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			for (int i = 0; i < palettes.size(); i++)
			{
				int wc = 0;

				// colors in each palette
				int numOfColorsInPalette = palettes[i].colours.size();
				for (int c = 0; c < numOfColorsInPalette; c++)
				{
					if (c != 0) ImGui::SameLine();

					ImGui::PushID(c);

					if (!MODE_fixedSize)
					{
						// different sizes with original colourLover Palettes
						wc = palettes[i].colorWidths[c] * w;
					}
					else
					{
						// same size for each color
						wc = w / numOfColorsInPalette;
					}

					std::string name = ("CL_" + ofToString(i) + "_" + ofToString(c));

					//-

					// draw border to selected color/palette
					bool bDrawBorder = false;
					if (i == currPalette)
					{
						bDrawBorder = true;
					}
					if (bDrawBorder)
					{
						//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, .40));//black
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, .50));//white
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
						//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					}

					//-

					//colored box
					if (ImGui::ColorButton(name.c_str(),
						palettes[i].colours[c],
						colorEdiFlags,
						ImVec2(wc, ww)))
					{
						//-

						// 1. filter name

						std::string whatList = name.substr(0, 2);
						std::string whatColId = name.substr(3, name.length() - 3);

						vector<std::string> seg = ofSplitString(name, "_");
						int pId = ofToInt(seg[1]);
						int cId = ofToInt(seg[2]);
						ColourLovePalette p = palettes[pId];

						lastPaletteName = "'" + p.title + "'";

#ifdef USE_OFX_UI
						lastPaletteName_UI->setLabel(lastPaletteName);
#endif
						//-

						// 2. pick palette and color

						//lastColor_clicked = palettes[i].colours[c];
						lastColor_clicked = p.colours[cId];

						// set BACK color clicked
						if (myColor_BACK != nullptr && MODE_PickColor_BACK)
						{
							myColor_BACK->set(lastColor_clicked);
						}

						// flag updater color ready
						if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
						{
							(*bUpdated_Color_BACK) = true;
						}

						//-

						// 3. set palette
						//currPalette = i;

						currPalette = pId;
						setPalette(currPalette);

						ofLogNotice(__FUNCTION__) << "palette Id : " << pId;
						ofLogNotice(__FUNCTION__) << "currPalette: " << currPalette;

						refreshPalette();
					}

					//-

					if (bDrawBorder)
					{
						ImGui::PopStyleColor();
						ImGui::PopStyleVar(1);
					}

					//-

					ImGui::PopID();
				}
			}
		}
		ofxImGui::EndWindow(mainSettings);
	}
	//ofxImGui::EndWindow(mainSettings);

	//-

#ifndef USE_OFX_IM_GUI_EXTERNAL
	gui_ImGui.end();
#endif
}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::setup()
{
	//--

#ifndef USE_OFX_IM_GUI_EXTERNAL
#ifdef USE_OFX_IM_GUI
	ofxSurfingHelpers::ImGui_FontCustom();

	gui_ImGui.setup();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().MouseDrawCursor = false;
	ImGui::GetIO().ConfigWindowsResizeFromEdges = true;

	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
#endif
#endif

	//--

#ifdef USE_OFX_UI

	if (gui_Lab)
	{
		ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
		delete gui_Lab;
		gui_Lab = 0;
	}
	if (gui)
	{
		ofRemoveListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);

		ofRemoveListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);
		delete gui;
		gui = 0;
	}

	//-

	int width = size.x;
	int xInit = 6;
	int dim = 30;

	gui = new ofxUICanvas(position.x, position.y, size.x, size.y);

	// custom bacground color
	ofFloatColor colorBg;
	//colorBg.set(0.06f, 0.06f, 0.06f, 0.94f);
	colorBg.set(0.06f, 0.06f, 0.06f, 0.8f);
	gui->setColorBack(ofColor(colorBg));

	gui->setFont("assets/fonts/PragmataProR_0822.ttf");
	gui->setFontSize(OFX_UI_FONT_LARGE, 9);
	gui->setFontSize(OFX_UI_FONT_MEDIUM, 7);
	gui->setFontSize(OFX_UI_FONT_SMALL, 6);

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));
	gui->addWidgetDown(new ofxUILabel("COLOUR LOVERS", OFX_UI_FONT_LARGE));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	ofxUITextInput *textinput = new ofxUITextInput("search", "Search keyword", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);
	textinput->setAutoClear(true);

	gui->addWidgetDown(textinput);
	textinput = new ofxUITextInput("loverId", "Lover id", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);

	gui->addWidgetDown(textinput);
	textinput = new ofxUITextInput("paletteId", "Palette id", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);
	gui->addWidgetDown(textinput);

	gui->addWidgetDown(new ofxUILabelButton("FAVS", false, 0.5 * width - xInit, dim, OFX_UI_FONT_MEDIUM));
	gui->addWidgetRight(new ofxUILabelButton("HISTORY", false, 0.5 * width - xInit, dim, OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	int tgSize1 = 10;
	int tgSize2 = 4;
	gui->addWidgetDown(new ofxUIToggle("FIXED WIDTHS", MODE_fixedSize, tgSize1, tgSize1, tgSize2));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));
	gui->addWidgetDown(new ofxUILabel("PALETTE NAME:", OFX_UI_FONT_MEDIUM));
	lastPaletteName_UI = new ofxUILabel(lastPaletteName, OFX_UI_FONT_SMALL);
	gui->addWidgetDown(lastPaletteName_UI);
	lastPaletteName_UI->setLabel(lastPaletteName);

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	gui->addWidgetDown(new ofxUILabelButton("ADD FAVOURITE", false, width - xInit, dim));
	gui->addWidgetDown(new ofxUILabelButton("REMOVE FAVS", false, width - xInit, dim, OFX_UI_FONT_SMALL));
	gui->addWidgetDown(new ofxUILabelButton("REMOVE HISTORY", false, width - xInit, dim, OFX_UI_FONT_SMALL));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	tgSize1 = 10;
	tgSize2 = 4;
	gui->addWidgetDown(new ofxUIToggle("PALETTE PICK", MODE_PickPalette_BACK, tgSize1, tgSize1, tgSize2));
	gui->addWidgetDown(new ofxUIToggle("COLOR PICK", MODE_PickColor_BACK, tgSize1, tgSize1, tgSize2));

	//TODO
	//getTopPalettesForLover
	//searchPalettes

	//-

	//callbacks
	ofAddListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);
#endif

	//-

	ofAddListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);

	//----

	// startup

	currPalette = -1;
	updateFlag = false;

#ifdef USE_OFX_UI
	gui_Palette = 0;
	gui_Lab = 0;
#endif


	//-

	loadFavourites();
	// auto load first palette of favourites
	if (palettes.size() > 0)
	{
		currPalette = 0;
		//updateFlag = true;
		//setPalette(currPalette);
		refreshPalette();
	}

	//loadHistory();
	//// auto load first palette of favourites
	//if (palettes.size()>0)
	//{
	// currPalette = 0;
	//// updateFlag = 1;
	//// setPalette(currPalette);
	// refreshPalette();
	// }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setup(glm::vec2 _position, glm::vec2 _size)
{
	position = _position;
	size = _size;

	setup();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPosition(glm::vec2 _position, glm::vec2 _size)
{
	position = _position;
	size = _size;

#ifdef USE_OFX_UI
	gui->setPosition(position.x, position.y);
#endif

}

//--------------------------------------------------------------
void ofxColourLoversHelper::setGrid(glm::vec2 _position, glm::vec2 _size)
{
	gridPosition = _position;
	gridSize = _size;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_ColourLovers(ColourLoveEvent &e)
{
	ofLogNotice(__FUNCTION__) << ofToString(e.message);

	palettes = e.palettes;

	if (!(palettes.size() > 0))
	{
		return;
	}

	// save results into history after succesfuly query search
	for (int i = 0; i < palettes.size(); i++)
	{
		e.palettes[i].save(path + "history/" + e.palettes[i].id + ".xml");
	}

	//return;
	//bg = palettes[0].sortedColours[0];
	//bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));

	updateFlag = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::build_Gui_Lab()
{
	ofLogNotice(__FUNCTION__);

	// populate color boxes of all palettes 

#ifdef USE_OFX_UI

	if (gui_Lab)
	{
		ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
		delete gui_Lab;
		gui_Lab = 0;
	}

	int xInit = 6;
	int padding = 5;
	int width;
	width = gridSize.x;

	gui_Lab = new ofxUIScrollableCanvas(gridPosition.x, gridPosition.y, width, gridSize.y);

	gui_Lab->setFont("assets/fonts/PragmataProR_0822.ttf");
	gui_Lab->setFontSize(OFX_UI_FONT_LARGE, 9);
	gui_Lab->setFontSize(OFX_UI_FONT_MEDIUM, 7);
	gui_Lab->setFontSize(OFX_UI_FONT_SMALL, 6);
	gui_Lab->setScrollableDirections(false, true);
	gui_Lab->setScrollAreaToScreenHeight();
	gui_Lab->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	ofAddListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);

	int cdim;
	int cdist = 1;
	int col = 0;
	int row = 0;
	int startY = 50;
	float guiWidth = width - 2;

	// height color/palette boxes
	//cdim = 20;
	//maybe some palette have less than 5 colors, ie: 4, and then can cause problems...
	//so we fix to 5 as usual they have..
	cdim = guiWidth / 5.;

	//gui_Lab->centerWidgets();
	gui_Lab->addWidgetDown(new ofxUILabel(lastSearch, OFX_UI_FONT_MEDIUM));
	gui_Lab->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	//-

	for (int i = 0; i < palettes.size(); i++)
	{
		int currX = 1;
		int currW = 0;

		// colors in each palette
		int numOfColorsInPalette = palettes[i].colours.size();
		for (int c = 0; c < numOfColorsInPalette; c++)
		{
			if (!MODE_fixedSize)
			{
				// different sizes with original colourLover Palettes
				currW = palettes[i].colorWidths[c] * guiWidth;
			}
			else
			{
				// same size for each color
				currW = guiWidth / numOfColorsInPalette;
			}

			std::string butName = ("CL_" + ofToString(i) + "_" + ofToString(c));
			ofxUIButton *btn = new ofxUIButton(butName, false,
				currW, cdim,
				currX, i * (cdim + 4) + startY);

			btn->setLabelVisible(0);
			gui_Lab->addWidget(btn);

			// For set colour issues, make sure to set fill colour after widget been added
			// color filled box
			btn->setDrawFill(true);
			btn->setColorFill(palettes[i].colours[c]);
			btn->setColorBack(palettes[i].colours[c]);
			btn->setDrawBack(true);

			// mark selector settings colors borders & disable
			btn->setColorOutline(colorMarked);
			btn->setDrawOutline(false);

			buttonColoursPalette.push_back(btn);

			currX += currW;
		}
	}

	gui_Lab->getRect()->setHeight(palettes.size() * (cdim + 4) + startY);
	gui_Lab->setSnapping(0);

	updateFlag = false;
#endif

	//-

//#ifdef USE_OFX_UI
	//workflow
	currPalette = 0;
	setPalette(currPalette);
	refreshPalette();
	//#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::update()
{
	//bool b = gui->bInsideCanvas;
	//cout << b << endl;

	if (updateFlag)
	{
		build_Gui_Lab();

		updateFlag = false;
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::draw()
{
#ifdef USE_OFX_IM_GUI
	if (bIsVisible)	drawImGui();
#endif

	////draw raw palettes without gui
	//if(palettes.size()>0){
	//    for(int i=0;i<palettes.size();i++){
	//         palettes[i].draw(500,25*i,500,20);
	//    }
	//}
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_Gui(ofxUIEventArgs &e)
{
	std::string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice(__FUNCTION__) << name;

	if (name == "search")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = "'" + textinput->getTextString() + "'";
		ofxColourLovers::searchPalettes(textinput->getTextString(), amountResults);
		ofLogNotice(__FUNCTION__) << "textinput: " << textinput->getTextString();
	}

	else if (name == "loverId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = textinput->getTextString();
		std::string s = textinput->getTextString();
		ofStringReplace(s, " ", "%20");
		ofxColourLovers::getTopPalettesForLover(s, amountResults);
		ofLogNotice(__FUNCTION__) << "loverId: " << textinput->getTextString();
	}

	else if (name == "paletteId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = textinput->getTextString();
		ofxColourLovers::getPalette(lastSearch);
		ofLogNotice(__FUNCTION__) << "paletteId: " << textinput->getTextString();

	}

	else if (name == "ADD FAVOURITE" && currPalette > -1)
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
		{
			std::string str = path + "favourites/" + palettes[currPalette].id + ".xml";
			palettes[currPalette].save(str);
			ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
		}
	}

	//-

	else if (name == "FAVS")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			loadFavourites();
	}
	else if (name == "HISTORY")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			loadHistory();
	}
	else if (name == "REMOVE FAVS")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			clearFavourites();
	}
	else if (name == "REMOVE HISTORY")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			clearHistory();
	}

	else if (name == "FIXED WIDTHS")
	{
		bool MODE_fixedSize_PRE = MODE_fixedSize;
		ofxUIToggle *toggle = e.getToggle();
		MODE_fixedSize = toggle->getValue();
		if (MODE_fixedSize != MODE_fixedSize_PRE)
		{
			build_Gui_Lab();
		}
	}
	else if (name == "PALETTE PICK")
	{
		ofxUIToggle *toggle = e.getToggle();
		MODE_PickPalette_BACK = toggle->getValue();
	}
	else if (name == "COLOR PICK")
	{
		ofxUIToggle *toggle = e.getToggle();
		MODE_PickColor_BACK = toggle->getValue();
	}

	//TODO: disable keys outside handler
	if (kind == OFX_UI_WIDGET_TEXTINPUT)
	{
		ofLogWarning(__FUNCTION__) << "TEXTINPUT FOCUS!";
		//ofLogWarning(__FUNCTION__) << "SHOULD DISABLE KEYS !";
		ofxUITextInput *ti = (ofxUITextInput *)e.widget;

		if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_FOCUS";
			//unfocusAllTextInputs(ti);
			ENABLER_Keys = false;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_LOAD)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_LOAD";
			ENABLER_Keys = false;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_ENTER";
			ENABLER_Keys = true;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_UNFOCUS";
			ENABLER_Keys = true;
		}

		ofLogWarning(__FUNCTION__) << "ENABLER_Keys: " << (ENABLER_Keys ? "TRUE" : "FALSE");
	}

	//-

	//TODO: disabled beacause dont know what does and blocks next/prev by keys
	//currPalette=-1;
}
#endif


// sends back pointers color/palette/name and mark gui selected..
//--------------------------------------------------------------
void ofxColourLoversHelper::refreshPalette()
{
	ofLogNotice(__FUNCTION__) << currPalette;

	//TODO:
	//Windows

	ColourLovePalette p;
	if (palettes.size() > 0)
	{
		p = palettes[currPalette];
		lastPaletteName = "'" + p.title + "'";

#ifdef USE_OFX_UI
		lastPaletteName_UI->setLabel(lastPaletteName);
#endif
	}

	//ColourLovePalette p = palettes[currPalette];
	//lastPaletteName = "'" + p.title + "'";
	//lastPaletteName_UI->setLabel(lastPaletteName);

	//-

	// get and set palette colors and name BACK

	int sizePalette = p.colours.size();
	if (sizePalette > 0 && myPalette_BACK != nullptr && MODE_PickPalette_BACK)
	{
		// set BACK name clicked
		if (myPalette_Name_BACK != nullptr)
		{
			(*myPalette_Name_BACK) = p.title;
		}

		//-

		// set BACK palette colors
		myPalette_BACK->clear();
		myPalette_BACK->resize(sizePalette);
		(*myPalette_BACK) = p.colours;

		// mark update flag
		if (bUpdated_Palette_BACK != nullptr)
		{
			(*bUpdated_Palette_BACK) = true;
		}
	}

	//-

	// TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)

	//    if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	////    if (MODE_PickColor_BACK)
	//    {
	//        ofColor c;
	//        c = p.colours[0];
	//
	//        // set BACK color clicked
	//        if (myColor_BACK!=nullptr)
	//        {
	////            myColor_BACK->set( lastColor_clicked );
	//            myColor_BACK->set( c );
	//        }
	//
	//        // flag updater color ready
	//        if (bUpdated_Color_BACK!=nullptr && MODE_PickColor_BACK)
	//        {
	//            (*bUpdated_Color_BACK) = true;
	//        }
	//    }

	//--

#ifdef USE_OFX_UI
	// mark borders in all colors in palette as active about currPalette

	for (int i = 0; i < palettes.size(); i++)
	{
		int numOfColorsInPalette = palettes[i].colours.size();

		for (int c = 0; c < numOfColorsInPalette; c++)
		{
			std::string butName = ("CL_" + ofToString(i) + "_" + ofToString(c));

			auto e = gui_Lab->getWidget(butName);
			ofxUIButton *btn = (ofxUIButton *)e;

			if (i == currPalette)
			{
				btn->setDrawOutline(true);
			}
			else
			{
				btn->setDrawOutline(false);
			}
		}
	}
#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::randomPalette()
{

	currPalette = (int)ofRandom(palettes.size());
	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::nextPalette()
{
	//if (!isKeysEnabled) return;

	ofLogNotice(__FUNCTION__) << currPalette;
	if (currPalette == -1)
	{
		ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
		return;
	}

	//if (currPalette < palettes.size() - 1)
	//	currPalette++;

	currPalette++;
	if (currPalette > palettes.size() - 1)
		currPalette = 0;


	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	//-

	//TODO: 
	//workflow
	//...auto trig last color too... 
	//(it's done before too..but require when controlling by keyboard next/prev palette)

	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::prevPalette()
{
	//if (!isKeysEnabled) return;

	ofLogNotice(__FUNCTION__) << currPalette;
	if (currPalette == -1)
	{
		ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
		return;
	}

	//if (currPalette > 0)
	//currPalette--;

	currPalette--;
	if (currPalette < 0)
		currPalette = palettes.size() - 1;

	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	//-

	//TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)

	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_Gui_Lab(ofxUIEventArgs &e)
{
	std::string name = e.widget->getName();
	int kind = e.widget->getKind();
	int uid = e.widget->getID();

	//-

	// filter mousePressed only to pick a palette/color

	bool isButtonColor_click = false;
	if (kind == OFX_UI_WIDGET_BUTTON)
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
		{
			isButtonColor_click = true;
			return;
		}
	}

	//-

	// 1. filter name

	std::string whatList = name.substr(0, 2);
	std::string whatColId = name.substr(3, name.length() - 3);

	vector<std::string> seg = ofSplitString(name, "_");
	int pId = ofToInt(seg[1]);
	int cId = ofToInt(seg[2]);
	ColourLovePalette p = palettes[pId];

	ofLogNotice(__FUNCTION__) << name << " " << kind << " " << uid << " colour: " << p.colours[cId] << " name: " << p.title;

	lastPaletteName = "'" + p.title + "'";
	lastPaletteName_UI->setLabel(lastPaletteName);

	//-

	// 2. pick color

	lastColor_clicked = p.colours[cId];

	// set BACK color clicked
	if (myColor_BACK != nullptr)
	{
		myColor_BACK->set(lastColor_clicked);
	}

	// flag updater color ready
	if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
	{
		(*bUpdated_Color_BACK) = true;
	}

	//-

	// 3. set palette
	//setPalette(pId);

	currPalette = pId;
	setPalette(currPalette);

	ofLogNotice(__FUNCTION__) << "palette Id : " << pId;
	ofLogNotice(__FUNCTION__) << "currPalette: " << currPalette;

	refreshPalette();

	//-
}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette(int pId)
{
	ofLogNotice(__FUNCTION__) << pId;

	//skip if its already loaded?
	if (currPalette == pId)
	{
		return;
	}
	currPalette = pId;

	//    ColourLovePalette p = palettes[pId];
	////    lastPaletteName = p.title;
	//
	//    //--
	//
	//    // get palettes BACK
	//
	////            // TODO: not required?
	////    int sizePalette = p.colours.size();
	////    if (sizePalette>0 && myPalette_BACK!= nullptr)
	////    {
	////        myPalette_BACK->clear();
	////        myPalette_BACK->resize(sizePalette);
	////        (*myPalette_BACK) = p.colours;
	//////        lastPaletteName = p.title;
	////    }
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_ColourPalette(ofxUIEventArgs &e)
{
	//    std::string name = e.widget->getName();
	//    int kind = e.widget->getKind();
	//    int uid = e.widget->getID();
	//
	//    // TODO: add button with same name
	//    if(name=="ADD FAVOURITE" && currPalette>-1)
	//    {
	//        std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
	//        palettes[currPalette].save(str);
	//        ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
	//    }
	//    else
	//    {
	//        vector<std::string> seg = ofSplitString(name,", ");
	//        int r = ofToInt(seg[0]);
	//        int g = ofToInt(seg[1]);
	//        int b = ofToInt(seg[2]);
	//
	//        ofLogNotice(__FUNCTION__)<<"Changed_ColourPalette: "<<r<<" g "<<g <<" b "<<b;
	//    }
}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::loadFavourites()
{
	ofLogNotice(__FUNCTION__);

	ofDirectory favs(path + "favourites");
	favs.listDir();
	palettes.clear();

	for (int i = 0; i < favs.numFiles(); i++)
	{
		ColourLovePalette cp;
		cp.load(path + "favourites/" + favs.getName(i));
		palettes.push_back(cp);
	}

	lastSearch = "FAVOURITES";
	//lastSearch_PRE = lastSearch;

	//TODO:
	//BUG: CRASHES IF EMPTY FOLDER
	if (favs.numFiles() > 0) build_Gui_Lab();

	//// TODO: startup
	//currPalette = 0;
	////updateFlag = 1;
	//setPalette(currPalette);
	//refreshPalette();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::clearFavourites()
{
	ofLogNotice(__FUNCTION__);

	ofDirectory favs(path + "favourites");
	
	ofxSurfingHelpers::CheckFolder(path + "favourites");
	
	favs.listDir();

	for (int i = 0; i < favs.numFiles(); i++)
	{
		favs[i].remove();
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::loadHistory()
{
	ofLogNotice(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path + "history");

	ofDirectory favs(path + "history");
	favs.listDir();
	palettes.clear();

	for (int i = 0; i < favs.numFiles(); i++)
	{
		ColourLovePalette cp;
		cp.load(path + "history/" + favs.getName(i));
		palettes.push_back(cp);
	}

	lastSearch = "HISTORY";
	//lastSearch_PRE = lastSearch;

	//TODO: 
	//BUG: CRASHES IF EMPTY FOLDER
	if (favs.numFiles() > 0) build_Gui_Lab();

	//// TODO: startup
	//currPalette = 1;
	////updateFlag = 1;
	//setPalette(currPalette);
	//refreshPalette();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::clearHistory()
{
	ofLogNotice(__FUNCTION__);
	
	ofxSurfingHelpers::CheckFolder(path + "history");

	ofDirectory favs(path + "history");
	favs.listDir();

	for (int i = 0; i < favs.numFiles(); i++)
	{
		favs[i].remove();
	}
}

// pointers back to 'communicate externally'

//--------------------------------------------------------------
void ofxColourLoversHelper::setColor_BACK(ofColor &c)
{
	myColor_BACK = &c;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_BACK(vector<ofColor> &p)
{
	myPalette_BACK = &p;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_bUpdated_Palette_BACK(bool &b)
{
	bUpdated_Palette_BACK = &b;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_bUpdated_Color_BACK(bool &b)
{
	bUpdated_Color_BACK = &b;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_Name_BACK(std::string &n)
{
	myPalette_Name_BACK = &n;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	//ofLogNotice(__FUNCTION__) << "key: " << key;

	//-

	//if (key == 'f')
	//{
	//    std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
	//    palettes[currPalette].save(str);
	//    ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
	//}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::keyReleased(ofKeyEventArgs &eventArgs)
{
	if (eventArgs.key == ' ')
	{
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::removeMouseListeners()
{
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::exit()
{

}

//--------------------------------------------------------------
ofxColourLoversHelper::~ofxColourLoversHelper()
{
	removeKeysListeners();
	removeMouseListeners();

#ifdef USE_OFX_UI
	ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
	//delete gui_Lab;

	ofRemoveListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);
	//delete gui;
#endif

	ofRemoveListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::windowResized(int w, int h)
{
	//build_Gui_Lab();
}
