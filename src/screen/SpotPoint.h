#pragma once

#define STIFFNESS 0.2
#define MASS 2.0
#define DAMPING    0.7
class SpotPoint {
	public:

		float x,y,vx,vy,scale,vscale;
		int alpha;
		
	
		SpotPoint(){
			x=0;
			y=0;
			scale=0;
			alpha=0;
		};

		SpotPoint(float _x,float _y,float _scale,int _alpha){
			set( _x, _y, _scale,_alpha);
		};

		void set(float _x,float _y,float _scale,int _alpha){
			x = _x;
			y = _y;
			scale = _scale;
			alpha = _alpha;
		};

		void setFromPoint (SpotPoint * aPoint){
			x = aPoint->x;
			y = aPoint->y;
			scale = aPoint->scale;
			alpha = aPoint->alpha;
		};


		void updateFromPoint (SpotPoint * targetPoint){
			
			 scale = targetPoint->scale;
			 alpha = targetPoint->alpha;
	  		 update(targetPoint->x,targetPoint->y,targetPoint->scale);

		};

		 void update(float targetX, float targetY,float targetScale) {
				float forceX = (targetX - x) * STIFFNESS;
				float ax = forceX / MASS;
				 vx = DAMPING * (vx + ax);
				x += vx;

				float forceY = (targetY - y) * STIFFNESS;
				//forceY += gravity;
				float ay = forceY / MASS;
				vy = DAMPING * (vy + ay);
				y += vy;

				float forceScale = (targetScale - scale) * STIFFNESS;
				//forceY += gravity;
				float aScale = forceScale / MASS;
				vscale = DAMPING * (vscale + aScale);
				scale += vscale;

		}
};

class TimePoint {

	public:
		string operation;
		string path;
		int moment;
		int id;


		TimePoint(){
			id = -1 ;
			operation = "none";
			path = "" ;
			moment  = -1 ;
		};

		TimePoint(int _id,string _operation,string _path,int _moment){
			set(_id, _operation, _path,_moment);
		};

		void set(int _id,string _operation,string _path,int _moment)
			{
				id = _id;
				operation = _operation;
				path = _path ;
				moment  = _moment ;
			};

	};