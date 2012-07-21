

short the_icon_image[] = {
	0x0000,0x0007,0xF000,0x0000, 0x0000,0x0038,0x0E00,0x0000,
	0x0000,0x0FC0,0x0100,0x0000, 0x000F,0xF180,0x0CC0,0x0700,
	0x00F0,0x020E,0x0621,0xFA00, 0x0040,0x041D,0x0F1E,0x0200,
	0x0020,0x0411,0x9990,0x0400, 0x0020,0x0C19,0x9198,0x0800,
	0x0020,0x081F,0x1F08,0x0800, 0x0010,0x0800,0x0008,0x1000,
	0x0010,0x0800,0x0008,0x2000, 0x0010,0x0C00,0x001C,0x2000,
	0x0010,0x1C10,0x0112,0x0000, 0x0000,0x6418,0x0611,0x8000,
	0x0001,0x8307,0x1C20,0x8000, 0x000E,0x0381,0xF0C0,0x4000,
	0x0018,0x0440,0x0121,0x2000, 0x0060,0x0838,0x0E22,0x1000,
	0x0080,0x0807,0xF010,0x0800, 0x0042,0x1000,0x000C,0x0400,
	0x0042,0x201F,0xF008,0x0800, 0x0022,0x40E0,0x0E04,0x1000,
	0x0022,0x2300,0x0194,0x1000, 0x0011,0x2C00,0x0062,0x2000,
	0x0011,0x1000,0x001C,0x4000, 0x0009,0x3000,0x0018,0x8000,
	0x0008,0xC800,0x0024,0x8000, 0x0004,0x8400,0x00C3,0x0000,
	0x0005,0x0400,0x0103,0x0000, 0x0002,0x0200,0x0204,0x8000,
	0x0002,0x0200,0x0C04,0x8000, 0x0005,0x0100,0x1008,0x4000,
	0x0005,0x0000,0x2010,0x4000, 0x0008,0x8000,0xC020,0x2000,
	0x0008,0x8001,0x0040,0x2000, 0x0008,0x4002,0x0040,0x2000,
	0x0010,0x4000,0x0080,0x1000, 0x0010,0x2000,0x0100,0x1000,
	0x0010,0x2000,0x0200,0x1000, 0x0010,0x1000,0x0200,0x1000,
	0x0010,0x1000,0x0400,0x1000, 0x0011,0x0800,0x0800,0x1000,
	0x0011,0x8800,0x1000,0x1000, 0x0010,0x8400,0x1000,0x1000,
	0x0010,0x8000,0x2000,0x1000, 0x0008,0xC000,0x4000,0x2000,
	0x0008,0x4000,0x0000,0x2000, 0x0008,0x6000,0x0000,0x2000,
	0x0004,0x2000,0x0000,0x4000, 0x0004,0x3000,0x0000,0x4000,
	0x0002,0x1000,0x0000,0x8000, 0x0002,0x0C00,0x0000,0x8000,
	0x0001,0x0600,0x0001,0x0000, 0x0000,0x8180,0x0002,0x0000,
	0x0000,0x40C0,0x0004,0x0000, 0x0000,0x2038,0x7008,0x0000,
	0x0000,0x100F,0xC010,0x0000, 0x0000,0x0C00,0x0060,0x0000,
	0x0000,0x0300,0x0180,0x0000, 0x0000,0x00E0,0x0E00,0x0000,
	0x0000,0x001F,0xF000,0x0000, 0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000,0x0000
	};


char *help_text = "For help, move the cursor over any item \
and press the HELP key on your keyboard. If you don't have \
a HELP key, you are sort of screwed, sorry. Perhaps you can \
get some X guru to bind the resource: OpenWindows.KeyboardCommand.Help \
to a key for you..\
\n\nThis program is an implementation of \
Thurston's algorithm for cubic polynomials and polynomials \
with just one critical point. The input data is a set of angles. The \
output is the polynomial with critical points whose combinatorial dynamics \
are given by the action of T -> d T, where d is the degree of the \
polynomial and T is an angle corresponding to a critical point. \
\n\n \
The program also draws parameter (Mandelbrot set) space pictures \
using code largely written by Marc Parmet based on appendix \
D of The Science of Fractal Images. The algorithms there are mostly  \
due to Milnor, Thurston, Wilkes, Hubbard, and myself. \
\n\n\
To use the program (also see the step by step tutorial at the end of \
this help listing), enter a rational angle using the NEW 2 or \
NEW 3 button. These buttons will bring up windows that allow entry \
of one angle or a set of two angles, respectively. Press the  \
Set Angles, or Set Repeating Expansion buttons to define the spider.  \
A Spider is a data type which is used in the algorithm, it is a  \
union of the image of segments, joined at infinity - each image  \
forms a leg of the spider. The spiders are displayed in the main  \
window. Iterate the algorithm \
by pressing the LIFT button or the Many Lifts button. \
When the spider converges, you can  \
find the polynomial in the Goings On window. You can see the Julia \
set for the polynomial at any time. You can see external rays to  \
the polynomial by using the Extra Spider button, entering an  \
angle, and lifting the extra spider until it converges \
to external rays. If you r e a l l y want to know about this \
stuff, invite me to give a talk about it sometime.... \
\n \
\n \
Some good values to try are angles of 1/6,9/59,1/15,1/31,1/63, \
with d = 2. \
\n\n \
The program is not dummy proofed, and I am sure it can be   \
made to crash and give crazy results. In fact, it may give \
crazy results even when you'd think it shouldn't. Let me know \
about it.  \
\n\n \
Yuval Fisher\
\n\n\n e-mail yfisher@ucsd.edu \n\
Institute for Non-Linear Science 0402 \n\n\
University of California, San Diego \n\
La Jolla, CA 92093\n\
\n\
\n\
\n\n\n\n\n\
\n\
A brief step by step tutorial:\n\
\n\
1) First click New 2...\n\
(Clicking it again toggles the window on and off)\n\
\n\
2) Select the Numerator Field and enter 1\n\
\n\
3) Select the Denominator field and enter 7\n\
\n\
4) Click Set Repeating Expansion\n\
a) note that the binary expansion of 1/7 = 001001001001..., \n\
and that this is shown in the Leading Binary Expansion \n\
field and Repeating Binary Expansion field. Entering \n\
values in these fields and clicking Set Angles will \n\
determine the rational angles with these expansions). \n\
\n\
b) note that some lines have appeared in the main window. \n\
\n\
5) Click on Many Lifts in the main window, 5 times. \n\
a) note that the image has stopped changing, more or less. \n\
\n\
b) note that the C value in the Goings On window has\n\
converged, more or less. The polynomial p(z) = z*z + c\n\
has Douady's rabbit as a Julia set. \n\
\n\
6) Click on Julia Set, and see the julia set for a \n\
polynomial whose critical value is periodic of \n\
period 3, that is \n\
c -> p(c) -> p(p(c)) -> p(p(p(c))) = c; just as \n\
1/7 -> 2/7 -> 4/7 -> 1/7 under  multiplication by\n\
2,  modulo 1. \n\
\n\
7) To generate a postscript image, click PostScript Spool, \n\
click Lift, Click Julia Set, then slick PostScript Spool again. \n\
This created a file called spider.ps containing the image. \n \
\n\
8) Click on the 1/4 Theorem button. \n \
\n\
9) Click on the Compute button in the new window. This will \n \
draw a parameter space picture. If the Lift button on the main \n \
window is pressed, the C value is displayed in the new parameter \n \
space window. \n  \
\n\n \
The following mouse actions do something \n\
in the Koebe 1/4 Theorem window. \n\
Right Button : select C or Z value. \n\
Left Button : Select first point for  \n\
              blow up zoom. \n\
Middle Button : Select second point \n\
                for blow up zoom. \n\
\n\n\n \
BUGS: \n \
There are many bugs I know about. Some are: \n \
\n \
0) The lifting algorithm is not correct, though it can be fixed.  \n \
   Sometimes the wrong root is chosen leading to problems.  \n \
1) No %%BoundingBox comment on the main window postscript file. \n \
2) A wrong %%BoundingBox on the Koebe Quarter Theorem PS file. \n \
3) Funny lines drawn sometimes when a spider is defined. \n \
\n\n"; 

