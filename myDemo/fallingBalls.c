#include <msp430.h>
  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#define GREEN_LED BIT6

//GREEN COLUMN

//Frett Verde
AbRect frettV = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */

//Fret Verde de abajo layer 
Layer lFrettVerde = {		/** Layer with a red square for testing*/
  (AbShape *)&frettV,
  {14, screenHeight-10},        /** bottom right */
  {0,0}, {0,0},		        /* last & next pos */
  COLOR_GREEN,
  0,
};

//Nota Verde0
Layer notaV0 = {		/** Layer with a green circle */
  (AbShape *)&circle12,
  {15, 5},              /** Top Left Corner */
  {0,0}, {0,0},	        /* last & next pos */
  COLOR_VIOLET,
  &lFrettVerde,
};

// frett bounds
AbRectOutline frettVOutline = {
    abRectOutlineGetBounds, abRectOutlineCheck, 
    {10, 10}  //size
};
 
Layer frettVfield = {		/* playing field as a layer */
  (AbShape *) &frettVOutline,
  {14, screenHeight-10}, //location
  {0,0}, {0,0},		/* last & next pos */
  COLOR_ORANGE_RED,
  &notaV0,
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

//nota Verde Cayendo
                                //Add if you use more layers
MovLayer mlV = { &notaV0, {0,1}, 0 }; 

//END GREEN COLUMN

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
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

void mlVAdv(MovLayer *ml, Region *frett){
//void mlVAdv(MovLayer *ml, Region *fence){
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  //Region frettBoundary;

  //layerGetBounds(&fieldLayer, &fieldFence);

  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
    ml->layer->posNext = newPos;
  } /** for ml */
}

u_int bgColor = COLOR_WHITE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  shapeInit();

  p2sw_init(1);

  shapeInit();

  layerInit(&frettVfield );
  layerDraw(&frettVfield );

  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  layerGetBounds(&frettVfield, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  //Forever in C
  for(;;) { 
      /** Watchdog will interrupt this and turn on the CPU */
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&mlV, &notaV0);
    //TEST SCORE
    drawString5x7(100,200, "hello", COLOR_GREEN, COLOR_RED);
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {
    mlVAdv(&mlV, &fieldFence);
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
