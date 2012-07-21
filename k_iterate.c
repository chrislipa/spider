#include "koebe.h"

extern gptr gp;
extern int width, iterations;

void newton(), calc_dfdc();
extern void (*line_to_routine)();
extern complex C;

void
setdist_m(c,iterations,escape,icnt,distance,seq,epsilon,gp)
	complex *c;
	int iterations;
	double escape;
	int *icnt;
	double *distance;
	complex *seq;
	double epsilon;
{
	complex dfdc;
	register double f_x,f_y;
	register double dfdc_x,dfdc_y;
	register double dfdz_x,dfdz_y;
	register double d2fdz2_x,d2fdz2_y;
	register double d2fdcdz_x,d2fdcdz_y;
	register double temp,size;
	register double A,B;
	int i;
	int p;
	complex result;
	
	*icnt = iterate2_m(&c->x,&c->y,iterations,&escape,seq);
	
	if (*icnt == iterations+1) {
		/* Never escaped.  Look for a periodic cycle. */
		p = find_cycle(seq,iterations,&epsilon);
		
		if (p == 0) {
			/* Couldn't find a periodic cycle. */
			*icnt = iterations+1;
			*distance = 0.0;
			return;
		}
		else {
			/* Found a periodic cycle of period p? */
			newton(&seq[iterations],p,&result,c);

			/* Check to see if the number that came out of
			   Newton's method is reasonably close
			   to the number that went in. */
			if (fabs(result.x - seq[iterations].x) > epsilon || 
					fabs(result.y - seq[iterations].y) > epsilon) {
				/* Guess of period was wrong */
				*icnt = iterations+1;
				*distance = 0.0;
				return;
			}
			
			f_x = result.x;
			f_y = result.y;
			dfdz_x = 1.0;
			dfdz_y = 0.0;
			dfdc_x = dfdc_y = 0.0;
			d2fdz2_x = d2fdz2_y = 0.0;
			d2fdcdz_x = d2fdcdz_y = 0.0;
			for (i = 1; i<=p; ++i) {
				/*
				 *          d d   j      d   j-1   d   j-1
				 * Compute: ---- f  =  2[-- f    * -- f    +
				 *          dcdz         dz        dc
				 *
				 *                           j-1   d d   j-1
				 *                          f    * ---- f   ]
				 *                                 dcdz
				 */
				temp = f_x * d2fdcdz_x - f_y * d2fdcdz_y;
				d2fdcdz_y = f_x * d2fdcdz_y + f_y * d2fdcdz_x;
				d2fdcdz_x = temp;
				d2fdcdz_x += dfdz_x * dfdc_x - dfdz_y * dfdc_y;
				d2fdcdz_y += dfdz_x * dfdc_y + dfdz_y * dfdc_x;
				d2fdcdz_x += d2fdcdz_x;
				d2fdcdz_y += d2fdcdz_y;
			
				/*
				 *          d d   j       d   j-1 2
				 * Compute: ---- f  =  2[(-- f   )  +
				 *          dzdz          dz
				 * 
				 *                           j-1   d d   j-1
				 *                          f    * ---- f   ]
				 *                                 dzdz
				 */
				temp = f_x * d2fdz2_x - f_y * d2fdz2_y;
				d2fdz2_y = f_x * d2fdz2_y + f_y * d2fdz2_x;
				d2fdz2_x = temp;
				d2fdz2_x += dfdz_x * dfdz_x - dfdz_y * dfdz_y;
				temp = dfdz_x * dfdz_y;
				d2fdz2_y += temp + temp;
				d2fdz2_x += d2fdz2_x;
				d2fdz2_y += d2fdz2_y;
			
				/*
				 *          d   j         j-1   d   j-1
				 * Compute: -- f  =  2 * f    * -- f
				 *          dz                  dz
				 */
				temp = f_x * dfdz_x - f_y * dfdz_y;
				temp += temp;
				dfdz_y = f_x * dfdz_y + f_y * dfdz_x;
				dfdz_y += dfdz_y;
				dfdz_x = temp;
			
				/*
				 *          d   j         j-1   d   j-1
				 * Compute: -- f  =  2 * f    * -- f    + 1
				 *          dc                  dc
				 */
				temp = f_x * dfdc_x - f_y * dfdc_y;
				temp += temp + 1.0;
				dfdc_y = f_x * dfdc_y + f_y * dfdc_x;
				dfdc_y += dfdc_y;
				dfdc_x = temp;
			
				/*
				 *           j      j-1 2
				 * Compute: f  =  (f   )  + c
				 *          
				 */
				temp = f_x * f_x - f_y * f_y + c->x;
				f_y = f_x * f_y;
				f_y += f_y + c->y;
				f_x = temp;
				
				/* See if this iterate is close to the first */
				if (i != p && fabs(f_x - result.x) < 1.0e-10 &&
					 fabs(f_y - result.y) < 1.0e-10) {
					/* Yes, the period is actually i, not p */
					p = i;
					break;
				}
			}
			
			/*
			 *                    d   p 2
			 * Compute: A = 1 - | -- f |
			 *                    dz
			 */
			A = 1.0 - (dfdz_x*dfdz_x + dfdz_y*dfdz_y);
			if (A < 0.0) {
				*distance = 0.0;
				*icnt = iterations+1;
				return;
			}
			
			/*                                      d   p
			 *                                      -- f
			 *               d d   p    d d   p     dc
			 * Compute: B = |---- f   + ---- f  * ----------|
			 *               dcdz       dzdz          d   p
			 *                                    1 - -- f
			 *                                        dz
			 */
			dfdz_x = 1.0 - dfdz_x;
			size = dfdz_x * dfdz_x + dfdz_y * dfdz_y;
			temp = dfdc_x * dfdz_x - dfdc_y * dfdz_y;
			dfdc_y = dfdc_x * dfdz_y + dfdc_y * dfdz_x;
			dfdc_x = temp;
			dfdc_x /= size;
			dfdc_y /= size;
			temp = d2fdz2_x * dfdc_x - d2fdz2_y * dfdc_y;
			d2fdz2_y = d2fdz2_x * dfdc_y + d2fdz2_y * dfdc_x;
			d2fdz2_x = temp;
			d2fdcdz_x += d2fdz2_x;
			d2fdcdz_y += d2fdz2_y;
			B = sqrt(d2fdcdz_x * d2fdcdz_x + d2fdcdz_y * d2fdcdz_y);

			*distance = 0.25 * A/B;
			*icnt = iterations+1;
			return;
		}
	}
	else {
		/* Point escaped.  Simply calculate radius of outside disk. */
		calc_dfdc(*icnt,seq,&dfdc);
		dfdc_x = dfdc.x;
		dfdc_y = dfdc.y;
		size = sqrt(seq[*icnt].x * seq[*icnt].x + seq[*icnt].y * seq[*icnt].y);
		*distance = 0.5*size*log(size)/sqrt(dfdc_x*dfdc_x + dfdc_y*dfdc_y);
		return;
	}
}

void
setdist_j(cx,cy,zx,zy,iterations,escape,icnt,distance)
	double cx,cy,zx,zy;
	int iterations;
	double escape;
	int *icnt;
	double *distance;
{
	double zx2,zy2;
	double dzx,dzy;
	int i;
	double temp,size;
	
	zx2 = zx * zx;
	zy2 = zy * zy;
	size = zx2 + zy2;
	dzx = 1.0;
	dzy = 0.0;
	if (size > escape) {
		size = sqrt(size);
		*distance = 0.5*size*log(size)/sqrt(dzx*dzx + dzy*dzy);
		*icnt = 0;
		return;
	}
	
	for (i = 1; i <= iterations; ++i) {
		temp = zx * dzx - zy * dzy;
		dzy = zx * dzy + zy * dzx;
		dzx = temp;
		dzx += dzx;
		dzy += dzy;
		
		zy = zx * zy;
		zy += zy + cy;
		zx = zx2 - zy2 + cx;
		
		zx2 = zx * zx;
		zy2 = zy * zy;
		size = zx2 + zy2;
		
		if (size > escape) {
			size = sqrt(size);
			*distance = 0.5*size*log(size)/sqrt(dzx*dzx + dzy*dzy);
			*icnt = i;
			return;
		}
	}
	
	*distance = 0.0;
	*icnt = iterations+1;
}
int
iterate_m(pcx,pcy,iterations,pescape)
	register double *pcx,*pcy;
	register int iterations;
	register double *pescape;
{
	register double zx,zy,zx2,zy2;
	register double cx,cy;
	register double escape;
	register int i;

	cx = *pcx;
	cy = *pcy;
	escape = *pescape;
	zx = zy = zx2 = zy2 = 0.0;

	for (i = 1; i<=iterations; ++i)
	{
		zy = zx*zy;
		zy += zy+cy;
		zx = zx2-zy2+cx;
		zx2 = zx*zx;
		zy2 = zy*zy;
		if (zx2+zy2 > escape)
			return i;
	}

	return iterations+1;
}

int
iterate2_m(pcx,pcy,iterations,pescape,seq)
	register double *pcx,*pcy;
	register int iterations;
	register double *pescape;
	register double *seq;
{
	register double zx,zy,zx2,zy2;
	register double cx,cy;
	register double escape;
	register int i;
	
	cx = *pcx;
	cy = *pcy;
	zx = zy = zx2 = zy2 = 0.0;
	*seq++ = zx;
	*seq++ = zy;
	escape = *pescape;

	i = iterations;
	while (i != 0) {
		zy = zx*zy;
		zy += zy+cy;
		zx = zx2-zy2+cx;
		zx2 = zx*zx;
		zy2 = zy*zy;
		*seq++ = zx;
		*seq++ = zy;
		if (zx2+zy2 <= escape) {
			--i;
			continue;
		}
		else
			return iterations-i+1;
	}

	return iterations+1;
}

void
calc_dfdc(icnt,seq,pdfdc)
	register int icnt;
	register complex *seq;
	register complex *pdfdc;
{
	register double dfdcx,dfdcy;
	register double temp;
	register int i;
	register int one = 1.0;
	register double seqx,seqy;

	dfdcx = dfdcy = 0.0;

	for (i = 1; i<=icnt; ++i, ++seq) {
		seqy = seq->y;
		seqx = seq->x;
		temp = seqx * dfdcx - seqy * dfdcy;
		temp += temp + one;
		dfdcy = seqx * dfdcy + seqy * dfdcx;
		dfdcy += dfdcy;
		dfdcx = temp;
	}

	pdfdc->x = dfdcx;
	pdfdc->y = dfdcy;
}

int
find_cycle(seq,iterations,pepsilon)
	register double *seq;
	register int iterations;
	register double *pepsilon;
{
	register double lowerx,lowery,upperx,uppery;
	register double epsilon;
	register double seqx,seqy;
	register int i;

	epsilon = *pepsilon;
	seq = &seq[2*(iterations+1)];
	lowerx = *--seq - epsilon;
	upperx = *seq + epsilon;
	lowery = *--seq - epsilon;
	uppery = *seq + epsilon;

	for (i = iterations-1; i >= 0; --i) {
		seqx = *--seq;
		seqy = *--seq;
		if (seqx > lowerx && seqx < upperx &&
				seqy > lowery && seqy < uppery)
			return iterations-i;
	}

	return 0;
}

int
iterate_j(pcx,pcy,pzx,pzy,iterations,pescape)
	double *pcx,*pcy,*pzx,*pzy;
	register int iterations;
	register double *pescape;
{
	register double zx,zy,zx2,zy2;
	register double cx,cy;
	register double escape;
	register int i;

	cx = *pcx;
	cy = *pcy;
	zx = *pzx; 
	zy = *pzy;
	escape = *pescape;
	zx2 = zx*zx;
	zy2 = zy*zy;

	if (zx2+zy2 > escape)
		return 0;
	
	for (i = 1; i<=iterations; ++i) {
		zy = zx*zy;
		zy += zy+cy;
		zx = zx2-zy2+cx;
		zx2 = zx*zx;
		zy2 = zy*zy;
		if (zx2+zy2 > escape)
			return i;
	}

	return iterations+1;
}


static void
push_search_coord1(gp,sset,snum,i,j)
	gptr gp;
{
	struct kqt_stack *stack;
	struct kqt_datum *datum,*temp;
	long size;
	
	stack = &gp->pict.stack[sset][snum];
	
	if (stack->size == stack->maxsize) {
		if (0 && stack->size < 1000) {
			temp = (struct kqt_datum *)realloc(stack->data,
				(stack->maxsize+EXPANSION)*
				sizeof(struct kqt_datum));
			if (temp == NULL) return;
			stack->data = temp;
			stack->maxsize += EXPANSION;
		}
		else
			return;
	}
	
	datum = &stack->data[stack->size++];
	datum->i = i;
	datum->j = j;
}

static void
push_search_coord(gp,sset,key,i,j)
	gptr gp;
{
	int snum;
	
	if ((unsigned)i >= gp->pict.width || (unsigned)j >= gp->pict.width ||
		gp->icnt[j*gp->pict.width+i] != 0xff)
		return;
			
	for (snum = 0; snum < NSTACKS-1; ++snum)
		if (key <= gp->pict.stack[sset][snum].maxradius) {
			push_search_coord1(gp,sset,snum,i,j);
			return;
		}
	
	push_search_coord1(gp,sset,NSTACKS-1,i,j);
}

static int
pop_search_coord1(gp,sset,snum,i,j)
	gptr gp;
	int sset,snum;
	int *i,*j;
{
	struct kqt_stack *stack;
	struct kqt_datum *datum,*temp;

	stack = &gp->pict.stack[sset][snum];
	
	if (stack->size == 0) {
		/* stack would underflow; reduce stack size */
		if (0 && stack->maxsize > EXPANSION) {
			temp = (struct kqt_datum *)realloc(stack->data,
				sizeof(struct kqt_datum)*EXPANSION);
			if (temp == NULL) return 0;
			stack->data = temp;
			stack->maxsize = EXPANSION;
		}
		return 0;
	}
	
	datum = &stack->data[--stack->size];
	*i = datum->i;
	*j = datum->j;
	return 1;
}

static int
pop_search_coord(gp,sset,i,j)
	gptr gp;
	int *sset,*i,*j;
{
	int snum;
	
	for (*sset = 1; *sset>=0 ; --*sset)	
		for (snum = NSTACKS-1; snum>=0; --snum)
			if (pop_search_coord1(gp,*sset,snum,i,j)) return 1;
	
	return 0;
}

static void
fill_icnt(gp,i1,i2,j,icnt)
	gptr gp;
{
	register unsigned short *s;
	register unsigned char *c;
	
	if ((unsigned)j > gp->pict.width-1) return;
	if (i1 < 0) i1 = 0;
	if (i2 > gp->pict.width-1) i2 = gp->pict.width-1;
	
		c = &gp->icnt[j*gp->pict.width+i1];
		i2 -= i1;
		for (; i2 >= 0; --i2)
			*c++ = icnt%254;
}

void
fast_draw()
{
}

static void
lines(gp,i,j,x,y,icnt,color)
	gptr gp;
{
	int left1,right1;
	int left2,right2;
	
	left1 = i-x;
	right1 = i+x;
	left2 = i-y;
	right2 = i+y;
	
	fill_icnt(gp,left1,right1,j+y,icnt);
	fill_icnt(gp,left1,right1,j-y,icnt);
	fill_icnt(gp,left2,right2,j+x,icnt);
	fill_icnt(gp,left2,right2,j-x,icnt);
	
	fast_draw(left1,right1,j-y,color);
	fast_draw(left1,right1,j+y,color);
	fast_draw(left2,right2,j-x,color);
	fast_draw(left2,right2,j+x,color);

	move_to(left1,j-y); (*line_to_routine)(right1,j-y);
	move_to(left1,j+y); (*line_to_routine)(right1,j+y);
	move_to(left2,j-x); (*line_to_routine)(right2,j-x);
	move_to(left2,j+x); (*line_to_routine)(right2,j+x);
}

static void
fill_disk(gp,i,j,radius,icnt)
	gptr gp;
{
	int color;
	int x,y,d;
	
	color = gp->icnt2color[icnt%COLOR_MAP_SIZE];
	
	set_forecolor(color);
	set_fmode(COPY);

	/* printf("icn color = %d %d\n\n", icnt, color); */
	switch (radius) {
	case 0:
		gp->icnt[j*gp->pict.width+i] = icnt%254;
		fast_draw(i,i,j,color);
		move_to(i,j); (*line_to_routine)(i,j);
		break;
	default:
		x = 0;
		y = radius;
		d = 3-2*radius;
		while (x<y) {
			lines(gp,i,j,x,y,icnt,color);
			if (d<0)
				d += (x<<2)+6;
			else {
				d += ((x-y)<<2)+10;
				--y;
			}
			++x;
		}
		if (x==y) lines(gp,i,j,x,y,icnt,color);
		break;
	}
}

int
kqt_stack_depth(gp)
	gptr gp;
{
	int i,j,sum;
	
	sum = 0;
	for (i = 0; i<=1; ++i)
		for (j = 0; j<NSTACKS; ++j)
			sum += gp->pict.stack[i][j].size;

	return sum;
}

int
step_kqt(gp)
	register gptr gp;
{
	complex v;
	double dist;
	int radiusi,i,j,sset;
	double radiusx;
	int icnt;
	int scanning_mode;
	
	if (pop_search_coord(gp,&sset,&i,&j)) {
		if (gp->icnt[j*gp->pict.width+i] == 0xff) {
			v.x = xp2v(gp,i);
			v.y = yp2v(gp,j);
		
			switch (gp->pict.mode) {
			case KQT_M:
				setdist_m(&v,gp->pict.iterations,
					gp->pict.escape,
					&icnt,&dist,
					gp->pict.seq,
					(2.0*gp->pict.radius)/
					gp->pict.width * 1.0e-2);
				break;
			case KQT_J:
				setdist_j(gp->pict.c.x,gp->pict.c.y,v.x,v.y,
					gp->pict.iterations,
					gp->pict.escape,&icnt,&dist,
					*gp->pict.seq);
				break;
			}
			
			/* Cover up the mistakes in the calculation
			   of the period */
			if (/* classdouble(dist)<=INFINITE || */
					dist < 0.0 || dist > gp->pict.radius)
				dist = 0.0;
			
			radiusx = dist * gp->pict.width / (2.0*gp->pict.radius);
			radiusi = (int)radiusx;
			
			if (radiusx < gp->pict.delta || icnt == gp->pict.iterations+1)
				/* icnt = gp->pict.iterations+1; */
					icnt = 0;
			else 
					icnt = BLACK; /* new for b/w display */

			fill_disk(gp,i,j,radiusi,icnt);
			
			if (radiusx >= gp->pict.recur) {
				++radiusi;
				push_search_coord(gp,sset,radiusi,
					i,j+radiusi);
				push_search_coord(gp,sset,radiusi,
					i,j-radiusi);
				push_search_coord(gp,sset,radiusi,
					i+radiusi,j);
				push_search_coord(gp,sset,radiusi,
					i-radiusi,j);
				radiusi = (int)((double)radiusi/
					sqrt(2.0));
				push_search_coord(gp,sset,radiusi,
					i+radiusi,j+radiusi);
				push_search_coord(gp,sset,radiusi,
					i+radiusi,j-radiusi);
				push_search_coord(gp,sset,radiusi,
					i-radiusi,j+radiusi);
				push_search_coord(gp,sset,radiusi,
					i-radiusi,j-radiusi);
			}
		}
	}
	else {
		/* scanning mode */
			register unsigned char *c;
			register int i,width;

			i = gp->pict.i;
			width = gp->pict.width;
			c = &(gp->icnt)[gp->pict.j*width+i];
			while (1) {
				/* i,j is now at last examined position.
				   Thus we increment, then examine. */
				if (++i == width) {
					i = 0;
					++gp->pict.j;
					if (gp->pict.j == width) {
						gp->pict.done = 1;
						return 1;
					}
				}

				if (*++c == 0xff) {
					push_search_coord(gp,0,0,
						i,gp->pict.j);
					gp->pict.i = i;
					return 0;
				}
			}
	}
	
	return 0;
}

static void
newton(x0,p,presult,c)
	complex *x0;
	int p;
	complex *presult,*c;
{
	int i,j;
	register double f_x,f_y;
	register double dfdz_x,dfdz_y;
	register double x_x,x_y;
	register double c_x,c_y;
	register double temp,size;
	
	x_x = x0->x;
	x_y = x0->y;
	c_x = c->x;
	c_y = c->y;
	
	for (i = 0; i<5; ++i) {
		/*                        p               p
		 * Compute: x    = x  - [f (x ) - x ] / df dz (x )
		 *           n+1    n        n     n            n
		 */
		 
		 f_x = x_x;
		 f_y = x_y;
		 dfdz_x = 1.0;
		 dfdz_y = 0.0;
		 for (j = 1; j<=p; ++j) {
				/*
				 *          d   j         j-1   d   j-1
				 * Compute: -- f  =  2 * f    * -- f
				 *          dz                  dz
				 */
				temp = f_x * dfdz_x - f_y * dfdz_y;
				dfdz_y = f_x * dfdz_y + f_y * dfdz_x;
				dfdz_y += dfdz_y;
				dfdz_x = temp+temp;
			
				/*
				 *           j      j-1 2
				 * Compute: f  =  (f   )  + c
				 *          
				 */
				temp = f_x * f_x - f_y * f_y;
				f_y = f_x * f_y;
				f_y += f_y + c_y;
				f_x = temp + c_x;
		}
		 
		f_x -= x_x;
		f_y -= x_y;
		size = dfdz_x * dfdz_x + dfdz_y * dfdz_y;
		temp = f_x * dfdz_x + f_y * dfdz_y;
		f_y = f_y * dfdz_x - f_x * dfdz_y;
		f_x = temp;
		f_x /= size;
		f_y /= size;
		x_x -= f_x;
		x_y -= f_y;
	}
	
	presult->x = x_x;
	presult->y = x_y;
}


void
check_for_null(p,s)
	char *p,*s;
{
	if (p == NULL) {
		fprintf(stderr,"%s: NULL pointer returned by alloc",s);
		exit(1);
	}
}

double
xp2v(gp,px)
	gptr gp;
	int px;
{
	return gp->pict.center.x-gp->pict.radius+
		(double)px/(gp->pict.width-1)*
		(2.0*gp->pict.radius);
}

double
yp2v(gp,py)
	gptr gp;
	int py;
{
	return gp->pict.center.y+gp->pict.radius-
		(double)py/(gp->pict.width-1)*
		(2.0*gp->pict.radius);
}

int
xv2p(gp,vx)
	gptr gp;
	double vx;
{
	return( (int)
		((vx-(gp->pict.center.x-gp->pict.radius))/(2.0*gp->pict.radius)
		*(double)(gp->pict.width-1)));
}

double
xv2pr(gp,vx)
	gptr gp;
	double vx;
{
	return (vx-(gp->pict.center.x-gp->pict.radius))/
		(2.0*gp->pict.radius)*(gp->pict.width-1);
}

int
yv2p(gp,vy)
	gptr gp;
	double vy;
{
	return ( (int)
		((gp->pict.center.y+gp->pict.radius-vy)/(2.0*gp->pict.radius)
		*(double)(gp->pict.width-1)));
}

double
yv2pr(gp,vy)
	gptr gp;
	double vy;
{
	return (gp->pict.center.y+gp->pict.radius-vy)/
		(2.0*gp->pict.radius)*(gp->pict.width-1);
}

static int
larger_power_of_2(in)
{
	int out;
	
	for (--in, out = 1; in > 0; in >>= 1, out <<= 1);
	return out;
}

void
resize(pleft,ptop,pright)
{
	int i,j;
	double vleft,vtop,vright;
	unsigned char *c;

	vleft = xp2v(gp,pleft);
	vtop = yp2v(gp,ptop);
	vright = xp2v(gp,pright);

	gp->pict.done = 0;
	gp->pict.radius = (vright - vleft)/2.0;
	gp->pict.center.x = vleft + gp->pict.radius;
	gp->pict.center.y = vtop - gp->pict.radius;
	gp->pict.i = -1;
	gp->pict.j = 0;
	for (i = 0; i <= 1; ++i)
		for (j = 0; j < NSTACKS; ++j)
			gp->pict.stack[i][j].size = 0;

		c = gp->icnt;
		for (i = gp->pict.width*gp->pict.width-1; i>=0; --i)
			*c++ = 255;
}

gptr
create_gstruct()
{
	gptr gp;
	int i,j;
	unsigned char *c;

	gp = (gptr)malloc(sizeof(gstruct));
	check_for_null(gp,"gp");
	gp->pict.done = 0;
	gp->pict.width = width;
	gp->pict.iterations = iterations;
	gp->pict.escape = 20.0;
	gp->pict.center.x = -0.7;
	gp->pict.center.y = 0.0;
	gp->pict.radius = 1.4;
	gp->pict.c.x = -0.11;
	gp->pict.c.y = 0.6557;
	gp->pict.delta = 0.15;
	gp->pict.recur = 0.1;

	gp->pict.mode = KQT_M;
	gp->pict.i = -1;
	gp->pict.j = 0;
	gp->pict.seq = (complex *)malloc((MAXITERATIONS+1)*sizeof(complex));
	check_for_null(gp->pict.seq,"seq");
	for (i = 0; i <= 1; ++i) {
		gp->pict.stack[i][0].maxradius = 2;
		gp->pict.stack[i][1].maxradius = 10;
		for (j = 0; j < NSTACKS; ++j) {
			gp->pict.stack[i][j].maxsize = EXPANSION;
			gp->pict.stack[i][j].size = 0;
			gp->pict.stack[i][j].data = (struct kqt_datum *)malloc(
				gp->pict.stack[i][j].maxsize*
				sizeof(struct kqt_datum));
			check_for_null(gp->pict.stack[i][j].data,"data");
		}
	}


		gp->icnt = c = (unsigned char *)malloc(gp->pict.width*
			gp->pict.width);
		check_for_null(gp->icnt,"icnt");

		for (i = gp->pict.width*gp->pict.width-1; i>=0; --i)
			*c++ = 255;

	gp->icnt2color = (short *)malloc(sizeof(short)*(COLOR_MAP_SIZE+1+1));
	check_for_null(gp->icnt2color,"icnt2color");

	return gp;
}

mark_m_space()
{
	int x,y;
	
	if (gp->pict.mode == KQT_M) { 
			x = xv2p(gp,C.x); 
			y = yv2p(gp,C.y);

			set_forecolor(BLACK);
			set_fmode(XOR);
			move_to(x-2,y); (*line_to_routine)(x+2,y);
			move_to(x,y-2); (*line_to_routine)(x,y+2);
			move_to(x,y); (*line_to_routine)(x,y);
	}
}
