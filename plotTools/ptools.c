/*************************************************************************/
//
// PROJECT	   = QuickPlot
// FILE		   = ptools.c
// VERSION	   = 2.0
// COMPANY	   = Apropos Software and Associates
// AUTHOR	   = T.Vernon
// DATE		   = 11/10/92
// DESCRIPTION =
//
//
/*************************************************************************/
/*************************************************************************/
//
//	   Part of (QuickPlot)
//
//	  Copyright Assignment 1994: NASA DFRC Assignee.
//
//	   NOTICE : Use of this software in source or object format is	
//				by written consent of Dryden Flight Research Center.
//
/*************************************************************************/
/* $Id: ptools.c,v 1.4 2009/02/03 21:49:09 rdees Exp $ */


/*************************************************************************/
/** ptools:includes														**/
/*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "ptools.h"
#include "dyn_string.h"



/*************************************************************************/
/** LinearInterpolate:							**/
/**		interpolate screen coordinates.					**/
/*************************************************************************/
int LinearInterpolate(double d,double dmin,double dmax,int imin,int imax)
{
		int	   TotalRange,ivalue;
		double TotalDataRange;
		double Percent;
		double AmountToAdd;
		double fvalue;

		TotalRange	   = imax - imin;
		TotalDataRange = dmax - dmin;

		Percent		   = (d - dmin) / TotalDataRange;

		AmountToAdd	   = Percent * TotalRange;

		fvalue		   = imin + AmountToAdd;

		ivalue = fvalue;

		return(ivalue);
}

/*************************************************************************/
/** LinearInterpolateD:							**/
/**		interpolate print coordinates.					**/
/*************************************************************************/
double LinearInterpolateD(double d,double dmin,double dmax,double min,double max)
{
		double TotalRange;
		double TotalDataRange;
		double Percent;
		double AmountToAdd;
		double fvalue;

		TotalRange	   = max  - min;
		TotalDataRange = dmax - dmin;

		Percent		   = (d - dmin) / TotalDataRange;

		AmountToAdd	   = Percent * TotalRange;

		fvalue		   = min + AmountToAdd;

		return(fvalue);
}

/*************************************************************************/
/* ReverseLinearInterpolate:						**/
/** Convert screen coordinates to real coordinates			**/
/**									**/
/*************************************************************************/
double	ReverseLinearInterpolate(int Value,int TotalRange,double DataMin,double DataMax)
{
		double TotalDataRange;
		double Percent;
		double AmountToAdd;
		double NewValue;

		TotalDataRange = DataMax - DataMin;

		Percent		   = (double)Value / (double)TotalRange;

		AmountToAdd	   = Percent * TotalDataRange;

		NewValue	   = DataMin + AmountToAdd;

		return(NewValue);
}

/*************************************************************************/
/** EU_exponent:							**/
/** Determine exponent given a number				**/
/**									**/
/*************************************************************************/
int PTOOLS_EU_exponent(double d)
{
	double x;
	int	   EUexp;
		
	x = fabs(d);
	
	x = nlog10(x);

	x = x / 3.0;

	x = floor(x);

	x = x * 3;

	EUexp = (int) x;

	return(EUexp);
}

/*************************************************************************/
/** EU_sprint:								**/
/** do an sprintf scaling for the exponent.				**/
/**									**/
/*************************************************************************/
void EU_sprint(char *str, double val, int exp, int before, int after)
{
//		  int c;
	char formatString[100];

	val = PTOOLS_Scale(val, exp);

	sprintf(formatString,"%%.%dlf",after);

	sprintf(str,formatString,val);
}

/*************************************************************************/
/** PTOOLS_Scale:							**/
/** Scale a number to a given exponent				**/
/**									**/
/*************************************************************************/
double PTOOLS_Scale(double val, int exp)
{
	int c;

	if (exp < 0) {
				for (c=exp; c<0; c++) {
						val *= 10.0;
				}
		} else {
				for (c=0; c<exp; c++) {
						val /= 10.0;
				}
		}

	return(val);
}

/*************************************************************************/
/** EU_noexpsprint:							**/
/** sprintf without exponent scaling				**/
/**									**/
/*************************************************************************/
void EU_noexpsprint(char *str, double val, int exp, int after)
{
	char formatString[100];

	if (exp < 0) {
		sprintf(formatString,"%%.%dlf",after-exp);
	}
	else {
		sprintf(formatString,"%%.%dlf",after);
	}
				
	sprintf(str,formatString,val);
}


/*************************************************************************/
/** PTOOLS_EU_logsprint							**/
/**									**/
/**									**/
/*************************************************************************/
void PTOOLS_EU_logsprint(char *str, double val)
{
		int num = (int) log10(val);
	sprintf(str,"%d",num);
}

/*************************************************************************/
/** PTOOLS_Log_Places							**/
/**									**/
/**									**/
/*************************************************************************/
int PTOOLS_Log_Places(double val)
{
		int num = (int) log10(val);

	num = num * -1;

	if (num > 0) return(num);

	return(0);
}

/*************************************************************************/
/** RoundDown10:							**/
/**		Round down to nearest decade.					**/
/**									**/
/*************************************************************************/
double RoundDown10(double d,int *integer_part,int *power_part)
{
	double dlog,fract,i;
	int	   stop;
	int	   c,tempint;


		dlog = nlog10(d);

		if (dlog >= 0) {

				stop = dlog;

				for (c=1; c<=stop; c++) {
						d = d / 10;
				}
		
		fract  = modf((double) d,&i);
		if (fract > .999999) {
			i++;
		}
		
		tempint = i;

		if (tempint == 10) {
			*integer_part = 1;
			*power_part	  = stop + 1;
		} else {
			*integer_part = tempint;
			*power_part	  = stop;
		}

		return(d);

		} else {

				stop = (int) fabs(dlog) + 1;

				for (c=1; c<=stop; c++) {
						d = d * 10;
				}

		fract  = modf((double) d,&i);
		if (fract > .999999) {
			i++;
		}

		tempint = i;

		if (tempint == 10) {
			*integer_part = 1;
			*power_part	  = stop - 1;
		} else {
			*integer_part = tempint;
			*power_part	  = stop;
		}

		*power_part = *power_part * -1;

		return(d);
		}
}

/*************************************************************************/
/** RoundUp10:								**/
/** Round up to nearest decade					**/
/**									**/
/*************************************************************************/
double RoundUp10(double d,int *integer_part,int *power_part)
{
	double dlog,fract,i;
	int	   stop;
	int	   c,tempint;

		dlog = nlog10(d);

		if (dlog >= 0) {

				stop = dlog;

				for (c=1; c<=stop; c++) {
						d = d / 10;
				}
		
		fract  = modf((double) d,&i);
		if (fract > .000001) {
			i++;
		}
		
		tempint = i;

		if (tempint == 10) {
			*integer_part = 1;
			*power_part	  = stop + 1;
		} else {
			*integer_part = tempint;
			*power_part	  = stop;
		}

		return(d);

		} else {

				stop = (int) fabs(dlog) + 1;

				for (c=1; c<=stop; c++) {
						d = d * 10;
				}

		fract  = modf((double) d,&i);
		if (fract > .000001) {
			i++;
		}
		
		tempint = i;

		if (tempint == 10) {
			*integer_part = 1;
			*power_part	  = stop - 1;
		} else {
			*integer_part = tempint;
			*power_part	  = stop;
		}
		
		*power_part = *power_part * -1;

		return(d);
		}
}

/*************************************************************************/
/** CountDecades:							**/
/** Count the number decades that a min and max span inclusive	**/
/**									**/
/*************************************************************************/
int CountDecades(int pmin, int pmax)
{
	int decades; 

	if ((pmin < 0) && (pmax >= 0)) {
		return(pmax-pmin+1);
	}

	if (pmin < 0) {
		pmin++;
	}

	if (pmax < 0) {
		pmax++;
	}

	decades = pmax - pmin + 1;

	return(decades);
}

/*************************************************************************/
/** CountTicks:								**/
/** Count the number of ticks that will be visible on the log	**/
/** scale plot, for a given min and max				**/
/**									**/
/*************************************************************************/
int CountTicks(int imin,int pmin,int imax,int pmax)
{
	int decades,ticks,ticks_first,ticks_last,c;

	decades = CountDecades(pmin,pmax);

	if (decades == 1) {
		ticks = imax - imin + 1;
		return(ticks);
	} 

	ticks_first = 9 - imin;

	ticks_last	= imax;

	ticks = ticks_first + ticks_last;

	if (decades > 2) {
		decades = decades - 2;
		for (c=1; c<=decades; c++) {
			ticks = ticks + 9;
		}
	}
	
	ticks++; /* inclusive	*/

	return(ticks);
}



/*************************************************************************/
/** PTOOLS_CalcLogTicks:						**/
/** Set tick min, max, inc, and number for log plots.		**/
/**									**/
/*************************************************************************/
void PTOOLS_CalcLogTicks(double min,double max,int *min_unit,int *min_pow,
	int *max_unit,int *max_pow,int *dec,int *ticks)
{
	RoundUp10(min,	 min_unit, min_pow);
	RoundDown10(max, max_unit, max_pow);

	*dec   = CountDecades(*min_pow, *max_pow);
	*ticks = CountTicks(*min_unit, *min_pow, *max_unit, *max_pow);	
}


/*************************************************************************/
/** PTOOLS_CalcTickInc:							**/
/** Calc tick min, inc, and number for linear plots			**/
/**									**/
/*************************************************************************/
int PTOOLS_CalcTickInc(double min,double max,double *min_tic,double *inc_tic, 
	 int *num_tic, int try_num_tic)
{
	double start;
	double inc;
	double eval;
	int	   first;
	int	   ct;


	start = min;

	/*****************************************************************/
	/** Find tick increment						**/
	/*****************************************************************/
	inc = (max - min) / try_num_tic;
	inc = RoundUp(inc);
	
	/*****************************************************************/
	/** Find a starting tick mark inside the plot			**/
	/*****************************************************************/
	round1(&start,(int) log10(inc));

	/*****************************************************************/
	/** backup 10 ticks, to be sure that we are outside the plot	**/
	/*****************************************************************/
	eval	 = start - (10*inc);
	*inc_tic = inc;

	/*****************************************************************/
	/** now loop over the plot until enter, then leave the plot **/
	/*****************************************************************/
	ct	  = 0;
	first = 1;
	while (1) {

		/*********************************************************/
		/** check if inside the plot (with fudge factors)	**/
		/*********************************************************/
		if ((eval >= min-1e-36) && (eval <= max+1e-36)) {  

			/*************************************************/
			/** add one more to the tick count		**/
			/*************************************************/
			ct++;

			/*************************************************/
			/** if this is the first one, set the min_tic	**/
			/*************************************************/
			if (first) {
				first=0;
				*min_tic = eval;
			}
		}

		/*********************************************************/
		/** if the number we are evaluating is greater then **/
		/** the right hand edge of the plot, were done		**/
		/*********************************************************/
				if (eval > max) {
		
			/*************************************************/
			/** set the "num_tic" and bail out		**/
			/*************************************************/
						*num_tic = ct;
						return(0);
				}

		/*********************************************************/
		/** grab the next number to evaluate.			**/
		/*********************************************************/
		eval = eval + inc;
	}
	
}

/*************************************************************************/
/** PTOOLS_CalcSubTickInc:						**/
/** Given defined tick information, build subtick information.	**/
/**									**/
/*************************************************************************/
int PTOOLS_CalcSubTickInc(double plot_min, double plot_max, 
			  double min_major_tic, double inc_major_tic, 
			  double *min_sub_tic, double *inc_sub_tic, 
			  int *num_sub_tic, int try_div)
{
//	double dummy1;
//	double dummy2;
//	double temp_upper_bound;
	double eval;
	int	   first;
	int	   ct;


/****	PTOOLS_CalcTickInc(min_major_tic, temp_upper_bound, 
		&dummy1, inc_sub_tic, &dummy2, try_div);
*****/


	/*****************************************************************/
	/** Calc number of sub ticks					**/
	/*****************************************************************/
	switch(PTOOLS_GetBase(inc_major_tic)) {

		case 1	:
			*inc_sub_tic = inc_major_tic / 2.0;
			break;
		case 2	:
			*inc_sub_tic = inc_major_tic / 2.0;
			break;
		case 5	:
			*inc_sub_tic = inc_major_tic / 5.0;
			break;
		case 10 :
			*inc_sub_tic = inc_major_tic / 2.0;
			break;
	}


	/*****************************************************************/
	/** Now use our original first major tick, backup 1 major tick	**/
	/** and start sweeping over the plot applying minor ticks	**/
	/*****************************************************************/
	eval  = min_major_tic - inc_major_tic;

	ct	  = 0;
	first = 1;
	while (1) {

		/*********************************************************/
		/** check if inside the plot (with fudge factors)	**/
		/*********************************************************/
		if ((eval >= plot_min - 1e-36) && (eval <= plot_max + 1e-36)) {	 

			/*************************************************/
			/** add one more to the tick count		**/
			/*************************************************/
			ct++;

			/*************************************************/
			/** if this is the first one, set the min_tic	**/
			/*************************************************/
			if (first) {
				first=0;
				*min_sub_tic = eval;
			}
		}

		/*********************************************************/
		/** if the number we are evaluating is greater then **/
		/** the right hand edge of the plot, were done		**/
		/*********************************************************/
				if (eval > plot_max) {
		
			/*************************************************/
			/** set the "num_sub_tic" and bail out		**/
			/*************************************************/
						*num_sub_tic = ct;
						return(0);
				}

		/*********************************************************/
		/** grab the next number to evaluate.			**/
		/*********************************************************/
		eval = eval + *inc_sub_tic;
	}
	
}




/*************************************************************************/
/** PTOOLS_GetBase:							**/
/** Get the base interval between major ticks			**/
/**									**/
/*************************************************************************/
int PTOOLS_GetBase(double val)
{
		int exponent, idx, ival;
	
		exponent = (int) floor(log10(val));
		if (exponent < 0) {
			for (idx = exponent;  idx < 0; idx++) {
					val *= 10.0;
			}
		} else {
			for (idx = 0;  idx < exponent; idx++) {
					val /= 10.0;
			}
		}

		if (val > 5.0) {
		ival = 10.0;
		} else
	
	if (val > 2.0) {
		ival = 5.0;
	} else 

	if (val > 1.0) {
		ival = 2.0;
		} else {
		ival = 1.0;
	}

	return(ival);
}


/*************************************************************************/
/** round1:								**/
/** Truncate remainder of number scaled to 0-10			**/
/**									**/
/*************************************************************************/
void round1(double *d,int exp)
{
	int idx;

		if (exp < 0) {
			for (idx = exp;	 idx < 0; idx++) {
					*d = *d * 10;
		}
		} else { /* leave */
			for (idx = 0;  idx <= exp; idx++) {
					*d = *d / 10;
			}
		}
	
	*d = floor(*d);

		if (exp < 0) {
			for (idx = exp;	 idx < 0;  idx++) {
					*d = *d /10;
			}	
		} else {
			for (idx = 0;  idx <= exp;	idx++) {
					*d = *d * 10;
			}
		}
}

/*************************************************************************/
/** RoundUp:								**/
/** This routine rounds up the given positive number such that	**/
/**		it is some power of ten times either 1, 2, or 5.  It is		**/
/**		used to find increments for grid lines.				**/
/**									**/
/*************************************************************************/
double RoundUp(double val)
{
		int exponent, idx;

		exponent = (int) floor(log10(val));
		if (exponent < 0) {
			for (idx = exponent;  idx < 0; idx++) {
					val *= 10.0;
			}
		} else {
			for (idx = 0;  idx < exponent; idx++) {
					val /= 10.0;
			}
		}
	
	if (val > 5.0) {
		val = 10.0;
	} else 

		if (val > 2.0) {
		val = 5.0;
	} else 

	if (val > 1.0) {
		val = 2.0;
	} else {
			val = 1.0;
	}

		if (exponent < 0) {
			for (idx = exponent;  idx < 0;	idx++) {
					val /= 10.0;
			}
		} else {
			for (idx = 0;  idx < exponent;	idx++) {
					val *= 10.0;
			}
		}
	
		return(val);
}



/*************************************************************************/
/** PTOOLS_Format:							**/
/**		format a number							**/
/**									**/
/*************************************************************************/
void PTOOLS_Format(double val, int dec, char *s)
{
	char format[100];

	sprintf(format, "%%.%dlf", dec);
	sprintf(s, format, val);
}


/*************************************************************************/
/** PTOOLS_GetPower:							**/
/** return exponent for range.					**/
/**									**/
/*************************************************************************/
int PTOOLS_GetPower(double min, double max)
{

	int		power;
	double	bpow, lpow, temp;

	if (max==0.0) max =	 1e-36;
	if (min==0.0) min = -1e-36;

	bpow=log10(fabs(max));
	lpow=log10(fabs(min));

	if (bpow < lpow) {
		temp = bpow;
		bpow = lpow;
		lpow = temp;
	}

	if ((bpow >= -3.0) && (bpow < 4.0)) {
		power = 0;
	} else if (((bpow-lpow) < 3.0) && ((bpow-lpow) >= 1.0)) {
		power = (int) lpow;
	} else {
		if (bpow > 0.0) power = ((int) bpow) - 1;
		if (bpow < 0.0) power = ((int) bpow) - 2;
	}

	return(power);
}

/*************************************************************************/
/** PTOOLS_GetDecimalPlaces:						**/
/** Get number of decimal using trial and error method,		**/
/** (works good)							**/
/**									**/ 
/*************************************************************************/
int PTOOLS_GetDecimalPlaces(int exp, double min_float, double inc_float, int n_divisions, int max_dec)
{
	int		n_decimals;
	int		two_similar;
	double	current_float;
	char	**list;
	int		c,i,code;
	char	format[100];
	char	str[100];
	double	num1, num2;

	min_float = PTOOLS_Scale(min_float, exp);
	inc_float = PTOOLS_Scale(inc_float, exp);

	for (n_decimals=0; n_decimals<max_dec; n_decimals++) {

		current_float  = min_float;
		list		   = InitStringList();
		two_similar	   = 0;

		for (c=0; c<n_divisions; c++) {

			sprintf(format,"%%.%dlf", n_decimals);
			sprintf(str, format, current_float);
			list = AppendStringList(list, str);

			for (i=0; i<c; i++) {

				if (!strcmp(list[i],list[c])) {
					two_similar=1;
				}

				/**************************/
				/** Added for 0, -0 case **/
				/**************************/

				code  = sscanf(list[i],"%lf", &num1);
				code += sscanf(list[c],"%lf", &num2);
	
				if (code==2) {
					if ((fabs(num1)==0.0) && (fabs(num2)==0.0)) {
					two_similar=1;
					}
				}
			}

			current_float = current_float + inc_float;
		}
	
		FreeStringList(list);

		if (!two_similar) {
			return(n_decimals);
		}	
	}

	return(-1);
}


