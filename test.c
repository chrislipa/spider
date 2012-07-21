#include <stdio.h>
//#include <xview/xview.h>
//#include <xview/panel.h>


/* XView stuff */
Frame 			frame, 
				new2_frame,
				new3_frame;


Panel 			panel, 
				new2_panel,
				new3_panel;

Panel_item new2_item, new3_item;

void quit_proc(),
 new2_proc(),
 new3_proc();

main(argc,argv)
int argc; char **argv;
{
	int i;

	xv_init(XV_INIT_ARGC_PTR_ARGV,&argc, argv, NULL);


	frame = (Frame)xv_create(NULL, FRAME, 
		XV_LABEL, "Spiders", 
		WIN_X,0,WIN_Y,0,XV_HEIGHT,900,XV_WIDTH,700,0);

	new2_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 475, XV_HEIGHT, 300,
						FRAME_LABEL, "New 2", XV_SHOW, FALSE, 0);

	new3_frame = (Frame) xv_create(frame, FRAME, 
						XV_WIDTH, 395, XV_HEIGHT, 550,
						FRAME_LABEL, "New 3", XV_SHOW, FALSE, 0);


	panel = (Panel)xv_create(frame, PANEL, 0);

	new2_panel = (Panel)xv_create(new2_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);

	new3_panel = (Panel)xv_create(new3_frame, PANEL, 
					PANEL_LAYOUT, PANEL_VERTICAL,0);


    new2_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"New 2...",0,
			PANEL_NOTIFY_PROC, new2_proc, XV_HELP_DATA, "spider:new2",0);

    new3_item = (Panel_item)xv_create(panel,PANEL_TOGGLE,
			PANEL_CHOICE_STRINGS,"New 3...",0,
			PANEL_NOTIFY_PROC, new3_proc, XV_HELP_DATA, "spider:new3",0);

   xv_create(panel,PANEL_BUTTON,PANEL_LABEL_STRING,"Quit",
		PANEL_NOTIFY_PROC, quit_proc, XV_HELP_DATA, "spider:quit",0);


/*
	window_fit_height(panel);
	window_fit_height(new3_panel);
*/

	window_main_loop(frame);
	exit(0);
}

/****************************************************************/
 void new2_proc()
{
	if ((int)xv_get(new2_item,  PANEL_/*TOGGLE_*/VALUE))  {
		xv_set(new2_frame, XV_SHOW, TRUE, 0);
		xv_set(new3_frame, XV_SHOW, FALSE, 0);
		xv_set(new3_item,  PANEL_/*TOGGLE_*/VALUE,0,0);
	} else 
		xv_set(new2_frame, XV_SHOW, FALSE, 0);
}

/****************************************************************/
 void new3_proc ()
{
	if ((int)xv_get(new3_item,  PANEL_/*TOGGLE_*/VALUE)) {
		xv_set(new3_frame, XV_SHOW, TRUE, 0);
		xv_set(new2_frame, XV_SHOW, FALSE, 0);
		xv_set(new2_item,  PANEL_/*TOGGLE_*/VALUE,0,0);
	} else 
		xv_set(new3_frame, XV_SHOW, FALSE, 0);

}

/****************************************************************/
void quit_proc()
{
xv_destroy_safe(frame);
}


