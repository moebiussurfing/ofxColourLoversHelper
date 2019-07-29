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
    }
    else
    {
        removeKeysListeners();
        removeMouseListeners();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setup(){

    int width = size.x;
//    int xInit = 20;
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

    ofxUITextInput* textinput = new ofxUITextInput("search", "Search",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    textinput = new ofxUITextInput("loverId", "Lover id",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    textinput = new ofxUITextInput("paletteId", "Palette id",width-xInit, OFX_UI_FONT_MEDIUM);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    gui->addWidgetDown(new ofxUILabelButton("Favs",false, 0.5*width-xInit, dim, OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUILabelButton( "History",false, 0.5*width-xInit, dim, OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 2));

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUILabel("PALETTE NAME:", OFX_UI_FONT_MEDIUM));
    lastPaletteName_UI = new ofxUILabel(lastPaletteName, OFX_UI_FONT_SMALL);
    gui->addWidgetDown(lastPaletteName_UI);
    lastPaletteName_UI->setLabel(lastPaletteName);

    gui->addWidgetDown(new ofxUISpacer(width-xInit, 0));
    gui->addWidgetDown(new ofxUILabelButton("FAVOURITE",false, width-xInit, dim));

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

    palettesX = width*2+40;

    // float length = 320-xInit;

    /*
    for(int i = 0; i < 30; i++)
    {
        colourLab->addSlider("SLIDER " +ofToString(i), 0, 100, i*3.3, length-xInit, dim);
        colourLab->addSpacer(length-xInit, 2);
    }

    vector<ofxUIWidget*> spacers = colourLab->getWidgetsOfType(OFX_UI_WIDGET_SPACER);

    for(int i = 0; i < spacers.size(); i++)
    {
        ofxUISpacer *s = (ofxUISpacer *) spacers[i];
        s->toggleVisible();
    }
    */
    //colourLab->autoSizeToFitWidgets();
    //colourLab->getRect()->setWidth(ofGetWidth());

    loadFavourites();
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

    //    int xInit = 20;
    int xInit = 6;
    int padding = 5;
    int width;
    width = gridSize.x;

    colourLab = new ofxUIScrollableCanvas(gridPosition.x, gridPosition.y, width, gridSize.y);

    colourLab->addWidgetDown(new ofxUISpacer(width-xInit, 0));

    colourLab->setFont("assets/fonts/PragmataProR_0822.ttf");
    colourLab->setFontSize(OFX_UI_FONT_LARGE, 9);
    colourLab->setFontSize(OFX_UI_FONT_MEDIUM, 7);
    colourLab->setFontSize(OFX_UI_FONT_SMALL, 6);

    colourLab->setScrollableDirections(false, true);
    colourLab->setScrollAreaToScreenHeight();
    // colourLab->setScrollAreaToScreen();
    // colourLab->autoSizeToFitWidgets();

    ofAddListener(colourLab->newGUIEvent, this, &ofxColourLoversHelper::colourLabEvent);

    /*
    for(int i=0;i<coloursBasic.size();i++){
        colourLab->removeWidget(coloursBasic[i]);
    }
    coloursBasic.erase(coloursBasic.begin(),coloursBasic.end());
    */

    int cdim;
    int cdist = 1;
    int col=0;
    int row=0;
    int startY = 50;

    //ofxUI suddenly introduced odd padding, using x 0 and full width will hide btn. Hmm....
    float guiWidth = width-2;

    //cdim = 20;

    cdim = guiWidth/5.;
    //maybe some palette have less than 5 colors, ie: 4, and then can cause problems...
    // so we fix to 5 as usual they have..

    // colourLab->centerWidgets();
    colourLab->addWidgetDown(new ofxUILabel(lastSearch, OFX_UI_FONT_MEDIUM));
    colourLab->addWidgetDown(new ofxUISpacer(guiWidth-20, 2));

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

            // different sizes with original colourLover Palettes
            //currW = palettes[i].colorWidths[c]*guiWidth;

            // same size for each color
            currW = guiWidth / numOfColorsInPalette;

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
        lastSearch = textinput->getTextString();
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

    else if(name=="FAVOURITE" && currPalette>-1)
    {
        string str = path+"favourites/"+palettes[currPalette].id+ ".xml";
        palettes[currPalette].save(str);
        ofLogNotice("ofxColourLoversHelper")<<"saved favorite: "<<str;
    }

        //-


    else if(name == "Favs")
    {
        loadFavourites();
    }
    else if(name == "History")
    {
        loadHistory();
    }
    else if(name == "REMOVE FAVS")
    {
        clearFavourites();
    }
    else if(name == "REMOVE HISTORY")
    {
        clearHistory();
    }

    currPalette=-1;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::nextPalette() {
    currPalette++;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::prevPalette() {

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

//    // create preview gradient
//    if(paletteView)
//    {
//        ofRemoveListener(paletteView->newGUIEvent, this, &ofxColourLoversHelper::colourPaletteEvent);
//        delete paletteView;
//        paletteView = 0;
//    }
////    paletteView  = new ofxUICanvas(palettesX, 0, ofGetWidth()-palettesX, ofGetHeight());
//    paletteView  = new ofxUICanvas(palettesX, 0, 300-palettesX, 400);
//
//    ofAddListener(paletteView->newGUIEvent, this, &ofxColourLoversHelper::colourPaletteEvent);
//
//    int cdist = 1;
//    int col=0;
//    int row=0;
//
//    int startY = 50;
////    float guiWidth = ofGetWidth()-palettesX-2;
//    float guiWidth = 300-palettesX-2;
//
//    int colourNum = 20;

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

    //--

//    // create preview gradient
//    paletteView->addWidgetRight(new ofxUILabel(p.id+" - ", OFX_UI_FONT_LARGE));
//    paletteView->addWidgetRight(new ofxUILabel(p.title, OFX_UI_FONT_LARGE));
//    paletteView->addWidget(new ofxUIImageButton(800,5,30,30,true,"GUI/images/fav.png","Favourite"));
//    paletteView->addWidgetDown(new ofxUISpacer(400, 2));

    //-

//    // create preview gradient
//
//    colourRanges.clear();
//
////    float cdim = (float)(ofGetHeight()-startY )/(float)colourNum;
//    float cdim = (float)(400-startY )/(float)colourNum;
//
//    vector<ofColor> colourRange;
//
//    int currX = 1;
//    int step = 2;
//    string hex;
//
//    for(int i=0;i<p.colours.size();i++)
//    {
//        float currW = 0;
//        currW = p.colorWidths[i]*guiWidth;
//
//        ofColor shade;
//        shade.set(p.colours[i].r,p.colours[i].g,p.colours[i].b);
//
//        int startBright = shade.getBrightness()+colourNum*step/2;
//        //cap within limits
//        startBright = max(0,startBright);
//        startBright = min(255-colourNum*step,startBright);
//
//        shade.setBrightness(startBright);
//
//        for(int c=0;c<colourNum;c++)
//        {
//            if(c==0){
//                hex = ofToString(p.colours[i]);
//            }else{
//                shade.setBrightness(shade.getBrightness()-step);
//                hex = ofToString(shade);
//            }
//
//
//            ofxUIButton * btn = new ofxUIButton(hex,false,currW,cdim,currX,(c)*(cdim)+startY);
//
//            //
//
//            btn->setLabelVisible(0);
//            paletteView->addWidget(btn);
//            btn->setDrawFill(true);
//            if(c==0){
//                btn->setColorFill(p.colours[i]);
//                btn->setColorBack(p.colours[i]);
//            }else{
//                btn->setColorFill(shade);
//                btn->setColorBack(shade);
//            }
//
//
//            btn->setDrawBack(true);
//            colourRanges.push_back(btn);
//        }
//        currX+=currW;
//        /*if((cdist+cdim)*col+cdim<guiWidth-20){
//         col++;
//         }else{*/
//        //  col=0;
//        // row++;
//        //}
//    }

    //--

    //gui->addWidgetDown(new ofxUISpacer(width-xInit, 2));
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourPaletteEvent(ofxUIEventArgs &e)
{
//    string name = e.widget->getName();
//    int kind = e.widget->getKind();
//    int uid = e.widget->getID();
//
//    // TODO: add button with same name
//    if(name=="FAVOURITE" && currPalette>-1)
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

    ofDirectory favs(path+"favourites");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load(path+"favourites/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="Favourites";
    updateColourLab();
}



//--------------------------------------------------------------
void ofxColourLoversHelper::clearFavourites(){

    ofDirectory favs(path+"favourites");
    favs.listDir();

    for(int i = 0; i < favs.numFiles(); i++){
        favs[i].remove();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::loadHistory(){

    ofDirectory favs(path+"history");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load(path+"history/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="History";
    updateColourLab();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::clearHistory(){

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


