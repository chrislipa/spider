#include "spider.h"



#define MOVETO 0
#define DRAWFROM 1
#define DRAWTO 2

/* Program Globals */
extern struct spider    sp, sp1;
extern long				d;
extern complex			A,B,C;
extern double 			wepsilon,fepsilon,furad;
extern int				scale;
extern int 				showticks;
char 					strng[300];

extern FILE				*output;

/* XView Globals */
//extern Pixwin 			*pw;
//extern Textsw			textsw;
//extern Frame 			frame;
extern int 				HilightedRay, XCen, YCen;
extern int 				ps_spooling;



//int		PenMode = PIX_SRC;
//int		PenSize = 1;


/*=========================== polar ==============================*/
complex polar(r,theta)
double r,theta;
{
	complex a;
	a.x = r*cos(theta);
	a.y = r*sin(theta);
	return(a);
}

/*=========================== check_ray ==============================*/
void check_ray(ray)
struct ray *ray;
/* checks the integrity of the ray  */
{
	struct node *node;
	int i;
	
	node = ray->ray;
	for (i=1; i<ray->length; ++i) {
		if (node->next->prev != node) disp("\n***\nPREV/NEXT FAILED\n***\n");
		node = node->next;
	}
	if (node != ray->end) disp("\n***\nRAY LENGTH ERROR\n***\n");
}

/*=========================== rotate_ray ==============================*/
void rotate_ray(ray,t)
struct ray *ray; double t;
/* checks the integrity of the ray  */
{
	struct node *node;
	complex etpit;
	int i;
	
	etpit.x = cos(TPI*t);
	etpit.y = sin(TPI*t);
	
	node = ray->ray;
	for (i=1; i<ray->length; ++i) {
		CMUL(node->node, etpit);
		node = node->next;
	}
	CMUL(node->node, etpit);

}

/*=========================== prune_ray ==============================*/
void prune_ray(ray, e,fe)
struct ray *ray; double e,fe;
/* cprune a ray so that no successive node are closer than e   */
{
	struct node *node, *tnode;
	int i;
	complex s;
	double t;
	
	if (e == 0.0 && fe == 0.0) return;
	
	node = ray->ray;
	do {
		tnode = node->next;
		CDIFF(s,tnode->node, node->node);
		if ((t=comp_abs(s)) < e && tnode != ray->end) {
			node->next = tnode->next;
			node->next->prev = node;
			free(tnode); --ray->length;
		} else if ((fe > 0.0) && (comp_abs(node->node) < furad) && (t > fe) ) {
			s = polar(fe, comp_arg(s));
			node->next = (struct node*)malloc(sizeof(struct node)); 
			CSUM(node->next->node, s, node->node);
			node->next->prev = node;
			node->next->next = tnode;
			tnode->prev = node->next;
			++ray->length;
			node = node->next;
		} else 
			node = node->next;
	} while (node != ray->end);
}


/*=========================== copy_ray ==============================*/
struct ray *copy_ray(ray)
struct ray *ray;
/* make a copy of the ray */
{
	int 	i;
	struct ray *new_ray;
	struct node *node, *old_ray_node;
	
	new_ray = (struct ray*)malloc(sizeof(struct ray)); 
	new_ray->length = ray->length;
	node = new_ray->ray = (struct node*)malloc(sizeof(struct node)); 
	old_ray_node = ray->ray;
	node->node = old_ray_node->node;
	
	for (i=1; i<ray->length; ++i) {
		node->next = (struct node*)malloc(sizeof(struct node)); 
		node->next->prev = node;
		old_ray_node = old_ray_node->next;
		node = node->next; 
		node->node = old_ray_node->node;
	}
	
	new_ray->end = node;
	return(new_ray);
}

/*=========================== free_ray ============================*/
void free_ray(ray)
struct ray *ray;
/* return memory space of a ray */
{
	struct node *node;
	int i;
	
	node = ray->ray;
	for (i=1; i<ray->length; ++i) {
		if (node != node->next->prev) 
			disp("\n***\nNODE POINTER ERROR\n***\n");
		node = node->next;
		free(node->prev);
	}
	if (node != ray->end) disp("\n End of ray error ");
	free(ray->end);
	free(ray);
}

/*=========================== free_spider ============================*/
void free_spider(sp)
struct spider *sp;
/* return memory space of a spider */
{
	int i,k;
	
	for (k=0; k<sp->Num_Crit_Pts; ++k) {
		for (i=0; i<sp->length[k]; ++i) 
			free_ray(sp->rays[k][i]);

		if (sp->num[0][0] != sp->num[0][1]) free_ray(sp->pivotray[k]); /* the extra spider doesnt have a pivot ray allocated */
	}
			
}

/*=========================== make_initial_leg ======================*/
struct ray *make_initial_leg(num, den)
/*construct an initial leg with angle num/den */
long num, den;
{
	double r,theta = TPI*(double)num/(double)den;
	int 	i;
	struct ray *ray;
	struct node *node1;
	

	if ((ray = (struct ray *)malloc(sizeof(struct ray))) == 0x0) {
						disp("\n***\nOUT OF MEMORY\n***\n");
						return;
	}
	ray->length =  NUM_INITIAL_RAY_NODES;
	node1 = ray->ray = (struct node*)malloc(sizeof(struct node)); 
	node1->node = polar((double)INITIAL_RAY_LOWER_RAD, theta);
	
	for (i=1; i<NUM_INITIAL_RAY_NODES; ++i) {
		if ((node1->next =  (struct node *)malloc(sizeof(struct node))) == 0x0) {
						disp("\n***\nOUT OF MEMORY\n***\n");
						return;
		}
		node1->next->prev = node1;
		node1 = node1->next;
		r = INITIAL_RAY_LOWER_RAD + ((double)i/(double)NUM_INITIAL_RAY_NODES)*(double)(INITIAL_RAY_UPPER_RAD-INITIAL_RAY_LOWER_RAD);
		node1->node = polar(r, theta);
	}
	ray->end = node1;
	return(ray);	
}		

/*=========================== spider_init ======================*/
void spider_init(sp, deg, num11, num12, den1, num21, num22, den2)
struct spider *sp; long deg, num11, num12, den1, num21, num22, den2;
/* get orbit of angle num/den under map t mapsto dt	*/
/* pivot is  the index of the first periodic point */
{
	int i,j,k,n;

	sp->deg = (int)deg;
	sp->num[0][0] = num11; sp->num[0][1] = num12; sp->den[0] = den1;
	sp->num[1][0] = num21; sp->num[1][1] = num22; sp->den[1] = den2;
	
	sp->length[0] = sp->length[1] = 0;
	
	/* first set up the first critical point */
	i = 2;

	do {
		if (i == MAX_ORBIT_LENGTH) 	{ disp("\n ORBIT TOO LONG "); return; }
 	   sp->num[0][i] = (deg*sp->num[0][i-1]) % sp->den[0];
	   
	   
	   for (j=0; j<i; ++j) 
	   	if (sp->num[0][i] == sp->num[0][j]) break;
	} while (j == i++);
	sp->pivot[0] = j;
	sp->length[0] = i-1;


	if (sp->Num_Crit_Pts > 1) {
		/* now set up the second critical point. */
		i = 2;
	
		do {
			if (i == MAX_ORBIT_LENGTH) 	{ disp("\n ORBIT TOO LONG "); return; }
	 	   sp->num[1][i] = (deg*sp->num[1][i-1]) % sp->den[1];
		   sp->Second_Hits_First = 0;
		   for (j=0; j<sp->length[0]; ++j) /* check for hitting first orbit */
		   		if ((sp->den[0]*sp->num[1][i]) == (sp->den[1]*sp->num[0][j])) {
		   								disp("\n Second orbit hits first !");
		   								++i; 
		   								sp->Second_Hits_First = 1;
		   								goto a1;                  
		   							}
			
			if (sp->Second_Hits_First == 0) 
		      for (j=0; j<i; ++j) /* now check if we have hit a periodic angle */
		   		  if (sp->num[1][i] == sp->num[1][j]) break;
		   
		} while (j == i++);
a1:		sp->pivot[1] = j;
		sp->length[1] = i-1;
	}
	
	for (k=0; k<sp->Num_Crit_Pts; ++k) {
		sprintf(strng, "\nOrbit %d:\n",k);
		disp(strng);
		for (i=0; i<sp->length[k]; ++i) {
			if (i==0) 
				disp("(");
			else if (i==2) 
				disp("), "); 
			else 
				disp(", "); 

			if (sp->Second_Hits_First && sp->Num_Crit_Pts == 2) {
				if (sp->pivot[1] == i && k == 0)
					disp("Pivot1=");
				if (sp->pivot[0] == i && k == 0) 
					disp("Pivot0=");
			} else if (i==sp->pivot[k]) disp(" Pivot=");
			
			sprintf(strng, "%ld/%ld",ReducedNum(sp->num[k][i], sp->den[k]),ReducedDen(sp->num[k][i], sp->den[k]));
			disp(strng);
		}
		if (sp->length[k] == 2) disp(")");

		for (i=0; i<sp->length[k]; ++i) {
			sp->rays[k][i] = make_initial_leg(sp->num[k][i], sp->den[k]);
			check_ray(sp->rays[k][i]);
	
		}

		if (sp->length[k] > 2) /* set the critical value ray index */
			sp->crit_value[k] = 2;
		else
			sp->crit_value[k] = sp->pivot[k];
		
	}

	/* since the first two rays are to the critical point */
	/* countine rays is a bit funny.. hence the following */
	if (sp->pivot[0] == 0 ||  sp->pivot[0] == 1) {
		i = sp->pivot[0];
		j =  sp->length[0]-sp->pivot[0]-1;
	} else { 
		i = sp->pivot[0]-1;
		j =  sp->length[0]-sp->pivot[0];
	}
	if (num11 != num12) {
		i = sp->pivot[0];
		j =  sp->length[0]-sp->pivot[0];
	}

	disp("\n\n");
	if (sp->Num_Crit_Pts > 1) 
		disp("Two orbits in the ");

	sprintf(strng, "spider with %ld preperiodic and %ld periodic legs", i,j);
	disp(strng);

	if (sp->Num_Crit_Pts > 1) {
	sprintf(strng, "\n second spider has %ld preperiodic and %ld periodic legs", 
		sp->pivot[1], sp->length[1]-sp->pivot[1]);
	disp(strng);
	}


}

/*=========================== oldspider_init ======================*/
void oldspider_init(sp, deg, num11, num12, den1, num21, num22, den2)
struct spider *sp; long deg, num11, num12, den1, num21, num22, den2;
/* get orbit of angle num/den under map t mapsto dt	*/
/* pivot is  the index of the first periodic point */
{
	int i,j,k,n;

	sp->deg = (int)deg;
	sp->num[0][0] = num11; sp->num[0][1] = num12; sp->den[0] = den1;
	sp->num[1][0] = num21; sp->num[1][1] = num22; sp->den[1] = den2;
	
	sp->length[0] = sp->length[1] = 0;
	
	for (k=0; k<sp->Num_Crit_Pts; ++k) {
		i = 2;

		do {
			if (i == MAX_ORBIT_LENGTH) 	{ disp("\n ORBIT TOO LONG "); return; }
	 	   sp->num[k][i] = (deg*sp->num[k][i-1]) % sp->den[k];
		   for (j=0; j<i; ++j) 
		   	if (sp->num[k][i] == sp->num[k][j]) break;
		} while (j == i++);
		sp->pivot[k] = j;
		sp->length[k] = i-1;

	
		for (i=0; i<sp->length[k]; ++i) {
			sp->rays[k][i] = make_initial_leg(sp->num[k][i], sp->den[k]);
			check_ray(sp->rays[k][i]);
	
		}
	
		if (sp->length[k] > 2) /* set the critical value ray index */
			sp->crit_value[k] = 2;
		else
			sp->crit_value[k] = sp->pivot[k];
		
	}	

	sprintf(strng, "\nSpider with %ld + %ld legs", sp->length[0], sp->length[1]);
	disp(strng);


}


/*=========================== draw_ray ==============================*/
void draw_ray(ray, center_x, center_y, scale)
struct ray *ray; int center_x, center_y, scale;
/* this routine simply draws a ray into the currect graph port. the */
/*  center and scale variables determine the drawing position and size*/
{
	int x,y,i;
	int lx,ly;
	struct node *node;
	

	node = ray->ray;
	lx = (node->node.x*(double)scale) + center_x; 
	ly = -(node->node.y*(double)scale) + center_y;
	for (i=1; i<ray->length-1; ++i) {
		node = node->next;
		x = (node->node.x*(double)scale) + center_x;
		y = -(node->node.y*(double)scale) + center_y;
  		if (showticks) {
  			line(lx-3,ly, lx+3,ly,2); 
  			line(lx,ly-3, lx,ly+3,2); 
  		}
		line(lx,ly,x,y,1);   /* draw a segment in the ray */
		if (PenSize != 1) {
			line(lx,ly+1,x,y+1,1); 
			line(lx+1,ly,x+1,y,1); 
			line(lx,ly+2,x,y+2,1); 
			line(lx+2,ly,x+2,y,1); 
			line(lx,ly+3,x,y+3,1); 
			line(lx+3,ly,x+3,y,1); 
		}
		lx = x; ly = y; 
	}

}
	


/*=========================== draw_spider ==============================*/
void draw_spider(sp, center_x, center_y, scale)
struct spider sp; int center_x, center_y, scale;
/* this routine simply draws the spider into the currect graph port */
/* the center and scale variables determine the drawing position and */
/*size 															*/
{
	int i,j,k,n;
	struct ray *sector_ray;

	for (k=0; k<sp.Num_Crit_Pts; ++k) 
		for (n=0; n<sp.length[k]; ++n) {
/*			PenSize(n,n);*/
			draw_ray(sp.rays[k][n], center_x, center_y, scale);
		}
		
	if (sp.Num_Crit_Pts == 1 && d > 2)  {
			sector_ray = copy_ray(sp.rays[0][0]);
			rotate_ray(sector_ray, 2.0/(double)d);
		for (i=2; i<d; ++i) {
			draw_ray(sector_ray,center_x, center_y, scale);
			rotate_ray(sector_ray, 1.0/(double)d);
		}
		free_ray(sector_ray);
	}
}

/*=========================== closest_root ==============================*/
complex closest_root(a,b)
complex a,b;
/* find solution of z^d+C = a closest to b. C is a global constant*/
{
	double t,r, min;
	int 	mini,i;
	complex s;
	
	CSUB(a,C);
	t = comp_arg(a)/(double)d;

/*	if (!((0.0 <= t) && (t < TPI))) { ready for elimination
		disp ("\n\n\comp_arg ERROR ... \n\n");
		sprintf(strng, "\na = %lf,%lf\n",a.x,a.y);
		disp(strng);
		}
*/
	r = pow(comp_abs(a), 1.0/(double)d);
 	min = 100000000000000000.0;
 	for (mini = i = 0; i<d; ++i) {
		s = polar(r,t+TPI*(double)i/(double)d);
 		CSUB(s,b);
 		if ((s.x*s.x+s.y*s.y)<min) {
 			min = s.x*s.x+s.y*s.y;
 			mini = i;
 		}
 	}
 	return(polar(r,t+TPI*(double)mini/(double)d));
 }
 			
/*=========================== closest_root1 ==============================*/
complex closest_root1(a,b)
complex a,b;
/* find solution of z^3+Az+B = a closest to b. A,B are global constants*/
{
	double d1, d2, d3;
	complex z1,z2,z3, zz1, zz2, zz3, BB;
	
	CDIFF(BB, B, a);
	cubic_roots(A,BB,&z1,&z2,&z3);
 
/*
 	sprintf(strng, "\nROOTS ARE\n%lf+i%lf\n%lf+i%lf\n%lf+i%lf",z1.x, z1.y, z2.x, z2.y, z3.x, z3.y);
 	disp(strng); 



	zz1 = z1; CMUL(zz1,z1); CMUL(zz1, z1); 
 	CPROD(zz2, A, z1); CADD(zz1, zz2); CADD(zz1, BB);
 	sprintf(strng, "\nroot = %lf+i%lf",zz1.x, zz1.y);
 	disp(strng); 
	zz1 = z2; CMUL(zz1,z2); CMUL(zz1, z2); 
 	CPROD(zz2, A, z2); CADD(zz1, zz2); CADD(zz1, BB);
 	sprintf(strng, "\nroot = %lf+i%lf",zz1.x, zz1.y);
 	disp(strng); 
	zz1 = z3; CMUL(zz1,z3); CMUL(zz1, z3); 
 	CPROD(zz2, A, z3); CADD(zz1, zz2); CADD(zz1, BB);
 	sprintf(strng, "\nroot = %lf+i%lf",zz1.x, zz1.y);
 	disp(strng); 
*/

	CDIFF(zz1, z1, b);
	CDIFF(zz2, z2, b);
	CDIFF(zz3, z3, b);
	d1 = zz1.x*zz1.x + zz1.y*zz1.y;
	d2 = zz2.x*zz2.x + zz2.y*zz2.y;
	d3 = zz3.x*zz3.x + zz3.y*zz3.y;
	
	if (d1 <= d2 && d1 <= d3) return(z1); 
	else if (d2 <= d1 && d2 <= d3) return(z2); 
	else return(z3);
	 
 }
 			
/*=========================== lift_ray ==============================*/
struct ray *lift_ray(ray, num, den, num_crit_pts)
struct ray *ray; long num, den; int num_crit_pts;
/* lifts the ray to the ray with asymptotic angle num/den at infty  */
{
	double r,theta = TPI*(double)num/(double)den;
	int 	i;
	struct ray *new_ray;
	struct node *node, *old_ray_node;
	
	new_ray = (struct ray*)malloc(sizeof(struct ray)); 
	new_ray->length = ray->length+1;
	node = new_ray->end = (struct node*)malloc(sizeof(struct node)); 
	node->node = polar((double)INITIAL_RAY_UPPER_RAD, theta);
	old_ray_node = ray->end;
	
	for (i=0; i<ray->length; ++i) {
		node->prev = (struct node*)malloc(sizeof(struct node)); 
		node->prev->next = node;
		node = node->prev; 
		if (num_crit_pts == 1) 
			node->node = closest_root(old_ray_node->node, node->next->node);
		else
			node->node = closest_root1(old_ray_node->node, node->next->node);

		old_ray_node = old_ray_node->prev;
	}
	
	new_ray->ray = node;
/*	check_ray(new_ray); when debuggin we can check the ray */
	prune_ray(new_ray, wepsilon, fepsilon);
	return(new_ray);

}		


/*=========================== lift_X_spider ==============================*/
void lift_X_spider(sp1)
struct spider *sp1;
/* lift the extra spider.  */
{
	int i,j,n;
	
		sp1->pivotray[0] = copy_ray(sp1->rays[0][sp1->pivot[0]]);

		free_ray(sp1->rays[0][0]);
		free_ray(sp1->rays[0][1]);
		if (sp1->length[0] > 2) {
			sp1->rays[0][0] = lift_ray(sp1->rays[0][2], sp1->num[0][0], sp1->den[0], sp.Num_Crit_Pts);
			sp1->rays[0][1] = lift_ray(sp1->rays[0][2], sp1->num[0][1], sp1->den[0], sp.Num_Crit_Pts);
		} else {
			sp1->rays[0][0] = lift_ray(sp1->pivotray[0], sp1->num[0][0], sp1->den[0], sp.Num_Crit_Pts);
			sp1->rays[0][1] = lift_ray(sp1->pivotray[0], sp1->num[0][1], sp1->den[0], sp.Num_Crit_Pts);
		}

		for (i=2; i<sp1->length[0]-1; ++i) {
			free_ray(sp1->rays[0][i]);
			sp1->rays[0][i] = lift_ray(sp1->rays[0][i+1], sp1->num[0][i], sp1->den[0], sp.Num_Crit_Pts);
		}
		
		if (sp1->length[0] > 2) {
			free_ray(sp1->rays[0][i]);
			sp1->rays[0][i] = lift_ray(sp1->pivotray[0], sp1->num[0][i], sp1->den[0], sp.Num_Crit_Pts);
		}
		
		free_ray(sp1->pivotray[0]);
}
	 
/*=========================== oldlift_spider ==============================*/
void oldlift_spider(sp)
struct spider *sp;
/* lift a spider with one critical point */
{
	int i,j,k,n;
	
	for (k=0; k<sp->Num_Crit_Pts; ++k) {
		if (sp->length[k]<3) continue;
		sp->pivotray[k] = copy_ray(sp->rays[k][sp->pivot[k]]);
		/* the critical point rays are lifted already when we find A,B,C */

/*		free_ray(sp->rays[k][0]);
		free_ray(sp->rays[k][1]);
		if (sp->length[k] > 2) {
			sp->rays[k][0] = lift_ray(sp->rays[k][2], sp->num[k][0], sp->den[k], sp->Num_Crit_Pts);
			sp->rays[k][1] = lift_ray(sp->rays[k][2], sp->num[k][1], sp->den[k], sp->Num_Crit_Pts);
		} else {
			sp->rays[k][0] = lift_ray(sp->pivotray[k], sp->num[k][0], sp->den[k], sp->Num_Crit_Pts);
			sp->rays[k][1] = lift_ray(sp->pivotray[k], sp->num[k][1], sp->den[k], sp->Num_Crit_Pts);
		}
*/		
		for (i=2; i<sp->length[k]-1; ++i) {
			free_ray(sp->rays[k][i]);
			sp->rays[k][i] = lift_ray(sp->rays[k][i+1], sp->num[k][i], sp->den[k], sp->Num_Crit_Pts);
		}
		free_ray(sp->rays[k][i]);
		sp->rays[k][i] = lift_ray(sp->pivotray[k], sp->num[k][i], sp->den[k], sp->Num_Crit_Pts);
		free_ray(sp->pivotray[k]);
	}	
}
			
	 
void oldsetABC()
{
/* set the values of A,B, and C as needed. Also lift the rays
   to the critical values. for symmetry, we do this for both 
   the single and double critical point case  */
   
	complex z0, temp;
	struct ray *rays[2][2], *pivotray[2];
	double r,t, t1;
	int i,j,k;
	static int shift = 0;
	
	
	if (sp.Num_Crit_Pts == 1) {
		C = sp.rays[0][sp.crit_value[0]]->ray->node;
		for (i=0;i<2; ++i ) {
				if (sp.length[0] > 2)  /* if the critical value is #2 */
					rays[0][i] =  lift_ray(sp.rays[0][2], sp.num[0][i], sp.den[0],1);
				else 
					rays[0][i] = lift_ray(sp.rays[0][sp.crit_value[0]], sp.num[0][i], sp.den[0], 1);
		}
		
		for (j=0; j<2; ++j) {
			free(sp.rays[0][j]);
			sp.rays[0][j] = rays[0][j];
		}

	} else {
		CSUM(B, sp.rays[0][sp.crit_value[0]]->ray->node,sp.rays[1][sp.crit_value[1]]->ray->node);

		B.x *= 0.5;
		B.y *= 0.5;
		CDIFF(temp, sp.rays[1][sp.crit_value[1]]->ray->node, sp.rays[0][sp.crit_value[0]]->ray->node);
		
		r = -3.0*pow(comp_abs(temp)*0.25, 2.0/3.0);
		t = 2.0*(comp_arg(temp)/3.0 );

		
		for (k=0; k<3; ++k) {
			t1 = t + (double)(k+shift)*TPI/3.0  /*+2.0*TPI/3.0*/;
			A = polar(r,t1);
			
			for (i=0;i<2; ++i ) {
				for (j=0; j<2; ++j) {
					if (sp.length[i] > 2)  /* if the critical value is #2 */
						rays[i][j] =  lift_ray(sp.rays[i][2], sp.num[i][j], sp.den[i],2);
					else 
						rays[i][j] = lift_ray(sp.rays[i][sp.crit_value[i]], sp.num[i][j], sp.den[i], sp.Num_Crit_Pts);
				}
			}
			
			if (comp_equal(rays[0][0]->ray->node, rays[0][1]->ray->node)) 
				disp("\n First critical rays meet");
				
			if	(comp_equal(rays[1][0]->ray->node, rays[1][1]->ray->node)) 
				disp("\n Second critical rays meet");

			if (comp_equal(rays[0][0]->ray->node, rays[0][1]->ray->node) && 
				comp_equal(rays[1][0]->ray->node, rays[1][1]->ray->node)) {
						
						disp("\n GOT IT");
						shift += k;
						for (i=0;i<2; ++i ) 
							for (j=0; j<2; ++j) {
							free(sp.rays[i][j]);
							sp.rays[i][j] = rays[i][j];
						}
						return;
			} else {
						for (i=0;i<2; ++i ) 
							for (j=0; j<2; ++j) 
							free(rays[i][j]);
						disp("\n TRYING AGAIN");
			}
		}
	} 
} /* remember to free the legs */
			
/*=========================== lift_spider ==============================*/
void lift_spider(sp)
struct spider *sp;
/* lift a spider. The critical rays are already lifted in SetABC */
/* The pivot ray is created in SetABC - in case  it is a critical ray */
{
	int i,j,k,n;
	
	for (k=0; k<sp->Num_Crit_Pts; ++k) {
		if (sp->length[k]<3) continue; /* first two rays (landing at
		                                  the critical point) are lifted
		                                  in SetABC */
		

		for (i=2; i<sp->length[k]-1; ++i) {
			free_ray(sp->rays[k][i]);
			sp->rays[k][i] = lift_ray(sp->rays[k][i+1], sp->num[k][i], sp->den[k], sp->Num_Crit_Pts);
		}
		free_ray(sp->rays[k][i]);
		sp->rays[k][i] = lift_ray(sp->pivotray[k], sp->num[k][i], sp->den[k], sp->Num_Crit_Pts);
		free_ray(sp->pivotray[k]);
	}	
}
			

void setABC()
{
/* set the values of A,B, and C as needed. Also lift the rays
   to the critical values. for symmetry, we do this for both 
   the single and double critical point case  */
   
	complex z0, temp;
	struct ray *rays[2][2], *pivotray[2];
	double r,t, t1;
	int i,j,k;
	static int shift = 0;
	
	/* we first set the pivot ray - this is because we lift the critical */
	/* rays in this routine. We need to do that in order to find the correct */
	/* polynomial (in the cubic case) and there is no point in lifting */
	/* the rays twice. For symmetry, we lift the critical rays for 
	/* the one critical point case also. We set the pivot rays because
	/* it may be that it is a critical ray, and would get overwritten
	/* by the lifting. The Pivot ray is either the first periodic ray */
	/* or the ray in the first cycle which is hit by the second cycle */

	sp.pivotray[0] = copy_ray(sp.rays[0][sp.pivot[0]]);
	if (sp.Num_Crit_Pts == 2) {
		if (!sp.Second_Hits_First)  
			sp.pivotray[1] = copy_ray(sp.rays[1][sp.pivot[1]]);
		else 
			sp.pivotray[1] = copy_ray(sp.rays[0][sp.pivot[1]]);
	}
	
	if (sp.Num_Crit_Pts == 1) {
		C = sp.rays[0][sp.crit_value[0]]->ray->node;
		for (i=0;i<2; ++i ) {
				if (sp.length[0] > 2)  /* if the critical value is #2 */
					rays[0][i] =  lift_ray(sp.rays[0][2], sp.num[0][i], sp.den[0],1);
				else 
					rays[0][i] = lift_ray(sp.rays[0][sp.crit_value[0]], sp.num[0][i], sp.den[0], 1);
		}
		
		for (j=0; j<2; ++j) {
			free(sp.rays[0][j]);
			sp.rays[0][j] = rays[0][j];
		}

	} else {
		/*sometimes the critical value of the second critical */
		/* point is in the orbit or the first critical point */
		if (sp.Second_Hits_First && sp.length[1] == 2) {
			CSUM(B, sp.rays[0][sp.crit_value[0]]->ray->node,sp.rays[0][sp.crit_value[1]]->ray->node);
		} else {
			CSUM(B, sp.rays[0][sp.crit_value[0]]->ray->node,sp.rays[1][sp.crit_value[1]]->ray->node);
		}
		
		B.x *= 0.5;
		B.y *= 0.5;
		if (sp.Second_Hits_First && sp.length[1] == 2)  {
			CDIFF(temp, sp.rays[0][sp.crit_value[1]]->ray->node, sp.rays[0][sp.crit_value[0]]->ray->node);
		} else {
			CDIFF(temp, sp.rays[1][sp.crit_value[1]]->ray->node, sp.rays[0][sp.crit_value[0]]->ray->node);
		}
		
		r = -3.0*pow(comp_abs(temp)*0.25, 2.0/3.0);
		t = 2.0*(comp_arg(temp)/3.0 );

		
		for (k=0; k<3; ++k) {
			t1 = t + (double)(k+shift)*TPI/3.0  /*+2.0*TPI/3.0*/;
			A = polar(r,t1);
			
			for (i=0;i<2; ++i ) {
				for (j=0; j<2; ++j) {
					if (sp.length[i] > 2)  /* if the critical value is #2 */
						rays[i][j] =  lift_ray(sp.rays[i][2], sp.num[i][j], sp.den[i],2);
					else 
						if (sp.Second_Hits_First && sp.length[1] == 2)
							rays[i][j] = lift_ray(sp.rays[0][sp.crit_value[i]], sp.num[i][j], sp.den[i], sp.Num_Crit_Pts);
						else
							rays[i][j] = lift_ray(sp.rays[i][sp.crit_value[i]], sp.num[i][j], sp.den[i], sp.Num_Crit_Pts);
				}
			}
			
/*			if (comp_equal(rays[0][0]->ray->node, rays[0][1]->ray->node)) 
				disp("\n First critical rays meet");
				
			if	(comp_equal(rays[1][0]->ray->node, rays[1][1]->ray->node)) 
				disp("\n Second critical rays meet");
*/
			if (comp_equal(rays[0][0]->ray->node, rays[0][1]->ray->node) && 
				comp_equal(rays[1][0]->ray->node, rays[1][1]->ray->node)) {
						
/*						disp("\n GOT IT");*/
						shift += k;
						for (i=0;i<2; ++i ) 
							for (j=0; j<2; ++j) {
							free(sp.rays[i][j]);
							sp.rays[i][j] = rays[i][j];
						}
						return;
			} else if (k<2) {
						for (i=0;i<2; ++i ) 
							for (j=0; j<2; ++j) 
								free(rays[i][j]);
/*						disp("\n TRYING AGAIN");*/
			}
		}
		disp("\n I must appologize, Gentle User. ");
		disp(" I Had difficulties lifting the spider. ");
		disp(" You see, I couldn't find a way to get ");
		disp(" the critical rays to land at the same points, ");
		disp("which is pretty serious. We can continue.. but....");
		for (i=0;i<2; ++i ) /* put something there, anyway */
		for (j=0; j<2; ++j) {
			free(sp.rays[i][j]);
			sp.rays[i][j] = rays[i][j];
		}
	} 
} /* remember to free the legs */
	 
/* This program emulates relative drawing commands */
/* the static variables are set to the last drawing position */

Line(x,y,type) /* get rid of this eventually */
int x,y,type;
{
	static int x0,y0;

	switch (type) {
	
		case DRAWTO:
			line(x0,y0,x,y,1);

		case MOVETO:
			x0 = x;
			y0 = y;
			break;

		case DRAWFROM:
			line(x0,y0,x0+x,y0+y,1);
			break;
	}
}

/*================HilightNextRay ====================================*/
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
		
	PenMode =  PIX_XOR;
	PenSize = 3;

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
	PenSize = 1;
	PenMode = PIX_SRC;
}
/* ==============================dispABC ====================================*/
/* display the values of AB and C (as appropriate in the goings on window */
dispABC()
{
		
		/* SetPort(Cspace_Window); */
		if (sp.Num_Crit_Pts ==1)  {
			if (C.y < 0.0) 
				sprintf(strng, "\nC = %lf - i %lf", C.x, -C.y);
			else
				sprintf(strng, "\nC = %lf + i %lf", C.x, C.y);
			disp(strng);

/*
			if (d == 2) {
				MoveTo((int)(256.0*(C.x+2.0)/2.5), 256 - (int)(256.0*(C.y+1.25)/2.5));
				Line(4,0); Line(-8,0); Line(4,0);
				Line(0,4); Line(0,-8); Line(0,4);
			} else if (d == 3) {
				MoveTo((int)(256.0*(C.x+1.644)/3.15), 256 - (int)(256.0*(C.y+1.58)/3.15));
				Line(4,0); Line(-8,0); Line(4,0);
				Line(0,4); Line(0,-8); Line(0,4);
			}
			
*/
		} else {
			if (A.y < 0.0) 
				sprintf(strng, "\n\nA = %lf - i %lf", A.x, -A.y);
			else
				sprintf(strng, "\n\nA = %lf + i %lf", A.x, A.y);
			disp(strng);
			
			if (B.y < 0.0) 
				sprintf(strng, "\nB = %lf - i %lf\n", B.x, -B.y);
			else
				sprintf(strng, "\nB = %lf + i %lf\n", B.x, B.y);
			disp(strng);
		}

}



