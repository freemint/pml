/************************************************************************
 *									*
 *				N O T I C E				*
 *									*
 *			Copyright Abandoned, 1987, Fred Fish		*
 *									*
 *	This previously copyrighted work has been placed into the	*
 *	public domain by the author (Fred Fish) and may be freely used	*
 *	for any purpose, private or commercial.  I would appreciate	*
 *	it, as a courtesy, if this notice is left in all copies and	*
 *	derivative works.  Thank you, and enjoy...			*
 *									*
 *	The author makes no warranty of any kind with respect to this	*
 *	product and explicitly disclaims any implied warranties of	*
 *	merchantability or fitness for any particular purpose.		*
 *									*
 ************************************************************************
 */


/*
 *  FUNCTION
 *
 *	acos   double precision arc cosine
 *
 *  KEY WORDS
 *
 *	acos
 *	machine independent routines
 *	trigonometric functions
 *	math libraries
 *
 *  DESCRIPTION
 *
 *	Returns double precision arc cosine of double precision
 *	floating point argument.
 *
 *  USAGE
 *
 *	double acos (x)
 *	double x;
 *
 *  REFERENCES
 *
 *	Fortran IV-plus user's guide, Digital Equipment Corp. pp B-1.
 *
 *  RESTRICTIONS
 *
 *	The maximum relative error for the approximating polynomial
 *	in atan is 10**(-16.82).  However, this assumes exact arithmetic
 *	in the polynomial evaluation.  Additional rounding and
 *	truncation errors may occur as the argument is reduced
 *	to the range over which the polynomial approximation
 *	is valid, and as the polynomial is evaluated using
 *	finite-precision arithmetic.
 *	
 *  PROGRAMMER
 *
 *	Fred Fish
 *
 *  INTERNALS
 *
 *	Computes arccosine(x) from:
 *
 *		(1)	If x < -1.0  or x > +1.0 then call
 *			matherr and return 0.0 by default.
 *
 *		(2)	If x = 0.0 then acos(x) = PI/2.
 *
 *		(3)	If x = 1.0 then acos(x) = 0.0
 *
 *		(4)	If x = -1.0 then acos(x) = PI.
 *
 *		(5)	If 0.0 < x < 1.0 then
 *			acos(x) = atan(Y)
 *			Y = sqrt[1-(x**2)] / x 
 *
 *		(4)	If -1.0 < x < 0.0 then
 *			acos(x) = atan(Y) + PI
 *			Y = sqrt[1-(x**2)] / x 
 *
 */

#include <stdio.h>
#include <math.h>
#include "pml.h"


#if !defined (__M68881__) && !defined (sfp004)		/* mjr++	*/

static char funcname[] = "acos";

double acos (x)
double x;
{
    struct exception xcpt;
    double y;
    
    if ( x > 1.0 || x < -1.0) {
	xcpt.type = DOMAIN;
	xcpt.name = funcname;
	xcpt.arg1 = x;
	if (!matherr (&xcpt)) {
	    //fprintf (stderr, "%s: DOMAIN error\n", funcname);
	    errno = EDOM;
	    xcpt.retval = HUGE_VAL;	/* for now, should ne NaN	*/
	}
    } else if (x == 0.0) {
	xcpt.retval = HALFPI;
    } else if (x == 1.0) {
	xcpt.retval = 0.0;
    } else if (x == -1.0) {
	xcpt.retval = PI;
    } else {
	y = atan ( sqrt (1.0 - (x * x)) / x );
	if (x > 0.0) {
	    xcpt.retval = y;
	} else {
	    xcpt.retval = y + PI;
	}
    }
    return (xcpt.retval);
}
 
#endif	/* !__M68881 && !sfp004	*/
#ifdef	sfp004

__asm("
comm =	 -6
resp =	-16
zahl =	  0
");	/* end asm	*/

#endif /* sfp004 */
#if defined (__M68881__) || defined (sfp004)

    __asm(".text; .even");

#ifdef ERROR_CHECK

__asm(
"_Overflow:\t\n"
"	.ascii \"OVERFLOW\\0\"\t\n"
"_Domain:\t\n"
"	.ascii \"DOMAIN\\0\"\t\n"
"_Error_String:\t\n"
"	.ascii \"acos: %s error\\n\\0\"\t\n"
".even\t\n"
);

#endif ERROR_CHECK

#ifdef ERROR_CHECK
    __asm(
"| m.ritzert 14.12.1991\t\n"
"| ritzert@dfg.dbp.de\t\n"
"|\t\n"
"|    /* NAN  = {7fffffff,ffffffff}		*/\t\n"
"|    /* +Inf = {7ff00000,00000000}		*/\t\n"
"|    /* -Inf = {fff00000,00000000}		*/\t\n"
"|    /* MAX_D= {7fee42d1,30773b76}		*/\t\n"
"|    /* MIN_D= {ffee42d1,30773b76}		*/\t\n"
"\t\n"
".even\t\n"
"double_max:\t\n"
"	.long	0x7fee42d1\t\n"
"	.long	0x30273b76\t\n"
"double_min:\t\n"
"	.long	0xffee42d1\t\n"
"	.long	0x30273b76\t\n"
"NaN:\t\n"
"	.long	0x7fffffff\t\n"
"	.long	0xffffffff\t\n"
"p_Inf:\t\n"
"	.long	0x7ff00000\t\n"
"	.long	0x00000000\t\n"
"m_Inf:\t\n"
"	.long	0xfff00000\t\n"
"	.long	0x00000000\t\n"
);
#endif ERROR_CHECK

__asm(
".even\t\n"
".globl _acos\t\n"
"_acos:\t\n"
    );	/* end asm	*/

#endif	/* __M68881__ || sfp004	*/
#ifdef	__M68881__

    __asm(
"	facosd	a7@(4), fp0	| acos\t\n"
"	fmoved	fp0,a7@-	| push result\t\n"
"	moveml	a7@+,d0-d1	| return_value\t\n"
    );	/* end asm	*/

#endif /* __M68881__ */
#ifdef	sfp004
    __asm(
"	lea	0xfffa50,a0\t\n"
"	movew	#0x541c,a0@(comm)	| specify function\t\n"
"	cmpiw	#0x8900,a0@(resp)	| check\t\n"
"	movel	a7@(4),a0@		| load arg_hi\t\n"
"	movel	a7@(8),a0@		| load arg_low\t\n"
"	movew	#0x7400,a0@(comm)	| result to d0\t\n"
"	.long	0x0c688900, 0xfff067f8	| wait\t\n"
"	movel	a0@,d0\t\n"
"	movel	a0@,d1\t\n"
    );	/* end asm	*/

#endif /* sfp004 */
#if defined (__M68881__) || defined (sfp004)

# ifdef ERROR_CHECK
    __asm(
"	lea	double_max,a0	|\t\n"
"	swap	d0		| exponent into lower word\t\n"
"	cmpw	a0@(16),d0	| == NaN ?\t\n"
"	beq	error_nan	|\t\n"
"	cmpw	a0@(24),d0	| == + Infinity ?\t\n"
"	beq	error_plus	|\t\n"
"	cmpw	a0@(32),d0	| == - Infinity ?\t\n"
"	beq	error_minus	|\t\n"
"	swap	d0		| result ok,\t\n"
"	rts			| restore d0\t\n"
);
#ifndef	__MSHORT__
__asm(
"error_minus:\t\n"
"	swap	d0\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movel	#63,_errno	| errno = ERANGE\t\n"
"	pea	_Overflow	| for printf\t\n"
"	bra	error_exit	|\t\n"
"error_plus:\t\n"
"	swap	d0\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movel	#63,_errno	| NAN => errno = EDOM\t\n"
"	pea	_Overflow	| for printf\t\n"
"	bra	error_exit	|\t\n"
"error_nan:\t\n"
"	moveml	a0@(24),d0-d1	| result = +inf\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movel	#62,_errno	| NAN => errno = EDOM\t\n"
"	pea	_Domain		| for printf\t\n"
);
#else	__MSHORT__
__asm(
"error_minus:\t\n"
"	swap	d0\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movew	#63,_errno	| errno = ERANGE\t\n"
"	pea	_Overflow	| for printf\t\n"
"	bra	error_exit	|\t\n"
"error_plus:\t\n"
"	swap	d0\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movew	#63,_errno	| NAN => errno = EDOM\t\n"
"	pea	_Overflow	| for printf\t\n"
"	bra	error_exit	|\t\n"
"error_nan:\t\n"
"	moveml	a0@(24),d0-d1	| result = +inf\t\n"
"	moveml	d0-d1,a7@-\t\n"
"	movew	#62,_errno	| NAN => errno = EDOM\t\n"
"	pea	_Domain		| for printf\t\n"
);
#endif	__MSHORT__
__asm(
"error_exit:\t\n"
"	pea	_Error_String	|\t\n"
"	pea	__iob+52	|\t\n"
//"	jbsr	_fprintf	|\t\n"
"	addl	#12,a7		|\t\n"
"	moveml	a7@+,d0-d1\t\n"
);
# endif /* ERROR_CHECK */
__asm("rts");

#endif /* __M68881__ || sfp004	*/
