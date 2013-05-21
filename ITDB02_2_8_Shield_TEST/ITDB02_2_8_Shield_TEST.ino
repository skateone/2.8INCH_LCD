/*
This sample application was written to test the functionality of the ITDB02-2.8 Display Shield
It allows to check if the touch sensor is operation as well as check screen integrity
This demo was put together the www.robotics.org.za 
NOTE: you will need to downlaod the UFTF and Utouch libs from: 
http://henningkarlsen.com/electronics/library.php?id=52
Authored by Shaun Reynolds (The Skate)
*/
#include <UTFT.h>
#include <UTouch.h>
/*
UTFT class initialization 
 UTFT(Model,RS,WR,CS,RST)
  Model = ITDB28
  RS    = A5
  WR    = A4
  CS    = A3
  RST   = A2
*/
UTFT           myGLCD(ITDB28,A5,A4,A3,A2);   //UFTF class init 
/*
 UTOUCH(TCLK, TCS, TDIN, TDOUT, IRQ)
 
 TCLK = A1
 TCS  =  (this pin is pulled low by default in the ITEAD 2.8 Touch SHIELD but you will still have to give it an assignment as the buidl will fail)
 TDIN = A0
 TDOUT= 8
 IRQ  = 9
 */
UTouch         myTouch(A1,A2,A0,8,9);        //Utouch class init
extern uint8_t SmallFont[];                  //specifies a font lib to use
boolean        touchLatch = false;          //touch latch varable 
long           x, y;                        //touch x and y vars
long           oldX = 0, oldY = 0;          

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);                //Init LCD as landscape
  myGLCD.clrScr();                          //clear LCD screen
  myTouch.InitTouch(LANDSCAPE);             //Init Touch as landscape
  myTouch.setPrecision(PREC_MEDIUM);        //Set touch precision to medium
}
//This function draws the cross that is displayed everytime you touch the screen
//it uses the old position to erase the old cross and then redraws it to the new position
void drawX() {
  myGLCD.setColor(0, 0, 0);
  myGLCD.drawLine(oldX-6,oldY,oldX+6,oldY);
  myGLCD.drawLine(oldX,oldY-6,oldX,oldY+6);  
  oldX = x;
  oldY = y;
  myGLCD.setColor(255, 255, 50);
  myGLCD.drawLine(x-5,y,x+5,y);
  myGLCD.drawLine(x,y-5,x,y+5);  
}
//Function to setup the basic layout of all the static elements
void drawScreen() {
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(170,217,317,237);
  myGLCD.print("X:",173,220);
  myGLCD.print("Y:",250,220);
  myGLCD.setFont(SmallFont);
}
void loop()
{
  drawScreen();
  while (myTouch.dataAvailable() == true)      //If touch is sensed then continue looping
  {
    // this function applies latching so when you touch it will only drawe the red square 
    //once. the best way to work with LCD's is to only update what is needed on the display 
    //as writing to the display is quite a process intensive task
    if(!touchLatch) {
      touchLatch = true;
      myGLCD.setColor(255, 0, 0);
      myGLCD.drawRect(170,217,317,237); 
    }
    myTouch.read();                           //Read touch sensor
    /*
    due to the layout of the ITDB02-2.8 display shield the x-axis is inverted when displaying and sensing in
    landscape mode.
    the below algorithm will correct the inversion as well as apply a basic offset to make the touch a little
    more accurate. 
    */
    x = abs(myTouch.getX()-320)-15;        
    y = myTouch.getY();
    if ((x!=-1) and (y!=-1))
    {
      //updates the latest touch co-ordinates to the screen for easy reading
      myGLCD.setColor(255, 255, 255);
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.printNumI(x,203,220,3);
      myGLCD.printNumI(y,280,220,3);
      drawX() ;
    }
  }
  //when the Touch is not active redraw the original white block once to indicate
  //no touch
  if(touchLatch) {
    touchLatch = false;
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRect(170,217,317,237); 
  }
}






