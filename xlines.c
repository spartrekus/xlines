

/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* include some silly stuff */
#include <stdio.h>
#include <stdlib.h>


int text_color = 1;

/* here are our X variables */
Display *dis;
int screen;
Window win;
GC gc;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();


void init_x() 
{
        /* get the colors black and white (see section for details) */        
	unsigned long black,white;

	dis=XOpenDisplay((char *)0);
   	screen=DefaultScreen(dis);
	black=BlackPixel(dis, screen),
	white=WhitePixel(dis, screen);
        // xserver android
   	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	1260, 690, 5, black, white);
	XSetStandardProperties(dis,win,"TextX11","Hi",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
        gc=XCreateGC(dis, win, 0,0);        
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
};

void close_x() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
	exit(1);				
};

void redraw() {
	XClearWindow(dis, win);
};

void mvprintw( int yy , int xx , char *mystring) 
{
      int x = xx; int y= yy;  char text[255];			
      strcpy(text, mystring );
      XDrawString(dis,win,gc,x,y, text, strlen(text));  
};



void mvlinew( int y0, int x0, int y1, int x1)
{
    XPoint points[] = {
      {y0, x0},
      {y1, x1}
    };
    int npoints = sizeof(points)/sizeof(XPoint);
    XDrawLines( dis , win, gc, points, npoints, CoordModeOrigin);
}


void mvrectanglew( int y0, int x0, int y1, int x1)
{
   XDrawRectangle(dis, win, gc, y0, x0, y1-y0, x1-x0);
}



int main () 
{
	XEvent event;		/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */	
	char text[255];		/* a char buffer for KeyPress Events */

	init_x();


        int gameover = 0;
	/* look for events forever... */
	while( gameover == 0 ) {		
		/* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
		XNextEvent(dis, &event);
	
		if (event.type==Expose && event.xexpose.count==0) {
		/* the window was exposed redraw it! */
			redraw();
		}
		if (event.type==KeyPress&&
		    XLookupString(&event.xkey,text,255,&key,0)==1) {
		/* use the XLookupString routine to convert the invent
		   KeyPress data into regular text.  Weird but necessary...
		*/
  		       
                       if (text[0]=='l') {    mvprintw( 250, 250, "line" );   } 

                       else if (text[0]=='u') { mvprintw( 50, 50, "hello (u)" );   } 
                       else if (text[0]=='d') { mvprintw( 75, 75, "hello (d)" );   } 

                       else if (text[0]=='c') {  
	                     XClearWindow(dis, win);
                       }

                       else if (text[0]=='o') { mvlinew( 50,50, 100,100 );   } 
                       else if (text[0]=='x') {      
                           XSetForeground(dis, gc, 240 );
                           mvlinew( 25,  50,  250 , 300 );
                           XSetForeground(dis, gc, 0 );
                       } 
                       else if (text[0]=='r') { 
                         XSetForeground(dis, gc, 1 );
                         mvrectanglew( 25 , 50 , 250 ,300 );   }  //350,275
                       //else if (text[0]=='r') { mvrectanglew( 25 , 50 , 250 ,300 );   } 
                       // y:49 x:25

  else if (text[0]=='w') 
  {
    XPoint points[] = {
      {0, 0},
      {150, 150},
      {100, 15},
      {100, 150}
    };
    int npoints = sizeof(points)/sizeof(XPoint);

    /* draw a small triangle at the top-left corner of the window. */
    /* the triangle is made of a set of consecutive lines, whose   */
    /* end-point pixels are specified in the 'points' array.       */
    /* draw the triangle in a yellow color. */
   // XSetForeground(display, gc, black);
    //XSetForeground(dis,gc,black);
    XDrawLines( dis , win, gc, points, npoints, CoordModeOrigin);
  }




                       else if (text[0]=='t') {    
                            int x = 100; int y= 100; 		
  	                    //XSetForeground(dis,gc,rand()%event.xbutton.x%255);
                            XSetForeground(dis, gc, text_color );
                            strcpy(text,"Hello World!"); 
			    XDrawString(dis,win,gc,x,y, text, strlen(text) );     
                            text_color++;

                            //strcpy(text,  ); 
                            snprintf( text, 50 , "%d %d", text_color, text_color );
			    XDrawString(dis,win,gc,x-20,y-20, text, strlen(text) );     
                       } 

                       else if (text[0]=='q') 
                       {
				gameover = 1;
				close_x();
		       }
		        printf("You pressed the %c key!\n",text[0]);
		}


		if (event.type==ButtonPress) 
                {
		        /* tell where the mouse Button was Pressed */
			int x=event.xbutton.x, y=event.xbutton.y;

			strcpy(text,"Mouse !");
			//XSetForeground(dis,gc,rand()%event.xbutton.x%255); 
                        XSetForeground(dis, gc, 0 );
			XDrawString(dis,win,gc,x,y, text, strlen(text));
                        printf( " the mouse position Y=%d X=%d \n",  event.xbutton.y, y=event.xbutton.x  );
		}
	}

       	close_x();
}



