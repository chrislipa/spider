#include <stdio.h>
#include <math.h> 
//#include <xview/xview.h>

//char *malloc();
//char *realloc();
/* void free(); */

#define PIX_SRC 0

#define SV 1
#define SIMPLE_M	0
#define SIMPLE_J	1
#define KQT_M		2
#define KQT_J		3

/* draing modes */
#define XOR 		PIX_NOT(PIX_DST)
#define COPY 		PIX_SRC

#define NSTACKS		3
#define EXPANSION	2000

#define MAXITERATIONS 1000
#define COLOR_MAP_SIZE 128

#define PS_SCALING          0.5 /* product of next two should be near 1 */
#define PS_MAG              1   /* they determine the size of the ps lines */

#define BLACK 127


double xp2v(),yp2v();
double xv2pr(),yv2pr();


typedef struct {
	double x,y;
} complex;

typedef struct {
	struct {
		int done;

		int width;
		int iterations;
		double escape;
		complex center;
		double radius;
		complex c;
		double delta,recur;

		int mode;
		int i,j;
		complex *seq;
		struct kqt_stack {
			int size,maxsize;
			double maxradius;
			struct kqt_datum {
				int i,j;
			} *data;
		} stack[2][NSTACKS];
	} pict;

	unsigned char *icnt;

	short *icnt2color;
} gstruct,*gptr;
