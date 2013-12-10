#include "Screen.h"

#define STRINGIFY(A) #A



	SingleScreen::SingleScreen(int _width,int _height,float xOff,float yOff,int vpw,int vph)
		{
			 width = _width;
			 height = _height;
			 viewPortWidth = vpw;
			 viewPortHeight = vph;
			 xOffset = xOff;
			 yOffset = yOff;
			 lastFrameAction=0;
			 init();
		}

	//TO-DO destructor
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::init()
		{
			cout << "SingleScreen:"<<" init() begin"<<endl ; 

			setFBO();
			setMotion();
			webkitSet();
			loadMotion();
			loadContent();


			setBlur();
			setScaling();
			setAlpha();
			firstRun = true ;
			lastFrame =0;
			verdana.loadFont("verdana.ttf", 10, true, true);
			
			cout << "SingleScreen:"<<" init() end"<<endl ; 
		}

	//------------------------------------------------------------------------------------------------------------------------
	/*
		void SingleScreen::setFBO(){

		fbo.allocate(width,height);
		fbo.begin();
		ofClear(255,255,255,255);
		fbo.end();

		string shaderProgram = STRINGIFY(uniform sampler2DRect tex0;
							void main (void){
								vec2 st = gl_TexCoord[0].st;
								vec4 image = texture2DRect(tex0, st);    
								gl_FragColor = vec4(image.g,image.b,image.a,image.r);\n\
							});
		
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
		shader.linkProgram(); 
	}
	*/
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setFBO(){

		fbo.allocate(width,height);
		fbo.begin();
		ofClear(255,255,255,255);
		fbo.end();

		string shaderProgram = STRINGIFY(uniform sampler2DRect texture0;
										uniform sampler2DRect alpha0;
							void main (void){
								vec2 st = gl_TexCoord[0].st;
								vec4 image = texture2DRect(texture0, st);
								vec4 alpha = texture2DRect(alpha0, st);

								 gl_FragColor = vec4(image.rgb,(1-max(alpha.r,max(alpha.g,alpha.b)))*image.a );\n\
							});
		
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
		shader.linkProgram(); 


		motionTexture.allocate(3840,1080,GL_RGB);
		motionAlphaTexture.allocate(3840,1080,GL_RGB);
	}

	//------------------------------------------------------------------------------------------------------------------------
	/*
	void SingleScreen::setMotion()
		{
			motion = new ofGstVideoPlayer();
			motion->setPixelFormat(OF_PIXELS_BGRA);
			cout << "motion:"<<" initialized"<<endl ; 
		}
	*/
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setMotion()
		{
			motion_left = new ofGstVideoPlayer();
			motion_left->setPixelFormat(OF_PIXELS_RGB);
			
			motion_left_alpha = new ofGstVideoPlayer();
			motion_left_alpha->setPixelFormat(OF_PIXELS_RGB);
			
			motion_right = new ofGstVideoPlayer();
			motion_right->setPixelFormat(OF_PIXELS_RGB);
			
			motion_right_alpha = new ofGstVideoPlayer();
			motion_right_alpha->setPixelFormat(OF_PIXELS_RGB);

			cout << "motion:"<<" initialized"<<endl ; 
		}



	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::webkitSet()
		{
			cout << "SingleScreen:"<<" webkitSet() begin"<<endl ; 

			initializingCounter = 0;
			setView();
			setBack();
			webkitCurrentIndex = 0 ; 
			
			cout << "SingleScreen:"<<" webkitSet() end"<<endl ; 
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	bool SingleScreen::webkitIsLoading()
		{
			return delayed.size() > 0;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::webkitReloadUrls()
		{
			cout << "webkitReloadUrls:"<<" begin"<<endl ; 
			bool gotJson = json.open("http://127.0.0.1/placeholders");

			while(!gotJson)
				{
					ofSleepMillis(500);
					gotJson = json.open("http://127.0.0.1/placeholders");
					cout << " NO CONTENT !!!!!! !!!!!!!!!!" <<endl;
				}

			for (int i = 0 ;i < json["placeholders"].size();i++)
				{
					//loadAction(i,json["placeholders"][i]["load"].asInt() ,"load",json["placeholders"][i]["path"].asString());
					webkitLoadVector[i]->set(i,"load",json["placeholders"][i]["path"].asString(),json["placeholders"][i]["load"].asInt());
					cout << "	placeholder  "<<ofToString(i)<<" "<<json["placeholders"][i]["path"].asString()<<"loaded  "<<endl ; 
				}

			cout << "webkitReloadUrls:"<<" end"<<endl ;
		}
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::webkitStartLoading()
		{
			cout << "SingleScreen:"<<" webkitStartLoading() begin"<<endl ; 
			webkitCurrentIndex=0;
			webkitReloadUrls();

			for (int i = 0 ; i <webkitLoadVector.size(); i ++)
				{
					cout << "	PLACEHOLDER: "<<ofToString(i)<<endl ; 
					cout << "			view id:"<<webkitLoadVector[i]->path<<endl ; 
					views[i]->setDelayedUrl(webkitLoadVector[i]->path);
					
					addToDelayed(views[i]);
					cout << "			back id:"<<webkitLoadVector[i]->path<<endl ; 
					backs[i]->setDelayedUrl(webkitLoadVector[i]->path);
					addToDelayed(backs[i]);

				}

			contentLoadDelayed();
			cout << "SingleScreen:"<<" webkitStartLoading() end"<<endl ; 
		}




	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::webkitUpdateLoading()
		{
			//cout << "SingleScreen:"<<" webkitUpdateLoading() begin"<<endl ; 
			int counter = 0 ;

			for (int i = 0 ;i < views.size();i++)
				{
					if (! views[i]->loaded || ! backs[i]->loaded )
						{
							counter ++ ;
						}
				}

			webkitLoading  = (counter != 0) ;
			//cout << "SingleScreen:"<<" webkitUpdateLoading() end"<<endl ; 
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setView()
		{
			cout << "SingleScreen:"<<" setView() begin"<<endl ; 
			//view = new ofxBerkeliumSpot();
			//view->setLoaderListener(this);
			//view->initialize("http://127.0.0.1/render.html");
			//view->setPos(0,0,0,0);
			
			for( int it = 0; it < 9; ++it )
				{
					ofxBerkeliumSpot * view = new ofxBerkeliumSpot();
					view->initialize("http://127.0.0.1/render.html",this);
					view->setPos(0,0,0,0);
					views.push_back(view);
				}

			cout << "SingleScreen:"<<" setView() end"<<endl ; 
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setBack()
		{
			cout << "SingleScreen:"<<" setBack() begin"<<endl ; 
			//back = new ofxBerkeliumSpot();
			//back->setLoaderListener(this);
			//back->initialize("http://127.0.0.1/back.html");
			//back->setPos(0,0,0,0);
			
			for( int it = 0; it < 9; ++it )
				{
					ofxBerkeliumSpot* back = new ofxBerkeliumSpot();
					back->initialize("http://127.0.0.1/back.html",this);
					back->setPos(0,0,0,0);
					backs.push_back(back);
				}
			cout << "SingleScreen:"<<" setBack() end"<<endl ; 
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::loadAction(int id, int time ,string _operation , string path)
		{
			cout << "loadAction:"<<" begin"<<endl ; 
			if (actionsMap.find(time) != actionsMap.end())
				{
					cout << "loadAction:"<<" update"<<endl ;
					actionsMap[time]->set(id,_operation,path,time);
				}
			else
				{
					cout << "loadAction:"<<"	new"<<endl ;
					actionsMap[time]= new TimePoint(id,_operation,path,time);

					actionsVector.push_back(actionsMap[time]);
					
					
					if (_operation == "load")
						{
							webkitLoadVector.push_back(actionsMap[time]);							
						}
				}
			cout << "loadAction:"<<" end"<<endl ; 
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::loadContent()
		{
			cout << "loadContent:"<<" begin"<<endl ; 
			bool gotJson = json.open("http://127.0.0.1/placeholders");

			while(!gotJson)
				{
					ofSleepMillis(500);
					gotJson = json.open("http://127.0.0.1/placeholders");
					cout << " WAITNG 4 CONTENT !!!!!! !!!!!!!!!!" <<endl;
				}

			cout << "loadContent:"<<"		json loaded  "<<endl ; 
			//laod steps into map
			cout << "loadContent:"<<"		"<< ofToString(json["placeholders"].size())<<"placeholders to load"<<endl ; 

			for (int i = 0 ;i < json["placeholders"].size();i++)
				{
					/*
					cout << "loadContent:"<<"		new placeholder: "<<ofToString(i)<<endl ;
					cout << "			:"<<"		path:  " << json["placeholders"][i]["path"].asString() << endl ;
					cout << "			:"<<"		load:  " << ofToString(json["placeholders"][i]["load"].asInt() )<<endl ;
					cout << "			:"<<"		start: " << ofToString(json["placeholders"][i]["start"].asInt() )<<endl ;
					cout << "			:"<<"		end:   " << ofToString(json["placeholders"][i]["end"].asInt() )<<endl ;
					*/

					loadAction(i,json["placeholders"][i]["load"].asInt() ,"load",json["placeholders"][i]["path"].asString());
					loadAction(i,json["placeholders"][i]["start"].asInt(),"start","");
					loadAction(i,json["placeholders"][i]["end"].asInt(),"end","");
					
					cout << "loadContent:"<<"		placeholder loaded  "<<endl ; 
				}

			cout << "loadContent:"<<" end"<<endl ; 
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::loadMotion()
		{
			cout << "motion:"<<" loadMotion"<<endl ; 
			motion_left->loadMovie("MGH01_LEFT.mp4");
			motion_left->setThreadAppSink(true);
			//setFrameByFrame
			motion_left->setLoopState(OF_LOOP_NORMAL);
			motionTexture_left.allocate(motion_left->getWidth(),motion_left->getHeight(),GL_RGB);

			motion_left_alpha->loadMovie("MGH01_LEFT_ALPHA.mp4");
			motion_left_alpha->setThreadAppSink(true);
			
			motion_left_alpha->setLoopState(OF_LOOP_NORMAL);
			motionTexture_left_alpha.allocate(motion_left_alpha->getWidth(),motion_left_alpha->getHeight(),GL_RGB);


			motion_right->loadMovie("MGH01_RIGHT.mp4");
			motion_right->setThreadAppSink(true);
			motion_right->setLoopState(OF_LOOP_NORMAL);
			motionTexture_right.allocate(motion_right->getWidth(),motion_right->getHeight(),GL_RGB);

			motion_right_alpha->loadMovie("MGH01_RIGHT_ALPHA.mp4");
			motion_right_alpha->setThreadAppSink(true);
			motion_right_alpha->setLoopState(OF_LOOP_NORMAL);
			motionTexture_right_alpha.allocate(motion_right_alpha->getWidth(),motion_right_alpha->getHeight(),GL_RGB);
			
			motionCurrentFrame = 0;
			//motion = motion_left ;
		}

	//------------------------------------------------------------------------------------------------------------------------
	/*
	void SingleScreen::motionPlay()
		{
			cout << "motion:"<<" play"<<endl ; 
			motion->setPaused(false);
			motion->play();
		}
	*/
	


		//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionPlay()
		{
			cout << "motion:"<<" play"<<endl ; 
			

			motion_left->setPaused(false);
			motion_right->setPaused(false);
			
			motion_left_alpha->setPaused(false);
			motion_right_alpha->setPaused(false);

			motion_left->play();
			motion_right->play();
			
			motion_left_alpha->play();
			motion_right_alpha->play();

		}




	//------------------------------------------------------------------------------------------------------------------------
	/*
		void SingleScreen::motionStop()
		{
			cout << "motion:"<<" stop"<<endl ; 
			motion->stop();
		}
	*/
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionStop()
		{
			cout << "motion:"<<" stop"<<endl ; 
		
			motion_left->stop();
			motion_right->stop();
			
			motion_left_alpha->stop();
			motion_right_alpha->stop();

		}


	//------------------------------------------------------------------------------------------------------------------------
	/*
	void SingleScreen::motionPause()
		{
			cout << "motion:"<<" pause"<<endl ; 
			motion->setPaused(true);
		}
	*/

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionPause()
		{
			cout << "motion:"<<" pause"<<endl ; 
//			motion->setPaused(true);

			motion_left->setPaused(true);
			motion_right->setPaused(true);
			
			motion_left_alpha->setPaused(true);
			motion_right_alpha->setPaused(true);

		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionFastFoward()
		{
			cout << "motion:"<<" FF"<<endl ; 
			//motion->setFrame((motion->getCurrentFrame()+30)%motion->getTotalNumFrames());


			motion_left->setFrame((motion_left->getCurrentFrame()+30)%motion_left->getTotalNumFrames());
			motion_left_alpha->setFrame((motion_left_alpha->getCurrentFrame()+30)%motion_left_alpha->getTotalNumFrames());
			motion_right->setFrame((motion_right->getCurrentFrame()+30)%motion_right->getTotalNumFrames());
			motion_right_alpha->setFrame((motion_right_alpha->getCurrentFrame()+30)%motion_right_alpha->getTotalNumFrames());
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionRewind()
		{
			cout << "motion:"<<" RW"<<endl ; 


			motion_left->setFrame((motion_left->getCurrentFrame()-30 > 0 )?motion_left->getCurrentFrame()-30 : motion_left->getTotalNumFrames()-30);
			motion_left_alpha->setFrame((motion_left_alpha->getCurrentFrame()-30 > 0 )?motion_left_alpha->getCurrentFrame()-30 : motion_left_alpha->getTotalNumFrames()-30);
			motion_right->setFrame((motion_right->getCurrentFrame()-30 > 0 )?motion_right->getCurrentFrame()-30 : motion_right->getTotalNumFrames()-30);
			motion_right_alpha->setFrame((motion_right_alpha->getCurrentFrame()-30 > 0 )?motion_right_alpha->getCurrentFrame()-30 : motion_right_alpha->getTotalNumFrames()-30);


		}
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::bakeFBO()
		{
			ofPushStyle();
			ofEnableAlphaBlending();
			
			//record front layer with alpha
			blurBegin();
			alphaApply();
			blurEnd();
			
			//join motion 
			motionTexture.begin();
			motionTexture_left.draw(0,0,1920,1080);
			motionTexture_right.draw(1920,0,1920,1080);
			motionTexture.end();

			//join motion alpha
			motionAlphaTexture.begin();
			motionTexture_left_alpha.draw(0,0,1920,1080);
			motionTexture_right_alpha.draw(1920,0,1920,1080);
			motionAlphaTexture.end();


			fbo.begin();
			ofClear(255, 255, 255,0);
			scalingApply();
			
			//views[webkitCurrentIndex]->draw();	

			if(isBlurry())
				{
					ofPushStyle();
					ofSetColor(255,blurCurrentAlpha);
					blur.draw(0,0,width,height);		
					ofPopStyle();
				}
				
			alphaApply();
			
			//ofSetco(255, 255, 255,128);
			shader.begin();
 			shader.setUniformTexture("texture0",motionTexture,1);
			shader.setUniformTexture("alpha0",motionAlphaTexture,2);
    
			glBegin(GL_QUADS);  
			glTexCoord2f(0, 0); glVertex3f(0, 0, 0);  
			glTexCoord2f(motionTexture.getWidth(), 0); glVertex3f(width, 0, 0);  
			glTexCoord2f(motionTexture.getWidth(), motionTexture.getHeight()); glVertex3f(width, height, 0);  
			glTexCoord2f(0,motionTexture.getHeight());  glVertex3f(0,height, 0);  
			glEnd();
			shader.end();
			
			fbo.end();
			ofDisableAlphaBlending();
			ofPopStyle();
		}


	//------------------------------------------------------------------------------------------------------------------------
	/*	void SingleScreen::bakeFBO()
		{
			ofPushStyle();
			ofEnableAlphaBlending();
			
			blurBegin();
			alphaApply();
			blurEnd();
			
			fbo.begin();
			ofClear(255, 255, 255,0);
			scalingApply();
			
			//views[webkitCurrentIndex]->draw();	

			if(isBlurry())
				{
						ofPushStyle();
						ofSetColor(255,blurCurrentAlpha);
					blur.draw(0,0,width,height);		
					ofPopStyle();
				}
				
			alphaApply();
			
			//ofSetco(255, 255, 255,128);
			shader.begin();
 			shader.setUniformTexture("tex0",motionTexture,0);
    
			glBegin(GL_QUADS);  
			glTexCoord2f(0, 0); glVertex3f(0, 0, 0);  
			glTexCoord2f(motion->getWidth(), 0); glVertex3f(width, 0, 0);  
			glTexCoord2f(motion->getWidth(), motion->getHeight()); glVertex3f(width, height, 0);  
			glTexCoord2f(0,motion->getHeight());  glVertex3f(0,height, 0);  
			glEnd();
			shader.end();
			
			fbo.end();
			ofDisableAlphaBlending();
			ofPopStyle();
		}*/
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::update()
		{
			//cout << "SingleScreen::update begin "  << endl;


				if  (initializationIsComplete())
					{
						if (firstRun)
							{
								firstRun = false;
								webkitStartLoading();
							}

						if (lastFrame > motionCurrentFrame && !webkitIsLoading() )
							{
								motionPause();
								webkitStartLoading();
								lastFrame = motionCurrentFrame;
								cout << "lastFrame > motionCurrentFrame" << " = " << ofToString(lastFrame)+ " > "+ ofToString(motionCurrentFrame) << endl ;
							}


						if (!webkitIsLoading())
						{
							motionUpdate();
							viewUpdate();
							scalingUpdate();
							blurUpdate();
							alphaUpdate();
							bakeFBO();
						}
					}

			

			//cout << "SingleScreen::update end " << endl;
		}

	//------------------------------------------------------------------------------------------------------------------------
	/*
		void SingleScreen::motionUpdate()
			{
				//cout << "SingleScreen::motionUpdate begin"<< endl;
				motion->update();
				motionTexture.loadData(motion->getPixels(),motion->getWidth(),motion->getHeight(),GL_RGBA);
				lastFrame = motionCurrentFrame;
				motionCurrentFrame = motion->getCurrentFrame();


				//cout << "SingleScreen::motionUpdate begin"<< endl;
			}

	*/

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::motionUpdate()
		{
			//cout << "SingleScreen::motionUpdate begin"<< endl;
			motion_left->update();
			motion_left_alpha->update();
			motion_right->update();
			motion_right_alpha->update();
			
			
			motionTexture_left.loadData(motion_left->getPixels(),motion_left->getWidth(),motion_left->getHeight(),GL_RGB);
			motionTexture_right.loadData(motion_right->getPixels(),motion_right->getWidth(),motion_right->getHeight(),GL_RGB);
			
			
			motionTexture_left_alpha.loadData(motion_left_alpha->getPixels(),motion_left_alpha->getWidth(),motion_left_alpha->getHeight(),GL_RGB);
			motionTexture_right_alpha.loadData(motion_right_alpha->getPixels(),motion_right_alpha->getWidth(),motion_right_alpha->getHeight(),GL_RGB);
			
			lastFrame = motionCurrentFrame;
			motionCurrentFrame = motion_left->getCurrentFrame();


			//cout << "SingleScreen::motionUpdate begin"<< endl;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::viewUpdate()
		{
			//cout << "SingleScreen::viewUpdate begin"<< endl;
			//cout << "			 ::currFrame:"<<ofToString(motionCurrentFrame)<< endl;
			//cout << "cf:" << ofToString(motionCurrentFrame) << endl;
		
			if (actionsVector[vectorIndex]->moment < motionCurrentFrame && actionsVector.size() > vectorIndex )
				{
					string operation = actionsVector[vectorIndex]->operation ;
					cout << "[[fra: "+ofToString(motionCurrentFrame)+"||cmd: "+operation+"]]" << endl;
					cout << "total:"+ofToString(motion_left->getTotalNumFrames()) << endl;

					if ( operation == "load")
						{
							if (vectorIndex != 0)
								{
									webkitCurrentIndex++;
									webkitCurrentIndex = webkitCurrentIndex%views.size();
								}
							ofPushStyle();
							blurBegin();
							ofClear(255);
							ofClearAlpha();
							ofSetColor(255,0);
							ofFill();
							ofRect(5,5,ofGetWidth()-10,ofGetHeight()-10);
							blurEnd();
							ofPopStyle();
						}
					else if (operation == "start")
						{
							views[webkitCurrentIndex]->call("show");
							scalingStart();
							alphaStartFadeIn();
							blurStartfocusIn();
						}
					else if (operation == "end")
						{	
							blurStartfocusOut();
							alphaStartFadeOut();
							views[webkitCurrentIndex]->call("hide");
						}
						
					if (!((vectorIndex == 0 ) &&  (actionsVector[actionsVector.size()-1]->moment < motionCurrentFrame) ))
						{
							vectorIndex++;
							vectorIndex = vectorIndex % actionsVector.size();
						}
				}
			//cout << "SingleScreen::viewUpdate end"<< endl;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::draw()
		{
			//cout << "SingleScreen::draw begin"<< endl;
			ofEnableAlphaBlending();

			if  (initializationIsComplete())
				{
					 if(!webkitIsLoading())
						fbo.draw(xOffset,yOffset,viewPortWidth,viewPortHeight);
					 else
						{
							ofPushStyle();
							ofSetColor(0);
							ofRect(0,0,ofGetWidth(),ofGetHeight());
							ofPopStyle();
						}
				}

			ofDisableAlphaBlending();
			//cout << "SingleScreen::draw end"<< endl;
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::drawDebug(){}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::drawInfo()
		{

			int box_x=20,box_y=660,box_w=ofGetScreenWidth()-40,box_h=300;
			int line_h=15,margin=15;
			int box_r=128,box_g=128,box_b=128;
			int font_r=255,font_g=255,font_b=255;

			ofPushMatrix();
			ofPushStyle();

			ofTranslate(box_x,box_y);
			
			ofSetColor(box_r,box_g,box_b,128);
			ofFill();
			ofRect(0,0,box_w,box_h);
			
			ofSetColor(box_r,box_g,box_b,255);
			ofNoFill();
			ofRect(0,0,box_w,box_h);
			
			ofTranslate(margin,line_h);

			ofSetColor(font_r,font_g,font_b,255);
			verdana.drawString("  fc: "+ofToString(motionCurrentFrame)+" fps: "+ofToString(ofGetFrameRate()), 0 ,0);
			verdana.drawString(" act: "+ofToString(vectorIndex)+" ta: "+ofToString(actionsVector.size()),0 ,line_h*1);
			
			if (isBlurry())
				ofSetColor(font_r,font_g*.5,font_b,255);

			verdana.drawString(" blur amount: "+ofToString(blurAmount)+" iter: "+ofToString(blurIterations) + "alpha amount: " + ofToString(alphaCurrent),0 ,line_h*2);

			ofTranslate(ofGetWidth()-500,0);
			ofPushMatrix();
			verdana.drawString("n:   view	back ",0,0);
			ofTranslate(0,line_h);
			for(int i =0 ;i < views.size();i++)
				{
					string unString = ofToString(i)+"	";
					verdana.drawString(unString,0,i*33);
					views[i]->drawDebug(20,i*33,40,30);
					backs[i]->drawDebug(65,i*33,40,30);
				}

			ofPopMatrix();
			ofTranslate(200,0);
			for(int i =0 ;i < actionsVector.size();i++)
				{
					if(vectorIndex == i )
						ofSetColor(255,0,0,255);
					else
						ofSetColor(font_r,font_g,font_b,128);
					verdana.drawString(ofToString(actionsVector[i]->id)+"]"+actionsVector[i]->operation + ": "+ofToString(actionsVector[i]->moment), ((line_h*i)/(box_h-line_h))*150 ,(line_h*i)%(box_h-line_h));
				}

			ofPopStyle();
			ofPopMatrix();

			ofPushMatrix();
			ofTranslate(box_x+margin+line_h,box_y+(line_h*3));
			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();
			backs[webkitCurrentIndex]->setPos(0,0,320,120);
			backs[webkitCurrentIndex]->draw();
			
			
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("back",3,line_h);
			ofPopStyle();
			
			ofTranslate(0,125);

			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();

			blur.draw(0,0,320,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("blur",3,line_h);
			ofPopStyle();
			
			ofTranslate(325,-125);

			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();

			views[webkitCurrentIndex]->setPos(0,0,320,120);
			views[webkitCurrentIndex]->draw();
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("view",3,line_h);
			ofPopStyle();
			

			ofTranslate(0,125);
			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();
			motionTexture.draw(0,0,320,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("motion",3,line_h);
			ofPopStyle();



			///////////////////////////// motion left
			ofTranslate(325,-125);
			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();
			//motionTexture_left.draw(0,0,160,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("left",3,line_h);
			ofPopStyle();

			///////////////////////////// motion right
			ofTranslate(160,0);
			ofPushStyle();
			ofFill();
			
			motionTexture_right.draw(0,0,160,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("right",3,line_h);
			ofPopStyle();

			///////////////////////////// motion left alpha
			ofTranslate(-160,125);
			ofPushStyle();
			ofFill();
			ofSetColor(255,255,255,128);
			ofRect(0,0,320,120);
			ofSetColor(255,255,255,255);
			ofNoFill();
			ofRect(0,0,320,120);
			ofPopStyle();
			motionTexture_left_alpha.draw(0,0,160,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("left",3,line_h);
			ofPopStyle();

			///////////////////////////// motion right alpha
			ofTranslate(160,0);
			ofPushStyle();
			ofFill();
			
			motionTexture_right_alpha.draw(0,0,160,120);
			ofPushStyle();
			ofSetColor(128);
			verdana.drawString("right",3,line_h);
			ofPopStyle();


			ofPopMatrix();
		}


		


	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setBlur()
		{
			  blur.setup(width,height);
			  blurIn  = false;
			  blurOut = false;
			  blurInDuration  = 5.0 ;
		 	  blurOutDuration = 1.5 ;
			  blurFrameStart  = 0 ;
			  blurFrameStop   = 0 ;
			  blurIterations  = 10;
			  blurAmount = 2 ;
			  blurCurrentAlpha =0 ;

			  blurBegin();
			  ofPushStyle();
			  ofSetColor(255);
			  ofRect(0,0,width,height);
			  ofPopStyle();
			  blurEnd();
		}
			
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen:: blurStartfocusIn()
		{
			blurIn = true;
			blurFrameStart = motionCurrentFrame ;
			blurFrameStop = blurFrameStart + 25*blurOutDuration ;
		}
			
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen:: blurStartfocusOut()
		{
			blurOut = true;
			blurFrameStart = motionCurrentFrame ;
			blurFrameStop = blurFrameStart + 25*blurInDuration ;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen:: blurUpdate()
		{
			if(blurIn)
				{
					if (motionCurrentFrame>blurFrameStop)
						{
							blurIn = false;
							blurIterations = 8;
						}				
					else
						{
							blurIterations = ofMap(motionCurrentFrame - blurFrameStart ,0,blurFrameStop - blurFrameStart,0,8);
							blurCurrentAlpha = ofMap(blurFrameStop - motionCurrentFrame ,0,blurFrameStop - blurFrameStart,0,255);
						}
				}
			else if (blurOut)
				{
					if (motionCurrentFrame>blurFrameStop)
						{
							blurOut = false;
							blurIterations = 0;
						}
					else
						{
							blurCurrentAlpha = ofMap(motionCurrentFrame - blurFrameStart   ,0,blurFrameStop - blurFrameStart,0,255);
							blurIterations = ofMap(blurFrameStop - motionCurrentFrame  ,0,blurFrameStop - blurFrameStart,0,8);
						
						}
				}
		}

	//------------------------------------------------------------------------------------------------------------------------
	bool SingleScreen::isBlurry()
		{
			return (blurIn || blurOut);
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::blurBegin()
		{
			blur.begin();
			//blur.amount		= 2;
			//blur.iterations = 8;
			blur.amount			= blurAmount;
			blur.iterations		= blurIterations;
			//blur.amount = ofMap(mouseX,0,ofGetWidth(),0,10,true);
			//blur.iterations = ofMap(mouseY,0,ofGetHeight(),1,10,true);
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::blurEnd()
		{
			blur.end();
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setScaling()
		{
			scaleAmount  = 1.2 ;
			scaleCurrent = 1.0 ;
			scalingDuration = 15.0;
			
			scaleStartFrame = motionCurrentFrame ;
			scaleEndFrame =(int)( motionCurrentFrame  + (scalingDuration * 25.0));
			scaling = false;
		}
			
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::scalingStart()
		{
			scaleAmount  = 1.2 ;
			scaleCurrent = 1.0 ;
			scaling = true;
		}
			
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::scalingUpdate()
		{
			if (isScaling())
				scaleCurrent = ofMap(motionCurrentFrame,scaleStartFrame,scaleEndFrame,1.0,1.2);
		}
			
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::scalingApply()
		{
			ofPushMatrix();
			ofTranslate(width*.5,height*.5);
			ofScale(scaleCurrent,scaleCurrent);
			ofTranslate(-width*.5,-height*.5);
			backs[webkitCurrentIndex]->setPos(0,0,width,height);
			backs[webkitCurrentIndex]->draw();
			ofPopMatrix();
		}

	//------------------------------------------------------------------------------------------------------------------------
	bool SingleScreen::isScaling()
		{
			return scaling;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::setAlpha()
		{
			alphaCurrent  = 255 ;
			isFadin		  = false ;
			ifFadeOut	  = false ;
			alphaDuration = 3.0 ;
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::alphaStartFadeIn()
		{
			isFadin = true;
			alphaStartFrame = motionCurrentFrame ;
			alphaStopFrame = alphaStartFrame + 25*alphaDuration ;
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::alphaStartFadeOut()
		{
			ifFadeOut = true;
			alphaStartFrame = motionCurrentFrame ;
			alphaStopFrame = alphaStartFrame + 25*alphaDuration ;
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::alphaUpdate()
		{
	
			if(isFadin)
				{
					if (motionCurrentFrame>alphaStopFrame)
						{
							isFadin = false;
						}				
					else
						alphaCurrent = ofMap(motionCurrentFrame - alphaStartFrame ,0,alphaStopFrame - alphaStartFrame,0,255);
				}
			else if (ifFadeOut)
				{
					if (motionCurrentFrame>alphaStopFrame)
						{
							ifFadeOut = false;
						}
					else
						alphaCurrent = ofMap(alphaStopFrame - motionCurrentFrame  ,0,alphaStopFrame - alphaStartFrame,0,255);
				}
	
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::alphaApply()
		{
			ofPushStyle();
			ofSetColor(255,alphaCurrent);
			views[webkitCurrentIndex]->setPos(0,0,width,height);
			views[webkitCurrentIndex]->draw();
			ofPopStyle();
		}

	//------------------------------------------------------------------------------------------------------------------------
	bool SingleScreen::isFading()
		{
			return(isFadin||ifFadeOut);
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::initializationBegin()
		{
			initializingCounter++;
			cout << "initializationBegin! "<<ofToString(initializingCounter)<<endl;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::initializationEnd()
		{
			initializingCounter--;
			cout << "initializationEnd! "<<ofToString(initializingCounter)<< endl;
		}

	//------------------------------------------------------------------------------------------------------------------------
	bool SingleScreen::initializationIsComplete()
		{
			return initializingCounter == 0 ;
		}

	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::contentLoading()
		{
			cout << "SINGLE SCREEN LOADING "<< endl;
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::contentReady()
		{
			cout << "SINGLE SCREEN ready "<< endl;

			if (delayed.size() > 0 )
				{
					contentLoadDelayed();
				}
			else 
				motionPlay();
				
		}
	
	//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::addToDelayed(ofxBerkeliumSpot * webkit)
		{
			delayed.push_back(webkit);
		}

		//------------------------------------------------------------------------------------------------------------------------
	void SingleScreen::contentLoadDelayed()
		{
			cout << "SingleScreen:"<<" contentBeginDelayedLoading() begin"<<endl ;
			delayed.back()->callLoader();
			delayed.pop_back();
			cout << "SingleScreen:"<<" contentBeginDelayedLoading() end"<<endl ;

		}



	/*
	{
placeholders: [
{
path: "5251df8407b95d480900000e",
load: 0,
start: 1,
end: 2
},
{
path: "5251df8407b95d4809000010",
load: 0,
start: 1,
end: 2
},
{
	*/