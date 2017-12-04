/* Small Demo that draws many colorfull squares */

#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <shape.h>

//Figures
const AbRect rect10          = {abRectGetBounds, abRectCheck, 10,10}; //black
const AbRect rBlue           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rRed            = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rGreen          = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rCyan           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rMagenta        = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rYellow         = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rOrange         = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rOrangeRed      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rDarkOrange     = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rGray           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rNavy           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rRoyalBlue      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rSkyBlue        = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rTurquoise      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rSteelBlue      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rLightBlue      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rAquamarine     = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rDarkGreen      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rDarkOliveGreen = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rSeaGreen       = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rSpringGreen    = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rPaleGreen      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rGreenYellow    = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rLimeGreen      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rForestGreen    = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rKhaki          = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rGold           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rGoldenrod      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rSienna         = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rBeige          = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rTan            = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rBrown          = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rChocolate      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rFirebrick      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rHotPink        = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rDeep           = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rViolet         = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rDarkViole      = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rPurple         = {abRectGetBounds, abRectCheck, 10,10};
const AbRect rMediumPurple   = {abRectGetBounds, abRectCheck, 10,10};

//Layers
Layer layer0 = {
  (AbShape *)&rect10,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /* position */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,  //only layer
};

//Color layers >:)
Layer lRects = {
  (AbShape *)&rBlue,
  {10, 10},
  {0,0}, {0,0},	
  COLOR_BLUE,
  0, 
};
Layer lRects2 = { 
  (AbShape *)&rRed,
  {30, 10},
  {0,0}, {0,0},
  COLOR_RED,
  &lRects,
};
Layer lRects3 = { 
  (AbShape *)&rGreen,
  {50, 10},
  {0,0}, {0,0},
  COLOR_GREEN,
  &lRects2,
};
Layer lRects4 = { 
  (AbShape *)&rCyan,
  {70, 10},
  {0,0}, {0,0},
  COLOR_CYAN,
  &lRects3,
};
Layer lRects5 = { 
  (AbShape *)&rMagenta,
  {90, 10},
  {0,0}, {0,0},
  COLOR_MAGENTA,
  &lRects4,
};
Layer lRects6 = { 
  (AbShape *)&rYellow,
  {110, 10},
  {0,0}, {0,0},
  COLOR_YELLOW,
  &lRects5,
};
Layer lRects7 = { 
  (AbShape *)&rOrange,
  {10, 30},
  {0,0}, {0,0},
  COLOR_ORANGE,
  &lRects6,
};
Layer lRects8 = { 
  (AbShape *)&rOrangeRed,
  {30, 30},
  {0,0}, {0,0},
  COLOR_ORANGE_RED,
  &lRects7,
};
Layer lRects9 = { 
  (AbShape *)&rDarkOrange,
  {50, 30},
  {0,0}, {0,0},
  COLOR_DARK_ORANGE,
  &lRects8,
};
Layer lRects10 = { 
  (AbShape *)&rGray,
  {70, 30},
  {0,0}, {0,0},
  COLOR_GRAY,
  &lRects9,
};
Layer lRects11 = { 
  (AbShape *)&rNavy,
  {90, 30},
  {0,0}, {0,0},
  COLOR_NAVY,
  &lRects10,
};
Layer lRects12 = { 
  (AbShape *)&rRoyalBlue,
  {110, 30},
  {0,0}, {0,0},
  COLOR_ROYAL_BLUE,
  &lRects11,
};
Layer lRects13 = { 
  (AbShape *)&rSkyBlue,
  {10, 50},
  {0,0}, {0,0},
  COLOR_SKY_BLUE,
  &lRects12,
};
Layer lRects14 = { 
  (AbShape *)&rTurquoise,
  {30, 50},
  {0,0}, {0,0},
  COLOR_TURQUOISE,
  &lRects13,
};
Layer lRects15 = { 
  (AbShape *)&rSteelBlue,
  {50, 50},
  {0,0}, {0,0},
  COLOR_STEEL_BLUE,
  &lRects14,
};
Layer lRects16 = { 
  (AbShape *)&rLightBlue,
  {70, 50},
  {0,0}, {0,0},
  COLOR_LIGHT_BLUE,
  &lRects15,
};
Layer lRects17 = { 
  (AbShape *)&rAquamarine,
  {90, 50},
  {0,0}, {0,0},
  COLOR_AQUAMARINE,
  &lRects16,
};
Layer lRects18 = { 
  (AbShape *)&rDarkGreen,
  {110, 50},
  {0,0}, {0,0},
  COLOR_DARK_GREEN,
  &lRects17,
};
Layer lRects19 = { 
  (AbShape *)&rDarkOliveGreen,
  {10, 70},
  {0,0}, {0,0},
  COLOR_DARK_OLIVE_GREEN,
  &lRects18,
};
/* Layer lRects20 = { 
  (AbShape *)&rSeaGreen,
  {30, 70},
  {0,0}, {0,0},
  COLOR_SEA_GREEN,
  &lRects19,
};
Layer lRects21 = { 
  (AbShape *)&rSpringGreen,
  {50, 70},
  {0,0}, {0,0},
  COLOR_SPRING_GREEN,
  &lRects20,
};
Layer lRects22 = { 
  (AbShape *)&rPaleGreen,
  {70, 70},
  {0,0}, {0,0},
  COLOR_PALE_GREEN,
  &lRects21,
};
Layer lRects23 = { 
  (AbShape *)&rGreenYellow,
  {90, 70},
  {0,0}, {0,0},
  COLOR_GREEN_YELLOW,
  &lRects22,
};
Layer lRects24 = { 
  (AbShape *)&rLimeGreen,
  {110, 70},
  {0,0}, {0,0},
  COLOR_LIME_GREEN,
  &lRects23,
};

*/

u_int bgColor = COLOR_WHITE;

void main(){
  configureClocks(); //Required
  lcd_init(); //Required
  or_sr(0x8);			/* GIE (enable interrupts) */
  u_char width = screenWidth, height = screenHeight;

  //Colors are stored in lcdutils.h
  clearScreen(COLOR_WHITE); //white background (for testing)


  layerInit(&lRects18);
  layerDraw(&lRects18);
}
