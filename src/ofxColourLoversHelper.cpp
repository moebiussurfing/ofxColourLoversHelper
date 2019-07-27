#include "ofxColourLoversHelper.h"

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
ofxColourLoversHelper::ofxColourLoversHelper()
{
    addKeysListeners();
    addMouseListeners();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setup(glm::vec2 _position, glm::vec2 _size){
    position = _position;
    size = _size;

    setup();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setup(){

//    bg.set(16);

    int width = size.x;
//    int width = 200;
    int xInit = 20;
    int dim = 30;

//    gui = new ofxUICanvas(0, 0, width, ofGetHeight());
    gui = new ofxUICanvas(position.x, position.y, size.x, size.y);

    gui->setFont("assets/fonts/PragmataProR_0822.ttf");                     //This loads a new font and sets the GUI font
    gui->setFontSize(OFX_UI_FONT_LARGE, 6);            //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);

    gui->addWidgetDown(new ofxUILabel("COLOUR LOVERS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(width-xInit, 2));

    ofxUITextInput* textinput = new ofxUITextInput("search", "Search",width-xInit);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    textinput = new ofxUITextInput("loverId", "Lover id",width-xInit);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    textinput = new ofxUITextInput("paletteId", "Palette id",width-xInit);
    textinput->setTriggerOnClick(false);
    gui->addWidgetDown(textinput);

    gui->addWidgetDown(new ofxUILabelButton("Favs",false, 84, dim));
    gui->addWidgetRight(new ofxUILabelButton( "History",false, 84, dim));

//    bgLabel = new ofxUILabel("BG: "+ofxColourLovers::hexToWeb(bg), OFX_UI_FONT_MEDIUM);
//    gui->addWidgetDown(bgLabel);

    //getTopPalettesForLover
    //searchPalettes

    //    gui->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 

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
    //  colourLab->getRect()->setWidth(ofGetWidth());

    loadFavourites();
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourLoveEvent(ColourLoveEvent &e) {
    palettes = e.palettes;

    if(!(palettes.size()>0)){
        return;
    }

    // save favorites after succesfuly query search
    for(int i=0;i<palettes.size();i++){
        e.palettes[i].save("palettes/history/"+e.palettes[i].id+ ".xml");
    }

    //return;
//    bg = palettes[0].sortedColours[0];
//    bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));
    updateFlag = 1;
}


//--------------------------------------------------------------
void ofxColourLoversHelper::updateColourLab(){

    if(colourLab){
        ofRemoveListener(colourLab->newGUIEvent, this, &ofxColourLoversHelper::colourLabEvent);

        delete colourLab;
        colourLab = 0;
    }
    int width = 200;

    colourLab = new ofxUIScrollableCanvas(width+20, 0, width, ofGetHeight());
    colourLab->setScrollableDirections(false, true);

    colourLab->setScrollAreaToScreenHeight();
    // colourLab->setScrollAreaToScreen();
    //colourLab->autoSizeToFitWidgets();
    //
    ofAddListener(colourLab->newGUIEvent, this, &ofxColourLoversHelper::colourLabEvent);

    /*
    for(int i=0;i<coloursBasic.size();i++){
        colourLab->removeWidget(coloursBasic[i]);
    }
    coloursBasic.erase(coloursBasic.begin(),coloursBasic.end());
    */

    // if(editMode == EDIT_MODE_FILL_COLOUR){
    int cdim = 20;
    int cdist = 1;
    int col=0;
    int row=0;

    int startY = 50;
    float guiWidth = 198;//ofxUI suddenly introduced odd padding, using x 0 and full width will hide btn. Hmm....

    // colourLab->centerWidgets();
    colourLab->addWidgetDown(new ofxUILabel(lastSearch, OFX_UI_FONT_LARGE));
    colourLab->addWidgetDown(new ofxUISpacer(guiWidth-20, 2));

//    // 1. make sizes like in web palette
//    for(int i=0;i<palettes.size();i++){
//        int currX = 1;
//        int currW = 0;
//        for(int c=0;c<palettes[i].colours.size();c++){
//            //For set colour issues, make sure to set fill colour after widget been added
//            currW = palettes[i].colorWidths[c]*guiWidth;
//            ofxUIButton * btn = new ofxUIButton(("CL_"+ofToString(i)+"_"+ofToString(c)),false,currW,cdim,currX,i*(cdim+4)+startY);
//
//            btn->setLabelVisible(0);
//            colourLab->addWidget(btn);
//            btn->setDrawFill(true);
//            btn->setColorFill(palettes[i].colours[c]);
//            btn->setColorBack(palettes[i].colours[c]);
//            btn->setDrawBack(true);
//            coloursPalette.push_back(btn);
//
//            currX+=currW;
//        }
//    }

    // 2. make all sizes the same in palette
    for(int i=0;i<palettes.size();i++){
        int currX = 1;
        int currW = 0;

        // colors in each palette
        for(int c=0;c<palettes[i].colours.size();c++)
        {
            int numOfColorsInPalette = palettes[i].colours.size();

            //For set colour issues, make sure to set fill colour after widget been added
//            currW = palettes[i].colorWidths[c]*guiWidth;
            currW = guiWidth / numOfColorsInPalette;

            ofxUIButton * btn = new ofxUIButton(("CL_"+ofToString(i)+"_"+ofToString(c)),false,currW,cdim,currX,i*(cdim+4)+startY);

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
};


//--------------------------------------------------------------
void ofxColourLoversHelper::update(){
    if(updateFlag)
    {
        updateColourLab();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::draw(){
//    ofBackground(bg);

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
    cout << "got event from: " << name << endl;

    if(name == "search")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = textinput->getTextString();
        ofxColourLovers::searchPalettes(textinput->getTextString(),40);
    }

    if(name == "loverId")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = textinput->getTextString();
        string s =  textinput->getTextString();
        ofStringReplace(s," ","%20");
        ofxColourLovers::getTopPalettesForLover(s,40);
    }

    if(name == "paletteId")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        lastSearch = textinput->getTextString();
        ofxColourLovers::getPalette(lastSearch);
    }

    currPalette=-1;

    if(name == "Favs")
    {
        loadFavourites();
    }
    if(name == "History")
    {
        loadHistory();
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::colourLabEvent(ofxUIEventArgs &e){
    //if(mouseDown){
    //    //gets event on both down and up
    //   // return;
    //}
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    int uid = e.widget->getID();

    string whatList =  name.substr(0,2);
    string whatColId =  name.substr(3,name.length()-3);

    vector<string> seg = ofSplitString(name,"_");
    int pId = ofToInt(seg[1]);
    int cId = ofToInt(seg[2]);
    ColourLovePalette p = palettes[pId];

//    bg = p.colours[cId];
    ofLog() << "colourLabEvent: "<<name<<" "<<kind <<" "<<uid<<" colour: "<< p.colours[cId]<<" name: "<<p.title<<endl;

    //-

    // set BACK color clicked
    if (myColor_BACK!=nullptr)
    {
        myColor_BACK->set( p.colours[cId] );
        //    (*myColor_BACK) = ( p.colours[cId] );
    }

    // set BACK name clicked
    if (myPalette_Name_BACK!=nullptr)
    {
        (*myPalette_Name_BACK) = p.title;
    }

    // set palette
    setPalette(pId);
}


//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette(int pId){
    ofLog()<<"setPalette "<<pId<<endl;
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

    //--

    // get palettes BACK
    int sizePalette = p.colours.size();
    if (sizePalette>0 && myPalette_BACK!= nullptr)
    {
        myPalette_BACK->clear();
        myPalette_BACK->resize(sizePalette);
        (*myPalette_BACK) = p.colours;
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
void ofxColourLoversHelper::colourPaletteEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    int uid = e.widget->getID();

    if(name=="Favourite" && currPalette>-1){
        palettes[currPalette].save("palettes/favourites/"+palettes[currPalette].id+ ".xml");
    }else{

        vector<string> seg = ofSplitString(name,", ");
        int r = ofToInt(seg[0]);
        int g = ofToInt(seg[1]);
        int b = ofToInt(seg[2]);

//        bg.set(r,g,b);
//        bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));

        ofLog()<<"colourPaletteEvent: "<<r<<" g "<<g <<" b "<<b <<endl;
    }
}


//--------------------------------------------------------------
void ofxColourLoversHelper::loadFavourites(){

    ofDirectory favs("palettes/favourites");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load("palettes/favourites/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="Favourites";
    updateColourLab();
};


//--------------------------------------------------------------
void ofxColourLoversHelper::loadHistory(){

    ofDirectory favs("palettes/history");
    favs.listDir();
    palettes.clear();

    for(int i = 0; i < favs.numFiles(); i++){
        ColourLovePalette cp;
        cp.load("palettes/history/"+favs.getName(i));
        palettes.push_back(cp);
    }

    lastSearch ="History";
    updateColourLab();
};

//--------------------------------------------------------------
void ofxColourLoversHelper::keyPressed( ofKeyEventArgs& eventArgs )
{
    const int & key = eventArgs.key;
    cout << "key: " << key << endl;

    //-

    if (key == 'm')
    {

    }
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
    ofLogNotice("ofxColourLoversHelper") << "mousePressed " <<  x << ", " << y << ", " << button;
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
    ofRemoveListener( ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed );
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


