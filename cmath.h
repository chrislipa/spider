
#define CADD(B,A)		{(B).x += (A).x; (B).y += (A).y;  				 }
#define CSUM(C,A,B)		{(C).x = (A).x + (B).x; (C).y = (A).y + (B).y;   }
#define CSUB(B,A)		{(B).x -= (A).x; (B).y -= (A).y;				 }
#define CDIFF(C,A,B)	{(C).x = (A).x - (B).x; (C).y = (A).y - (B).y;   }
#define CMUL(B,A)		{ complex _Z; _Z.x=(A).x*(B).x-(A).y*(B).y;_Z.y=(A).x*(B).y+(A).y*(B).x;B=_Z;}
#define CINV(B,A)		{ double _T1; _T1=(A).x*(A).x+(A).y*(A).y;(B).x=(A).x/_T1;(B).y=-(A).y/_T1;}
#define CPROD(C,A,B) 	{ complex _Z1; _Z1=A; CMUL(_Z1,B); C=_Z1;			 }
#define CDIV(B,A)   	{ complex _Z2; CINV(_Z2,A); CMUL(B,_Z2);		 	 }
#define COVER(C,A,B) 	{ complex _Z3; CINV(_Z3,B); CPROD(C,A,_Z3);		 	 }
#define PI				(3.14159265358979)
#define TPI				(2*PI)
#define comp_abs(z)  	sqrt(z.x*z.x+z.y*z.y)
#define comp_arg(z)		(z.x==0&&z.y==0 ? 0 : ((t = atan2(z.y, z.x))<0 ? TPI+t : t))
#define comp_equal(a,b)	(fabs((a).x-(b).x) <0.1 && fabs((a).y-(b).y) < 0.1)
#define comp_equal1(a,b)	((a).x == (b).x && (a).y == (b).y)

struct complex {double x,y;};

typedef struct complex complex;

void   	print_comp();
complex get_comp();
complex cmplx();
complex polar();
complex comp_exp();
complex comp_log();
complex new_abs();
complex new_arg();
