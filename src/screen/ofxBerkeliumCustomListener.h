#pragma once
#include "ofMain.h"
#include "ofxBerkeliumHandler.h"


class ofxBerkeliumMenuListener {

	public:	
		virtual void menuCommand(string cmd)
			{
				cout << "[ofxBerkeliumCustomListerner]new command from menu \n cmd : ["+cmd+"]" << endl;
			}

		virtual void changeTo(int sectionNumber)
			{
				cout << "[ofxBerkeliumSectionListener:changeSection]["+ofToString(sectionNumber)+"]" << endl;
			}
};


class ofxBerkeliumSpotListener {
	public:	

		virtual void contentLoading()
			{
				cout << "[ofxBerkeliumSectionListener:loading]" << endl;
			}

		virtual void contentReady()
			{
				cout << "[ofxBerkeliumSectionListener:ready]" << endl;
			}
		
		virtual void initializationBegin()
			{
				cout << "[ofxBerkeliumSectionListener:ready]" << endl;
			}

		virtual void initializationEnd()
			{
				cout << "[ofxBerkeliumSectionListener:ready]" << endl;
			}

};




class ofxBerkeliumMenu :  public  ofxBerkeliumHandler {

	private:
		ofxBerkeliumMenuListener * listener;
		bool listenerSeted;
		
	
	public:
		virtual void setCustomListener(ofxBerkeliumMenuListener * _customListener)
			{
				listener = _customListener;
			}
		virtual void onConsoleMessage(wstring message, wstring sourceId, int line_no)
			{
				wcout << "ofxBerkeliumMenu" << endl;
				wcout << "message : [" << message << "]" << endl;
				string str( message.begin(), message.end() );
				if ( str == "ready")
					
				{
						loaded=true;
			
				}
				
				
				string delimiter = "=";
				size_t pos = 0;
				std::string token;
				vector <string> command;
				pos = str.find(delimiter);
				token = str.substr(0, pos);
				cout << "CMD:" + token << endl;
				if (token == "section")
					{
						str.erase(0, pos + delimiter.length());
						pos = str.find(delimiter);
						token = str.substr(0, pos);
						cout << "CHAGNE TO SECTION:" + token << endl;
						listener->changeTo(ofToInt(token));
					}

			
				}



};



 class ofxBerkeliumSpot :  public  ofxBerkeliumHandler {

	private:
	
	
		ofxBerkeliumSpotListener * loader;


		bool listenerSeted;
		int  clickedScreen;
		int  clickedSpot;
		int  id;
		int  requested;
		
		string delayedUrl;

		
		ofFbo			fbo;
		int				reloadCount;
		string			areEqual;

	public:
		
		void  initialize(string url,ofxBerkeliumSpotListener * loader)
			{
	
				initializeBrowser();
				loadUrl(url);
				loaded=false;
				myAlpha =255;
				setLoaderListener(loader);
				loader->initializationBegin();
			}

		void drawDebug(int x,int y, int w,int h)
			{
				ofPushStyle();
				if (!loaded){	ofSetColor(255,0,0);}
				else	{	ofSetColor(0,255,0);}

				ofNoFill();
				ofPushMatrix();
				ofTranslate(x,y);
				ofSetLineWidth(2);
				ofRect(1,1,w-1,h-1);
				ofSetColor(255,255,255);
				browser->draw(0,0,w,h);

				ofPopMatrix();
				ofPopStyle();
			}



		void setLoaderListener(ofxBerkeliumSpotListener * _customListener)
			{
				loader = _customListener;
			}

		void onConsoleMessage(wstring message, wstring sourceId, int line_no)
			{
				wcout << "ofxBerkeliumSpot" << endl;
				wcout << "message : [" << message << "]" << endl;
				string str( message.begin(), message.end() );
				if ( str == "ready") 
					{	
						onReady();
					}
			}

		void onReady()
			{
				loaded=true;
				loader->contentReady();
				cout << "onReady --> IS READY!"<<endl;
			}

		void callLoader()
			{
				cout << " CALL LOADER " + delayedUrl << endl;
				//string cmd = "loadContent('"+delayedUrl+"');";
				string cmd = "load('"+delayedUrl+"');";

				cout << " call loader CMD :"+ cmd << endl;
				std::wstring str2(cmd.length(), L' '); // Make room for characters

				// Copy string to wstring.
				std::copy(cmd.begin(), cmd.end(), str2.begin());

				//wstring script(L"getData();"); 
				browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(str2)); 
				loader->contentLoading();
				loaded=false;
			}

		void call(string op)
			{
				cout << " CALL  " + delayedUrl << endl;
				//string cmd = "loadContent('"+delayedUrl+"');";
				string cmd = op+"();";

				cout << " call  "+op+"();"+ cmd << endl;
				std::wstring str2(cmd.length(), L' '); // Make room for characters

				// Copy string to wstring.
				std::copy(cmd.begin(), cmd.end(), str2.begin());

				//wstring script(L"getData();"); 
				browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(str2)); 
			}

		void setDelayedUrl(string newUrl)
			{
				delayedUrl = newUrl;
			}

		string getUrl()
			{
				return url;
			}

		void onLoad()
			{
				//cout << url + " LOADED" << endl;
				loader-> initializationEnd();
			}

		//--------------------------------------------------------------
		void onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction)
			{
		
			}

		void onCrashed()
			{
				cout<< "[[ ON CRASHED ]]"+url + url<<endl;
			}

		void onTitleChanged(wstring title)
			{
				//loader->requested();
				//requested = true;
				cout<< "[[ REQ ]]" + url<<endl;
			}
};
