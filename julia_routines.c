#include "spider.h"
//#include <xview/fullscreen.h>

#define my_clear_event(event) event.ie_code = 0; event.ie_flags = 0; event.ie_shiftmask= 0; event.action= 0; event.ie_string= 0; event.ie_xevent= 0;

#define TRUE			1
#define FALSE			0


extern	int 			XCen, YCen;
extern long 			d;
extern int			 	scale;
extern int			 	ps_spooling;
extern complex 			A,B,C;
extern double 			julia_depth;

extern FILE				*output;

/*XView variables */
extern Cursor 	cursor, wait_cursor;
extern Canvas	canvas;
extern Frame    frame;
extern Pixwin 	*pw;

Event			event;
Inputmask		im;
Fullscreen 		fs;

/*julia set  variables */
short Jfinished;

extern int PenMode;

StackSpace() /* kluge for now */
{
return(100000);
}

/* solve for roots of cubic of the form z^3 + Az + B */
void cubic_roots(A,B,z1,z2,z3)
complex A,B,*z1,*z2,*z3;
{
	complex Bo2, Ao3, Bo22, Ao33, S,S1, S2,  mAo3, sqrm3;
	double r,t;
	 
	if (A.x == 0.0 && A.y == 0.0) {
		r = pow(comp_abs(B), 1.0/3.0);
		t = 0.5*TPI + comp_arg(B)/3.0;
		
		*z1 = polar(r,t);
		*z2 = polar(r,t+TPI/3.0);
		*z3 = polar(r,t+2.0*TPI/3.0);
	} else {
		sqrm3.x = 0.0;
		sqrm3.y = sqrt(3.0);
		
		Bo2.x = -B.x/2.0;
		Bo2.y = -B.y/2.0;
	
		Ao3.x = A.x/3.0;
		Ao3.y = A.y/3.0;
	
		mAo3.x = -Ao3.x;
		mAo3.y = -Ao3.y;
	
		Bo22 = Bo2;
		CMUL(Bo22, Bo2);
		Ao33 = Ao3; 
		CMUL(Ao33, Ao3);
		CMUL(Ao33, Ao3);
		
		CSUM(S,Bo22, Ao33);
		r = sqrt(comp_abs(S));
		t = comp_arg(S)/2.0;
		
		S = polar(r,t);
		
		CADD(S, Bo2);
	
		r = pow(comp_abs(S), 1.0/3.0);
		t = comp_arg(S)/3.0;
		
		S = polar(r,t);
		
		COVER(*z1, mAo3, S);
		CADD(*z1, S);
		
		COVER(S1, Ao3, S);
		CADD(S1, S);
		CMUL(S1, sqrm3);
		
		COVER(S2, Ao3, S);
		CSUB(S2, S);
	
		CSUM(*z2, S2, S1);
		(*z2).x /= 2.0;
		(*z2).y /= 2.0;
		
/* later change the following to (*z3).x = (*z2).x; (*z3).y = -(*z2).y; */
		
		CDIFF(*z3, S2, S1);	
		(*z3).x /= 2.0;
		(*z3).y /= 2.0;
	}
}
 

/*------------------------ InverseImage ----------------------------------*/
void InverseImage(z, deriv)
complex z; double deriv;
{
/*    purpose         takes inverse image of Zr+i*Zi and plots the points  */
/*              recusively until deriv > julia_depth yielding a julia set  */


	double 			t,r;
	short 			scx, scy, i;
	complex 		w;

	
	/* xv_input_readevent(canvas_paint_window(canvas), &event, 0,0,&im); */
	xv_input_readevent(frame, &event, 0,0,&im);
	if (event_is_button(&event)) Jfinished = TRUE;

 /* notify_dispatch();  */

	scx = xwindow(z.x);
 	scy = ywindow(z.y);
	cline(scx, scy,scx+1,scy);

	w = z;
	CSUB(w, C);

	if (w.x == 0 && w.y == 0) 
		t = 0;
	else 
		t = atan2(w.y,w.x)/(double)d;
	r = pow(comp_abs(w), 1.0/(double)d);
	
	deriv *= 2.0*r; /* should be d*r, but 2 works better .. */
		
	if (deriv == 0) deriv = 1.0;
	
	if ((deriv < julia_depth) && (!Jfinished) && (StackSpace() > 1000)) 
		for (i=0; (!Jfinished) && (i<d); ++i) {
			t += TPI/(double)d;
			w = polar(r, t);
			InverseImage(w,deriv);
		}
}

/*-------------------------- InverseImage1 ----------------------------------*/
void InverseImage1(z, deriv)
complex z; double deriv;
{
/*    purpose         takes inverse image of Zr+i*Zi and plots the points  */
/*              recusively until deriv > julia_depth yielding a julia set  */


	short 			scx, scy, i;
	complex 		z1, z2, z3, BB;
	double 			temp_deriv;

	xv_input_readevent(frame, &event, 0,0,&im);
	if (event_is_button(&event)) Jfinished = TRUE;

	scx = xwindow(z.x);
 	scy = ywindow(z.y);
	cline(scx, scy,scx+1,scy);

	CDIFF(BB, B, z);
	cubic_roots(A,BB, &z1, &z2, &z3);


	CPROD(BB, z1, z1); 
	BB.x *= 3.0;
	BB.y *= 3.0;
	CADD(BB, A);
	
	temp_deriv = deriv*comp_abs(BB);
	if (temp_deriv == 0) temp_deriv = 1.0;
	
	if ((temp_deriv < julia_depth) && (!Jfinished) && (StackSpace() > 1000)) 
			InverseImage1(z1,temp_deriv);


	CPROD(BB, z2, z2); 
	BB.x *= 3.0;
	BB.y *= 3.0;
	CADD(BB, A);
	
	temp_deriv = deriv*comp_abs(BB);
	if (temp_deriv == 0) temp_deriv = 1.0;
	
	if ((temp_deriv < julia_depth) && (!Jfinished) && (StackSpace() > 1000)) 
			InverseImage1(z2,temp_deriv);

	CPROD(BB, z3, z3); 
	BB.x *= 3.0;
	BB.y *= 3.0;
	CADD(BB, A);
	
	temp_deriv = deriv*comp_abs(BB);
	if (temp_deriv == 0) temp_deriv = 1.0;
	
	if ((temp_deriv < julia_depth) && (!Jfinished) && (StackSpace() > 1000)) 
			InverseImage1(z3,temp_deriv);



}



/*---------------------- repelling_fx_pt ----------------------------------*/

complex repelling_fx_pt()
{
/*find a repelling fixed point for f... sigh..									*/

	complex z;
	int i;
	double t,r;
		
	z.x = z.y = 10.0;
	for (i=0; i<50; ++i) {
		CSUB(z,C);
		t = atan2(z.y,z.x)/(double)d;
		r = pow(comp_abs(z), 1.0/(double)d);
		z = polar(r,t);
	}
	CSUB(z,C);
	t = atan2(z.y,z.x)/(double)d;
	r = pow(comp_abs(z), 1.0/(double)d);
	z = polar(r,t+TPI/(double)d);

	return(z);


}


/*------------------------------ DoJulia1 ----------------------------------*/

void DoJulia1()
{
/*   purpose         Draws a julia set	for z^3 + Az + B		*/


	complex  		z, A1, z2, z3, BB;
	int				i;
	
	xv_set(frame,WIN_CURSOR,wait_cursor, 0);
	fs = xv_create(frame, FULLSCREEN, WIN_CONSUME_EVENTS, WIN_MOUSE_BUTTONS,
					0,0);
		
	/* find a repelling fixed point */
	z = A;
	for (i=0; i<50; ++i) { /* get atracted onto the julia set */
		CDIFF(BB, B, z);
		cubic_roots(A,BB, &z, &z2, &z3);
	}
	CDIFF(BB, B, z);
	cubic_roots(A,BB, &z2, &z, &z3); /* now avoid the fixed point */

	my_clear_event(event);
	Jfinished= FALSE;
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,wait_cursor, 0);
	InverseImage1(z,1.0);
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,cursor, 0);
	xv_destroy(fs);
}

/*------------------------------ DoJulia ----------------------------------*/

void DoJulia()
{
/*   purpose         Draws a julia set	for z^d + C		*/

	complex  		z;
		
	
	xv_set(frame,WIN_CURSOR,wait_cursor, 0);
	fs = xv_create(frame, FULLSCREEN, WIN_CONSUME_EVENTS, WIN_MOUSE_BUTTONS,
					0,0);
	my_clear_event(event);
	Jfinished= FALSE;
	z = repelling_fx_pt();
	InverseImage(z,1.0);
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,cursor, 0);
	xv_destroy(fs);
}

