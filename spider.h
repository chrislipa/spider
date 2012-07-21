#include <stdio.h>
#include <math.h>
////#include <xview/xview.h>
//#include <xview/panel.h>
//#include <xview/canvas.h>
//#include <xview/cms.h>
//#include <xview/cursor.h>
//#include <xview/textsw.h>

#include "cmath.h"

#define FALSE 0
#define TRUE 1

#define NUM_CRIT_PTS		2
#define MAX_CRIT_PT_DEG		2
#define MAX_ORBIT_LENGTH	300
#define MAX_TOTAL_RAYS		300
#define Num_Digits 			100

#define NUM_INITIAL_RAY_NODES	30
#define INITIAL_RAY_LOWER_RAD	1.0
#define INITIAL_RAY_UPPER_RAD	1000.0

#define PS_SCALING 			0.5 /* product of next two should be near 1 */
#define PS_MAG				1   /* they determine the size of the ps lines */

#define PIX_XOR PIX_SRC ^ PIX_DST

#define disp(s) textsw_insert(textsw,s,strlen(s))
#define ReducedNum(a,b) ((long)gcd(a,b) ? a/(long)gcd(a,b) : a)
#define ReducedDen(a,b) ((long)gcd(a,b) ? b/(long)gcd(a,b) : b)

#define xwindow(x) 			(XCen + x*scale)
#define ywindow(y) 			(YCen - y*scale)
#define windowx(x) 			((double)x-Xcen)/scale
#define windowy(y) 			(YCen-(double)y)/scale

/* #define line(a,b,a_o,b_o,color) 	XDrawLine(dpy,xwin,gc,a,b,a_o,b_o); */
#define line(a,b,a_o,b_o,color) pw_vector(pw,a,b,a_o,b_o,PenMode,color);\
	if (ps_spooling) \
		fprintf(output,"%d %d %d %d ln\n ",\
		PS_MAG*a,PS_MAG*(800-b),PS_MAG*a_o,PS_MAG*(800-b_o));

#define cline(a,b,a_o,b_o) 		pw_vector(pw,a,b,a_o,b_o,PenMode,1); \
	if (ps_spooling) \
		fprintf(output,"%d %d %d %d ln\n ",\
		PS_MAG*a,PS_MAG*(800-b),PS_MAG*a_o,PS_MAG*(800-b_o));


struct node {
	complex node;
	struct node *next, *prev;
	};
	
struct ray {
	int length;
	struct node *ray, *end;	
	};
	

struct spider {
	long   num[NUM_CRIT_PTS][MAX_TOTAL_RAYS],den[NUM_CRIT_PTS], pivot[NUM_CRIT_PTS], length[NUM_CRIT_PTS]; 
	struct ray *rays[2][MAX_TOTAL_RAYS],  *pivotray[NUM_CRIT_PTS];
	int		deg; /* global degree of polynomial */
	int		crit_value[NUM_CRIT_PTS]; /* index of critical value ray */
	int		Num_Crit_Pts;	/*number of critical points */
	int		Second_Hits_First; /*boolean that indicates if the second
	                            critical rays land on the orbit of the
	                            first */
	};

struct mader /* a mated spider */ {
	long   num[2][MAX_TOTAL_RAYS],den[2], pivot[2], length[2]; 
	struct ray *rays[2][MAX_TOTAL_RAYS],  *pivotray[2], *loop;
	int		deg; /* global degree of polynomial */
	int		crit_value[2]; /* index of critical value ray */
	/* the data type is, interestingly, almost identical to a spider*/
	};
	

