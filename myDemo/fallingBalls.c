#include <msp430.h>
  
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include <stdlib.h>
#include <stdio.h>

#define GREEN_LED BIT6

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
  (AbShape *)&circle12,
  {15, 5},              /** Top Left Corner */
  {0,0}, {0,0},	        /* last & next pos */
  COLOR_GREEN,
  &lFrettVerde2,
};

//Frett Rojo Shapes
//AbRect frettR0= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone
//AbRect frettR1= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone
//AbRect frettR2= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone

//Fret Rojo layer
/** 
Layer lFrettRojo0 = {		
  (AbShape *)&frettV0,
  {30, screenHeight-20},       
  {0,0}, {0,0},		      
  COLOR_YELLOW,
  0,
};
Layer lFrettRojo1 = {	
  (AbShape *)&frettV1,
  {30, screenHeight-10},
  {0,0}, {0,0},	
  COLOR_GREEN,
  &lFrettRojo0,
};
Layer lFrettRojo2 = {	
  (AbShape *)&frettV2,
  {30, screenHeight},  
  {0,0}, {0,-5},
  COLOR_ORANGE,
  &lFrettRojo1,
};
*/

//Nota Roja0
Layer notaR0 = {		
  (AbShape *)&circle12,
  {50, 5},              
  {0,0}, {0,0},	       
  COLOR_RED,
  &notaV0,
};

//Frett Amarillo
//AbRect frettA0= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone
//AbRect frettA1= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone
//AbRect frettA2= {abRectGetBounds, abRectCheck, {30,5}}; //Rectangle zone

//Fret Amarillo layer 
/**
Layer lFrettAmarillo0 = {
  (AbShape *)&frettV0,
  {70, screenHeight-20},       
  {0,0}, {0,0},		        
  COLOR_YELLOW,
  0,
};
Layer lFrettAmarillo1 = {
  (AbShape *)&frettV1,
  {70, screenHeight-10},        
  {0,0}, {0,0},		
  COLOR_GREEN,
  &lFrettAmarillo0,
};
Layer lFrettAmarillo2 = {
  (AbShape *)&frettV2,
  {70, screenHeight},        
  {0,0}, {0,-5},
  COLOR_ORANGE,
  &lFrettAmarillo1,
};
*/
//Nota Amarilla
Layer notaA0 = {		
  (AbShape *)&circle12,
  {80, 5},              
  {0,0}, {0,0},	       
  COLOR_YELLOW,
  &notaR0,
};

//Fret Azul de abajo layer 
/**
Layer lFrettAzul0 = {
  (AbShape *)&frettV0,
  {110, screenHeight-20},       
  {0,0}, {0,0},		        
  COLOR_BLACK,
  0,
};
Layer lFrettAzul1 = {
  (AbShape *)&frettV1,
  {110, screenHeight-10},        
  {0,0}, {0,0},		
  COLOR_BLACK,
  &lFrettAzul0,
};
Layer lFrettAzul2 = {
  (AbShape *)&frettV2,
  {110, screenHeight},        
  {0,0}, {0,-5},
  COLOR_BLACK,
  &lFrettAzul1,
};
*/
//Nota Azul
Layer notaB0 = {		
  (AbShape *)&circle12,
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

/** Glonal Variables */
u_int bgColor = COLOR_WHITE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

char butt0 = 0; //button 0 deactivated
char butt1 = 0; //button 1 deactivated
char butt2 = 0; //button 2 deactivated
char butt3 = 0; //button 3 deactivated

char* score; 

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  p2sw_init(15);

  //layerInit(&lFrettVerde2 );
  //layerDraw(&lFrettVerde2 );

  /**
  layerInit(&notaV0 );
  layerDraw(&notaV0 );

  layerInit(&notaR0 );
  layerDraw(&notaR0 );

  layerInit(&notaA0 );
  layerDraw(&notaA0 );

  */
//  layerInit(&notaV0 );
 // layerDraw(&notaV0 );

  layerInit(&notaB0 );
  layerDraw(&notaB0 );
  score = 0;

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  //Forever in C
  for(;;) { 
      //drawString5x7(80,40, score, COLOR_BLACK, bgColor);
      butt0 = 0;
      butt1 = 0;
      butt2 = 0;
      butt3 = 0;
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
      //drawString5x7(80,20, str, COLOR_GREEN, bgColor);

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


void mlAdvance(MovLayer *ml, Layer *zone1, Layer *zone2, Layer *zone3, char carril){
    //Vector2 for location
    Vec2 newPos;

    //Boundaries for "Objects"
    Region noteBound;
    Region noteBound1;
    Region noteBound2;
    Region noteBound3;
    Region z1Bound;
    Region z2Bound;
    Region z3Bound;
    layerGetBounds(zone1, &z1Bound);
    layerGetBounds(zone2, &z2Bound);
    layerGetBounds(zone3, &z3Bound);

    // Get switches
    u_int switches = p2sw_read(), i;
    char str[5];

    //Infinite for loop
    for (; ml; ml = ml->next) {
        vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
        //Get bounds for each new posigion
        abShapeGetBounds(ml->layer->abShape, &newPos, &noteBound);

        for (i = 0; i < 4; i++)
            str[i] = (switches & (1<<i));
        str[4] = 0;

        //Hit first zone
        if((noteBound.botRight.axes[1] > z1Bound.topLeft.axes[1]) 
                && (str[0] == 1)
                && (noteBound.botRight.axes[1] < z2Bound.topLeft.axes[1])
                && (noteBound.botRight.axes[1] < z3Bound.topLeft.axes[1]) )
        {
            //Missing && PRESS BUTT -> GIVE 1 POINT
            newPos = selectPosition(carril);
        }
        //Hit second zone CHANGE TO BUTT 1
        if((noteBound.botRight.axes[1] > z2Bound.topLeft.axes[1]) && (str[carril] == 1)
                && (noteBound.botRight.axes[1] < z3Bound.topLeft.axes[1]) )
        {
            newPos = selectPosition(carril);
        }
        //Hit third zone
        if((noteBound.botRight.axes[1] > z3Bound.topLeft.axes[1]) && (str[carril] == 1) )
        {
            newPos = selectPosition(carril);
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
   // mlAdvance(&mlV,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,0);
   // mlAdvance(&mlR,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,1);
   // mlAdvance(&mlA,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,2);
//    mlAdvance(&mlB,&lFrettVerde0,&lFrettVerde1,&lFrettVerde2,3);
    //mlAdvance(&mlR,&lFrettRojo0,&lFrettRojo1,&lFrettRojo2,1);
    //mlAdvance(&mlA,&lFrettAmarillo0,&lFrettAmarillo1,&lFrettAmarillo2,2);
    //mlAdvance(&mlA,&lFrettAzul0,&lFrettAzul1,&lFrettAzul2,3);
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
