#pragma once


#define WEBCAM

#include "ofMain.h"
#include "ofGstVideoPlayer.h"
#include "ofxJSONElement.h"


#include "ofxBerkelium.h"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"
#include "ofxBerkeliumHandler.h"

//My includes

#include "Spot.h"
#include "Screen.h"


class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();



		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void urlResponse(ofHttpResponse & response);

		
		bool frameByframe;

		int nResults;
		int current;
		float size;
		float mockBoxWidth;
		float mockBoxHeight;
		int currentFrame;

		bool debug;


		bool updating;


		SingleScreen* myScreen;
};
