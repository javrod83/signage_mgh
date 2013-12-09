#pragma once

#include "ofMain.h"
#include "ofGstVideoPlayer.h"

#include "ofxJSONElement.h"
#include "ofxBlurShader.h"

#include "ofxBerkelium.h"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"
#include "ofxBerkeliumHandler.h"
#include "ofxBerkeliumCustomListener.h"
#include "Spot.h"




class SingleScreen: public ofxBerkeliumSpotListener  {

		private:
			int width;
			int height;
			int viewPortWidth;
			int viewPortHeight;
			float xOffset;
			float yOffset;
			int motionCurrentFrame;

			//MOTION 
			ofGstVideoPlayer * motion;
			ofTexture motionTexture;

			//FBO
			 ofFbo       fbo;
			 ofShader    shader;
		
			// WEBKIT
			//ofxBerkeliumSpot * view;
			//ofxBerkeliumSpot * back;

			 int initializingCounter;
			vector<ofxBerkeliumSpot *> delayed;
			vector<ofxBerkeliumSpot *> views;
			vector<ofxBerkeliumSpot *> backs;

			map <int,TimePoint*> actionsMap;
			vector<TimePoint*> actionsVector;
			int vectorIndex ;
			int lastFrameAction;
			ofxJSONElement json;
	
			bool webkitLoading;
			vector<TimePoint*> webkitLoadVector;
			int webkitCurrentIndex;
			bool firstRun;
			int lastFrame;

			//FONT 
			ofTrueTypeFont	verdana;

			//BLUR
			ofxBlurShader blur;
			bool blurIn;
			bool blurOut;
			int blurFrameStart;
			int blurFrameStop;
			float blurInDuration;
			float blurOutDuration;
			int blurAmount;
			int blurIterations;
			int blurCurrentAlpha;

			//RESIZER
			float scaleAmount;
			float scaleCurrent;
			float scaleStartFrame;
			float scaleEndFrame;
			float scalingDuration;
			bool  scaling;

			//ALPHA
			int alphaCurrent;
			bool isFadin;
			bool ifFadeOut;
			int  alphaStartFrame;
			int  alphaStopFrame;
			float alphaDuration;

		public:
	
			SingleScreen(int _width,int _height,float xOff,float yOff,int vpw,int vph);
			//TO-DO destructor
			void init();
			
			void setFBO();
			void setMotion();
			void setView();
			void setBack();
			void loadContent();
			void loadAction(int id,int time ,string _operation , string path);
			void loadMotion();
			void motionUpdate();
			void motionPlay();
			void motionStop();
			void motionPause();
			void motionFastFoward();
			void motionRewind();
			void bakeFBO();
			void update();
			void draw();
			void drawDebug();
			void drawInfo();
			void viewUpdate();

			void setBlur();
			void blurStartfocusIn();
			void blurStartfocusOut();
			void blurUpdate();
			void blurBegin();
			void blurEnd();
			void blurDraw();
			bool isBlurry(); 

			void setScaling();
			void scalingStart();
			void scalingUpdate();
			void scalingApply();
			bool isScaling();

			void setAlpha();
			void alphaStartFadeIn();
			void alphaStartFadeOut();
			void alphaUpdate();
			void alphaApply();
			bool isFading(); 

			void webkitStartLoading();
			bool webkitIsLoading();
			void webkitUpdateLoading();
			void webkitReloadUrls();
			void webkitSet();

			void contentLoading();
			void contentReady();
			void contentLoadDelayed();
			
			void initializationEnd();
			void initializationBegin();
			bool initializationIsComplete();
			void addToDelayed(ofxBerkeliumSpot * webkit);

	};