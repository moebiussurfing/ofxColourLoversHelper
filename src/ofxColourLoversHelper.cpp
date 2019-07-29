#include "ofxColourLoversHelper.h"


//--------------------------------------------------------------
ofxColourLoversHelper::ofxColourLoversHelper()
{
    // default settings
    position = glm::vec2(1000,10);
    size = glm::vec2(200, 200);
    gridPosition = glm::vec2(1210,10);
    gridSize = glm::vec2(200, ofGetHeight());

//    setVisible(true);
    addKeysListeners();
    addMouseListeners();
    isKeysEnabled = true;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setVisible(bool b) {
    isVisible = b;
    colourLab->setVisible(isVisible);
    gui->setVisible(isVisible);

    if(isVisible)
    {
        addKeysListeners();
        addMouseListeners();
        isKeysEnabled = true;
    }
    else
    {
        removeKeysListeners();
        removeMouseListeners();
        isKeysEnabled = false;
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setup(){

    int width = size.x;
    int xInit = 6;
    int dim = 30;

    gui = new ofxUICanvas(position.x, position.y, size.x, size.y);

    gui->setFont("assets/fonts/PragmataProR_0822.ttf");
    gui->setFontSize(OFX_UI_FONT_LARGE, 9);
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 7);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUILabel("COLOUR LOVERS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(width-xInit, 2));

    ofxUITextInput* textinput = new ofxUITextInput("search", "Search keyword",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);
    textinput = new ofxUITextInput("loverId", "Lover id",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);
    textinput = new ofxUITextInput("paletteId", "Palette id",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    gui->addWidgetDown(new ofxUILabelButton("FAVS",false, 0.5*width-xInit, dim, OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUILabelButton( "HISTORY",false, 0.5*width-xInit, dim, OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUISpacer(width-xInit, 2));

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUILabel("PALETTE NAME:", OFX_UI_FONT_MEDIUM));
    lastPaletteName_UI = new ofxUILabel(lastPaletteName, OFX_UI_FONT_SMALL);
    gui->addWidgetDown(lastPaletteName_UI);
    lastPaletteName_UI->setLabel(lastPaletteName);

    gui->addWidgetDown(new ofxUIToggle("FIXED WIDTHS",MODE_fixedSize, 10,10,5));

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUILabelButton("ADD FAVOURITE",false, width-xInit, dim));

    gui->addWidgetDown(new ofxUILabelButton("REMOVE FAVS",false, width-xInit, dim, OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUILabelButton("REMOVE HISTORY",false, width-xInit, dim, OFX_UI_FONT_SMALL));

    //getTopPalettesForLover
    //searchPalettes

    //gui->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM));

    currPalette=-1;
    paletteView = 0;
    updateFlag = 0;
    colourLab = 0;

    ofAddListener(gui->newGUIEvent,this,&ofxColourLoversHelper::guiEvent);
    ofAddListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::colourLoveEvent);

    loadFavourites();

    // auto load first palette of favourites
    if (palettes.size()>0)
    {
        currPalette = 0;
//        updateFlag = 1;
//        setPalette(currPalette);
//        refreshPalette();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setup(glm::vec2 _position, glm::vec2 _size){
    position = _position;
    size = _size;

    setup();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setGrid(glm::vec2 _position, glm::vec2 _size){
    gridPosition = _position;
    gridSize = _size;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourLoveEvent(ColourLoveEvent &e) {
    palettes = e.palettes;

    if(!(palettes.size()>0)){
        return;
    }

    // save results into history after succesfuly query search
    for(int i=0;i<palettes.size();i++){
        e.palettes[i].save(path+"history/"+e.palettes[i].id+ ".xml");
    }

    //return;
//    bg = palettes[0].sortedColours[0];
//    bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));

    updateFlag = 1;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::updateColourLab(){

    if(colourLab)
    {
        ofRemoveListener(colourLab->newGUIEvent, this, &ofxColourLoversHelper::colourLabEvent);
        delete colourLab;
        colourLab = 0;
    }

    int xInit = 6;
    int padding = 5;
    int width;
    width = gridSize.x;

    colourLab = new ofxUIScrollableCanvas(gridPosition.x, gridPosition.y, width, gridSize.y);

    colourLab->setFont("assets/fonts/PragmataProR_0822.ttf");
    colourLab->setFontSize(OFX_UI_FONT_LARGE, 9);
    colourLab->setFontSize(OFX_UI_FONT_MEDIUM, 7);
    colourLab->setFontSize(OFX_UI_FONT_SMALL, 6);
    colourLab->setScrollableDirections(false, true);
    colourLab->setScrollAreaToScreenHeight();
    colourLab->addWidgetDown(new ofxUISpacer(width-xInit, 0));

    ofAddListener(colourLab->newGUIEvent, this, &ofxColourLoversHelper::colourLabEvent);

    int cdim;
    int cdist = 1;
    int col=0;
    int row=0;
    int startY = 50;

    float guiWidth = width-2;

    //cdim = 20;
    cdim = guiWidth/5.;
    //maybe some palette have less than 5 colors, ie: 4, and then can cause problems...
    // so we fix to 5 as usual they have..

    // colourLab->centerWidgets();
    colourLab->addWidgetDown(new ofxUILabel(lastSearch, OFX_UI_FONT_MEDIUM));
//    colourLab->addWidgetDown(new ofxUISpacer(guiWidth-20, 2));
    colourLab->addWidgetDown(new ofxUISpacer(width-xInit, 2));

    //-

    for(int i=0;i<palettes.size();i++)
    {
        int currX = 1;
        int currW = 0;

        // colors in each palette
        for(int c=0;c<palettes[i].colours.size();c++)
        {
            int numOfColorsInPalette = palettes[i].colours.size();

            // For set colour issues, make sure to set fill colour after widget been added

            if (!MODE_fixedSize)
            {
                // different sizes with original colourLover Palettes
                currW = palettes[i].colorWidths[c]*guiWidth;
            }
            else
            {
                // same size for each color
                currW = guiWidth / numOfColorsInPalette;
            }

            ofxUIButton * btn = new ofxUIButton(("CL_"+ofToString(i)+"_"+ofToString(c)),false,
                    currW, cdim,
                    currX, i*(cdim+4)+startY);

            btn->setLabelVisible(0);
            colourLab->addWidget(btn);
            btn->setDrawFill(true);
            btn->setColorFill(palettes[i].colours[c]);
            btn->setColorBack(palettes[i].colours[c]);
            btn->setDrawBack(true);
            coloursPalette.push_back(btn);

            currX+=currW;
        }
    }

    colourLab->getRect()->setHeight(palettes.size()*(cdim+4)+startY);
    colourLab->setSnapping(0);
    updateFlag = 0;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::update()
{
    if(updateFlag)
    {
        updateColourLab();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::draw(){
//    //draw raw palettes without gui
//    if(palettes.size()>0){
//        for(int i=0;i<palettes.size();i++){
//             palettes[i].draw(500,25*i,500,20);
//        }
//    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    ofLogNotice("ofxColourLoversHelper") << "got event from: " << name;
//    ofLogNotice("ofxColourLoversHelper") << "currPalette: " << currPalette;

    if(name == "search")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = "'"+textinput->getTextString()+"'";
        ofxColourLovers::searchPalettes(textinput->getTextString(),40);
    }

    else if(name == "loverId")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = textinput->getTextString();
        string s =  textinput->getTextString();
        ofStringReplace(s," ","%20");
        ofxColourLovers::getTopPalettesForLover(s,40);
    }

    else if(name == "paletteId")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = textinput->getTextString();
        ofxColourLovers::getPalette(lastSearch);
    }

    else if(name=="ADD FAVOURITE" && currPalette>-1)
    {
        ofxUIButton *but = e.getButton();
        if(but->getValue())
        {
            string str = path+"favourites/"+palettes[currPalette].id+ ".xml";
        palettes[currPalette].save(str);
        ofLogNotice("ofxColourLoversHelper")<<"saved favorite: "<<str;
        }
    }

        //-


    else if(name == "FAVS")
    {
        ofxUIButton *but = e.getButton();
        if(but->getValue())
            loadFavourites();
    }
    else if(name == "HISTORY")
    {
        ofxUIButton *but = e.getButton();
        if(but->getValue())
            loadHistory();
    }
    else if(name == "REMOVE FAVS")
    {
        ofxUIButton *but = e.getButton();
        if(but->getValue())
            clearFavourites();
    }
    else if(name == "REMOVE HISTORY")
    {
        ofxUIButton *but = e.getButton();
        if(but->getValue())
            clearHistory();
    }

    else if(name == "FIXED WIDTHS")
    {
//        cout << "FIXED WIDTHS" << endl;
        bool MODE_fixedSize_PRE = MODE_fixedSize;
        ofxUIToggle *toggle = e.getToggle();
        MODE_fixedSize = toggle->getValue();
//        cout << MODE_fixedSize << endl;
        if (MODE_fixedSize != MODE_fixedSize_PRE)
        {
            updateColourLab();
        }
    }

    currPalette=-1;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::refreshPalette()
{
    ofLogNotice("ofxColourLoversHelper") << "refreshPalette:currPalette: " << currPalette;
    ColourLovePalette p = palettes[currPalette];
    lastPaletteName = "'"+p.title+"'";
    lastPaletteName_UI->setLabel(lastPaletteName);

    //-

    // set BACK name clicked
    if (myPalette_Name_BACK!=nullptr)
    {
        (*myPalette_Name_BACK) = p.title;
    }

    // get and set palettes BACK
    int sizePalette = p.colours.size();
    if (sizePalette>0 && myPalette_BACK!= nullptr)
    {
        myPalette_BACK->clear();
        myPalette_BACK->resize(sizePalette);
        (*myPalette_BACK) = p.colours;
    }

    //-
}


//--------------------------------------------------------------
void ofxColourLoversHelper::nextPalette() {
    if (!isKeysEnabled) return;

    ofLogNotice("ofxColourLoversHelper") << "nextPalette:";
    if (currPalette==-1)
    {
        ofLogWarning("ofxColourLoversHelper") << "PALETTE NOT LOADED. SKIP jump:";
        return;
    }

    if (currPalette < palettes.size()-1)
        currPalette++;
    ofLogNotice("ofxColourLoversHelper") << "currPalette: " << currPalette;

    refreshPalette();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::prevPalette() {
    if (!isKeysEnabled) return;

    ofLogNotice("ofxColourLoversHelper") << "prevPalette:";
    if (currPalette==-1)
    {
        ofLogWarning("ofxColourLoversHelper") << "PALETTE NOT LOADED. SKIP jump:";
        return;
    }

    if (currPalette > 0)
        currPalette--;
    ofLogNotice("ofxColourLoversHelper") << "currPalette: " << currPalette;

    refreshPalette();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourLabEvent(ofxUIEventArgs &e){

    string name = e.widget->getName();
    int kind = e.widget->getKind();
    int uid = e.widget->getID();

    string whatList =  name.substr(0,2);
    string whatColId =  name.substr(3,name.length()-3);

    vector<string> seg = ofSplitString(name,"_");
    int pId = ofToInt(seg[1]);
    int cId = ofToInt(seg[2]);
    ColourLovePalette p = palettes[pId];

    ofLogNotice("ofxColourLoversHelper") << "colourLabEvent: "<<name<<" "<<kind<<" "<<uid<<" colour: "<< p.colours[cId]<<" name: "<<p.title;

    lastPaletteName = "'"+p.title+"'";
    lastPaletteName_UI->setLabel(lastPaletteName);

    //-

    // set BACK color clicked
    if (myColor_BACK!=nullptr)
    {
        myColor_BACK->set( p.colours[cId] );
    }

    // set BACK name clicked
    if (myPalette_Name_BACK!=nullptr)
    {
        (*myPalette_Name_BACK) = p.title;
    }

    // set palette
    setPalette(pId);
    ofLogNotice("ofxColourLoversHelper") << "colourLabEvent:setPalette:pId: "<<pId;
    ofLogNotice("ofxColourLoversHelper") << "colourLabEvent:currPalette: " << currPalette;

}


//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette(int pId)
{
    ofLogNotice("ofxColourLoversHelper")<<"setPalette "<<pId;
    if(currPalette == pId){
        return;
    }
    currPalette = pId;

    ColourLovePalette p = palettes[pId];
//    lastPaletteName = p.title;

    //--

    // get palettes BACK

    int sizePalette = p.colours.size();
    if (sizePalette>0 && myPalette_BACK!= nullptr)
    {
        myPalette_BACK->clear();
        myPalette_BACK->resize(sizePalette);
        (*myPalette_BACK) = p.colours;
//        lastPaletteName = p.title;
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourPaletteEvent(ofxUIEventArgs &e)
{
//    string name = e.widget->getName();
//    int kind = e.widget->getKind();
//    int uid = e.widget->getID();
//
//    // TODO: add button with same name
//    if(name=="ADD FAVOURITE" && currPalette>-1)
//    {
//        string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
//        palettes[currPalette].save(str);
//        ofLogNotice("ofxColourLoversHelper")<<"saved favorite: "<<str;
//    }
//    else
//    {
//        vector<string> seg = ofSplitString(name,", ");
//        int r = ofToInt(seg[0]);
//        int g = ofToInt(seg[1]);
//        int b = ofToInt(seg[2]);
//
//        ofLogNotice("ofxColourLoversHelper")<<"colourPaletteEvent: "<<r<<" g "<<g <<" b "<<b;
//    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::loadFavourites(){
    ofLogNotice("ofxColourLoversHelper")<<"loadFavourites";

    ofDirectory favs(path+"favourites");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load(path+"favourites/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="FAVOURITES";
    updateColourLab();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::clearFavourites(){
    ofLogNotice("ofxColourLoversHelper")<<"clearFavourites";

    ofDirectory favs(path+"favourites");
    favs.listDir();

    for(int i = 0; i < favs.numFiles(); i++){
        favs[i].remove();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::loadHistory(){
    ofLogNotice("ofxColourLoversHelper")<<"loadHistory";

    ofDirectory favs(path+"history");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load(path+"history/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="HISTORY";
    updateColourLab();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::clearHistory(){
    ofLogNotice("ofxColourLoversHelper")<<"clearHistory";

    ofDirectory favs(path+"/history");
    favs.listDir();

    for(int i = 0; i < favs.numFiles(); i++){
        favs[i].remove();
    }
}


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
void ofxColourLoversHelper::setPalette_Name_BACK(string &n)
{
    myPalette_Name_BACK = &n;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::keyPressed( ofKeyEventArgs& eventArgs )
{
    const int & key = eventArgs.key;
//    cout << "key: " << key << endl;

    //-

//    if (key == 'f')
//    {
//        string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
//        palettes[currPalette].save(str);
//        ofLogNotice("ofxColourLoversHelper")<<"saved favorite: "<<str;
//    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::keyReleased( ofKeyEventArgs& eventArgs )
{
    if( eventArgs.key == ' ')
    {
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::addKeysListeners()
{
    ofAddListener( ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed );
}


//--------------------------------------------------------------
void ofxColourLoversHelper::removeKeysListeners()
{
    ofRemoveListener( ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed );
}


//--------------------------------------------------------------
void ofxColourLoversHelper::mouseDragged(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColourLoversHelper") << "mouseDragged " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColourLoversHelper") << "mousePressed " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColourLoversHelper") << "mouseReleased " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::addMouseListeners()
{
    ofAddListener( ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged );
    ofAddListener( ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed );
    ofAddListener( ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased );
}


//--------------------------------------------------------------
void ofxColourLoversHelper::removeMouseListeners()
{
    ofRemoveListener( ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged );
    ofRemoveListener( ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed );
    ofRemoveListener( ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased );
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
}


