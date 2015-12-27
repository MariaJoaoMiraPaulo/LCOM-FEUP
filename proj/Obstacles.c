#include <minix/drivers.h>
#include "Obstacles.h"
#include "Margins.h"
#include "video_gr.h"
#include "KBD.h"

short HEIGHT=100, WIDTH=30;

Obstacle* newObstacle( Margin* margin){
	Obstacle* obs;
	obs=(Obstacle *)malloc(sizeof(Obstacle));

	if(obs==NULL)
		return NULL;

	obs->x= getHres()-1;
	short yMin=margin->y+margin->height;
	short yMax=yMin+margin->distanceToOtherMargin-HEIGHT;
	obs->y=numberGeneration(yMin,yMax);

	return obs;
}

void setObstacle(Obstacle* obs, Margin* margin){
	short newX=getHres()-1;

	if(margin->x+margin->width < newX+WIDTH){
		return;
	}
	obs->x= newX;
	short yMin=margin->y+margin->height;
	short yMax=yMin+margin->distanceToOtherMargin-HEIGHT;
	obs->y=numberGeneration(yMin,yMax);
}

void deleteObstacle(Obstacle* obs){
	free(obs);
}

void drawObstacle(Obstacle* obs){
	unsigned int Hres=getHres();

	short i,j;

	if(obs->x+WIDTH>Hres){
		for(i=obs->x;i<=Hres;i++){
			for(j=obs->y;j<obs->y+HEIGHT;j++){
				vg_print_pixel(i,j,OBSTACLES_COLOR);
			}
		}
	}
	else
	{
		for(i=obs->x;i<obs->x+WIDTH;i++){
			for(j=obs->y;j<obs->y+HEIGHT;j++){
				vg_print_pixel(i,j,OBSTACLES_COLOR);
			}
		}
	}

}

void obsPullToTheLeft(Obstacle* obs){

	unsigned short numberOfpixelsPushed=5;

	obs->x-=numberOfpixelsPushed;
}
