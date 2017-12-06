#include <msp430.h>
  
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"

#include <stdlib.h>
#include <stdio.h>

#define GREEN_LED BIT6

#define A 2273
#define B 2025
#define CN 1911
#define D 1703
#define E 659
#define F 716
#define G 391
#define REST 1


// A lo sholo song
static short MARIO ;


static int JP[] = {D,CN,D,D,REST,REST,REST,REST,D,CN,D,D
		   ,REST,REST,REST,REST,D,CN,D,D,REST,E
		   ,REST,E,REST,G,REST,G,G,REST,F,D,E,REST,CN,A
		   ,REST,F,D,E,E,E,E,REST,A,D,G,REST,F,REST,F
		   ,REST,E,REST,E,E,E,E,REST,REST,D,CN,D,REST,A,G,REST
		   ,D,CN,D,REST,A,G,REST,REST,D,CN,CN,D,REST
		   ,A,A,D,D,CN,CN,CN,CN,0};

//GREEN COLUMN

//Frett Verde
AbRect frettV0= {abRectGetBounds, abRectCheck, {10,5}}; /**< 10x10 rectangle */
AbRect frettV1= {abRectGetBounds, abRectCheck, {10,5}}; /**< 10x10 rectangle */
AbRect frettV2= {abRectGetBounds, abRectCheck, {10,5}}; /**< 10x10 rectangle */

//Fret Verde de abajo layer 
Layer lFrettVerde0 = {		/** Layer with a red square for testing*/
  (AbShape *)&frettV0,
  {0, screenHeight-20},        /** bottom right */
  {0,0}, {0,0},		        /* last & next pos */
  COLOR_YELLOW,
  0,
};
Layer lFrettVerde1 = {		/** Layer with a red square for testing*/
  (AbShape *)&frettV1,
  {0, screenHeight-10},        /** bottom right */
  {0,0}, {0,0},		        /* last & next pos */
  COLOR_GREEN,
  &lFrettVerde0,
};
Layer lFrettVerde2 = {		/** Layer with a red square for testing*/
  (AbShape *)&frettV2,
  {0, screenHeight},        /** bottom right */
  {0,0}, {0,-5},		        /* last & next pos */
  COLOR_ORANGE,
  &lFrettVerde1,
};

//Nota Verde0
Layer notaV0 = {		/** Layer with a green circle */
  (AbShape *)&circle10,
  {20, 5},              /** Top Left Corner */
  {0,0}, {0,0},	        /* last & next pos */
  COLOR_GREEN,
  &lFrettVerde2,
};

//Nota Roja0
Layer notaR0 = {		
  (AbShape *)&circle10,
  {50, 5},              
  {0,0}, {0,0},	       
  COLOR_RED,
  &notaV0,
};

//Nota Amarilla
Layer notaA0 = {		
  (AbShape *)&circle10,
  {80, 5},              
  {0,0}, {0,0},	       
  COLOR_YELLOW,
  &notaR0,
};

//Nota Azul
Layer notaB0 = {		
  (AbShape *)&circle10,
  {110, 5},              
  {0,0}, {0,0},	       
  COLOR_BLUE,
  &notaA0,
};


/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/** Moving layers are independent from each other */
MovLayer mlV = { &notaV0, {0,3}, 0 }; 
MovLayer mlR = { &notaR0, {0,2}, 0 }; 
MovLayer mlA = { &notaA0, {0,3}, 0 }; 
MovLayer mlB = { &notaB0, {0,4}, 0 }; 

//END GREEN COLUMN

void movLayerDraw(MovLayer *movLayers, Layer *layers){
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) start interrups */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  

/** return a Vector on position */
Vec2 selectPosition(char p){
    Vec2 newPos;
    if (p == 0){
        newPos.axes[0] = 15;
        newPos.axes[1] = 5;
    }
    if (p == 1){
        newPos.axes[0] = 50;
        newPos.axes[1] = 5;
    }
    if (p == 2){
        newPos.axes[0] = 80;
        newPos.axes[1] = 5;
    }
    if (p == 3){
        newPos.axes[0] = 110;
        newPos.axes[1] = 5;
    }
    return newPos;
}

/** Global Variables */
u_int bgColor = COLOR_WHITE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

char butt0 = 0; //button 0 deactivated until triggered
char butt1 = 0; //button 1 deactivated until triggered
char butt2 = 0; //button 2 deactivated until triggered
char butt3 = 0; //button 3 deactivated until triggered

char* score; //Keep track of the score

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main(){
  P1DIR |= GREEN_LED;		
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  p2sw_init(15);
  buzzer_init();
  

  int c2 = 0;

    
      if(c2<sizeof(JP))
	buzzer_set_period(JP[c2++]);  /* iterates through notes */
	//if(JP[c2]==0)                 /* red led = end of song*/
    else
	buzzer_set_period(0);  /* iterates through notes */


  layerInit(&notaB0); //Init nota azul (linked list to all layers)
  layerDraw(&notaB0); //Draw nota azul (linked list to all layers)
  score = 0; //Score set to 0?? TODO check this out

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

     
    //if(MARIO[c2]==0) 
        //buzzer_set_period(0);



  //Forever in C
  for(;;) { 
      //drawString5x7(80,40, score, COLOR_BLACK, bgColor);

      /** Button setup and updates */
      butt0 = butt1 = butt2 = butt3 = 0;
      u_int switches = p2sw_read(), i;
      char str[5];
      for (i = 0; i < 4; i++)
          str[i] = (switches & (1<<i)) ? '-' : '1'+i;;
      str[4] = 0;
      

      if (str[0] == '1')
          butt0 = 1;
      if (str[1] == '2')
          butt1 = 1;
      if (str[2] == '3')
          butt2 = 1;
      if (str[3] == '4')
          butt3 = 1;

      /** Watchdog will interrupt this and turn on the CPU */
      while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
          P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
          or_sr(0x10);	      /**< CPU OFF */
      }
      P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
      redrawScreen = 0;
      movLayerDraw(&mlV, &notaV0);
      movLayerDraw(&mlR, &notaR0);
      movLayerDraw(&mlA, &notaA0);
      movLayerDraw(&mlB, &notaB0);

  }
}


void kamikazeV(MovLayer *ml, Layer *zone1, Layer *zone2, Layer *zone3, char color, char butt){
    //Vector2 for location
    Vec2 newPos;

    Region noteBound;
    Region z1Bound;
    Region z2Bound;
    Region z3Bound;
    layerGetBounds(zone1, &z1Bound);
    layerGetBounds(zone2, &z2Bound);
    layerGetBounds(zone3, &z3Bound);
    
    //Infinite for loop
    for (; ml; ml = ml->next) {
        vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
        abShapeGetBounds(ml->layer->abShape, &newPos, &noteBound);
        //Hit first zone
        if((noteBound.botRight.axes[1] > z1Bound.topLeft.axes[1]) 
                && (butt == 1)
                && (noteBound.botRight.axes[1] < z2Bound.topLeft.axes[1])
                && (noteBound.botRight.axes[1] < z3Bound.topLeft.axes[1]) )
        {
            //Missing && PRESS BUTT -> GIVE 1 POINT
            newPos = selectPosition(color);
            //butt = 0;
        }
        //Hit second zone CHANGE TO BUTT 1
        if((noteBound.botRight.axes[1] > z2Bound.topLeft.axes[1]) 
                && (butt == 1)
                && (noteBound.botRight.axes[1] < z3Bound.topLeft.axes[1]) )
        {
            newPos = selectPosition(color);
            //butt = 0;
        }
        //Hit third zone
        if((noteBound.botRight.axes[1] > z3Bound.topLeft.axes[1]) 
                && (butt == 1) )
        {
            newPos = selectPosition(color);
            //butt = 0;
        }

        ml->layer->posNext = newPos;
    } //end infinite for
}//end mlAdvance

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {
      
    kamikazeV(&mlV,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,0,butt0);
    kamikazeV(&mlR,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,1,butt1);
    kamikazeV(&mlA,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,2,butt2);
    kamikazeV(&mlB,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,3,butt3);

    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
