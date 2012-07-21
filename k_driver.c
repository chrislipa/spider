//#include <xview/xview.h>
//#include <xview/panel.h>
//#include <xview/canvas.h>
//#include <xview/cms.h>
//#include <xview/cursor.h>
#include "koebe.h"

#define SWAPI(A,B) { int temp; temp = A; A = B; B = temp; }
//
//Frame 				M_frame;
//Canvas 				M_canvas;
//static Panel 		panel;
//static Pixwin		*pw;
//static Cms          cms;
//struct rect 		r;
//
//static Panel_item 	iterations_item,
//					where_item, 
//					ps_spool_item, 
//					color_item,
//					type_item;
//
//Menu				menu;

int fcolor = 0;
int fmode = PIX_SRC;
int cx,cy;
static int internal_state = 0;
static FILE *output;


//extern Frame frame;
extern gptr gp;
//extern Cursor cursor, wait_cursor;
extern complex C;
extern int left, top, right, width, iterations, color_monitor; 
static char red[128],green[128],blue[128];

void menu_proc(),
	go(), 
	do_Mcanvas_mouse_event(), 
	ps_spool_proc(), 
	color_proc(), 	
	line_to(),
	ps_line_to(),
	(*line_to_routine)();



void terminate_graphics()
{
//xv_destroy_safe(M_frame);
}

void compute_it()
{
	char s[100];
	int i;

//	xv_set(canvas_paint_window(M_canvas),WIN_CURSOR,wait_cursor, 0);
//	resize(left,top,right);
	left = top = 0; right = width-1;
//	gp->pict.c.x = C.x; /* in case we are drawing julia sets */
// 	gp->pict.c.y = C.y;

	sprintf(s, "(%lf,%lf) %lf", 
		gp->pict.center.x, gp->pict.center.y, gp->pict.radius);

//	xv_set(where_item, PANEL_VALUE, s, 0);
	internal_state = 0;
//	pw_rop(pw,0,0,width,width,PIX_SRC | PIX_COLOR(0), NULL,0,0);
//	iterations = (int)xv_get(iterations_item,PANEL_VALUE );

	gp->pict.iterations = iterations;
	/* for (i = 1; i<=gp->pict.iterations; ++i) */
		/* gp->icnt2color[i] = (i*iterations)/(127-1); */
	gp->icnt2color[0] = 127;
	gp->icnt2color[gp->pict.iterations+1] = 0; /* new was 127 here */

	while (1) {
		for (i = 0; i<50; ++i)
			if (!gp->pict.done)
				step_kqt(gp);
			else {
//				xv_set(canvas_paint_window(M_canvas),WIN_CURSOR,cursor, 0);
				return;
			}
	}
}

void
init_graphics(left,top)
	int left,top;
{
	int i;
	char s[400];
	static char cms_name[10] = "kqt";
//    Display *dpy;
//	Xv_singlecolor colors[256];


//	M_frame = xv_create(frame,FRAME,
//		XV_LABEL,"Koebe Quarter theorem",
//		WIN_X,left,
//		WIN_Y,top,
//		XV_WIDTH,width,
//		0);

/*
    dpy = (Display *)xv_get(M_frame, XV_DISPLAY);
    if (DefaultDepth(dpy, DefaultScreen(dpy)) < 2)
        color_monitor = 0;
    else
        color_monitor = 1;

*/
//    if (color_monitor) {
//    	cms = (Cms)xv_create(NULL, CMS, CMS_SIZE, 128, 
//								CMS_TYPE, XV_DYNAMIC_CMS, 0);
//    	xv_set(M_frame, WIN_CMS, cms,0);
//    }
//
//
//	panel = xv_create(M_frame,PANEL, 0);
//
//	xv_create(panel,PANEL_BUTTON,
//		PANEL_LABEL_STRING, "Compute", 
//		PANEL_NOTIFY_PROC,compute_it,
//		XV_HELP_DATA, "spider:M_compute",
//		0);
//
//	color_item = (Panel_item)xv_create(panel,PANEL_BUTTON,
//		PANEL_LABEL_STRING, "Color", 
//		PANEL_NOTIFY_PROC,color_proc,
//		XV_HELP_DATA, "spider:M_color",
//		PANEL_INACTIVE, TRUE,
//		0);
//
//	menu = (Menu)xv_create(NULL, MENU, MENU_NOTIFY_PROC, menu_proc,
//				MENU_STRINGS, "Parameter Space", "Dynamical Space", NULL, 
//				0);
//				
//	type_item= (Panel_item)xv_create(panel,PANEL_BUTTON,
//		PANEL_LABEL_STRING, "Space", 
//		PANEL_ITEM_MENU, menu, 
//		XV_HELP_DATA, "spider:M_type",
//		/* PANEL_INACTIVE, TRUE, */
//		0);
//
//	ps_spool_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
//		PANEL_CHOICE_STRINGS,"PostScript Spool",0,
//		PANEL_NOTIFY_PROC,ps_spool_proc,
//		XV_HELP_DATA, "spider:M_ps_spool",
//		0);
//
///*
//	xv_create(panel,PANEL_BUTTON,
//		PANEL_LABEL_STRING, "Quit", 
//		PANEL_NOTIFY_PROC,terminate_graphics,
//		0);
//*/
//
//	sprintf(s,"(Center X, Center Y) Radius");
//	where_item=xv_create(panel,PANEL_TEXT,
//		PANEL_LABEL_STRING, "Window:", 
//		PANEL_VALUE, s,
//		XV_HELP_DATA, "spider:M_where",
//		0);
//
//
//	iterations_item = xv_create(panel,PANEL_NUMERIC_TEXT,
//		PANEL_LABEL_STRING,"Iterations:",
//		PANEL_MIN_VALUE, 10, PANEL_MAX_VALUE, MAXITERATIONS ,
//		PANEL_VALUE,iterations,
//		XV_HELP_DATA, "spider:M_iterations",
//		0);
//	window_fit_height(panel);
//	/* window_fit_width(M_frame); */
//
//	M_canvas = (Canvas)xv_create(M_frame,CANVAS,
//		XV_HELP_DATA, "spider:M_canvas",
//		XV_WIDTH,width,
//		XV_HEIGHT,width,
//		WIN_BELOW,panel,
//		0);
//
//   xv_set(canvas_paint_window(M_canvas),WIN_CONSUME_EVENTS,
//        WIN_NO_EVENTS,/*WIN_DYNAMIC_VISUAL,*/
//        WIN_MOUSE_BUTTONS,LOC_DRAG,LOC_MOVE,0,
//        WIN_EVENT_PROC,do_Mcanvas_mouse_event,
//		XV_HELP_DATA, "spider:M_canvas",
//        0);
//
//	window_fit(M_frame);
//	
//	pw = canvas_pixwin(M_canvas);
//	r.r_left = r.r_top = 0;
//	r.r_width = r.r_height = width;
//
//	pw_setcmsname(pw,cms_name);
//	
//	/* set up a color map */
//	for (i = 0; i<127; ++i) {
//		colors[i].red = i<<8;
//		colors[i].green = i<<4;
//		colors[i].blue = i<<5;
//	}
///*
//	for (i = 16; i<127; ++i) {
//		colors[i].red = 0;
//		colors[i].green = 255;
//		colors[i].blue = 0;
//	}
//*/
//
//	colors[127].red = colors[127].green = colors[127].blue = 0;
//	colors[0].red = colors[0].green = colors[0].blue = 255;
//
//	if (color_monitor)
//            xv_set(cms, CMS_COLORS, colors,0);
//
//	line_to_routine = line_to;
//
//	/* window_main_loop(M_frame); */ /* need for the stand alone version*/
}



set_fmode(mode)
	int mode;
{
	fmode= mode;
}

set_forecolor(color)
	int color;
{
	fcolor = color;
}

move_to(x,y)
	int x,y;
{
	cx = x;
	cy = y;
}

void ps_line_to(x,y)
	int x,y;
{
//	pw_vector(pw,cx,cy,x,y,fmode,fcolor);
	if (fcolor == BLACK) {
		if (cx == x && cy == y) ++cx;
    	fprintf(output,"%d %d %d %d ln\n ",
        	cx,(width-cy),x,(width-y));
	}

	cx = x;
	cy = y;
}

void line_to(x,y)
	int x,y;
{
//	pw_vector(pw,cx,cy,x,y,fmode,fcolor);
	cx = x;
	cy = y;
}

static void
normalize_box(left,top,right,bottom)
	int *left,*top,*right,*bottom;
{
	/* this is not optimal.. there should be 4 conditions oh well */
	if (abs(*right - *left) > abs(*top - *bottom))
		*bottom = *top + (*right-*left);
	else
		*right = *left + (*bottom - *top);
}


static void
draw_box(left,top,right,bottom,c)
{
//	pw_vector(pw,left,top,right,top,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,right,top,right,bottom,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,right,bottom,left,bottom,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,left,bottom,left,top,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,left,top,left,top,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,left,bottom,left,bottom,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,right,top,right,top,PIX_NOT(PIX_DST),0xff);
//	pw_vector(pw,right,bottom,right,bottom,PIX_NOT(PIX_DST),0xff);
}



void
do_Mcanvas_mouse_event(void* c, void* e)
//	Xv_Window c;
//	Event *e;
{
	static int t_left,t_top,t_right,t_bottom;

//	if (event_id(e)==MS_RIGHT && event_is_down(e)) {
//		 C.x =  (double)xp2v(gp,event_x(e));
//		 C.y =  (double)yp2v(gp,event_y(e));
//		 dispABC();
//	} else 
//	if (event_id(e)==MS_LEFT && event_is_down(e)) {
//		if (internal_state > 0) 
//			draw_box(t_left,t_top,t_right,t_bottom,1);
//		t_right = t_left = event_x(e);
//		t_bottom = t_top = event_y(e);
//		draw_box(t_left,t_top,t_right,t_bottom,1);
//		internal_state = 1;
//	} else 
//	  if (event_id(e)==MS_MIDDLE && event_is_down(e) && internal_state==1) {
//		draw_box(t_left,t_top,t_right,t_bottom,1);
//		t_right =  event_x(e);
//		t_bottom = event_y(e);
//		/* if (t_left>t_right) SWAPI(t_left,t_right); */
//		/* if (t_top>t_bottom) SWAPI(t_top,t_bottom); */
//		normalize_box(&t_left,&t_top,&t_right,&t_bottom);
//		draw_box(t_left,t_top,t_right,t_bottom,1);
//		internal_state = 2;
//		top = t_top; left = t_left; right = t_right;
//	} else if (event_id(e) == LOC_MOVE && internal_state == 1) {
//			draw_box(t_left,t_top,t_right,t_bottom,1);
//			t_right =  event_x(e);
//			t_bottom = event_y(e);
//			/* if (t_left>t_right) SWAPI(t_left,t_right); */
//			/* if (t_top>t_bottom) SWAPI(t_top,t_bottom); */
//			normalize_box(&t_left,&t_top,&t_right,&t_bottom);
//			draw_box(t_left,t_top,t_right,t_bottom,1);
//	}
}


/****************************************************************/
void ps_spool_proc()
{
	if ((int)xv_get(ps_spool_item,  PANEL_TOGGLE_VALUE,0)) {
		output = fopen("spider1.ps", "w");
		line_to_routine = ps_line_to;
    	fprintf(output,"%%!");
    	fprintf(output,"\n%%BoundingBox: %d %d %d %d scale\n", 0,
			width, 0,width); /* is this right ? */
    	fprintf(output,"\n/ln { %% stack x y z w=> -- ");
    	fprintf(output,"\n       newpath");
    	fprintf(output,"\n       moveto");
    	fprintf(output,"\n       lineto");
    	fprintf(output,"\n       stroke } def");
    	fprintf(output,"\n %lf %lf  scale\n", 1.0,1.0);
	} else {
		line_to_routine = line_to;
    	fprintf(output,"\nshowpage");
		fclose(output);	
	}
}


void color_proc()
{
}

void menu_proc(/*menu, menu_item*/)
//Menu menu;
//Menu_item menu_item;
{
	int i,j;
	unsigned char *c;
	char *s;	

	pw_rop(pw,0,0,width,width,PIX_SRC | PIX_COLOR(0), NULL,0,0);

	s = (char *)xv_get(menu_item, MENU_STRING);
	if (s[0] == 'P') {
		gp->pict.mode = KQT_M;
		gp->pict.center.x = -0.7;
		gp->pict.center.y = 0.0;
		gp->pict.radius = 1.4;
	} else {
		gp->pict.mode = KQT_J;
		gp->pict.center.x = 0.0;
		gp->pict.center.y = 0.0;
		gp->pict.radius = 2;
	}

	xv_set(menu, MENU_DEFAULT_ITEM, menu_item,0);

	top = left = 0; right = width-1; 
	gp->pict.done = 0;
	gp->pict.width = width;
	gp->pict.iterations = iterations = 
			(int)xv_get(iterations_item,PANEL_VALUE );
	gp->pict.escape = 20.0;
	gp->pict.c.x = C.x;
	gp->pict.c.y = C.y;
	gp->pict.delta = 0.15;
	gp->pict.recur = 0.1;
	gp->pict.i = -1;
	gp->pict.j = 0;
	for (i = 0; i <= 1; ++i) {
		gp->pict.stack[i][0].maxradius = 2;
		gp->pict.stack[i][1].maxradius = 10;
		for (j = 0; j < NSTACKS; ++j) {
			gp->pict.stack[i][j].maxsize = EXPANSION;
			gp->pict.stack[i][j].size = 0;
		}
	}


	c = gp->icnt;
	for (i = gp->pict.width*gp->pict.width-1; i>=0; --i)
			*c++ = 255;

}
