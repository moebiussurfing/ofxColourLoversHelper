#include "ofxColourLoversHelper.h"

//--------------------------------------------------------------
ofxColourLoversHelper::ofxColourLoversHelper()
{
	//ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetLogLevel("ofxColourLovers", OF_LOG_VERBOSE);

	//--

	path_Global = "ofxColourLoversHelper/";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_AppSettings = path_Global + "ofxColourLoversHelper_Settings.xml";

	//--

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

	//setEnableKeys(true);
	addKeysListeners();
	isKeysEnabled = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setToggleVisible()
{
	bGui = !bGui;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setVisible(bool b)
{
	bGui = b;

	//-

#ifdef USE_OFX_UI
	if (gui_Lab) gui_Lab->setVisible(bGui);
	//gui->setVisible(bGui);
	if (bGui && gui) setVisibleSearcher(bSearcherVisible);
#endif

	//-

	if (bGui)
	{
		addMouseListeners();

		addKeysListeners();
		isKeysEnabled = true;
		//setEnableKeys(true);
	}
	else
	{
		removeMouseListeners();

		removeKeysListeners();
		isKeysEnabled = false;
		//setEnableKeys(false);
	}
}

#ifdef USE_OFX_IM_GUI

//--------------------------------------------------------------
void ofxColourLoversHelper::draw_ImGui_Search()
{
	if (bGui_Search)
	{
		float _w1;
		float _w2;
		float _w3;
		float _w4;
		float _h;
		float _hh;

		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_WIDTH / 3;

		ImGuiWindowFlags flags = guiManager.bAutoResize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
		if (!guiManager.bAutoResize) ImGui::SetWindowSize(ImVec2(ww, hh));

		//----

		// 1. search panel

		//static bool open_popup = false;
		//open_popup = bGui_Search;

		ImGuiWindowFlags flagsw2;
		flagsw2 = guiManager.bAutoResize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		//flagsw2 |= flagsWindows;
		//flagsw2 |= ImGuiWindowFlags_NoTitleBar;
		//flagsw2 |= ImGuiWindowFlags_NoBackground;
		//flagsw2 |= ImGuiWindowFlags_NoDecoration;

		guiManager.beginWindow(bGui_Search.getName().c_str(), (bool*)&bGui_Search.get(), flagsw2);
		{
			_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
			_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
			_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
			_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
			_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
			_hh = 2 * _h;

			ImGui::PushItemWidth(-1);

			// search
			ImGui::Text("Type Keyword:");

			char tab1[32];
			strncpy(tab1, textInput_temp1.c_str(), sizeof(tab1));
			tab1[sizeof(tab1) - 1] = 0;

			//if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1)), ImGuiInputTextFlags_EnterReturnsTrue))
			if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1)))
			{
				textInput_temp1 = ofToString(tab1);
				ofLogNotice(__FUNCTION__) << "input: " << textInput_temp1;

				if (textInput_temp1 != textInput_temp1_PRE)
				{
					textInput_temp1_PRE = textInput_temp1;
				}
			}

			ImGui::PopItemWidth();

			//--

			//TODO:
			//to disable all other key commands
			bool b = bTextInputActive;
			bTextInputActive = ImGui::IsItemActive();
			//changed
			if (bTextInputActive != b) ofLogNotice(__FUNCTION__) << "TextInput : " << (bTextInputActive ? "ACTIVE" : "DISABLED");

			//--

			//blink when a new preset is editing
			float freq = 0.15;//speed freq
			float min = 0.20;
			float max = 0.80;
			float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));

			if (ImGui::Button("SEARCH", ImVec2(_w1, _h * 2)))
			{
				ofLogNotice(__FUNCTION__) << endl << "--------------------------------------------------------------" << endl << "searchPalettes: " << textInput_temp1_PRE;

				bSearching = true;
				bSearch = true;

				ofxColourLovers::searchPalettes(textInput_temp1_PRE, amountResults);

				//receiver callback will be called when colour lovers server replies to the quaery search 

				lastSearch_Str = textInput_temp1_PRE;

				// workflow
				//close search floating window
				//bGui_Search = false;//close
			}

			ImGui::PopStyleColor();

			//hide. not required
			//ImGui::PushItemWidth(-65);
			//ImGui::SliderInt("Amnt Max", &amountResults, 10, 100);
			//ImGui::PopItemWidth();

			// spinner
			if (bSearching) ImGui::Text("Searching  %c", "|/-\\"[(int)(ImGui::GetTime() / 0.07f) & 3]);

			//----

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

				lastSearch_Str = textInput_temp2;
				std::string s = textInput_temp2;
				ofStringReplace(s, " ", "%20");
				ofxColourLovers::getTopPalettesForLover(s, amountResults);
			}

			//-

			// palette
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

				lastSearch_Str = textInput_temp3;
				ofxColourLovers::getPalette(lastSearch_Str);
			}
#endif
			//ImGui::Dummy(ImVec2(0, 2));
			//ImGui::PushItemWidth(-40);
			//ImGui::SliderInt("Amnt Max", &amountResults, 25, 100);
			//ImGui::PopItemWidth();
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::draw_ImGui_Browser()
{
	float _w1;
	float _w2;
	float _w3;
	float _w4;
	float _h;
	float _hh;

	_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
	_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
	_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
	_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
	_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
	_hh = 2 * _h;

	//--

	float ww = 200;
	float hh = PANEL_WIDGETS_HEIGHT;


	//----

	// scrollable list

	//ImGui::Dummy(ImVec2(0, 5));

	//if (!bSearch /*&& bFavorites*/)//TODO: showing wrong, the favs names not the serach list (?) 

	if (!bSearch)
	{
		if (palettes.size() != 0)
		{
			int _i = currPalette;

			ImGui::PushItemWidth(_w1 - 10);

			if (ofxImGuiSurfing::VectorCombo(" ", &_i, pNames))
			{
				ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

				if (_i < pNames.size())
				{
					currPalette = _i;
					pName = pNames[_i];
					ofLogNotice(__FUNCTION__) << "Combo select: " << _i;

					if (pNames.size() > 0 && currPalette < pNames.size())
					{
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

						if (!AutoScroll) AutoScroll = true;
					}
				}
			}

			ImGui::PopItemWidth();
		}
	}

	ImGui::Dummy(ImVec2(0, 2));

	//-

	// index/max
	std::string s;
	if (_files.numFiles() > 0)
	{
		s = ofToString(currPalette) + "/" + ofToString(palettes.size() - 1);
	}
	else
	{
		s = ofToString("-1/0");
	}
	ImGui::Text(s.c_str());

	// name
	//if (bSearch)
	{
		ImGui::Text(lastPaletteName.get().c_str());
	}

	//-

	// palette colors

	ImGuiColorEditFlags colorEdiFlags =
		ImGuiColorEditFlags_NoAlpha |
		ImGuiColorEditFlags_NoPicker |
		ImGuiColorEditFlags_NoTooltip;

	// macOS
	if (palettes.size() > 0)
	{
		vector<ofColor> p = palettes[currPalette].colours;
		//const auto p = palettes[currPalette].colours;
		//auto p = palettes[currPalette].colours;

		//myPalette_BACK

		if (p.size() > 0)
		{
			// fit width
			float wb = (_w1 / (int)p.size());// -_spc;
			float hb = _hh;
			//float hb = BUTTON_BIG_HEIGHT;

			for (int n = 0; n < p.size(); n++)
			{
				ImGui::PushID(n);

				// fit width
				if (n != 0) ImGui::SameLine(0, 0);

				// box colors
				if (ImGui::ColorButton("##paletteLover", p[n], colorEdiFlags, ImVec2(wb, hb))) {}

				ImGui::PopID();
			}
		}
		else
		{
			ofLogWarning(__FUNCTION__) << "empty palette!";
		}
	}

	//-

	ImGui::PushButtonRepeat(true);
	{
		if (ImGui::Button("<", ImVec2(_w2, _h * 2)))
		{
			prevPalette();

			//-

			// worfklow
			//if (!AutoScroll) AutoScroll = true;
		}

		ImGui::SameLine();

		if (ImGui::Button(">", ImVec2(_w2, _h * 2)))
		{
			nextPalette();

			//-

			// worfklow
			//if (!AutoScroll) AutoScroll = true;
		}
	}
	ImGui::PopButtonRepeat();

	//-

	if (ImGui::Button("Randomize", ImVec2(_w1, 2 * _h)))
	{
		randomPalette();

		//-

		// worfklow
		//if (!AutoScroll) AutoScroll = true;
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::draw_ImGui_Main()
{
	float _w1;
	float _w2;
	float _w3;
	float _w4;
	float _h;
	float _hh;

	//--

	float ww = 200;
	float hh = PANEL_WIDGETS_HEIGHT;

	ImGuiWindowFlags flags = guiManager.bAutoResize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	//if (!guiManager.bAutoResize) ImGui::SetWindowSize(ImVec2(ww, hh));

	//----

	// 2. main panel

	guiManager.beginWindow(bGui.getName().c_str(), (bool*)&bGui.get(), flags);
	{
		_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
		_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
		_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
		_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
		_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
		_hh = 2 * _h;

		//-

		//ofxImGuiSurfing::AddBigToggle(bGui_KitPalettes, _w1, _h);
		ofxImGuiSurfing::AddToggleRoundedButton(bGui_KitPalettes);

		//-

		//ofxImGuiSurfing::AddToggleRoundedButton(bGui_Search);
		ofxImGuiSurfing::AddBigToggle(bGui_Search, _w1, _hh);

		std::string s;
		if (lastSearch_Str == "HISTORY" || lastSearch_Str == "FAVORITES") s = " ";
		else s = "Keyword: " + lastSearch_Str;
		ImGui::Text(s.c_str());

		//s = "Keyword:";
		//ImGui::Text(s.c_str());
		//s = lastSearch_Str;
		//ImGui::Text(s.c_str());

		ImGui::Dummy(ImVec2(0, 2));

		//-

		draw_ImGui_Browser();

		// spinner
		if (bSearching) ImGui::Text("Searching  %c", "|/-\\"[(int)(ImGui::GetTime() / 0.07f) & 3]);
		//if (bSearching) ofxImGuiSurfing::AddPro

		//-

#ifndef USE_FLOATING_SEARCH_WINDOW_WORAROUND
		if (ImGui::CollapsingHeader("HTTP SEARCH"))
		{
			//open_popup = true;

			ImGui::Dummy(ImVec2(0, 5));

			//----

			// search
			ImGui::Text("Type Keyword:");
			//std::string textInput_temp1 = "";

			if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1)))
			{
				textInput_temp1 = ofToString(tab1);
				ofLogNotice(__FUNCTION__) << "input: " << textInput_temp1;

				//ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab1);
				//lastSearch_PRE = lastSearch_Str;

				if (textInput_temp1 != textInput_temp1_PRE)
				{
					textInput_temp1_PRE = textInput_temp1;
				}
			}

			//ImGui::SameLine();
			if (ImGui::Button("SEARCH", ImVec2(_w1, _h * 2)))
			{
				ofLogNotice(__FUNCTION__) << "searchPalettes: " << textInput_temp1_PRE;

				ofxColourLovers::searchPalettes(textInput_temp1_PRE, amountResults);

				lastSearch_Str = textInput_temp1_PRE;

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

				lastSearch_Str = textInput_temp2;
				std::string s = textInput_temp2;
				ofStringReplace(s, " ", "%20");
				ofxColourLovers::getTopPalettesForLover(s, amountResults);
			}

			//-

			// palette
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

				lastSearch_Str = textInput_temp3;
				ofxColourLovers::getPalette(lastSearch_Str);
			}
#endif
			//ImGui::Dummy(ImVec2(0, 2));

			ImGui::PushItemWidth(-40);
			ImGui::SliderInt("Amnt Max", &amountResults, 25, MAX_SEARCH_RESULTS);
			ImGui::PopItemWidth();
		}
		else open_popup = false;
#endif
		ImGui::Dummy(ImVec2(0, 2));

		//----

		if (ImGui::CollapsingHeader("BROWSE", ImGuiWindowFlags_NoCollapse))
		{
			_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
			_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
			_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
			_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
			_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
			_hh = 2 * _h;

			// mode selectors

			ofxImGuiSurfing::AddBigToggle(bFavorites, _w2, _h * 2);
			ImGui::SameLine();
			ofxImGuiSurfing::AddBigToggle(bHistory, _w2, _h * 2);

			//ofxSurfingHelpers::AddBigToggle(bFavorites, _w3, _h * 2);
			//ImGui::SameLine();
			//ofxSurfingHelpers::AddBigToggle(bHistory, _w3, _h * 2);
			//ImGui::SameLine();
			//ofxSurfingHelpers::AddBigToggle(bSearch, _w3, _h * 2);

			//--

			if (ImGui::CollapsingHeader("TOOLS", ImGuiWindowFlags_None))
			{
				_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
				_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
				_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
				_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
				_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
				_hh = 2 * _h;

				//--

				// top palettes

				if (ImGui::Button("GET TOP", ImVec2(_w2, _hh)))
				{
					ofxColourLovers::getTopPalettes(amnt, page.get() * amnt.get());
				}

				ImGui::SameLine();

				//-

				// random palettes

				if (ImGui::Button("GET NEW", ImVec2(_w2, _hh)))
				{
					ofxColourLovers::getNewPalettes(amnt, page.get() * amnt.get());
				}

				//-

				// random palettes

				if (ImGui::Button("GET A RANDOM", ImVec2(_w1, _h)))
				{
					ofxColourLovers::getRandomPalette();
				}

				//-

				ImGui::Dummy(ImVec2(0, 2));

				// manager

				{
					if (ImGui::Button("ADD FAVORITE", ImVec2(_w1, _hh)))
					{
						ofxSurfingHelpers::CheckFolder(path_Global + "favorites/");

						std::string str = path_Global + "favorites/" + palettes[currPalette].id + ".xml";

						palettes[currPalette].save(str);
						ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
					}

					//--

					//ImGui::Text("CLEAR:");

					if (ImGui::Button("CLEAR FAVS", ImVec2(_w2, _hh))) ImGui::OpenPopup("CLEAR FAVORITES?");

					if (ImGui::BeginPopupModal("CLEAR FAVORITES?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("All Favorite palettes will be deleted.\nThis operation cannot be undone!\n\n");
						ImGui::Separator();

						static bool dont_ask_me_next_timeFavs = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_timeFavs);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(_w1, 0))) {
							clearFavourites();
							// workflow
							loadFavorites();

							ImGui::CloseCurrentPopup();
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(_w1, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
				}
				ImGui::SameLine();
				{
					if (ImGui::Button("CLEAR HISTORY", ImVec2(_w2, _hh))) ImGui::OpenPopup("CLEAR HISTORY?");

					if (ImGui::BeginPopupModal("CLEAR HISTORY?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("All History will be deleted.\nThis operation cannot be undone!\n\n");
						ImGui::Separator();

						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						if (!dont_ask_me_next_time) {
							if (ImGui::Button("OK", ImVec2(_w1, 0))) {
								clearHistory();
								// workflow
								loadHistory();

								ImGui::CloseCurrentPopup();
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(_w1, 0))) { ImGui::CloseCurrentPopup(); }
						}
						else {
							clearHistory();
							// workflow
							loadHistory();

							//ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
				}

				//--

				ImGui::Dummy(ImVec2(0, 5));

				////if (SHOW_AdvancedLayout)
				//{
				//	//ImGui::SameLine();
				//	ofxImGuiSurfing::AddToggleRoundedButton(SHOW_AdvancedLayout);
				//	//ofxImGuiSurfing::AddParameter(SHOW_AdvancedLayout);
				//}
			}
		}

		//-

		//ImGui::Dummy(ImVec2(0, 2));

		//ImGui::Text(lastSearch_Str.c_str());

		//// history or favorites
		//std::string ss;
		//ss = lastSearch_Str;
		////ss = lastSearch_Str + "      " + s;
		//ImGui::Text(ss.c_str());
		////ImGui::Text(s.c_str());

		//--

		// extra
		ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
		if (guiManager.bExtra)
		{
			ImGui::Indent();

			ImGui::Text("Search Results:");
			ofxImGuiSurfing::AddParameter(amnt);
			if (ofxImGuiSurfing::AddParameter(page))
			//ofxImGuiSurfing::AddStepper(amnt);
			//if (ofxImGuiSurfing::AddStepper(page.get(), 1, 100)) 
			{
				ofxColourLovers::getTopPalettes(amnt, page.get() * amnt.get());
			}

			//-

			// I prefer to control this toggle on a main app
			// but we can include it on standalone mode
			ofxImGuiSurfing::AddToggleRoundedButton(SHOW_AdvancedLayout);
			//#ifndef USE_OFX_IM_GUI_EXTERNAL
			//ofxImGuiSurfing::AddBigToggle(SHOW_AdvancedLayout, _w1, _h);
			//#endif
			if (SHOW_AdvancedLayout)
			{
				if (ImGui::CollapsingHeader("Advanced"))
				{
					_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
					_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
					_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
					_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
					_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
					_hh = 2 * _h;

					//if (ofxImGuiSurfing::AddParameter(amountResults)) 
					//{}

					//ofxImGuiSurfing::AddParameter(MODE_PickPalette_BACK);
					//ofxImGuiSurfing::AddParameter(MODE_PickColor_BACK);
					//ofxImGuiSurfing::AddParameter(ENABLER_Keys);
					ofxImGuiSurfing::AddToggleRoundedButton(ENABLER_Keys);

					if (ImGui::CollapsingHeader("Layout"))
					{
						//if (ImGui::Checkbox("Fixed Width", &MODE_FixedSize))
						bool bPre = MODE_FixedSize;

						if (ofxImGuiSurfing::AddToggleRoundedButton(MODE_FixedSize))
							//if (ofxImGuiSurfing::AddParameter(MODE_FixedSize))
						{
							if (MODE_FixedSize != bPre)
							{
								build_Gui_Lab();
							}
						}

						ofxImGuiSurfing::AddToggleRoundedButton(MODE_Slim);
						//if (ofxImGuiSurfing::AddParameter(MODE_Slim))
						//{
						//}

						//-

						//ImGui::Dummy(ImVec2(0, 5)); // spacing

						// layout
						ImGui::Checkbox("Focus", &bfocus);

						//--

						//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
						if (guiManager.bExtra) guiManager.drawAdvancedSubPanel();
					}
				}
			}

			//-

			ImGui::Unindent();
		}

		//if (bGui_KitPalettes) ofxImGuiSurfing::AddParameter(AutoScroll);
	}
	guiManager.endWindow();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::draw_ImGui_Kit()
{
	// palette rows with all color boxes
	if (bGui_KitPalettes)
	{
		float _w1;
		float _w2;
		float _w3;
		float _w4;
		float _h;
		float _hh;
		float _hhB;

		ImGuiWindowFlags flags = guiManager.bAutoResize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
		//ImGuiWindowFlags flags = auto_resize2 ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT / 2));
		{
			guiManager.beginWindow(bGui_KitPalettes.getName().c_str(), (bool*)&bGui_KitPalettes.get(), flags);
			{
				float _spc2 = 2;// spacing between palettes 

				////label: favorites or history
				//std::string ss = lastSearch_Str;
				//ImGui::Text(ss.c_str());

				//--

				//TODO:
				// tween sizes
				bool btween = false;
				static float alpha;
				static int indexExt_PRE;
				const float step = 0.1f;
				if (btween)
				{
					if (currPalette != indexExt_PRE)
					{
						indexExt_PRE = currPalette;
						alpha = 1;
					}
					else
					{
						if (alpha > 0) alpha -= step;
						alpha = ofClamp(alpha, 0, 1);
					}
					//ofLogNotice(__FUNCTION__) << "alpha:  " << alpha;
				}

				//--

				_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
				_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
				_w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
				_w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
				_h = ofxImGuiSurfing::getWidgetsHeightRelative(); // one unit height relative to ImGui theme
				_hh = 2 * _h;

				// border style

				// blink when selected
				float freq = 0.15;//speed freq
				float min = 0.20;
				float max = 0.80;
				float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

				float borderLineWidth = 2.0;
				ImVec4 borderLineColor{ 0, 0, 0, a };
				auto color_Pick32 = IM_COL32(borderLineColor.x*255.f, borderLineColor.y*255.f, borderLineColor.z*255.f, borderLineColor.w*255.f);

				//-

				ImGuiColorEditFlags _flags =
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip;

				//--

				for (size_t p = 0; p < palettes.size(); p++)
				{
					// group border
					//auto pos1 = ImGui::GetCursorScreenPos();

					//-

					// 1. spacing
					ImGui::Dummy(ImVec2(0, _spc2));

					// autoscroll
					if (p == currPalette) if (AutoScroll) ImGui::SetScrollHereY(0.0f); // 0.0f:top, 0.5f:center, 1.0f:bottom

					// 2. label tittle
					std::string _name = palettes[p].title;

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
					ImGui::Text(_name.c_str());
					ImGui::PopStyleVar();

					//-

					// set color heights
					if (MODE_Slim.get()) _h = WIDGETS_HEIGHT * 0.5f;
					else _h = WIDGETS_HEIGHT;

					////TODO:
					////check offset..
					//float _offset = 0;
					//for (int c = 0; c < _sizeP; c++) {
					//	_offset += palettes[i].colorWidths[c];
					//}

					// 3. colors in each palette

					int _sizeP = palettes[p].colours.size();

					for (size_t c = 0; c < _sizeP; c++)
					{
						if (c != 0) ImGui::SameLine(0, 0);

						//-

						ImGui::PushID(c);

						float _wwB;
						if (MODE_FixedSize)
						{
							// same size for each color
							_wwB = (_w1 / _sizeP);
						}
						else
						{
							// different sizes with original colourLover Palettes
							_wwB = (palettes[p].colorWidths[c] * _w1);
						}

						//-

						//////TODO:
						//////check offset..
						//if (_offset != 1.0f) {
						//	if (c == _sizeP - 1) {//add the offset to the last color box
						//		if (_offset != 1.0f) {
						//			_wwB = _wwB + _offset;
						//		}
						//	}
						//}
						////// adjust better to fit to border..
						////if (_sizeP % 2 == 0)// || _sizeP % 3 == 0)
						////{
						////	_wwB = _wwB + (_spc / _sizeP);
						////}

						//-

						std::string name = "CL_" + ofToString(p) + "_" + ofToString(c);

						//-

						// draw border to selected color/palette
						bool bDrawBorder = false;
						float _scale = 1.0f;

						if (p == indexExt_PRE)
						{
							if (btween)
							{
								if (bfocus) _scale = ofMap(alpha, 1, 0, 1.75f, 1.0f, true);
							}
							else
							{
								if (bfocus) _scale = 1.0f;
							}

							_hhB = _h * _scale;
						}

						else if (p == currPalette)// highlight selected
						{
							bDrawBorder = true;

							//if (!MODE_Slim) 
							//{
							//	_scale = 0.45f;
							//}
							//else 
							//{
							//	_scale = 1.0f;
							//}

							if (btween)
							{
								if (bfocus) _scale = ofMap(alpha, 1, 0, 1.75f, 1.0f, true);
							}
							else
							{
								if (bfocus) _scale = 1.75f;
							}

							_hhB = _h * _scale;

							//_hhB = _h;

							//-

							ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
							//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, 3.0);

							//-

							////group border
							//ImGui::BeginGroup();
						}
						else
						{
							_hhB = _h * _scale;
						}

						//------------

						// colored box

						//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, 1.0);

						const ImVec4 col = palettes[p].colours[c];
						if (ImGui::ColorButton(name.c_str(), col, _flags, ImVec2(_wwB, _hhB)))
						{
							//-

							// 1. filter name

							std::string whatList = name.substr(0, 2);
							std::string whatColId = name.substr(3, name.length() - 3);

							vector<std::string> seg = ofSplitString(name, "_");
							int pId = ofToInt(seg[1]);
							int cId = ofToInt(seg[2]);
							ColourLovePalette p = palettes[pId];

							lastPaletteName = p.title;

#ifdef USE_OFX_UI
							lastPaletteName_UI->setLabel(lastPaletteName);
#endif
							//-

							// 2. pick palette and color

							lastColor_clicked = p.colours[cId];
							//lastColor_clicked = palettes[i].colours[c];

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

							//-

							// worfklow
							if (AutoScroll) AutoScroll = false;
						}

						//ImGui::PopStyleVar(1);

						//-

						if (bDrawBorder)
						{
							ImGui::PopStyleColor();
							ImGui::PopStyleVar(1);

							////group border
							//ImGui::EndGroup();
							//auto pos2 = ImGui::GetCursorScreenPos();
							//float pad = 2.0f;
							//ImGui::GetWindowDrawList()->AddRect(ImVec2(pos1.x - pad, pos1.y), ImVec2(pos1.x + _w + pad, pos2.y + pad), color_Pick32);
						}

						//-

						ImGui::PopID();
					}
				}

				//--

				//TODO:
				//disable scroll
				auto &io = ImGui::GetIO();
				if (io.MouseHoveredViewport)
				{
					float wheel = io.MouseWheel;
					if (wheel != 0)
					{
						if (AutoScroll) AutoScroll = false;
					}
				}
			}
			guiManager.endWindow();
		}
		ImGui::PopStyleVar();
	}
}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::setup_ImGui()
{
	guiManager.setImGuiAutodraw(true);
	guiManager.setup();
	guiManager.bAutoResize = false;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setup()
{
	//--

#ifndef USE_OFX_IM_GUI_EXTERNAL
	setup_ImGui();
#endif

	//-

	ofAddListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);

	//-

	params.setName("ofxColourLoversHelper");
	params.add(MODE_PickPalette_BACK);
	params.add(MODE_PickColor_BACK);
	params.add(MODE_FixedSize);
	params.add(MODE_Slim);
	params.add(ENABLER_Keys);
	params.add(bGui_KitPalettes);
	params.add(AutoScroll);
	params.add(lastMenuTab_Str);
	params.add(bFavorites);
	params.add(bHistory);
	params.add(bSearch);
	params.add(amnt);
	params.add(page);
	//params.add(lastSearch_Str);
	//params.add(amountResults);

	//exclude
	//bFavorites.setSerializable(false);
	//bHistory.setSerializable(false);
	bSearch.setSerializable(false);

	//ofxSurfingHelpers::loadGroup(params, path_AppSettings);

	//--------

	// startup

	currPalette = -1;
	bUpdateSearchFlag = false;

	//-

	//--------------------------------------------------------------
	listener_bFavorites = bFavorites.newListener([this](bool &b) {
		ofLogNotice("ofxColourLovers > bFavorites: ") << b;

		//bHistory = !bFavorites;

		if (bFavorites)
		{
			bHistory = false;
			bSearch = false;
		}

		if (b) loadFavorites();
	});

	//--------------------------------------------------------------
	listener_bHistory = bHistory.newListener([this](bool &b) {
		ofLogNotice("ofxColourLovers > bHistory: ") << b;

		//bFavorites = !bHistory;

		if (bHistory)
		{
			bFavorites = false;
			bSearch = false;
		}

		if (b) loadHistory();
	});

	//--------------------------------------------------------------
	listener_bSearch = bSearch.newListener([this](bool &b) {
		ofLogNotice("ofxColourLovers > bSearch: ") << b;

		if (bSearch)
		{
			bFavorites = false;
			bHistory = false;
		}
	});

	//----


	// startup

	if (lastMenuTab_Str.get() == "FAVORITES") loadFavorites();
	else if (lastMenuTab_Str.get() == "HISTORY") loadHistory();
	loadFavorites();

	//--

	// auto load first palette of favourites
	if (palettes.size() > 0)
	{
		currPalette = 0;
		//bUpdateSearchFlag = true;
		//setPalette(currPalette);
		refreshPalette();
	}

	//loadHistory();
	//// auto load first palette of favourites
	//if (palettes.size()>0)
	//{
	// currPalette = 0;
	//// bUpdateSearchFlag = 1;
	//// setPalette(currPalette);
	// refreshPalette();
	// }

	ofxSurfingHelpers::loadGroup(params, path_AppSettings);

	////TODO:
	//bFavorites = true;
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
	// this happens after a query seach

	ofLogNotice(__FUNCTION__) << "message :" << ofToString(e.message);
	//ofLogNotice(__FUNCTION__) << "type    :" << ofToString(e.type);
	////ofLogNotice(__FUNCTION__) << "events  :" << endl << ofToString(e.events);
	//ofLogNotice(__FUNCTION__) << "xml     :" << endl << ofToString(e.xml);

	if ((e.palettes.size() <= 0))
	{
		ofLogError(__FUNCTION__) << "Size is 0! Return";
		if (bSearching) bSearching = false;
		return;
	}

	//palettes.clear();

	palettes = e.palettes;
	//palettesSearch = e.palettes;

	//if ((palettes.size() <= 0))
	//{
	//	return;
	//}

	// save results into history after succesfuly query search
	for (int i = 0; i < palettes.size(); i++)
	{
		e.palettes[i].save(path_Global + "history/" + e.palettes[i].id + ".xml");
	}

	//return;
	//bg = palettes[0].sortedColours[0];
	//bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));

	bUpdateSearchFlag = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::build_Gui_Lab()
{
	ofLogNotice(__FUNCTION__);

	// populate color boxes of all palettes 

	//-

	////TODO:
	////crash?
	////#ifdef USE_OFX_UI
	//	// workflow
	currPalette = 0;
	setPalette(currPalette);
	//setPalette(0);
	refreshPalette();
	//#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::update()
{
	//bool b = gui->bInsideCanvas;
	//cout << b << endl;

	//if (0)
	if (bUpdateSearchFlag)
	{
		bUpdateSearchFlag = false;

		//lastSearch_Str = "SEARCH";

		build_Gui_Lab();

		if (bSearching) bSearching = false;

		if (palettes.size() > 0)
		{
			//TODO:
			//BUG: names
			pNames.clear();
			for (int i = 0; i < _files.numFiles() && i < palettes.size(); i++)
			{
				ColourLovePalette cp = palettes[i];
				pNames.push_back(cp.title);
			}
		}
	}
}

//--------------------------------------------------------------
bool ofxColourLoversHelper::draw()
{
	bCheckMouseOverTextInputLovers = bTextInputActive;

	//--

#ifdef USE_OFX_IM_GUI
	if (bGui)
	{
		guiManager.begin(); // global begin
		{
			draw_ImGui_Search();
			draw_ImGui_Main();
			draw_ImGui_Kit();
		}
		guiManager.end(); // global end
	}
	else bCheckMouseOverTextInputLovers = false;
#endif

	////draw raw palettes without gui
	//if(palettes.size()>0){
	//    for(int i=0;i<palettes.size();i++){
	//         palettes[i].draw(500,25*i,500,20);
	//    }
	//}

	return bCheckMouseOverTextInputLovers;
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
		lastSearch_Str = textinput->getTextString();
		ofxColourLovers::searchPalettes(textinput->getTextString(), amountResults);
		ofLogNotice(__FUNCTION__) << "textinput: " << textinput->getTextString();
	}

	else if (name == "loverId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch_Str = textinput->getTextString();
		std::string s = textinput->getTextString();
		ofStringReplace(s, " ", "%20");
		ofxColourLovers::getTopPalettesForLover(s, amountResults);
		ofLogNotice(__FUNCTION__) << "loverId: " << textinput->getTextString();
	}

	else if (name == "paletteId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch_Str = textinput->getTextString();
		ofxColourLovers::getPalette(lastSearch_Str);
		ofLogNotice(__FUNCTION__) << "paletteId: " << textinput->getTextString();

	}

	else if (name == "ADD FAVOURITE" && currPalette > -1)
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
		{
			std::string str = path_Global + "favorites/" + palettes[currPalette].id + ".xml";
			palettes[currPalette].save(str);
			ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
		}
	}

	//-

	else if (name == "FAVS")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			loadFavorites();
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
		bool MODE_fixedSize_PRE = MODE_FixedSize;
		ofxUIToggle *toggle = e.getToggle();
		MODE_FixedSize = toggle->getValue();
		if (MODE_FixedSize != MODE_fixedSize_PRE)
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
	ofLogNotice(__FUNCTION__) << "currPalette: " << currPalette;

	//TODO:
	//Windows

	if (palettes.size() > 0)
	{
		ColourLovePalette p;
		//if (currPalette > palettes.size())// ?? uncommenting breaks the local refresh palette after load
		{
			p = palettes[currPalette];
			lastPaletteName = p.title;

#ifdef USE_OFX_UI
			lastPaletteName_UI->setLabel(lastPaletteName);
#endif
		}

		//ColourLovePalette p = palettes[currPalette];
		//lastPaletteName = p.title;
		//lastPaletteName_UI->setLabel(lastPaletteName);

		//-

		// get and set palette colors and name BACK

		int _sz = p.colours.size();

		if (_sz > 0 && myPalette_BACK != nullptr && MODE_PickPalette_BACK)
		{
			// set BACK name clicked
			if (myPalette_Name_BACK != nullptr) (*myPalette_Name_BACK) = p.title;
			//(*myPalette_Name_BACK) = p.title;

			// set BACK palette colors
			if (myPalette_BACK != nullptr)
			{
				myPalette_BACK->clear();
				myPalette_BACK->resize(_sz);
				(*myPalette_BACK) = p.colours;
			}

			// mark update flag
			if (bUpdated_Palette_BACK != nullptr) (*bUpdated_Palette_BACK) = true;
		}
	}

	//-

	//TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)

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
		int _sizeP = palettes[i].colours.size();

		for (int c = 0; c < _sizeP; c++)
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
#endif
}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::randomPalette()
	{
		if (palettes.size() == 0) return;

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
	void ofxColourLoversHelper::nextPalette(bool cycled)
	{
		ofLogNotice(__FUNCTION__) << currPalette;

		if (currPalette == -1 || palettes.size() == 0)
		{
			ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
			return;
		}

		if (!cycled)//clamped to last
		{
			if (currPalette >= palettes.size() - 1)
			{
				currPalette = 0;
			}
			else currPalette++;
		}
		else//cycled browse
		{
			currPalette++;
			if (currPalette > palettes.size() - 1)
				currPalette = 0;
		}

		ofLogNotice(__FUNCTION__) << currPalette;

		refreshPalette();

		//-

		//TODO: 
		// workflow
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

		if (!AutoScroll) AutoScroll = true;
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::prevPalette()
	{
		ofLogNotice(__FUNCTION__) << currPalette;
		if (currPalette == -1 || palettes.size() == 0)
		{
			ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
			return;
		}

		currPalette--;
		if (currPalette < 0) currPalette = palettes.size() - 1;

		ofLogNotice(__FUNCTION__) << currPalette;

		refreshPalette();

		//-

		//TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)
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

		if (!AutoScroll) AutoScroll = true;
	}

	/*
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

		lastPaletteName = p.title;
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
	*/

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
		////            //TODO: not required?
		////    int _sz = p.colours.size();
		////    if (_sz>0 && myPalette_BACK!= nullptr)
		////    {
		////        myPalette_BACK->clear();
		////        myPalette_BACK->resize(_sz);
		////        (*myPalette_BACK) = p.colours;
		//////        lastPaletteName = p.title;
		////    }
	}

	//#ifdef USE_OFX_UI
	//	//--------------------------------------------------------------
	//	void ofxColourLoversHelper::Changed_ColourPalette(ofxUIEventArgs &e)
	//	{
	//		//    std::string name = e.widget->getName();
	//		//    int kind = e.widget->getKind();
	//		//    int uid = e.widget->getID();
	//		//
	//		//    //TODO: add button with same name
	//		//    if(name=="ADD FAVOURITE" && currPalette>-1)
	//		//    {
	//		//        std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
	//		//        palettes[currPalette].save(str);
	//		//        ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
	//		//    }
	//		//    else
	//		//    {
	//		//        vector<std::string> seg = ofSplitString(name,", ");
	//		//        int r = ofToInt(seg[0]);
	//		//        int g = ofToInt(seg[1]);
	//		//        int b = ofToInt(seg[2]);
	//		//
	//		//        ofLogNotice(__FUNCTION__)<<"Changed_ColourPalette: "<<r<<" g "<<g <<" b "<<b;
	//		//    }
	//	}
	//#endif

		//-

		//--------------------------------------------------------------
	void ofxColourLoversHelper::loadFavorites()
	{
		ofLogNotice(__FUNCTION__);

		//ofDirectory _files(path_Global + "favorites");
		_files.reset();
		_files.open(path_Global + "favorites");
		_files.listDir();
		palettes.clear();
		pNames.clear();

		for (int i = 0; i < _files.numFiles(); i++)
		{
			ColourLovePalette cp;
			cp.load(path_Global + "favorites/" + _files.getName(i));
			palettes.push_back(cp);
			pNames.push_back(cp.title);
		}

		lastSearch_Str = "FAVORITES";
		lastMenuTab_Str = lastSearch_Str;

		if (!bFavorites) bFavorites = true;

		//lastSearch_PRE = lastSearch_Str;

		//TODO:
		//BUG: CRASHES IF EMPTY FOLDER
		if (_files.numFiles() > 0) build_Gui_Lab();

		////TODO: startup
		//currPalette = 0;
		////bUpdateSearchFlag = 1;
		//setPalette(currPalette);
		//refreshPalette();

		if (bSearch) bSearch = false;
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::clearFavourites()
	{
		ofLogNotice(__FUNCTION__);

		//ofDirectory _files(path_Global + "favorites");
		_files.reset();
		_files.open(path_Global + "favorites");

		ofxSurfingHelpers::CheckFolder(path_Global + "favorites");

		_files.listDir();

		for (int i = 0; i < _files.numFiles(); i++)
		{
			_files[i].remove();
		}
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::loadHistory()
	{
		ofLogNotice(__FUNCTION__);

		ofxSurfingHelpers::CheckFolder(path_Global + "history");

		//ofDirectory _files(path_Global + "history");
		_files.reset();
		_files.open(path_Global + "history");
		_files.listDir();
		palettes.clear();
		pNames.clear();

		for (int i = 0; i < _files.numFiles(); i++)
		{
			ColourLovePalette cp;
			cp.load(path_Global + "history/" + _files.getName(i));
			palettes.push_back(cp);
			pNames.push_back(cp.title);
		}

		lastSearch_Str = "HISTORY";
		lastMenuTab_Str = lastSearch_Str;

		if (!bHistory) bHistory = true;

		//lastSearch_PRE = lastSearch_Str;

		//TODO: 
		//BUG: CRASHES IF EMPTY FOLDER
		//if (_files.numFiles() > 0) build_Gui_Lab();

		build_Gui_Lab();

		////TODO: startup
		//currPalette = 1;
		////bUpdateSearchFlag = 1;
		//setPalette(currPalette);
		//refreshPalette();
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::clearHistory()
	{
		ofLogNotice(__FUNCTION__);

		ofxSurfingHelpers::CheckFolder(path_Global + "history");

		//ofDirectory _files(path_Global + "history");
		_files.reset();
		_files.open(path_Global + "history");
		_files.listDir();

		for (int i = 0; i < _files.numFiles(); i++)
		{
			_files[i].remove();
		}
	}

	//-

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
	void ofxColourLoversHelper::setPalette_BACK_Refresh(bool &b)
	{
		bUpdated_Palette_BACK = &b;
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::setColor_BACK_Refresh(bool &b)
	{
		bUpdated_Color_BACK = &b;
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::setPalette_BACK_Name(std::string &n)
	{
		myPalette_Name_BACK = &n;
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::keyPressed(ofKeyEventArgs &eventArgs)
	{
		if (ENABLER_Keys && !bCheckMouseOverTextInputLovers)
		{
			const int key = eventArgs.key;

			// modifiers
			bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
			bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
			bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
			bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

			ofLogNotice(__FUNCTION__) << " : " << key;

			//-

			if (key == 'g')
			{
				setToggleVisible();
			}

			// browse
			if (key == OF_KEY_DOWN /*|| key == ' ' && !mod_CONTROL*/)
			{
				nextPalette();
			}
			if (key == OF_KEY_UP /*|| key == ' ' && mod_CONTROL*/)
			{
				prevPalette();
			}

			// select mode: favs, history, search results
			if (key == OF_KEY_BACKSPACE)
			{
				//bFavorites = !bFavorites;

				if (!bFavorites && !bHistory) {
					bFavorites = true;
					bHistory = false;
					bSearch = false;
				}
				else if (bFavorites) {
					bFavorites = false;
					bHistory = true;
					bSearch = false;
				}
				else if (bHistory) {
					bHistory = false;
					bFavorites = true;
					bSearch = false;
				}
			}

			if (key == OF_KEY_RETURN)
			{
				randomPalette();
			}

			//if (key == 'f')
			//{
			//    std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
			//    palettes[currPalette].save(str);
			//    ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
			//}
		}
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
		ofxSurfingHelpers::saveGroup(params, path_AppSettings);

		//-

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
	ofxColourLoversHelper::~ofxColourLoversHelper()
	{
		exit();
	}

	//--------------------------------------------------------------
	void ofxColourLoversHelper::windowResized(int w, int h)
	{
		//build_Gui_Lab();
	}
