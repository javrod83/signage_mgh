#include "testApp.h"

#define STRINGIFY(A) #A

//Resolution of the app
#define WIDTH 3840
#define HEIGHT 1080

//warp to viewport

#define PRODUCTION false 
#define VIEWPORTWIDTH 3840
#define VIEWPORTHEIGHT 1080

//#define VIEWPORTWIDTH 1920
//#define VIEWPORTHEIGHT 1080


#define VIEWPORTWIDTH_DEBUG 1920
#define VIEWPORTHEIGHT_DEBUG 538

#define USE_TRANSPARENCY true

//--------------------------------------------------------------
void testApp::setup(){
	debug= false;
	if(debug)cout << " testApp::setup begin" << endl ; 
	
	Berkelium::init( Berkelium::FileString::empty() );
	
	if(PRODUCTION)ofHideCursor();

	updating=true;
	
	production = PRODUCTION;

	if (PRODUCTION) myScreen = new SingleScreen(WIDTH,HEIGHT,0,0,VIEWPORTWIDTH,VIEWPORTHEIGHT);
	else
		myScreen = new SingleScreen(WIDTH,HEIGHT,0,0,VIEWPORTWIDTH_DEBUG,VIEWPORTHEIGHT_DEBUG);
	


	if(debug)cout << " testApp::setup end" << endl ; 

	ofSetBackgroundColor(0);
}


//--------------------------------------------------------------
void testApp::update()
	{
		if(debug)cout << "testApp::update begin" << endl;

		myScreen->update();

		try 
			{
				Berkelium::update();
			}
		catch( std::exception& e)
			{
				std::cout << " IN BERKELIUM UPDATE EVETN" << e.what() << "'\n";
			}
		if(debug)cout << "testApp::update end" << endl;
	}

//--------------------------------------------------------------
void testApp::draw()
	{
		
			myScreen->draw();


		if (!production)
			myScreen->drawInfo();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
	case 'o':
	case 'O':
		myScreen->motionPlay();
		break;
	case 'p':
	case 'P':
		myScreen->motionPause();
		break;
	case 'L':
	case 'l':
		myScreen->motionRewind();
		break;
	case 'k':
	case 'K':
		myScreen->motionFastFoward();
		break;
	case 'j':
	case 'J':
		myScreen->jump();
		break;
	case 'q':
	case 'Q':
		myScreen->blurStartfocusIn();
		break;
	case 'w':
	case 'W':
		myScreen->blurStartfocusOut();
		break;
	case 'z':
	case 'Z':
		myScreen->webkitStartLoading();
		break;
	case 'd' :
	case 'D' :
		production = !production;
	}
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
	{}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
	{	
		//mySection.mouseMoved(x,y);
	}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
	{
	//	mySection.mouseDragged(x,y,button);
	}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
	{
		//mySection.mousePressed(x,y,button);
	}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
	{
 		//mySection.mouseReleased(x,y,button);
	}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}

//--------------------------------------------------------------
void testApp::urlResponse(ofHttpResponse & response){}



