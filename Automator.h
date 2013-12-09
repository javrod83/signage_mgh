#pragma once

#include "ofMain.h"
#include "Section.h"

class Automator{

	private:
		bool running;
		float lastActivity;
		float inactiveCount;
		float postOppenedTime;
		float postshowTime;
		bool sectionChanged;
		bool postOppened;

		Section * section;

	public:
		Automator(Section *sect,float inactiveC,float postOT)
			{
				section = sect;
				inactiveCount = inactiveC;
				postOppenedTime = postOT;

				inactiveCount=0;
				lastActivity=ofGetElapsedTimeMillis();

				running=false;
				sectionChanged= false;
				postOppened = false;
			}

	void activity()
		{
			running=false;
			sectionChanged= false;	
			postOppened = false;
			inactiveCount=0;
			lastActivity=ofGetElapsedTimeMillis();
		}

	void update()
	{
		if ( ! running )
			{
				if (ofGetElapsedTimeMillis() > inactiveCount + lastActivity )
					{
						running=true;
						lastActivity=ofGetElapsedTimeMillis();
						section->changeTo((int)ofRandom(1,8));
					}
			}
		else
			{
				if (! section->isLoading())
			{
		
		section->inScreenShowFull(1,clickedSpot);
		}

		}
	}
	};