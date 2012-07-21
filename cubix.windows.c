/*********************************************************************

	cubix.windows.c
	
	window functions for cubix
	
*********************************************************************/
#include "cubix.windows.h"
#include "spider.h"

#include "cmath.h"
//
//
//#include <QuickDraw.h>
//#include <MacTypes.h>
//#include <WindowMgr.h>
//#include <TextEdit.h>
//#include <ControlMgr.h>
//#include <EventMgr.h>
//#include <Storage.h>
//#include <Strings.h>
//#include <Stdio.h>
//#include <DialogMgr.h>

//#if EIGHTEIGHTYONE
//#include <math881.h>
//#else
//#include <math.h>
//#endif

//#include "windows.h"

//WindowRecord			Ghost_wRec, Cspace_wRec, Actual_wRec, Goingson_wRec;
//extern 	WindowPtr	 	Ghost_Window, Cspace_Window, Actual_Window, Goingson_Window;

extern 	long 			den, num, d;
extern	long 			den1, den2, num11, num12, num21, num22;
extern  long 			xden,xnum;
extern 	complex			A,B,C;
extern 	float			wepsilon, fepsilon, furad;
extern 	int				scale, XCen, YCen, showticks;			
extern	char 			strng[80];
extern 	double			depth;
					
//extern GrafPtr		 	ActualPtr;
//GrafPort	     		ActualPort;
	   
//extern ControlHandle vScroll, radScroll;
//ControlHandle		 but[NUM_BUTTONS];
//extern TEHandle 	 TEH;
extern char			 dirty;


extern int			 linesInFolder, radius;

//extern Cursor 		waitCursor;

int					Nlifts = 10, HilightedRay = -1;
;
//Pattern				**greypat, **blackpat;

//PicHandle			QuadraticLocus, CubicLocus, QuarticLocus;

long Str_to_Int();
float Str_to_Float();


//void disp(char* str) {
//    
//}
////char *str;
//{
////	int i, l;
////	l = strlen(str);
////	for (i=0; i<l; ++i) 
////		if (str[i] == '\n') str[i] = 13;
////		
////	TEInsert( str, (long)l,TEH );
////	ShowSelect();
//	
//}


int SetUpWindows()
{
//    /*
//	Rect	destRect, viewRect;
//	Rect	vScrollRect;
//	FontInfo	myInfo;
//	int		height, cont_h, cont_v,i;
//	/*
//	Actual_Window = GetNewWindow( 129, &Actual_wRec, -1L );
//	Cspace_Window = GetNewWindow( 131, &Cspace_wRec, -1L );
//	QuadraticLocus = (PicHandle)GetResource('PICT', 1000);
//	CubicLocus     = (PicHandle)GetResource('PICT', 1001);
//	QuarticLocus   = (PicHandle)GetResource('PICT', 1002);
//	SetWindowPic(Cspace_Window, QuadraticLocus);
//	Ghost_Window =  GetNewWindow( 130, &Ghost_wRec, -1L );
//*/
//	for (i=0; i< NUM_BUTTONS; ++i) {
//		but[i]    = GetNewControl( 1001+i, Ghost_Window);
//	}
//	
//	cont_h = 2;
//	cont_v = 5;
//	for (i=0; i<NUM_BUTTONS; ++i, cont_h += /*BUTTON_WIDTH*/(*but[i-1])->contrlRect.right-(*but[i-1])->contrlRect.left+2) 
//		MoveControl(but[i], cont_h, cont_v);
//
//	ActualPtr = &ActualPort;
//	
//	
//	SetPort((Goingson_Window = GetNewWindow( windowID, &Goingson_wRec, -1L )));
//	TextFont(4);
//	TextSize(9);
//	vScrollRect = (*Goingson_Window).portRect;
//	vScrollRect.left = vScrollRect.right-15;
//	vScrollRect.right += 1;
//	vScrollRect.bottom -= 14;
//	vScrollRect.top -= 1;
//	vScroll = NewControl( Goingson_Window, &vScrollRect, "\p", 1, 0, 0, 0,
//		scrollBarProc, 0L);
//
//	viewRect = thePort->portRect;
//	viewRect.right -= SBarWidth;
//	viewRect.bottom -= SBarWidth;
//	InsetRect(&viewRect, 4, 4);
//	TEH = TENew( &viewRect, &viewRect );
//	
//	blackpat = (Pattern **)GetPattern(1000);
//	greypat  = (Pattern **)GetPattern(1001);
//	
//	HiliteControl (but[XLIFT_BUT],255); /* disable x lift button initially */
//	
//	SetView(thePort);
//	SelectWindow(Ghost_Window);
//	HiliteWindow(Goingson_Window,1);
//	dirty = 0;
}


AdjustText ()

{
//	int		oldScroll, newScroll, delta;
//	
//	oldScroll = (**TEH).viewRect.top - (**TEH).destRect.top;
//	newScroll = GetCtlValue(vScroll) * (**TEH).lineHeight;
//	delta = oldScroll - newScroll;
//	if (delta != 0)
//	  TEScroll(0, delta, TEH);
}


SetVScroll()
{
//	register int	n;
//	
//	n = (**TEH).nLines-linesInFolder;
//
//	if ((**TEH).teLength > 0 && (*((**TEH).hText))[(**TEH).teLength-1]=='\r')
//		n++;
//
//	SetCtlMax(vScroll, n > 0 ? n : 0);
}

void ShowSelect()

{
//	register	int		topLine, bottomLine, theLine;
//	
//	SetVScroll();
//	AdjustText();
//	
//	topLine = GetCtlValue(vScroll);
//	bottomLine = topLine + linesInFolder;
//	
//	if ((**TEH).selStart < (**TEH).lineStarts[topLine] ||
//			(**TEH).selStart >= (**TEH).lineStarts[bottomLine]) {
//		for (theLine = 0; (**TEH).selStart >= (**TEH).lineStarts[theLine]; theLine++)
//			;
//		SetCtlValue(vScroll, theLine - linesInFolder / 2);
//		AdjustText();
//	}
}

void SetView(w)
//WindowPtr w;
{
//	(**TEH).viewRect = w->portRect;
//	(**TEH).viewRect.right -= SBarWidth;
//	(**TEH).viewRect.bottom -= SBarWidth;
//	InsetRect(&(**TEH).viewRect, 4, 4);
//	linesInFolder = ((**TEH).viewRect.bottom-(**TEH).viewRect.top)/(**TEH).lineHeight;
//	(**TEH).viewRect.bottom = (**TEH).viewRect.top + (**TEH).lineHeight*linesInFolder;
//	(**TEH).destRect.right = (**TEH).viewRect.right;
//	TECalText(TEH);
}

void UpdateWindow(theWindow)
//WindowPtr	theWindow;
{
//	GrafPtr	savePort;
//	Rect 	rct;
	char	str[255];
	
//	GetPort( &savePort );

	SetPort( theWindow );
/*	CopyRgn(((WindowPeek)theWindow)->contRgn,((WindowPeek)theWindow)->updateRgn); */
	BeginUpdate( theWindow );


//	if (theWindow == Goingson_Window) {
//		EraseRect(&theWindow->portRect);
//		DrawControls( theWindow );
//		DrawGrowIcon( theWindow );
//		TEUpdate( &theWindow->portRect, TEH );
//	} 
//	else if (theWindow == Ghost_Window) { 
//		DrawControls( theWindow );
//	}
	
	/*	else if (theWindow == DomainWindow) 
		CopyBits(&DomainBitmap, &(theWindow->portBits), &DomainBitmap.bounds, 
			&(theWindow->portRect),srcCopy, 0L);
	
	else if (theWindow == RangeWindow) 
		CopyBits(&RangeBitmap, &theWindow->portBits, &RangeBitmap.bounds, 
			&theWindow->portRect,srcCopy, 0L);
	
	else if (theWindow == radWindow) {
		radius = GetCtlValue(radScroll);
		sprintf(str, "\p %d", radius); 
		MoveTo(40,95);
		rct.left = 40;
		rct.right = 140;
		rct.top = 75;
		rct.bottom = 141;
		EraseRect(&rct);
		DrawString(str);
		rct.left = 113 - radius;
		rct.right = 113 + radius;
		rct.top = 113 - radius;
		rct.bottom = 113 + radius;
		FrameOval(&rct);
		DrawControls( theWindow );
	}
*/	
//	EndUpdate( theWindow );
//	
//	if (theWindow == Cspace_Window) {
//		SetPort(Cspace_Window);
//		if (d == 2) {
//			MoveTo((int)(256.0*(C.x+2.0)/2.5), 256 - (int)(256.0*(C.y+1.25)/2.5));
//			Line(4,0); Line(-8,0); Line(4,0);
//			Line(0,4); Line(0,-8); Line(0,4);
//		} else if (d==3 ||  d == 4) {
//			MoveTo((int)(256.0*(C.x+1.644)/3.15), 256 - (int)(256.0*(C.y+1.58)/3.15));
//			Line(4,0); Line(-8,0); Line(4,0);
//			Line(0,4); Line(0,-8); Line(0,4);
//		}
//	}
//
//
//
//	SetPort( savePort );
}




void ScrollProc(/*theControl,*/ theCode)
//ControlHandle	theControl;
int				theCode;
{
//	int	pageSize;
//	int	scrollAmt;
//	
//	if (theCode == 0)
//		return ;
//	
//	pageSize = ((**TEH).viewRect.bottom-(**TEH).viewRect.top) / 
//			(**TEH).lineHeight - 1;
//			
//	switch (theCode) {
//		case inUpButton: 
//			scrollAmt = -1;
//			break;
//		case inDownButton: 
//			scrollAmt = 1;
//			break;
//		case inPageUp: 
//			scrollAmt = -pageSize;
//			break;
//		case inPageDown: 
//			scrollAmt = pageSize;
//			break;
//		}
//	SetCtlValue( theControl, GetCtlValue(theControl)+scrollAmt );
//	AdjustText();

}

/* ===================================== SetParams ============================*/
void SetParams()
{
	int  			itemHit, i, typ;
//	Handle			textItem;
//	DialogPtr		param_window;
//	DialogRecord	param_DRecord;
//	Rect			box;
//	NSString			*eps_str, scale_str, depth_str, str;
//
//	param_window = GetNewDialog(1002, &param_DRecord, -1L);
//	SetPort(param_window);
//
//		
//	
//	
//	Float_to_Str(wepsilon, eps_str);
//	GetDItem(param_window, 3, &typ, &textItem, &box);
//	SetIText(textItem, eps_str);
//	
//	Float_to_Str(fepsilon, eps_str);
//	GetDItem(param_window, 4, &typ, &textItem, &box);
//	SetIText(textItem, eps_str);
//	
//	Float_to_Str(furad, eps_str);
//	GetDItem(param_window, 5, &typ, &textItem, &box);
//	SetIText(textItem, eps_str);
//	
//	
//	
//	Int_to_Str((long)Nlifts, scale_str);
//	GetDItem(param_window, 6, &typ, &textItem, &box);
//	SetIText(textItem, scale_str);
//	
//	Int_to_Str((long)scale, scale_str);
//	GetDItem(param_window, 7, &typ, &textItem, &box);
//	SetIText(textItem, scale_str);
//	Int_to_Str((long)XCen, scale_str);
//	GetDItem(param_window, 8, &typ, &textItem, &box);
//	SetIText(textItem, scale_str);
//	Int_to_Str((long)YCen, scale_str);
//	GetDItem(param_window, 9, &typ, &textItem, &box);
//	SetIText(textItem, scale_str);
//	
//	Float_to_Str(depth, depth_str);
//	GetDItem(param_window, 10, &typ, &textItem, &box);
//	SetIText(textItem, depth_str);
//
//	GetDItem(param_window, 11, &typ, &textItem, &box);
//	SetCtlValue(textItem, showticks);
//
//	do {
//		ModalDialog(0L, &itemHit);
//	
//		if (itemHit == 11) {
//			GetDItem(param_window, 11, &typ, &textItem, &box);
//			showticks = !GetCtlValue(textItem);
//			SetCtlValue(textItem, showticks);
//		}
//	
//	} while (itemHit !=1 && itemHit !=2);
//	        	
//	if (itemHit == 2) {
//	 	CloseDialog(param_window);
//		return;
//	}
//	if (itemHit == 1) {
//			GetDItem(param_window, 3, &typ, &textItem, &box);
//	     	GetIText(textItem, eps_str);
//	        wepsilon = Str_to_Float(eps_str);
//	
//			GetDItem(param_window, 4, &typ, &textItem, &box);
//	     	GetIText(textItem, eps_str);
//	        fepsilon = Str_to_Float(eps_str);
//	
//			GetDItem(param_window, 5, &typ, &textItem, &box);
//	     	GetIText(textItem, eps_str);
//	        furad = Str_to_Float(eps_str);
//	
//			GetDItem(param_window, 6, &typ, &textItem, &box);
//	     	GetIText(textItem, scale_str);
//	        Nlifts = Str_to_Int(scale_str);
//
//			GetDItem(param_window, 7, &typ, &textItem, &box);
//	     	GetIText(textItem, scale_str);
//	        scale = Str_to_Int(scale_str);
//
//			GetDItem(param_window, 8, &typ, &textItem, &box);
//	     	GetIText(textItem, scale_str);
//	        XCen = Str_to_Int(scale_str);
//
//			GetDItem(param_window, 9, &typ, &textItem, &box);
//	     	GetIText(textItem, scale_str);
//	        YCen = Str_to_Int(scale_str);
//
//			GetDItem(param_window, 10, &typ, &textItem, &box);
//	     	GetIText(textItem, depth_str);
//	        depth = Str_to_Float(depth_str);
//	}
//	 	CloseDialog(param_window);
//	sprintf(strng, "\nwepsilon = %f, scale = %d", wepsilon, scale);
//	disp(strng);
}

/* ==============================dispABC ====================================*/
/* display the values of AB and C (as appropriate in the goings on window */
void dispABC()
{
		
//		SetPort(Cspace_Window);
		if (sp.Num_Crit_Pts ==1)  {
			if (C.y < 0.0) 
				sprintf(strng, "\nC = %lf - i %lf\n", C.x, -C.y);
			else
				sprintf(strng, "\nC = %lf + i %lf\n", C.x, C.y);
//			disp(strng);

			if (d == 2) {
//				MoveTo((int)(256.0*(C.x+2.0)/2.5), 256 - (int)(256.0*(C.y+1.25)/2.5));
//				Line(4,0); Line(-8,0); Line(4,0);
//				Line(0,4); Line(0,-8); Line(0,4);
			} else if (d == 3) {
//				MoveTo((int)(256.0*(C.x+1.644)/3.15), 256 - (int)(256.0*(C.y+1.58)/3.15));
//				Line(4,0); Line(-8,0); Line(4,0);
//				Line(0,4); Line(0,-8); Line(0,4);
			}
			
		} else {
			if (A.y < 0.0) 
				sprintf(strng, "\n\nA = %lf - i %lf", A.x, -A.y);
			else
				sprintf(strng, "\n\nA = %lf + i %lf", A.x, A.y);
//			disp(strng);
			
			if (B.y < 0.0) 
				sprintf(strng, "\nB = %lf - i %lf\n", B.x, -B.y);
			else
				sprintf(strng, "\nB = %lf + i %lf\n", B.x, B.y);
//			disp(strng);
		}

}

/* =======================HilightNextRay ====================================*/
void HilightNextRay()
/* highlight the next ray in the list - if -1 then we are just starting */
{
	int old_crit_pt_number, new_crit_pt_number, old_ray, new_ray, next_ray;
	
	if (sp.Num_Crit_Pts == 1) {
		old_crit_pt_number = new_crit_pt_number = 0;
		old_ray = HilightedRay;
		if (HilightedRay+1 >= sp.length[0]) 
			new_ray = next_ray = sp.pivot[0];
		else
			new_ray = next_ray = HilightedRay + 1;
	} else {
		if (HilightedRay < sp.length[0]) {
			old_crit_pt_number = 0;
			old_ray = HilightedRay;
			if (HilightedRay+1 >= sp.length[0]) {
				new_crit_pt_number = 1;
				new_ray = 0;
			} else {
				new_crit_pt_number = 0;
				new_ray = HilightedRay+1;
			}
			next_ray = HilightedRay+1;
		} else {
			old_crit_pt_number = 1;
			old_ray = HilightedRay - sp.length[0];
			if (HilightedRay+1 >= (sp.length[0]+sp.length[1])) {
				new_crit_pt_number = 0;
				next_ray = new_ray = 0;
			} else {
				new_crit_pt_number = 1;
				next_ray =  HilightedRay+1;
				new_ray = HilightedRay - sp.length[0]+1;
			}
		}			
	}		
		
	SetPort(Actual_Window);
	PenMode(patXor);
	PenSize(3,3);
	if (HilightedRay != -1) {
		if (old_ray == 0) 
			draw_ray(sp.rays[old_crit_pt_number][1], XCen, YCen, scale);
		
		draw_ray(sp.rays[old_crit_pt_number][old_ray], XCen, YCen, scale);
	}
	if (new_ray == 0) 
		draw_ray(sp.rays[new_crit_pt_number][1], XCen, YCen, scale);
	
	draw_ray(sp.rays[new_crit_pt_number][new_ray], XCen, YCen, scale);
	
	if (sp.Second_Hits_First) {
		if (sp.pivot[1] == new_ray && new_crit_pt_number == 0)
			disp("Pivot1=");
		if (sp.pivot[0] == new_ray && new_crit_pt_number == 0) 
			disp("Pivot0=");
	} else if (new_ray==sp.pivot[new_crit_pt_number]) disp("Pivot=");
	sprintf(strng, "%ld/%ld\n",ReducedNum(sp.num[new_crit_pt_number][new_ray], sp.den[new_crit_pt_number]),ReducedDen(sp.num[new_crit_pt_number][new_ray], sp.den[new_crit_pt_number]));
	disp(strng);
	
	HilightedRay = next_ray;
	PenNormal();
}

DoContent(theWindow, theEvent)
WindowPtr	theWindow;
EventRecord	*theEvent;
{
	int				cntlCode;
	ControlHandle 	theControl,theUpControl;
	int				pageSize,i;
	GrafPtr			savePort;
	Point			pt;
	Rect			rect;
	
	GetPort(&savePort);
	SetPort(theWindow);
	GlobalToLocal( &theEvent->where );
	cntlCode = FindControl(theEvent->where, theWindow, &theControl);
	
	if (theWindow == Goingson_Window) {
		TEActivate( TEH );
		if (cntlCode == 0) {
			if (PtInRect( theEvent->where, &(**TEH).viewRect ))
				TEClick( theEvent->where, (theEvent->modifiers & shiftKey )!=0, TEH);
		}
		else if (cntlCode == inThumb) {
			TrackControl(theControl, theEvent->where, 0L);
			AdjustText();
		}
		else
			TrackControl(theControl, theEvent->where, &ScrollProc);
	
	} else if (theWindow == Cspace_Window) {
		if (d == 2) {
			C.x = 2.5*(double)theEvent->where.h/256.0-2.0;
			C.y = 2.5*(double)(256 - theEvent->where.v)/256.0 - 1.25;
		} else if (d == 3 || d == 4) {
			C.x = 3.15*(double)theEvent->where.h/256.0-1.644;
			C.y = 3.15*(double)(256 - theEvent->where.v)/256.0 - 1.58;
		}
		if (C.y < 0.0) 
			sprintf(strng, "\nC = %lf - i %lf", C.x, -C.y);
		else
			sprintf(strng, "\nC = %lf + i %lf", C.x, C.y);
		disp(strng);

	} else if (theWindow == Ghost_Window) {
				if (cntlCode != 0) {
						HiliteControl(theControl, 1);
						/* draw help line */ 
							SetPort(Ghost_Window);
							MoveTo(10,40);
							if (theControl == but[NEW2_BUT]) DrawString("\pCreate a new spider of any degree with one critical point.");
							if (theControl == but[NEW3_BUT]) DrawString("\pCreate a new cubic spider (with two critical points).");
							if (theControl == but[MATE_BUT]) DrawString("\pDefine a mating.");
							if (theControl == but[LIFT_BUT]) DrawString("\pLift the currently defined spider.");
							if (theControl == but[LIFTN_BUT]) DrawString("\pLift currently defined spider N times (define N using param button).");
							if (theControl == but[PARAM_BUT]) DrawString("\pSet various program parameters.");
							if (theControl == but[JULIA_BUT]) DrawString("\pDraw Julia set of z^d+C or z^3+Az+B. (Click mouse to stop)");
							if (theControl == but[XSPID_BUT]) DrawString("\pDefine an extra spider.");
							if (theControl == but[XLIFT_BUT]) DrawString("\pLift the extra spider.");
							if (theControl == but[MARK_BUT]) DrawString("\pHighlight successive rays (use Julia button to reset).");
							if (theControl == but[EXIT_BUT]) DrawString("\pQuit the program.");
						while (Button());
						HiliteControl(theControl, 0);
						SetRect(&rect,10,30, 450, 50);
						EraseRect(&rect);
						GetMouse(&pt);
						FindControl(pt, theWindow, &theUpControl);
						if (theUpControl == theControl) {
							for (i=0; i<3; ++i) {
								long dummy;
								Delay(3,&dummy);
								HiliteControl(theControl, 1);
								Delay(3,&dummy);
								HiliteControl(theControl, 0);
							}
							
/**********************************************************************/
/**********************************************************************/
						/* list of possible controls and actions here */
							if (theControl == but[EXIT_BUT]) ExitToShell();
							
							if (theControl == but[MARK_BUT]) HilightNextRay();
							
							if (theControl == but[XLIFT_BUT]) {
								SetCursor(&waitCursor);  
								lift_X_spider(&sp1);
								SetPort(Actual_Window);
								EraseRect(&Actual_Window->portRect);
								draw_spider(sp,XCen,YCen,scale);
								PenPat(**greypat);
								for (i=0; i<sp1.length[0]; ++i) 
									draw_ray(sp1.rays[0][i], XCen, YCen, scale);
								PenPat(**blackpat);
 								sprintf(strng,"\nEnd of ray with angle %ld/%ld is (%lf,%lf)", xnum, xden, sp1.rays[0][1]->ray->node.x, sp1.rays[0][1]->ray->node.y);
 								disp(strng);
 								HilightedRay = -1;
 								InitCursor(); 								
							}
							
							if (theControl == but[XSPID_BUT]) /* extra spider init */
								if (Set_X_Angles()) {
								
									sprintf(strng, "\nangles are:\n(%ld/%ld) with deg %d", xnum,xden, sp.deg); 
									disp(strng);
									free_spider(&sp1);
									sp1.Num_Crit_Pts = 1;
									sp1.deg = sp.deg;
									spider_init(&sp1, (long)sp.deg, xnum, xnum, xden, (long)0, (long)0, (long)1);
									HiliteControl (but[XLIFT_BUT],0);
								}
							
							if (theControl == but[JULIA_BUT]) {
								HilightedRay = -1;
								SetPort(Actual_Window);
								if (sp.Num_Crit_Pts == 1) 
									DoJulia();
								else
									DoJulia1();
							}
							
							if (theControl == but[PARAM_BUT]) SetParams();
							
							if (theControl == but[LIFTN_BUT]) {
								EventRecord 	event;
								int 			temp;
								
								SetCursor(&waitCursor);  
								disp("\n");
								for (i=0; i<Nlifts; ++i ) {
										lift_spider(&sp);
										disp(".");
										setABC();
										
										temp = GetNextEvent(everyEvent, &event);
										if (event.what == mouseDown) break;
									}

 								dispABC();
								SetPort(Actual_Window);
								EraseRect(&Actual_Window->portRect);
								draw_spider(sp,XCen,YCen,scale);
 								InitCursor(); 								
								HilightedRay = -1;
							}
							

							if (theControl == but[LIFT_BUT]) {
								SetCursor(&waitCursor);  
								lift_spider(&sp);
								setABC();
								dispABC();
								SetPort(Actual_Window);
								EraseRect(&Actual_Window->portRect);
								draw_spider(sp,XCen,YCen,scale);
 								InitCursor(); 								
								HilightedRay = -1;
							}

							
							if (theControl == but[MATE_BUT]) 
								if (Set_Mate_Angles()) {
									HideWindow(Cspace_Window);
/*									free_mate(&mate);*/
									d = 0; /* this indicates matemode */
/*									mate_init(&mate, num11, den1, num22, den2);*/
									SetPort(Actual_Window);
/*									set_mate_ABC();*/
									EraseRect(&Actual_Window->portRect);
/*									draw_mate(sp,XCen,YCen,scale);*/
									HiliteControl (but[XLIFT_BUT],255);
									HilightedRay = -1;
								}
							 
							if (theControl == but[NEW2_BUT]) 
								if (Set_Quad_Angles()) {
									
									if (d == 2) 
										SetWindowPic(Cspace_Window, QuadraticLocus);
									else if (d == 3)
										SetWindowPic(Cspace_Window, CubicLocus);
									else if (d == 4)
										SetWindowPic(Cspace_Window, QuarticLocus);
									else 
										HideWindow(Cspace_Window);
					
									if (d==2 || d==3 || d==4) {
										ShowWindow(Cspace_Window);
										SetPort(Cspace_Window);
										EraseRect(&Cspace_Window->portRect);
										InvalRect(&Cspace_Window->portRect);
									}
													
									free_spider(&sp);
									sp.Num_Crit_Pts = 1;
									sp.deg = d;
									spider_init(&sp, d, num, num+den, d*den, (long)0, (long)0, (long)1);
									SetPort(Actual_Window);
									setABC();
									EraseRect(&Actual_Window->portRect);
									draw_spider(sp,XCen,YCen,scale);
									/*setup quad rays here */
									HiliteControl (but[XLIFT_BUT],255);
									HilightedRay = -1;
								}
							
							if (theControl == but[NEW3_BUT]) 
								if (Set_Cube_Angles())  {
									HideWindow(Cspace_Window);
									free_spider(&sp);
									sp.Num_Crit_Pts = 2;
									sp.deg = d = 3;
									sprintf(strng, "\nangles are:\n(%ld/%ld, %ld/%ld)\n(%ld/%ld, %ld/%ld) ", num11,den1, num12, den1, num21, den2 , num22, den2); 
									disp(strng);
									spider_init(&sp, d, num11, num12, den1, num21, num22, den2);
									SetPort(Actual_Window);
									setABC();
									EraseRect(&Actual_Window->portRect);
									draw_spider(sp,XCen,YCen,scale);
									HiliteControl (but[XLIFT_BUT],255);
									HilightedRay = -1;
								}
							
/**********************************************************************/
/**********************************************************************/
						}
				}
	}


	SetPort(savePort);
}


void MyGrowWindow( w, p )
//WindowPtr w;
//Point p;
{
//	GrafPtr	savePort;
//	long	theResult;
//	int		oScroll;
//	Rect 	r, oView;
//	
//	GetPort( &savePort );
//	SetPort( w );
//
//	SetRect(&r, 80, 80, screenBits.bounds.right, screenBits.bounds.bottom);
//	theResult = GrowWindow( w, p, &r );
//	if (theResult == 0)
//	  return;
//	SizeWindow( w, LoWord(theResult), HiWord(theResult), 1);
//
//	InvalRect(&w->portRect);
//	oView = (**TEH).viewRect;
//	oScroll = GetCtlValue(vScroll);
//	
//	SetView(w);
//	HidePen();
//	MoveControl(vScroll, w->portRect.right - SBarWidth, w->portRect.top-1);
//	SizeControl(vScroll, SBarWidth+1, w->portRect.bottom - w->portRect.top-(SBarWidth-2));
//	ShowPen();
//
//
//	SetVScroll();
//	AdjustText();
//	
//	SetPort( savePort );
}



CloseMyWindow()
{
	HideWindow( Actual_Window );
	TESetSelect( 0, (**TEH).teLength, TEH );
	TEDelete( TEH );
	SetVScroll();
	/*SetUpFiles();*/
}
