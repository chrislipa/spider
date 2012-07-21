#include "spider.h"  

extern long num, den, d;
extern long Leading_Part[Num_Digits], Repeating_Part[Num_Digits];
double 	THETA;

extern char sec_str[255];

/* The following Routines are used by the Modal Dialog to read in a    	*/
/* repeating decimal or a fraction and a degree d for the branched map 	*/



/*------------------------------- XPwrY --------------------------------------*/
long XPwrY(x,y)
long x,y; 
{
	long z;
	y = abs(y);
	for (z=1; y>0; --y) z *= x;
	return(z);	
}


/*------------------------------- gcd ----------------------------------------*/
long gcd(a,b)
long a,b;
{
	if (b==0) 
		return(a);
	else
		return(gcd(b, a%b));
}

/*------------------------------- dec_to_frac --------------------------------*/
/*					Convert repeating decimal to fraction					  */

dec_to_frac(num,den,Leading_Part,Repeating_Part)
long *num, *den, Leading_Part[], Repeating_Part[];
{
	long m, n, g, den1, den2;
	int i;

	if (Repeating_Part[0] == 0) {
			Repeating_Part[0] = 1; 
			Repeating_Part[1] = 0; 
	}

	/*printf("\n in dec to frac num = %ld; den = %ld; Leading_Part[0] = %ld", *num, *den, Leading_Part[0]); 
*/	for (m=0, i = 1; i <= Leading_Part[0];  m *= d, m+=Leading_Part[i],++i) ;
	for (n=0, i = 1; i <= Repeating_Part[0];  n*=d, n+=Repeating_Part[i], ++i);

	*num = m * (XPwrY(d, Repeating_Part[0]) - 1) + n;
	den1 = XPwrY(d, Leading_Part[0]);
	den2 = XPwrY(d, Repeating_Part[0]) - 1;

	if ((g = gcd(*num, den1)) !=0) {
			*num /= g;
			den1 /= g;
	}
	if ((g = gcd(*num, den2)) != 0) {
			*num /= g;
			den2 /= g;
	}
	*den = den1*den2;
} 

/*------------------------------- frac_to_dec --------------------------------*/
/*					Convert fraction to repeating decimal					  */

frac_to_dec(num,den,Leading_Part,Repeating_Part)
long num, den, Leading_Part[], Repeating_Part[];
{

	long num1[Num_Digits], digits[Num_Digits];
	int i,j,k,Not_Repeating;

	i = 1;
	num1[0] = num;
	Not_Repeating = (den != 0); 	/*Are the fractions repeating ?*/
	while (Not_Repeating && i<Num_Digits) {
		digits[i] = (d * num1[i - 1]) / den;
		num1[i] = (num1[i - 1] * d) % den;

		for (j = 0; j <= i-1; ++j) 
		  	/*Check for recurring fraction */
			if (num1[j] == num1[i]) {
					Not_Repeating = FALSE;
					Leading_Part[0] = j;
					Repeating_Part[0] = i-j;
					for (k = 1; k<=j; ++k) 
							Leading_Part[k] = digits[k];
					for (k = 1; k<=i-j; ++k)
							Repeating_Part[k] = digits[k + j];
			}
		++i;
	}
	return((i!=Num_Digits) && den != 0);
 	/*return FALSE when the routine fails , ie #Pf >Num_Digits*/			
}

   		


/*--------------------- find_sector_string ------------------------------*/
void find_sector_string()
{
/*	compute a string for the modal window which contains the sector sequence */
	int i, sectr1, num_rays, sectr;
	double theta;
	
	theta = THETA = (double)num/(double)den;
	sectr1 = ((int)floor((double)d*THETA - THETA) + (int)d)	% d;
						/* THETA = num/den */
	num_rays = Leading_Part[0]+Repeating_Part[0];

	/* sec_str[0] = 2*num_rays-1; */ /* pascal style stings vestige*/

	sec_str[0] = ' '; 
	sec_str[1] = ' '; /* in case we are perdiodic in which */
	/* case we want sec_str[1] = ';' */

	for (i=1; i<=num_rays && i<127; ++i) {
		sectr = ((int)floor((double)d*theta -  THETA) + (int)d) % d;
		sec_str[2*i] = '0' + sectr-sectr1;
		sec_str[2*i+1] = ',';
		if (i==Leading_Part[0]) sec_str[2*i+1] = ';';
		theta *= (double)d;
	}
	sec_str[2*i-1] = '\0';

}

#ifdef KAKA

/*-------------------------- Set_Quad_Angles ------------------------------*/

int Set_Quad_Angles()
{
	int  			still_reading ,itemHit, typ, i, wn[6];
	Handle			textItem;
	DialogPtr		eqn_window;
	DialogRecord	eqn_DRecord;
	Rect			box;
	char			sn[6];
	long 			g;

	/* store old values in case of a cancel */
	temp_d = d; temp_num = num; temp_den = den; 
	
/* wn converts from the item numbers in the dialog box def to 1..5*/
	wn[1] = 11;
	wn[2] = 9;
  	wn[3] = 7;
	wn[4] = 3;
	wn[5] = 4;
    
  	if (d ==0) d = 2;
	if (den == 0) { den = 6; num = 1; }
	eqn_window = GetNewDialog(1000, &eqn_DRecord, -1L);
	
	SetPort(eqn_window);


	still_reading = TRUE;
	while (still_reading) {
		
		Int_to_Str(d,sn[1]);
    	Int_to_Str(num,sn[4]);
		Int_to_Str(den,sn[5]);

		for (i=1; i<=Leading_Part[0]; ++i)
          	 sn[2][i] = '0' + (int)Leading_Part[i];
		for (i=1; i<=Repeating_Part[0]; ++i) 
           	sn[3][i] =  '0' + (int)Repeating_Part[i];

 /*		set length for pascal type strings		*/
        sn[3][0] =  Repeating_Part[0];
        sn[2][0] =  Leading_Part[0];

		for (i=1; i<6; ++i) {
			GetDItem(eqn_window, wn[i], &typ, &textItem, &box);
			SetIText(textItem, sn[i]);
		}

		GetDItem(eqn_window, 14, &typ, &textItem, &box);
		find_sector_string();
		SetIText(textItem, sec_str);

		do
			ModalDialog(0L, &itemHit);
		while (itemHit !=1 && itemHit !=2 && itemHit != 5 && itemHit!= 6 && itemHit!= 16);
	        	
		if (itemHit == 5) { /* SET button of fraction */
			GetDItem(eqn_window, wn[1], &typ, &textItem, &box);
	     	GetIText(textItem, sn[1]);
	        d = Str_to_Int(sn[1]);
	
	        GetDItem(eqn_window, wn[4], &typ, &textItem, &box);
	     	GetIText(textItem, sn[4]);
	        num = Str_to_Int(sn[4]);
	        
	        GetDItem(eqn_window, wn[5], &typ, &textItem, &box);
	     	GetIText(textItem, sn[5]);
	        den = Str_to_Int(sn[5]);
	        
			/* reduce num and den */
			if (den != 0) num %=den;
			if ((g = gcd(num, den)) != 0) {
					num /= g; den/= g;
			}
			
	        if (!frac_to_dec(num, den, Leading_Part, Repeating_Part))
	        {Leading_Part[0] = Repeating_Part[0] = 1;
	        Leading_Part[1] = Repeating_Part[1] = 65530;}
	        /*check if the conversion was made properly */
		}
	
		if (itemHit == 6) {  /* SET button of repeating decimal */
			GetDItem(eqn_window, wn[1], &typ, &textItem, &box);
	 		GetIText(textItem, sn[1]);
			d = Str_to_Int(sn[1]);
	        
	        GetDItem(eqn_window, wn[2], &typ, &textItem, &box);
	     	GetIText(textItem, sn[2]);
	        Leading_Part[0] = (long)sn[2][0]; 
	        /*length from pascal type string*/
	        
	        for (i=1; i<=Leading_Part[0]; ++i)  
	                   Leading_Part[i] = (long)(sn[2][i] - '0');
	
			GetDItem(eqn_window, wn[3], &typ, &textItem, &box);
			GetIText(textItem, sn[3]);
	 		Repeating_Part[0] = (long)sn[3][0];
	
			for (i=1; i<=Repeating_Part[0]; ++i)
	                   Repeating_Part[i] = (long)(sn[3][i] - '0');
	                   
	
			dec_to_frac(&num, &den, Leading_Part, Repeating_Part);
	        if (!frac_to_dec(num, den, Leading_Part, Repeating_Part))
	        {Leading_Part[0] = Repeating_Part[0] = 1;
	        Leading_Part[1] = Repeating_Part[1] = 65530;}
	        /*check if the conversion was made properly */
		} 
	
		if (itemHit == 1 || itemHit == 2 || itemHit == 16)  still_reading = FALSE; 
	} /*while Still_reading do  */
	THETA = (double)num/(double)den;
 	CloseDialog(eqn_window);
 	if (itemHit == 16) { /* in case of cancel, use old values */
 		d = temp_d;
 		num = temp_num;
 		den = temp_den;
 	}
 	return(itemHit != 16);	
} 

/*--------------------------- Set_Cube_Angles ------------------------------*/

int Set_Cube_Angles()
{
	int  			still_reading ,itemHit, typ, i, wn[8];
	long			otherden1, otherden2, t_num11, t_num12, t_num21, t_num22, t_den1, t_den2;
	int			 	oneortwo, dx, dy, cx=120, cy=200,r=50, musthitset1, musthitset2;
	Handle			textItem[8];
	DialogPtr		eqn_window;
	DialogRecord	eqn_DRecord;
	Rect			box;
	Str255			sn[8];
	long 			g;

	/* store old values in case of a cancel */
	t_num11 = num11;
	t_num21 = num21;
	t_num12 = num12;
	t_num22 = num22;
	t_den1 = den1;
	t_den2 = den2;
	
/* wn converts from the item numbers in the dialog box def to 1..5*/
	wn[0] = 3;
	wn[1] = 4;
	wn[2] = 5;
  	wn[3] = 6;
	wn[4] = 7;
	wn[5] = 8;
	wn[6] = 9;
	wn[7] = 10;
    
	if (den1 == 0) { den1 = 6;num11 = 1;num12 = 3; }
	if (den2 == 0) { den2 = 9;num21 = 1;num22 = 7; }
	eqn_window = GetNewDialog(1101, &eqn_DRecord, -1L);
	
	SetPort(eqn_window);

	otherden1 = den1;
	otherden2 = den2;
	
	if ((g = gcd(num11, den1)) != 0) 
		num11 /= g; den1/= g;
	
	if ((g = gcd(num12, otherden1)) != 0) 
		num12 /= g; otherden1/= g;
	
	if ((g = gcd(num21, den2)) != 0) 
		num21 /= g; den2/= g;
	
	if ((g = gcd(num22, otherden2)) != 0) 
		num22 /= g; otherden2/= g;
	
	if (num12 > otherden1) num12 -= otherden1;
	
 	if (num22 > otherden2) num22 -= otherden2;
 	
   		Int_to_Str(num11,sn[0]);
    	Int_to_Str(den1,sn[1]);
    	Int_to_Str(num12,sn[2]);
    	Int_to_Str(otherden1,sn[3]);
    	Int_to_Str(num21,sn[4]);
    	Int_to_Str(den2,sn[5]);
    	Int_to_Str(num22,sn[6]);
    	Int_to_Str(otherden2,sn[7]);


		for (i=0; i<8; ++i) {
			GetDItem(eqn_window, wn[i], &typ, &textItem[i], &box);
			SetIText(textItem[i], sn[i]);
		}


		do {
			ModalDialog(0L, &itemHit);
			
			if (itemHit == 3 || itemHit == 4) musthitset1 = 1;

			if (itemHit == 7 || itemHit == 8) musthitset2 = 1;
			
			/* when fractions are changed, set flag to require
			   that the set buttons be hit before we can OKAY out */
			   
			if (itemHit == 1 || itemHit == 12 || itemHit == 13
				|| itemHit == 17 || itemHit == 18) { 
			/* Okay button or any button that requires reading the current setting */
				
				for (i=0; i<8; ++i) {
		     		GetIText(textItem[i], sn[i]);
		     	}
		     	
		     	num11 = Str_to_Int(sn[0]);
    			den1 = Str_to_Int(sn[1]);
    			num12 = Str_to_Int(sn[2]);
    			otherden1 = Str_to_Int(sn[3]);
    			num21 = Str_to_Int(sn[4]);
    			den2 = Str_to_Int(sn[5]);
    			num22 = Str_to_Int(sn[6]);
    			otherden2 = Str_to_Int(sn[7]);

			}
			
			if (itemHit == 12 || itemHit == 13) { /* add one third to first angle */
				musthitset1 = 0;
				if (itemHit == 12) 
					oneortwo = 1; 
				else 
					oneortwo = 2;
					
				if ((den1%3) == 0) {
					num12 = num11 + oneortwo*den1/3;
					otherden1 = den1;
				} else {
					num12 = 3*num11 + oneortwo*den1;
					otherden1 = 3*den1;
				}
				
				if (num12 > otherden1) num12 %= otherden1;
   				
   				Int_to_Str(num12,sn[2]);
				SetIText(textItem[2], sn[2]);
    			Int_to_Str(otherden1,sn[3]);
				SetIText(textItem[3], sn[3]);
			}

			if (itemHit == 17 || itemHit == 18) { /* add one third to first angle */
				musthitset2 = 0;
				if (itemHit == 17) 
					oneortwo = 1; 
				else 
					oneortwo = 2;
					
				if ((den2%3) == 0) {
					num22 = num21 + oneortwo*den2/3;
					otherden2 = den2;
				} else {
					num22 = 3*num21 + oneortwo*den2;
					otherden2 = 3*den2;
				}
				
				if (num22 > otherden2) num22 %= otherden2;
				
   				Int_to_Str(num22,sn[6]);
				SetIText(textItem[6], sn[6]);
    			Int_to_Str(otherden2,sn[7]);
				SetIText(textItem[7], sn[7]);
			}
	
			if (itemHit == 12 || itemHit == 13
				|| itemHit == 17 || itemHit == 18) { 
				SetPort(eqn_window);
				SetRect(&box, cx-r, cy-r, cx+r, cy+r);
				EraseRect(&box);
				FrameOval(&box);
				dx = (int)((double)r*cos(TPI*(double)num11/(double)den1));
				dy = (int)((double)r*sin(TPI*(double)num11/(double)den1));
				MoveTo(cx+dx, cy-dy);
				dx = (int)((double)r*cos(TPI*(double)num12/(double)otherden1));
				dy = (int)((double)r*sin(TPI*(double)num12/(double)otherden1));
				LineTo(cx+dx, cy-dy);

				dx = (int)((double)r*cos(TPI*(double)num21/(double)den2));
				dy = (int)((double)r*sin(TPI*(double)num21/(double)den2));
				MoveTo(cx+dx, cy-dy);
				dx = (int)((double)r*cos(TPI*(double)num22/(double)otherden2));
				dy = (int)((double)r*sin(TPI*(double)num22/(double)otherden2));
				LineTo(cx+dx, cy-dy);
			}
			
			if (itemHit ==1 && (musthitset1 == 1 || musthitset2 == 1))
				itemHit = 0;
				
		} while (itemHit !=2 && itemHit !=1);
	        	
	        
	        if (otherden1 == 3*den1) {
	        	den1 = otherden1;
	        	num11 *= 3;
	        }
	        
	        else if (3*otherden1 == den1) 
	        	num12 *= 3;
	        
	        
	        if (otherden2 == 3*den2) {
	        	den2 = otherden2;
	        	num21 *= 3;
	        }
		
 	        else if (3*otherden2 == den2) 
	        	num22 *= 3;
	        
		
 	if (itemHit == 2) { 
		 num11 = t_num11;
		 num21 = t_num21;
		 num12 = t_num12;
		 num22 = t_num22;
		 den1 = t_den1;
		 den2 = t_den2;
 	}


 	CloseDialog(eqn_window);
 	return(itemHit != 2);
} 


/*------------------------------- Set_Mate_Angles ------------------------------*/

/*
int Set_Mate_Angles()
{
	int  			itemHit, typ, i, wn[8];
	Handle			textItem[8];
	DialogPtr		eqn_window;
	DialogRecord	eqn_DRecord;
	Rect			box;
	Str255			sn[8];
	long 			g;

	
   	if (den1 == 0) { den1 = 7;num11 = 1;}
	if (den2 == 0) { den2 = 3;num22 = 1; }
	eqn_window = GetNewDialog(1201, &eqn_DRecord, -1L);
	
	SetPort(eqn_window);

	
	if ((g = gcd(num11, den1)) != 0) 
		num11 /= g; den1/= g;
	
		
	if ((g = gcd(num22, den2)) != 0) 
		num22 /= g; den2/= g;
	
 	
   		Int_to_Str(num11,sn[3]);
    	Int_to_Str(den1,sn[4]);
    	Int_to_Str(num22,sn[5]);
    	Int_to_Str(den2,sn[6]);


		for (i=3; i<7; ++i) {
			GetDItem(eqn_window, i, &typ, &textItem[i], &box);
			SetIText(textItem[i], sn[i]);
		}


		do 
			ModalDialog(0L, &itemHit);
		 while (itemHit !=2 && itemHit !=1);
	        	
		if (itemHit == 1) {				
			for (i=3; i<7; ++i) 
			     		GetIText(textItem[i], sn[i]);
			     	
			     	
			num11 = Str_to_Int(sn[3]);
	    	den1 = Str_to_Int(sn[4]);
	    	num22 = Str_to_Int(sn[5]);
	    	den2 = Str_to_Int(sn[6]);
		}
			

 	CloseDialog(eqn_window);
 	return(itemHit != 2);
} 
*/

#endif
