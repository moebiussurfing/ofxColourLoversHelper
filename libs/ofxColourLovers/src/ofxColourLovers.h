/*
 *  ofxColourLovers.h
 *  This is a selective goodies version of the ColourLovers API
	http://www.colourlovers.com/api
 *
 *  Created by Andreas Borg on 05/10/2012
 *  Copyright 2012 http://crea.tion.to
 *
 *  https://github.com/borg/ofxColourLovers
 */

#ifndef _ofxColourLovers
#define _ofxColourLovers

#include "ofMain.h"

#include "ofxHTTP.h"

#include "ofxXmlSettings.h"
#include "ColourCallTypes.h"

#include "ColourLoveEvent.h"

//http://www.colourlovers.com/api
//http://www.colourlovers.com/api/palettes/top/?lover=andreasborg&showPaletteWidths=1

#define CL_URL_TOP "http://www.colourlovers.com/api/palettes/top/?showPaletteWidths=1"
#define CL_URL_NEW "http://www.colourlovers.com/api/palettes/new/?showPaletteWidths=1"
#define CL_URL_PALETTE "http://www.colourlovers.com/api/palette/"

/*
 These string to hex conversions aren't trivial.
 */
static int stringToHex(string hex) {
	int aHex;
	stringstream convert(hex);
	convert >> std::hex >> aHex;
	return aHex;
}
static void hexToColor(ofColor &col, string hex) {
	string r = hex.substr(0, 2);
	int ri = stringToHex(r);
	string g = hex.substr(2, 2);
	int gi = stringToHex(g);
	string b = hex.substr(4, 2);
	int bi = stringToHex(b);
	col.set(ri, gi, bi);
}

class ofxColourLovers {

public:

	ofxColourLovers();
	~ofxColourLovers();

	/*
	 orderCol =	X [Where X can be: dateCreated, score, name, numVotes, or numViews]
	 sortBy	= X [Where X can be: ASC or DESC. Default ASC]
	 */

	static void getTopPalettes(int numResults = 20, int resultOffset = 0, string orderCol = "numVotes", string sortBy = "DESC") {

		getSingleton().callType = CL_GET_TOP_PALETTES;

		string url = CL_URL_TOP;
		url += "&numResults=" + ofToString(numResults) + "&resultOffset=" + ofToString(resultOffset) + "&orderCol=" + orderCol + "&sortBy=" + sortBy;
		ofLogNotice(__FUNCTION__) << endl << "url: " << url;

		// Create a client.
		ofxHTTP::Client client;
		ofxHTTP::GetRequest request(url);
		ofxHTTP::Context context;
		ofxHTTP::ClientSessionSettings sessionSettings;
		context.setClientSessionSettings(sessionSettings);

		//ofLogNotice() << "submitting getTopPalettes";
		////status = PENDING;
		//getSingleton().http.addForm(form);
		//if (!getSingleton().http.isThreadRunning()) {
		//	getSingleton().http.start();
		//}
		////getSingleton().http.submitForm(form);

		// ofxHTTP
		{
			try
			{
				auto response = client.execute(context, request);
				if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
				{
					ofLogNotice(__FUNCTION__) << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
					ofBuffer buffer(response->stream());

					ofLogNotice(__FUNCTION__) << "Content Begin";
					ofLogNotice(__FUNCTION__) << buffer << endl;
					ofLogNotice(__FUNCTION__) << "Content End";

					ofxXmlSettings xml;
					if (response->isXml())
					{
						xml.loadFromBuffer(response->buffer());
					}
					newResponse(xml);
				}
				else
				{
					ofLogError(__FUNCTION__) << response->getStatus() << " " << response->getReason();
				}
			}
			catch (const Poco::Exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.displayText();
			}
			catch (const std::exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.what();
			}
		}
	}

	static void getNewPalettes(int numResults = 20, int resultOffset = 0, string orderCol = "numVotes", string sortBy = "DESC") {

		getSingleton().callType = CL_GET_NEW_PALETTES;

		string url = CL_URL_NEW;
		url += "&numResults=" + ofToString(numResults) + "&resultOffset=" + ofToString(resultOffset) + "&orderCol=" + orderCol + "&sortBy=" + sortBy;
		ofLogNotice(__FUNCTION__) << endl << "url: " << url;

		// Create a client.
		ofxHTTP::Client client;
		ofxHTTP::GetRequest request(url);
		ofxHTTP::Context context;
		ofxHTTP::ClientSessionSettings sessionSettings;
		context.setClientSessionSettings(sessionSettings);

		//ofLogNotice() << "submitting getTopPalettes";
		////status = PENDING;
		//getSingleton().http.addForm(form);
		//if (!getSingleton().http.isThreadRunning()) {
		//	getSingleton().http.start();
		//}
		////getSingleton().http.submitForm(form);

		// ofxHTTP
		{
			try
			{
				auto response = client.execute(context, request);
				if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
				{
					ofLogNotice(__FUNCTION__) << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
					ofBuffer buffer(response->stream());

					ofLogNotice(__FUNCTION__) << "Content Begin";
					ofLogNotice(__FUNCTION__) << buffer << endl;
					ofLogNotice(__FUNCTION__) << "Content End";

					ofxXmlSettings xml;
					if (response->isXml())
					{
						xml.loadFromBuffer(response->buffer());
					}
					newResponse(xml);
				}
				else
				{
					ofLogError(__FUNCTION__) << response->getStatus() << " " << response->getReason();
				}
			}
			catch (const Poco::Exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.displayText();
			}
			catch (const std::exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.what();
			}
		}
	}
	
	static void getRandomPalette() {

		getSingleton().callType = CL_GET_RANDOM_PALETTE;

		string url = "http://www.colourlovers.com/api/palettes/random";

		ofxHTTP::Client client;
		ofxHTTP::GetRequest request(url);
		ofxHTTP::Context context;
		ofxHTTP::ClientSessionSettings sessionSettings;
		context.setClientSessionSettings(sessionSettings);

		// ofxHTTP
		{
			try
			{
				auto response = client.execute(context, request);
				if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
				{
					ofLogNotice(__FUNCTION__) << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
					ofBuffer buffer(response->stream());

					ofLogNotice(__FUNCTION__) << "Content Begin";
					ofLogNotice(__FUNCTION__) << buffer << endl;
					ofLogNotice(__FUNCTION__) << "Content End";

					ofxXmlSettings xml;
					if (response->isXml())
					{
						xml.loadFromBuffer(response->buffer());
					}
					newResponse(xml);
				}
				else
				{
					ofLogError(__FUNCTION__) << response->getStatus() << " " << response->getReason();
				}
			}
			catch (const Poco::Exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.displayText();
			}
			catch (const std::exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.what();
			}
		}
	}

	
	//static void getTopPalettesForLover(string lover, int numResults = 20, int resultOffset = 0, string orderCol = "numVotes", string sortBy = "DESC") {

	//	//getSingleton().callType = CL_GET_TOP_PALETTES_FOR_LOVER;

	//	//ofxHttpForm form;//submitting form
	//	//form.name = "getTopPalettesForLover";
	//	//form.method = OFX_HTTP_POST;

	//	//string url = CL_URL_TOP;
	//	//url += "&lover=" + lover + "&numResults=" + ofToString(numResults) + "&resultOffset=" + ofToString(resultOffset) + "&orderCol=" + orderCol + "&sortBy=" + sortBy;
	//	//form.action = url;

	//	//ofLogNotice(__FUNCTION__) << endl << "url: " << url;

	//	////form.clearFormFields();//clear out old send data

	//	///*
	//	// for(int i=0;i<form1.elements.size();i++){
	//	// string v = form1.elements[i]->getValue();
	//	// //  isEmpty = ((v=="0" || v =="") && isEmpty);//only test if not already proved wrong
	//	// form.addFormField(form1.elements[i]->name,v);
	//	// // ofLogNotice(__FUNCTION__)<<form1.elements[i]->name<< v <<endl;
	//	// }*/

	//	//ofLogNotice() << "submitting getTopPalettesForLover: " << lover;
	//	////status = PENDING;

	//	//getSingleton().http.addForm(form);
	//	//if (!getSingleton().http.isThreadRunning()) {
	//	//	getSingleton().http.start();
	//	//}

	//	//////getSingleton().http.submitForm(form);//not threaded// ofxHTTP
	//	//{
	//	//	// Do the query!
	//	//	try
	//	//	{
	//	//		// Execute the request within the given context.
	//	//		auto response = client.execute(context, request);
	//	//		//std::unique_ptr<ofx::HTTP::Response>

	//	//		// Check the response.
	//	//		if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
	//	//		{
	//	//			ofLogNotice(__FUNCTION__) << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
	//	//			ofBuffer buffer(response->stream());

	//	//			ofLogNotice(__FUNCTION__) << "Content Begin";
	//	//			ofLogNotice(__FUNCTION__) << buffer << endl;
	//	//			ofLogNotice(__FUNCTION__) << "Content End";
	//	//			
	//	//			ofxXmlSettings xml;
	//	//			if (response->isXml())
	//	//			{
	//	//				xml.loadFromBuffer(response->buffer());
	//	//			}
	//	//			newResponse(xml);
	//	//		}
	//	//		else
	//	//		{
	//	//			ofLogError(__FUNCTION__) << response->getStatus() << " " << response->getReason();
	//	//		}
	//	//	}
	//	//	catch (const Poco::Exception& exc)
	//	//	{
	//	//		ofLogError(__FUNCTION__) << exc.displayText();
	//	//	}
	//	//	catch (const std::exception& exc)
	//	//	{
	//	//		ofLogError(__FUNCTION__) << exc.what();
	//	//	}
	//	//}
	//}

	static void searchPalettes(string keywords, int numResults = 20, int resultOffset = 0, string orderCol = "numVotes", string sortBy = "DESC") {
		ofLogNotice(__FUNCTION__) << endl << "--------------------------------------------------------------" << endl << "search: " << keywords << " max: " << numResults;

		getSingleton().callType = CL_SEARCH;

		ofLogNotice(__FUNCTION__) << "submitting search: " << keywords;

		string url = CL_URL_TOP;
		url += "&keywords=" + keywords + "&numResults=" + ofToString(numResults) + "&resultOffset=" + ofToString(resultOffset) + "&orderCol=" + orderCol + "&sortBy=" + sortBy;
		ofLogNotice(__FUNCTION__) << "url: " << url;

		ofxHTTP::Client client;
		ofxHTTP::GetRequest request(url);
		ofxHTTP::Context context;
		ofxHTTP::ClientSessionSettings sessionSettings;
		context.setClientSessionSettings(sessionSettings);

		////status = PENDING;
		//getSingleton().http.addForm(form);
		//if (!getSingleton().http.isThreadRunning()) {
		//	getSingleton().http.start();
		//}
		////getSingleton().http.submitForm(form);

		// ofxHTTP
		{
			try
			{
				auto response = client.execute(context, request);
				if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
				{
					ofLogNotice(__FUNCTION__) << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
					ofBuffer buffer(response->stream());

					ofLogNotice(__FUNCTION__) << "Content Begin";
					ofLogNotice(__FUNCTION__) << buffer << endl;
					ofLogNotice(__FUNCTION__) << "Content End";
					
					ofxXmlSettings xml;
					if (response->isXml())
					{
						xml.loadFromBuffer(response->buffer());
					}
					newResponse(xml);
				}
				else
				{
					ofLogError(__FUNCTION__) << response->getStatus() << " " << response->getReason();
				}
			}
			catch (const Poco::Exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.displayText();
			}
			catch (const std::exception& exc)
			{
				ofLogError(__FUNCTION__) << exc.what();
			}
		}
	}

	/*
	static void getPalette(string id) {

		//getSingleton().callType = CL_GET_PALETTE;
		//ofxHttpForm form;//submitting form
		//form.name = "getPalette";
		//form.method = OFX_HTTP_POST;
		//string url = CL_URL_PALETTE + id + "?showPaletteWidths=1";
		//form.action = url;

		////ofLogNotice(__FUNCTION__)<<"getPalette: "<<id<<endl;
		//ofLogNotice(__FUNCTION__) << "getPalette: " << id;

		////status = PENDING;
		//getSingleton().http.addForm(form);

		//if (!getSingleton().http.isThreadRunning()) {
		//	getSingleton().http.start();
		//}
		////getSingleton().http.submitForm(form);
	}
	*/

	static string hexToWeb(ofColor col) {
		return "#" + ofToHex(col.r) + ofToHex(col.g) + ofToHex(col.b);
	}

	//--

private:

	static ofxXmlSettings serverReply;

	ColourCallType callType;

	static ofxColourLovers &getSingleton(); // check out this pattern...it's quite sweet

	//--

	static void newResponse(ofxXmlSettings _xml)
	{
		//-

		ColourLoveEvent newEvent;
		//newEvent.message = responseStr;

		switch (getSingleton().callType) {

		case CL_SEARCH:
			newEvent.type = CL_SEARCH;
			//parsePalettes(serverReply, newEvent);
			parsePalettes(_xml, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_SEARCH reply";
			break;

		case CL_GET_TOP_PALETTES:
			newEvent.type = CL_GET_TOP_PALETTES;
			parsePalettes(_xml, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_TOP_PALETTES reply";
			break;

		case CL_GET_NEW_PALETTES:
			newEvent.type = CL_GET_NEW_PALETTES;
			parsePalettes(_xml, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_NEW_PALETTES reply";
			break;

		case CL_GET_RANDOM_PALETTE:
			newEvent.type = CL_GET_RANDOM_PALETTE;
			parsePalettes(_xml, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_RANDOM_PALETTE reply";
			break;

			//case CL_SEARCH:
			//	newEvent.type = CL_SEARCH;
			//	parsePalettes(serverReply, newEvent);
			//	ofLogNotice(__FUNCTION__) << "CL_SEARCH reply";
			//	break;
			//case CL_GET_TOP_PALETTES:
			//	newEvent.type = CL_GET_TOP_PALETTES;
			//	parsePalettes(serverReply, newEvent);
			//	ofLogNotice(__FUNCTION__) << "CL_GET_TOP_PALETTES reply";
			//	break;
			//case CL_GET_TOP_PALETTES_FOR_LOVER:
			//	newEvent.type = CL_GET_TOP_PALETTES_FOR_LOVER;
			//	parsePalettes(serverReply, newEvent);
			//	ofLogNotice(__FUNCTION__) << "CL_GET_TOP_PALETTES_FOR_LOVER reply";
			//	break;
			//case CL_GET_PALETTE:
			//	newEvent.type = CL_GET_PALETTE;
			//	parsePalettes(serverReply, newEvent);
			//	ofLogNotice(__FUNCTION__) << "CL_GET_PALETTE reply";
			//	break;
		default:
			break;
		}

		//pass this on so you can save palettes as you please
		//newEvent.xml = serverReply;

		ofNotifyEvent(ColourLoveEvent::events, newEvent);
	}

	//--

	/*
	virtual void newResponse(ofxHttpResponse &response) {
		// printf("%s\n", response.responseBody.c_str());
		string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
		//ofLogNotice(__FUNCTION__)<<responseStr<<endl;
		serverReply.loadFromBuffer(response.responseBody); // parse string

		ofLogNotice(__FUNCTION__) << endl << "--------------------------------------------------------------" << endl << "response: : " << responseStr;

		//TODO:
		if (responseStr == "-1")
		{
			ofLogError(__FUNCTION__) << "INVALID RESPONSE !";
			return;
		}

		//-

		ColourLoveEvent newEvent;
		//newEvent.message = responseStr;

		switch (getSingleton().callType) {
		case CL_GET_TOP_PALETTES:
			newEvent.type = CL_GET_TOP_PALETTES;
			parsePalettes(serverReply, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_TOP_PALETTES reply";
			break;
		case CL_GET_TOP_PALETTES_FOR_LOVER:
			newEvent.type = CL_GET_TOP_PALETTES_FOR_LOVER;
			parsePalettes(serverReply, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_TOP_PALETTES_FOR_LOVER reply";
			break;
		case CL_SEARCH:
			newEvent.type = CL_SEARCH;
			parsePalettes(serverReply, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_SEARCH reply";
			break;

		case CL_GET_PALETTE:
			newEvent.type = CL_GET_PALETTE;
			parsePalettes(serverReply, newEvent);
			ofLogNotice(__FUNCTION__) << "CL_GET_PALETTE reply";
			break;

		default:
			break;
		}

		//pass this on so you can save palettes as you please
		newEvent.xml = serverReply;

		ofNotifyEvent(ColourLoveEvent::events, newEvent);
	}
	*/

	//--

	static void parsePalettes(ofxXmlSettings &palettes, ColourLoveEvent &palette) {
		ofLogNotice(__FUNCTION__) << endl << "--------------------------------------------------------------" << endl;

		palettes.pushTag("palettes");
		int numPalettes = palettes.getNumTags("palette");
		int numCols;
		if (numPalettes > 0) {
			for (int i = 0; i < numPalettes; i++) {
				palettes.pushTag("palette", i);

				ColourLovePalette cp;
				cp.parseXML(palettes);

				palette.palettes.push_back(cp);
				palettes.popTag();
			}
		}
		palettes.popTag();

		//ofLogNotice(__FUNCTION__) << endl << ofToString(palettes);
	}
};

#endif
