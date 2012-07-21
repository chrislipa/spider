#include "spider.h"
#include "misc.h"
#define K_INCLUDE 0

#define HELPPATHNAME "."


/* XView stuff */
//Frame 			frame, 
//				help_frame, 
//				goingson_frame, 
//				xspid_frame, 
//				param_frame, 
//				new2_frame,
//				new3_frame,
//				mate_frame;
//
//
//#if !K_INCLUDE
//extern Frame	M_frame; 
//extern Canvas   M_canvas;
//#endif
//
//Panel 			panel, 
//				xspid_panel, 
//				param_panel,
//				new2_panel,
//				new3_panel,
//				mate_panel;
//
//Canvas 			canvas,
//				new3_canvas;
//
//Pixwin 			*pw,
//				*new3_pw;
//
//Cursor 			cursor,
//				wait_cursor;
//
//Cms 			cms;
//Icon 			icon;
//Server_image 	new_image;

//Panel_item new2_item, new3_item,  mate_item, param_item, xspid_item, 
//			xspid_lift_item, file_item, n_item,coor_item,map_item,message_item,
//			goingson_item, help_item, setting_color, toggle_item, show_trans, 
//			xspid_num, xspid_den, param_wepsilon, param_fepsilon, param_furad , 
//			param_Nlifts, param_scale , param_XCen , param_YCen, param_depth,
//			param_ticks,new2_potential, new2_repeating , 
//			new2_leading , new2_den, new2_num, new2_deg, new2_sector,
//			new3_potential, new3_den1, new3_num1, new3_den2, new3_num2,
//			new3_set_item, new3_other_angle1, new3_other_angle2, ps_item, 
//			k_item;
//
//Textsw 		textsw,
//			help_textsw;

/* Program globals */
FILE			*output; 

int				ps_spooling = 0;
int 			color_monitor;
int				XCen = 350, YCen=400;
int			 	scale= 180,
				Nlifts = 10, 
				showticks = 0;
extern int				HilightedRay = -1,
				new3_button_invalid_flag = 0;

complex 		A = {0.0,  0.0},
				B = {0.0,  0.0},
				C = {-1.0, 0.0}, 		
				Z = {0.0, 0.0};

double			fepsilon = 0.00, 
				wepsilon=0.001, 
				furad = 20.0,
				julia_depth = 2000;

long 			xnum = 1, 
				num = 1, 
				xden = 6, 
				den = 6,
				num22 = 1,
				den22= 2,
				num21 = 1,
				den21= 6,
				num12 = 1,
				den12= 2,
				num11 = 1,
				den11= 6,
				d = 2,
				Leading_Part[Num_Digits],
				Repeating_Part[Num_Digits];

struct spider   sp, sp1;

long 			gcd();
char 			sec_str[500];


void goingson_proc(), help_proc(),quit_proc(), julia_proc(), new2_proc(),
		mate_proc(), new3_proc() , param_proc() , xspid_proc() , lift_proc() ,
		 liftN_proc() , xlift_proc(), scale_proc(), ps_proc(),
		xspid_set_angles(), param_accept_proc(), param_cancel_proc(),
		new2_set_rb(), new2_set_angles(), new3_set_angles(), HilightNextRay(), 
		new3_set_invalid(), new3_toggle_angle1(), new3_toggle_angle2(),
		k_proc(), clear_proc(), Z_reset_proc(), Z_iterate_proc();

void mouse_e_proc();

main(argc,argv)
int argc; char **argv;
{
	int i;
	char *helppath, buf[BUFSIZ];
	extern char *getenv();

	/* various XView initializations */
	xv_init(XV_INIT_ARGC_PTR_ARGV,&argc, argv, NULL);

	/* set the path for the help file */
	sprintf(buf, "HELPPATH=/usr/lib/help:%s:", HELPPATHNAME);
	if (helppath = getenv("HELPPATH"))
		strcat(buf, helppath);
	putenv(buf);

	create_frames();
	create_panels();

	new_image = (Server_image)xv_create(NULL, SERVER_IMAGE, 
		XV_HEIGHT, 64, XV_WIDTH, 64,
		SERVER_IMAGE_BITS, the_icon_image, NULL);
					
	icon = (Icon)xv_get(frame, FRAME_ICON);
	xv_set(icon, ICON_IMAGE, new_image, NULL);

	create_panel_items();

	textsw = (Textsw)xv_create(goingson_frame, TEXTSW,
		TEXTSW_IGNORE_LIMIT, TEXTSW_INFINITY, TRUE,0);
	

	create_canvas_items();



	/*  * * * * * * * * * *various program initializations * * * * * * * * */
	disp("Spider:\n Written by Yuval Fisher. \n\
Questions, comments:\n  yfisher@ucsd.edu\n\n");

	sp.Num_Crit_Pts = 1;
	sp.deg = d; 
	spider_init(&sp1, d, xnum, xnum, xden, (long)0, (long)0, (long)1); 
	spider_init(&sp, d, num, num+den, 2*den, 0, 0, 1); 
	setABC();

	window_main_loop(frame);

	exit(0);
}

/* **************************create_panels********************************** */
create_panels()
{

	panel = (Panel)xv_create(frame, PANEL, 0);

	xspid_panel = (Panel)xv_create(xspid_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);

	param_panel = (Panel)xv_create(param_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);

	new2_panel = (Panel)xv_create(new2_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);

	new3_panel = (Panel)xv_create(new3_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);

	mate_panel = (Panel)xv_create(mate_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);
}

/* **************************create_frames********************************** */
create_frames()
{

	Display *dpy;

	frame = (Frame)xv_create(NULL, FRAME, 
		XV_LABEL, "Spiders", 
		WIN_X,0,WIN_Y,0,XV_HEIGHT,900,XV_WIDTH,700,0);

	dpy = (Display *)xv_get(frame, XV_DISPLAY);
	if (DefaultDepth(dpy, DefaultScreen(dpy)) < 2)
		color_monitor = 0;
	else 
		color_monitor = 1;

	/* for now, lets leave it b/w - maybe add color later */
	color_monitor = 0;
	if (color_monitor) {
	cms = (Cms)xv_create(NULL, CMS, CMS_SIZE, 256, CMS_TYPE, XV_DYNAMIC_CMS, 0);
	xv_set(frame, WIN_CMS, cms,0);
	}

	goingson_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 300, XV_HEIGHT, 300,
						FRAME_LABEL, "Goings on", XV_SHOW, TRUE, 0);

	help_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 700, XV_HEIGHT, 500,
						FRAME_LABEL, "Help...", XV_SHOW, FALSE, 0);

	xspid_frame = (Frame) xv_create(frame, FRAME, 
					XV_WIDTH, 330, XV_HEIGHT, 150,
					FRAME_LABEL, "Set Extra Spider Angle", XV_SHOW, FALSE, 0);


	param_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 375, XV_HEIGHT, 450,
						FRAME_LABEL, "Set Parameters", XV_SHOW, FALSE, 0);

	new2_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 475, XV_HEIGHT, 350,
						FRAME_LABEL, "New 2", XV_SHOW, FALSE, 0);

	new3_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 395, XV_HEIGHT, 550,
						FRAME_LABEL, "New 3", XV_SHOW, FALSE, 0);

	mate_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 395, XV_HEIGHT, 550,
						FRAME_LABEL, "Matings - Not Yet Implemented", 
						XV_SHOW, FALSE, 0);

	/* create 1/4 theorem stuff, which is stand alone */
#if !K_INCLUDE
	k_main();
#endif
}

create_canvas_items()
{
	Xv_singlecolor fg;

	if (color_monitor) 
		fg.blue = fg.green = fg.red = 255; 
	else 
		fg.blue = fg.green = fg.red = 0; 

	canvas = (Canvas)xv_create(frame, CANVAS, 0);
	pw = canvas_pixwin(canvas);
	
	new3_canvas = (Canvas)xv_create(new3_frame, CANVAS,
			XV_HELP_DATA, "spider:new3_canvas",0);
	new3_pw = canvas_pixwin(new3_canvas);
	

	cursor = xv_create(NULL, CURSOR, CURSOR_SRC_CHAR, OLC_BASIC_PTR,
			CURSOR_FOREGROUND_COLOR,&fg, NULL);

	xv_set(panel, WIN_CURSOR,cursor, 0);

	wait_cursor = xv_create(NULL, CURSOR, CURSOR_SRC_CHAR, OLC_BUSY_PTR,
			CURSOR_FOREGROUND_COLOR,&fg, NULL);

	cursor = xv_create(NULL, CURSOR, CURSOR_SRC_CHAR, OLC_PANNING_PTR,
			CURSOR_FOREGROUND_COLOR,&fg, NULL);


	xv_set(canvas_paint_window(canvas),WIN_CONSUME_EVENTS,
		WIN_NO_EVENTS,/*WIN_DYNAMIC_VISUAL,*/
		WIN_MOUSE_BUTTONS,0,
		WIN_EVENT_PROC, mouse_e_proc, 
		WIN_CURSOR,cursor, 0);

	xv_set(canvas_paint_window(new3_canvas),WIN_CONSUME_EVENTS,
		WIN_NO_EVENTS, WIN_CURSOR,cursor, 0);

}

create_panel_items()
{
	char s[300];

	help_textsw = (Textsw)xv_create(help_frame, TEXTSW,
		TEXTSW_IGNORE_LIMIT, TEXTSW_INFINITY, TRUE,0);
	textsw_insert(help_textsw,help_text,strlen(help_text));

	/* main panel items */
    new2_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"New 2...",0,
			PANEL_NOTIFY_PROC, new2_proc, XV_HELP_DATA, "spider:new2",0);

    new3_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"New 3...",0,
			PANEL_NOTIFY_PROC, new3_proc, XV_HELP_DATA, "spider:new3",0);

    mate_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"Mating...",0,
			PANEL_NOTIFY_PROC, mate_proc, 
			XV_HELP_DATA, "spider:mate",
			/* PANEL_INACTIVE, TRUE, */
			0);

    param_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"Set Parameters...",0,
			PANEL_NOTIFY_PROC, param_proc, XV_HELP_DATA, "spider:parameters",0);

    xspid_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"Extra Spider...",0,
			PANEL_NOTIFY_PROC, xspid_proc, XV_HELP_DATA, "spider:xspid",0);

    xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Mark Orbit",
		PANEL_NOTIFY_PROC, HilightNextRay, XV_HELP_DATA, "spider:mark",0);

    xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Julia Set",
		PANEL_NOTIFY_PROC, julia_proc , XV_HELP_DATA, "spider:julia",0);

    xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Clear",
		PANEL_NOTIFY_PROC, clear_proc ,  XV_HELP_DATA, "spider:lift",0);

    xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"LIFT",
		PANEL_NOTIFY_PROC, lift_proc ,  XV_HELP_DATA, "spider:lift",0);

    xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Many Lifts ",
		PANEL_NOTIFY_PROC, liftN_proc , XV_HELP_DATA, "spider:manylift",0);

    xspid_lift_item = (Panel_item)xv_create(panel,PANEL_BUTTON,
					PANEL_LABEL_STRING, "Lift extra Spider",
					PANEL_NOTIFY_PROC, xlift_proc , XV_HELP_DATA, "spider:xlift",
					PANEL_INACTIVE, TRUE, 
					0);

    goingson_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"Goings On...",0,
			PANEL_NOTIFY_PROC, goingson_proc, 
			XV_HELP_DATA, "spider:goingson",0);

    ps_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"PostScript Spool",0,
			PANEL_NOTIFY_PROC, ps_proc,
			XV_HELP_DATA, "spider:ps", 0);

    help_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"Help",0,
			PANEL_NOTIFY_PROC, help_proc,XV_HELP_DATA, "spider:help", 0);


    k_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"1/4 Theorem",0,
			PANEL_NOTIFY_PROC, k_proc,XV_HELP_DATA, "spider:k_button", 0);

   xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Quit",
		PANEL_NOTIFY_PROC, quit_proc, XV_HELP_DATA, "spider:quit",0);


	/* ********************xspid_panel items *********************/

    xv_create(xspid_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Set Angles",
		PANEL_NOTIFY_PROC, xspid_set_angles , 
		XV_HELP_DATA, "spider:xspid_set_angles",0);

	xspid_num=(Panel_item)xv_create(xspid_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,xnum,
		PANEL_LABEL_STRING,	"Numerator:    ",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:xspid_numerator",
		0);

	xspid_den=(Panel_item)xv_create(xspid_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,xden,
		PANEL_LABEL_STRING,	"Denominator: ",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:xspid_denominator",
		0);

    xv_create(xspid_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Reset Z",
		PANEL_NOTIFY_PROC, Z_reset_proc, 
		XV_HELP_DATA, "spider:xspid_Z_reset",0);
	
    xv_create(xspid_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Iterate Z",
		PANEL_NOTIFY_PROC, Z_iterate_proc, 
		XV_HELP_DATA, "spider:xspid_Z_iterate",0);
	

	/* ********************param_panel items *********************/

    param_scale = (Panel_item)xv_create(param_panel,PANEL_SLIDER,
			PANEL_LABEL_STRING,"Scale:",
			PANEL_VALUE, 200,
			PANEL_MIN_VALUE, 1,
			PANEL_MAX_VALUE, 2000,
			PANEL_TICKS, 100, 
			/* PANEL_NOTIFY_PROC, scale_proc, */
			XV_HELP_DATA, "spider:scale", 0);

	param_XCen=(Panel_item)xv_create(param_panel,PANEL_SLIDER,
		PANEL_VALUE,XCen,
		PANEL_LABEL_STRING,	"X Center:",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 1000,
		XV_HELP_DATA, "spider:param_xcen",
		0);

	param_YCen=(Panel_item)xv_create(param_panel,PANEL_SLIDER,
		PANEL_VALUE,YCen,
		PANEL_LABEL_STRING,	"Y Center:",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 1000,
		XV_HELP_DATA, "spider:param_ycen",
		0);


	param_Nlifts=(Panel_item)xv_create(param_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,Nlifts,
		PANEL_LABEL_STRING,	"N lifts:",
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 100,
		XV_HELP_DATA, "spider:param_nlifts",
		0);

	param_depth=(Panel_item)xv_create(param_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,(int)julia_depth,
		PANEL_LABEL_STRING,	"Julia Depth:",
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 10000000000,
		XV_HELP_DATA, "spider:param_depth",
		0);

	sprintf(s,"%lf",wepsilon);
	param_wepsilon =(Panel_item)xv_create(param_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Weed Distance:",
		XV_HELP_DATA, "spider:param_weps",
		0);


	sprintf(s,"%lf",fepsilon);
	param_fepsilon =(Panel_item)xv_create(param_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Fill Distance:",
		XV_HELP_DATA, "spider:param_feps",
		0);


	sprintf(s,"%lf",furad);
	param_furad =(Panel_item)xv_create(param_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Fill Upper Radius:",
		XV_HELP_DATA, "spider:param_fur",
		0);

	param_ticks = (Panel_item)xv_create(param_panel, PANEL_CHECK_BOX, 
    			PANEL_LABEL_STRING,"Show points on rays:",
				PANEL_VALUE, showticks,
				XV_HELP_DATA, "spider:param_ticks",0);

	/* ********************new2_panel items *********************/

    xv_create(new2_panel,PANEL_BUTTON,PANEL_LABEL_STRING,
		"Set Repeating Expansion",
		PANEL_NOTIFY_PROC, new2_set_rb , 
		XV_HELP_DATA, "spider:new2_set_rb",0);

    xv_create(new2_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Set Angles",
		PANEL_NOTIFY_PROC, new2_set_angles , 
		XV_HELP_DATA, "spider:new2_set_angles",0);

	new2_num=(Panel_item)xv_create(new2_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,num,
		PANEL_LABEL_STRING,	"Numerator:    ",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new2_numerator",
		0);

	new2_den=(Panel_item)xv_create(new2_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,den,
		PANEL_LABEL_STRING,	"Denominator: ",
		PANEL_MIN_VALUE,0, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new2_denominator",
		0);
	

	new2_deg=(Panel_item)xv_create(new2_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,d,
		PANEL_LABEL_STRING,	"Degree: ",
		PANEL_MIN_VALUE,2, PANEL_MAX_VALUE, 1000,
		XV_HELP_DATA, "spider:new2_degree",
		0);
	

	sprintf(s,"0");
	new2_leading =(Panel_item)xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Leading d-ary Expansion:",
		XV_HELP_DATA, "spider:new2_leading",
		0);


	sprintf(s,"01");
	new2_repeating =(Panel_item)xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Repeating d-ary Expansion:",
		XV_HELP_DATA, "spider:new2_repeating",
		0);

	sprintf(s,"Not Yet Implemented = 0.0");
	new2_potential =(Panel_item)xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Potential:",
		XV_HELP_DATA, "spider:new2_potential",
		0);

	sprintf(s,"No Initial Sequence");
	new2_sector=(Panel_item)xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Sector Sequence:",
		XV_HELP_DATA, "spider:new2_sector",
		0);


		sprintf(s,"Cut and Paste anything into here");
		xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Working Register",
		XV_HELP_DATA, "spider:new2_register",
		0);

		sprintf(s,"Cut and Paste anything into here");
		xv_create(new2_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Working Register",
		XV_HELP_DATA, "spider:new2_register",
		0);

	/* ********************new3_panel items *********************/

    new3_set_item = (Panel_item)xv_create(new3_panel,PANEL_BUTTON,
		PANEL_LABEL_STRING,"Set Angles",
		PANEL_INACTIVE, FALSE,
		PANEL_NOTIFY_PROC, new3_set_angles , 
		XV_HELP_DATA, "spider:new3_set_angles",0);

	new3_num1 =(Panel_item)xv_create(new3_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,num11,
		PANEL_LABEL_STRING,	"Numerator 1:    ",
		PANEL_NOTIFY_PROC, new3_set_invalid, 
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new3_numerator1",
		0);

	new3_den1 =(Panel_item)xv_create(new3_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,den11,
		PANEL_LABEL_STRING,	"Denominator 1: ",
		PANEL_NOTIFY_PROC, new3_set_invalid, 
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new3_denominator1",
		0);


	sprintf(s,"Other critical angle = %d/%d", num12, den12);
	new3_other_angle1 =(Panel_item)xv_create(new3_panel,PANEL_MESSAGE,
		PANEL_LABEL_STRING,	s, 
		XV_HELP_DATA, "spider:new3_other_angle1",
		0);


    xv_create(new3_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Set 1/3 or 2/3",
		PANEL_NOTIFY_PROC, new3_toggle_angle1, 
		XV_HELP_DATA, "spider:new3_toggle_angle",0);

	new3_num2 =(Panel_item)xv_create(new3_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,num,
		PANEL_LABEL_STRING,	"Numerator 2:    ",
		PANEL_NOTIFY_PROC, new3_set_invalid, 
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new3_numerator2",
		0);

	new3_den2 =(Panel_item)xv_create(new3_panel,PANEL_NUMERIC_TEXT,
		PANEL_VALUE,den,
		PANEL_LABEL_STRING,	"Denominator 2: ",
		PANEL_NOTIFY_PROC, new3_set_invalid, 
		PANEL_MIN_VALUE,1, PANEL_MAX_VALUE, 6666666,
		XV_HELP_DATA, "spider:new3_denominator2",
		0);
	

	sprintf(s,"Other critical angle = %d/%d", num22, den22);
	new3_other_angle2 =(Panel_item)xv_create(new3_panel,PANEL_MESSAGE,
		PANEL_LABEL_STRING,	s, 
		XV_HELP_DATA, "spider:new3_other_angle2",
		0);


    xv_create(new3_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Set 1/3 or 2/3",
		PANEL_NOTIFY_PROC, new3_toggle_angle2, 
		XV_HELP_DATA, "spider:new3_toggle_angle",0);

	sprintf(s,"Not Yet Implemented (If Ever) = 0.0");
	new3_potential =(Panel_item)xv_create(new3_panel,PANEL_TEXT,
		PANEL_VALUE,s,
		PANEL_LABEL_STRING,	"Potential:",
		XV_HELP_DATA, "spider:new3_potential",
		0);

/*
    xv_create(param_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Cancel",
		PANEL_NOTIFY_PROC, param_cancel_proc,
		XV_HELP_DATA, "spider:param_cancel",0);

    xv_create(param_panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Accept",
		PANEL_NOTIFY_PROC, param_accept_proc,
		XV_HELP_DATA, "spider:param_accept",0);
*/
	window_fit_height(panel);
	window_fit_height(new3_panel);
}

void 
mouse_e_proc(w,e)
Xv_Window w;
	Event *e;
{}


/****************************************************************/
void help_proc()
{

	if ((int)xv_get(help_item,  PANEL_VALUE)) 
			xv_set(help_frame, XV_SHOW, TRUE, NULL);
	else
			xv_set(help_frame, XV_SHOW, FALSE, NULL);
	
}

/****************************************************************/
void goingson_proc()
{

	if ((int)xv_get(goingson_item,  PANEL_VALUE)) 
			xv_set(goingson_frame, XV_SHOW, TRUE, NULL);
	else
			xv_set(goingson_frame, XV_SHOW, FALSE, NULL);
	
}


/****************************************************************/
void julia_proc()
{
	param_accept_proc(); /* read parameters */
	param_cancel_proc(); /* write them back (eliminates string problms */

	HilightedRay = -1;
	if (sp.Num_Crit_Pts == 1)
		DoJulia();
	else
		DoJulia1();
	
}

/****************************************************************/
 void mate_proc()
{
	if ((int)xv_get(mate_item,  PANEL_VALUE))  {
		xv_set(mate_frame, XV_SHOW, TRUE, 0);
		xv_set(new3_frame, XV_SHOW, FALSE, 0);
		xv_set(new3_item,  PANEL_VALUE,0,0);
		xv_set(new2_frame, XV_SHOW, FALSE, 0);
		xv_set(new2_item,  PANEL_VALUE,0,0);
#if !K_INCLUDE
		xv_set(M_frame, XV_SHOW, FALSE, 0);
#endif
		xv_set(k_item,  PANEL_VALUE,0,0);
		xv_set(k_item,  PANEL_INACTIVE, TRUE, 0);
	} else 
		xv_set(mate_frame, XV_SHOW, FALSE, 0);
}

/****************************************************************/
 void new2_proc()
{
	if ((int)xv_get(new2_item,  PANEL_VALUE))  {
		xv_set(new2_frame, XV_SHOW, TRUE, 0);
		xv_set(new3_frame, XV_SHOW, FALSE, 0);
		xv_set(new3_item,  PANEL_VALUE,0,0);
		xv_set(mate_frame, XV_SHOW, FALSE, 0);
		xv_set(mate_item,  PANEL_VALUE,0,0);
		xv_set(k_item,  PANEL_INACTIVE, FALSE, 0);
	} else 
		xv_set(new2_frame, XV_SHOW, FALSE, 0);
}

/****************************************************************/
 void new3_proc ()
{
	if ((int)xv_get(new3_item,  PANEL_VALUE)) {
		xv_set(new3_frame, XV_SHOW, TRUE, 0);
		xv_set(new2_frame, XV_SHOW, FALSE, 0);
		xv_set(new2_item,  PANEL_VALUE,0,0);
		xv_set(mate_frame, XV_SHOW, FALSE, 0);
		xv_set(mate_item,  PANEL_VALUE,0,0);
#if !K_INCLUDE
		xv_set(M_frame, XV_SHOW, FALSE, 0);
#endif
		xv_set(k_item,  PANEL_VALUE,0,0);
		xv_set(k_item,  PANEL_INACTIVE, TRUE, 0);
	} else 
		xv_set(new3_frame, XV_SHOW, FALSE, 0);

}

/* **************************param_okay********************************** */
void param_accept_proc()
{
	char *s;

	scale = (int)xv_get(param_scale, PANEL_VALUE);
	XCen = (int)xv_get(param_XCen, PANEL_VALUE);
	YCen = (int)xv_get(param_YCen, PANEL_VALUE);
	Nlifts= (int)xv_get(param_Nlifts, PANEL_VALUE);
	julia_depth = (double)(int)xv_get(param_depth, PANEL_VALUE);
	showticks = (int)xv_get(param_ticks, PANEL_VALUE);

	s = (char *)xv_get(param_wepsilon, PANEL_VALUE);
	sscanf(s,"%lf",&wepsilon);

	s = (char *)xv_get(param_fepsilon , PANEL_VALUE);
	sscanf(s,"%lf",&fepsilon);

	s = (char *)xv_get(param_furad , PANEL_VALUE);
	sscanf(s,"%lf",&furad);

}


/* **************************param_cancel********************************** */
void param_cancel_proc()
{

	char s[100];

	xv_set(param_scale, PANEL_VALUE, scale,0);
	xv_set(param_XCen, PANEL_VALUE,XCen,0);
	xv_set(param_YCen, PANEL_VALUE,YCen,0);
	xv_set(param_Nlifts, PANEL_VALUE,Nlifts,0);
	xv_set(param_depth, PANEL_VALUE,(int)julia_depth,0);
	xv_set(param_ticks, PANEL_VALUE,showticks,0);

	sprintf(s,"%lf",wepsilon);
	xv_set(param_wepsilon, PANEL_VALUE,s,0);

	sprintf(s,"%lf",fepsilon);
	xv_set(param_fepsilon , PANEL_VALUE,s,0);

	sprintf(s,"%lf",furad);
	xv_set(param_furad , PANEL_VALUE,s,0);

}


/* ***************************param_proc ********************************* */

void param_proc ()
{
	if ((int)xv_get(param_item,  PANEL_VALUE)) 
		xv_set(param_frame, XV_SHOW, TRUE, 0);
	else 
		xv_set(param_frame, XV_SHOW, FALSE, 0);
}


/* ***************************xspid_proc ********************************* */
void  xspid_proc ()
{
	if ((int)xv_get(xspid_item,  PANEL_VALUE)) 
		xv_set(xspid_frame, XV_SHOW, TRUE, 0);
	else 
		xv_set(xspid_frame, XV_SHOW, FALSE, 0);
}


/* ***************************lift_proc ********************************* */
void  lift_proc ()
{
	param_accept_proc(); /* read parameters */
	param_cancel_proc(); /* rewrite them parameters */

	xv_set(canvas_paint_window(canvas),WIN_CURSOR,wait_cursor, 0);
	lift_spider(&sp);
	setABC();
	dispABC();
	pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
	draw_spider(sp,XCen,YCen,scale);
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,cursor, 0);
#if !K_INCLUDE
	mark_m_space();
#endif
	HilightedRay = -1;
}

/* ***************************liftN_proc ********************************* */
void  liftN_proc ()
{
	int i;

	param_accept_proc(); /* read parameters */
	param_cancel_proc(); /* rewrite them parameters */

	xv_set(canvas_paint_window(canvas),WIN_CURSOR,wait_cursor, 0);
	disp("\n");
	for (i=1; i<Nlifts; ++i) {
		lift_spider(&sp);
		setABC();
		disp(".");
	}
	dispABC();
	pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
	draw_spider(sp,XCen,YCen,scale);
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,cursor, 0);
	HilightedRay = -1;
}


/* ***************************xlift_proc ********************************* */
void  xlift_proc ()
{
	char strng[300];
	int i;

	param_accept_proc(); /* read parameters */
	param_cancel_proc(); /* rewrite them parameters */

	xv_set(canvas_paint_window(canvas),WIN_CURSOR,wait_cursor, 0);
	lift_X_spider(&sp1);
	pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
	/* draw_spider(sp,XCen,YCen,scale); */

	/* PenPat(**greypat); */
	for (i=0; i<sp1.length[0]; ++i) 
		draw_ray(sp1.rays[0][i], XCen, YCen, scale);
	/* PenPat(**blackpat); */
	sprintf(strng,"\nEnd of ray with angle %ld/%ld is (%lf,%lf)",
		 xnum, xden, sp1.rays[0][1]->ray->node.x, sp1.rays[0][1]->ray->node.y);
	disp(strng);
	xv_set(canvas_paint_window(canvas),WIN_CURSOR,cursor, 0);
	HilightedRay = -1;
}

/* ***************************xspid_set_angles***************************** */
void xspid_set_angles()
{
	long			g;
	char 			strng[300];
	int 			i;

	xnum = (int)xv_get(xspid_num, PANEL_VALUE);
	xden = (int)xv_get(xspid_den, PANEL_VALUE);

	if (xden == 0)  
			xden = 1; 
	
	if ((g = gcd(xnum , xden)) != 0) 
	xnum /= g; xden/= g;
								
	xv_set(xspid_num, PANEL_VALUE, xnum, 0);
	xv_set(xspid_den, PANEL_VALUE, xden, 0);

	xv_set(xspid_lift_item, PANEL_INACTIVE, FALSE,0 );
	sprintf(strng, "\nangles are:\n(%ld/%ld) with deg %d", xnum,xden, sp.deg); 
	disp(strng);
	free_spider(&sp1);
	sp1.Num_Crit_Pts = 1;
	sp1.deg = sp.deg;
	spider_init(&sp1, (long)sp.deg, xnum, xnum, xden, (long)0, (long)0, (long)1);
	for (i=0; i<sp1.length[0]; ++i) 
		draw_ray(sp1.rays[0][i], XCen, YCen, scale);

}

/* ***************************new2_set_rb***************************** */
void new2_set_rb()
{
	char lead[255], repeat[255];
	long g;
	int i;

	num = (long)xv_get(new2_num, PANEL_VALUE);
	den = (long)xv_get(new2_den, PANEL_VALUE);
	d = (long)xv_get(new2_deg, PANEL_VALUE);

    /* reduce num and den */
    if (den != 0) num %=den;
    if ((g = gcd(num, den)) != 0) {
           num /= g; den/= g;
     }

	xv_set(new2_num, PANEL_VALUE, (int)num,0);
	xv_set(new2_den, PANEL_VALUE, (int)den,0);
	xv_set(new2_deg, PANEL_VALUE, (int)d,0);

	if (!frac_to_dec(num, den, Leading_Part, Repeating_Part)) {
		sprintf(lead, "Error..");
		sprintf(repeat, "Error..");
		Leading_Part[0] = Repeating_Part[0] = 0;
		xv_set(new2_leading , PANEL_VALUE, lead, 0);
		xv_set(new2_repeating , PANEL_VALUE, repeat, 0);
	}
	else {
        for (i=0; i<Leading_Part[0]; ++i)
             lead[i] = '0' + (int)Leading_Part[i+1];
		lead[i] = '\0';

        for (i=0; i<Repeating_Part[0]; ++i)
             repeat[i] =  '0' + (int)Repeating_Part[i+1];
		repeat[i] = '\0';

    	find_sector_string();
	
		xv_set(new2_leading , PANEL_VALUE, lead, 0);
		xv_set(new2_repeating , PANEL_VALUE, repeat, 0);
		xv_set(new2_sector, PANEL_VALUE, sec_str, 0);
	
		free_spider(&sp);
		sp.Num_Crit_Pts = 1;
		sp.deg = d;
		spider_init(&sp, d, num, num+den, d*den, (long)0, (long)0, (long)1);
		setABC(); 
		pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
		draw_spider(sp,XCen,YCen,scale);
		/* disallow xtra spider lifting */
		xv_set(xspid_lift_item, PANEL_INACTIVE, TRUE,0);
		HilightedRay = -1;
	}
}


/* ***************************new2_set_angles***************************** */
void new2_set_angles()
{
	char *lead, *repeat;
	char llead[255], rrepeat[255];
	long g;
	int i;

	lead  = (char *)xv_get(new2_leading, PANEL_VALUE);
	repeat = (char *)xv_get(new2_repeating, PANEL_VALUE);
	d = (long)xv_get(new2_deg, PANEL_VALUE);

	Leading_Part[0] = strlen(lead);
	Repeating_Part[0] = strlen(repeat);

    for (i=1; i<=Leading_Part[0]; ++i)
            Leading_Part[i] = (long)(lead[i-1] - '0');

    for (i=1; i<=Repeating_Part[0]; ++i)
                       Repeating_Part[i] = (long)(repeat[i-1] - '0');


    dec_to_frac(&num, &den, Leading_Part, Repeating_Part);

	if (!frac_to_dec(num, den, Leading_Part, Repeating_Part)) {
		sprintf(llead, "Error..");
		xv_set(new2_sector, PANEL_VALUE, llead, 0);
	}
	else {
        for (i=0; i<Leading_Part[0]; ++i)
             llead[i] = '0' + (int)Leading_Part[i+1];
		llead[i] = '\0';

        for (i=0; i<Repeating_Part[0]; ++i)
             rrepeat[i] =  '0' + (int)Repeating_Part[i+1];
		rrepeat[i] = '\0';

    	find_sector_string();
	
		xv_set(new2_leading , PANEL_VALUE, llead, 0);
		xv_set(new2_repeating , PANEL_VALUE, rrepeat, 0);
		xv_set(new2_sector, PANEL_VALUE, sec_str, 0);
		xv_set(new2_den, PANEL_VALUE, (int)den,0);
		xv_set(new2_num, PANEL_VALUE, (int)num,0);
		xv_set(new2_deg, PANEL_VALUE, (int)d,0);
	
		free_spider(&sp);
		sp.Num_Crit_Pts = 1;
		sp.deg = d;
		spider_init(&sp, d, num, num+den, d*den, (long)0, (long)0, (long)1);
		setABC(); 
		pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
		draw_spider(sp,XCen,YCen,scale);
		/* disallow xtra spider lifting */
		xv_set(xspid_lift_item, PANEL_INACTIVE, TRUE,0);
		HilightedRay = -1;
	}
}

/* ***************************new3_set_angles***************************** */
void new3_set_angles()
{
	char strng[300];
	int tnum11, tnum12, tnum21, tnum22, tden1, tden2;

	free_spider(&sp);
	sp.Num_Crit_Pts = 2;
	sp.deg = d = 3;
	sprintf(strng, "\nangles are:\n(%ld/%ld, %ld/%ld)\n(%ld/%ld, %ld/%ld) ", 
		num11,den11, num12, den12, num21, den21 , num22, den22);
	disp(strng);
	/* we want a common denominator */
	tnum11 = num11; tnum12 = num12;
	tnum21 = num21; tnum22 = num22;
	tden1 = den12;  tden2 = den22;

	if (den12 == 3*den11) { 
		tden1 = den12; tnum11 *= 3;
	} else if (den11 == 3*den12) { 
		tden1 = den11; tnum12 *= 3;
	}

	if (den22 == 3*den21) { 
		tden2 = den22; tnum21 *= 3;
	} else if (den21 == 3*den21) { 
		tden2 = den21; tnum22 *= 3;
	}
		

	spider_init(&sp, d, tnum11, tnum12, tden1, tnum21, tnum22, tden2);
    setABC();
	pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
    draw_spider(sp,XCen,YCen,scale);
	xv_set(xspid_lift_item, PANEL_INACTIVE, TRUE,0);
    HilightedRay = -1;
}

/* ***************************new3_toggle_angle1************************** */
void new3_toggle_angle1()
{
	long g;
	char s[300];
	long t_num12, t_den12;
	int r = 80, cx = 170, cy = 110, dx, dy, dx1, dy1; 


	if (new3_button_invalid_flag == 2) 
		xv_set(new3_set_item, PANEL_INACTIVE, FALSE,0);
	else 
		new3_button_invalid_flag = 1;

	num11 = xv_get(new3_num1, PANEL_VALUE);
	den11 = xv_get(new3_den1, PANEL_VALUE);
	num11 = num11%den11;

   	if ((g = gcd(num11, den11)) != 0) {
         		num11 /= g; 
				den11/= g;
   	}
	xv_set(new3_num1, PANEL_VALUE, num11, 0);
	xv_set(new3_den1, PANEL_VALUE, den11, 0);

	t_den12 = 3*den11;
	t_num12 = (3*num11 + den11)%t_den12;;
   	if ((g = gcd(t_num12, t_den12)) != 0) {
         		t_num12 /= g; 
				t_den12/= g;
   	}

	if (num12 == t_num12 && den12 == t_den12) {
			den12 = 3*den11;
			num12 = (3*num11 + 2*den11)%den12;
    		if ((g = gcd(num12, den12)) != 0) {
           		num12 /= g; 
				den12/= g;
     		}
	} else {
			den12 = 3*den11;
			num12 = (3*num11 + den11)%den12;
    		if ((g = gcd(num12, den12)) != 0) {
           		num12 /= g; 
				den12/= g;
     		}
	}

	sprintf(s,"Other critical angle = %d/%d", num12, den12);
	xv_set(new3_other_angle1, PANEL_LABEL_STRING,	s,0 );

	pw_writebackground(new3_pw,0,0,500,500,PIX_SRC);

	/* draw a circle */
	for (g=0; g<100; ++g) {
    	dx = (int)((double)r*cos(TPI*(double)g/100.0));
    	dy = (int)((double)r*sin(TPI*(double)g/100.0));
    	dx1 = (int)((double)r*cos(TPI*(double)(g+1)/100.0));
    	dy1 = (int)((double)r*sin(TPI*(double)(g+1)/100.0));
		pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);
	}
			

	/* draw line connecting angles to show if angles cross */
    dx = (int)((double)r*cos(TPI*(double)num11/(double)den11));
    dy = (int)((double)r*sin(TPI*(double)num11/(double)den11));
    dx1 = (int)((double)r*cos(TPI*(double)num12/(double)den12));
    dy1 = (int)((double)r*sin(TPI*(double)num12/(double)den12));
	pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);


	/* draw line connecting angles to show if angles cross */
    dx = (int)((double)r*cos(TPI*(double)num11/(double)den11));
    dy = (int)((double)r*sin(TPI*(double)num11/(double)den11));
    dx1 = (int)((double)r*cos(TPI*(double)num12/(double)den12));
    dy1 = (int)((double)r*sin(TPI*(double)num12/(double)den12));
	pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);

	/* draw line connecting angles to show if angles cross */
    dx = (int)((double)r*cos(TPI*(double)num21/(double)den21));
    dy = (int)((double)r*sin(TPI*(double)num21/(double)den21));
    dx1 = (int)((double)r*cos(TPI*(double)num22/(double)den22));
    dy1 = (int)((double)r*sin(TPI*(double)num22/(double)den22));
	pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);

}

/* ***************************new3_toggle_angle2************************** */
void new3_toggle_angle2()
{
	long g;
	char s[300];
	long t_num12, t_den12;
	int r = 80, cx = 170, cy = 110, dx, dy, dx1, dy1; 

	if (new3_button_invalid_flag == 1) 
		xv_set(new3_set_item, PANEL_INACTIVE, FALSE,0);
	else 
		new3_button_invalid_flag = 2;

	num21 = xv_get(new3_num2, PANEL_VALUE);
	den21 = xv_get(new3_den2, PANEL_VALUE);
	num21 = num21%den21;

   	if ((g = gcd(num21, den21)) != 0) {
         		num21 /= g; 
				den21/= g;
   	}
	xv_set(new3_num2, PANEL_VALUE, num21, 0);
	xv_set(new3_den2, PANEL_VALUE, den21, 0);

	t_den12 = 3*den21;
	t_num12 = (3*num21 + den21)%t_den12;
   	if ((g = gcd(t_num12, t_den12)) != 0) {
         		t_num12 /= g; 
				t_den12/= g;
   	}

	if (num22 == t_num12 && den22 == t_den12) {
			den22 = 3*den21;
			num22 = (3*num21 + 2*den21)%den22;
    		if ((g = gcd(num22, den22)) != 0) {
           		num22 /= g; 
				den22/= g;
     		}
	} else {
			den22 = 3*den21;
			num22 = (3*num21 + den21)%den22;
    		if ((g = gcd(num22, den22)) != 0) {
           		num22 /= g; 
				den22/= g;
     		}
	}

	sprintf(s,"Other critical angle = %d/%d", num22, den22);
	xv_set(new3_other_angle2, PANEL_LABEL_STRING,	s,0 );

	pw_writebackground(new3_pw,0,0,500,500,PIX_SRC);

	/* draw a circle */
	for (g=0; g<100; ++g) {
    	dx = (int)((double)r*cos(TPI*(double)g/100.0));
    	dy = (int)((double)r*sin(TPI*(double)g/100.0));
    	dx1 = (int)((double)r*cos(TPI*(double)(g+1)/100.0));
    	dy1 = (int)((double)r*sin(TPI*(double)(g+1)/100.0));
		pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);
	}

			

	/* draw line connecting angles to show if angles cross */
    dx = (int)((double)r*cos(TPI*(double)num11/(double)den11));
    dy = (int)((double)r*sin(TPI*(double)num11/(double)den11));
    dx1 = (int)((double)r*cos(TPI*(double)num12/(double)den12));
    dy1 = (int)((double)r*sin(TPI*(double)num12/(double)den12));
	pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);

	/* draw line connecting angles to show if angles cross */
    dx = (int)((double)r*cos(TPI*(double)num21/(double)den21));
    dy = (int)((double)r*sin(TPI*(double)num21/(double)den21));
    dx1 = (int)((double)r*cos(TPI*(double)num22/(double)den22));
    dy1 = (int)((double)r*sin(TPI*(double)num22/(double)den22));
	pw_vector(new3_pw,cx+dx, cy-dy, cx+dx1, cy-dy1, PIX_SRC, 1);

}

/****************************************************************/
void new3_set_invalid(item, event)
Panel_item item; Event *event;
{
	xv_set(new3_set_item, PANEL_INACTIVE, TRUE,0);
	new3_button_invalid_flag = 0;
 }

/****************************************************************/
void ps_proc()
{
	if ((int)xv_get(ps_item,  PANEL_VALUE)) {
		ps_spooling = 1;
		output = fopen("spider.ps", "w");
    	fprintf(output,"%%!");
    	fprintf(output,"\n/ln { %% stack x y z w=> -- ");
    	fprintf(output,"\n       newpath");
    	fprintf(output,"\n       moveto");
    	fprintf(output,"\n       lineto");
    	fprintf(output,"\n       stroke } def");
    	fprintf(output,"\n %lf %lf  scale\n", PS_SCALING,PS_SCALING);
	} else {
		ps_spooling = 0;
    	fprintf(output,"\nshowpage");
		fclose(output);	
	}
}

/****************************************************************/
void k_proc()
{
#if !K_INCLUDE
	if ((int)xv_get(k_item,  PANEL_VALUE)) 
		xv_set(M_frame, XV_SHOW, TRUE, 0);
	else 
		xv_set(M_frame, XV_SHOW, FALSE, 0);
#endif
}


/****************************************************************/
void clear_proc()
{
	pw_writebackground(pw,0,0,1500,1500,PIX_SRC);
}


/****************************************************************************/
void Z_iterate_proc()
{
	double temp;
	int scx, scy, PenMode;

	PenMode = PIX_SRC;

	temp = Z.x*Z.x - Z.y*Z.y + C.x;
	Z.y = 2.0*Z.x*Z.y + C.y;
	Z.x = temp;

	scx = xwindow(Z.x);
 	scy = ywindow(Z.y);
	cline(scx-3, scy,scx+3,scy);
	cline(scx, scy-3,scx,scy+3);

}


/****************************************************************************/
void Z_reset_proc()
{
	int scx, scy, PenMode;

	PenMode = PIX_SRC;
	Z.x = Z.y = 0.0;

	scx = xwindow(Z.x);
 	scy = ywindow(Z.y);
	cline(scx-3, scy,scx+3,scy);
	cline(scx, scy-3,scx,scy+3);
}


/****************************************************************************/
void quit_proc()
{
xv_destroy_safe(frame);
}


