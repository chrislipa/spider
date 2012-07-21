/* the k_*.c files can be compiled on their own, almost */
/* make k_main = main and eliminate externals you dont know */
/* about  during compile time, and that should do it */

#include "koebe.h"

int		left = 0,
		top = 0,
		right = 600, 
		width = 601,
		iterations = 126, 
		color_monitor;

gptr gp, create_gstruct();


k_main()
{
	int i;

	gp = create_gstruct();
	init_graphics(100,100);
}
