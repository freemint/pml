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
 *	cadd   double precision complex addition
 *
 *  KEY WORDS
 *
 *	cadd
 *	complex functions
 *	machine independent routines
 *	math libraries
 *
 *  DESCRIPTION
 *
 *	Computes double precision complex result of addition of
 *	first double precision complex argument with second double
 *	precision complex argument.
 *
 *	Note that the complex addition function is
 *	so simple that it would not normally be called as a function
 *	but simply done "inline".  It is supplied mostly for
 *	completeness.
 *
 *  USAGE
 *
 *	COMPLEX cadd (z1, z2)
 *	COMPLEX z1;
 *	COMPLEX z2;
 *
 *  PROGRAMMER
 *
 *	Fred Fish
 *	Tempe, Az 85281
 *	(602) 966-8871
 *
 *  INTERNALS
 *
 *	Computes cadd(z1,z2) from:
 *
 *		1.	Let z1 = a + j b
 *			Let z2 = c + j d
 *
 *		2.	Then cadd(z1,z2) = (a + c) + j (b + d)
 *
 */

#if !defined (__M68881__) && !defined (sfp004)

#include <stdio.h>
#include <math.h>
#include "pml.h"

COMPLEX cadd (z1, z2)
COMPLEX z1;
COMPLEX z2;
{
    z1.real += z2.real;
    z1.imag += z2.imag;
    return (z1);
}
#endif /* !defined (__M68881__) #endif !defined (__M68881__) && !defined (sfp004)#endif !defined (__M68881__) && !defined (sfp004) !defined (sfp004) */

#ifdef	__M68881__
__asm(
".text\t\n"
"_funcname:\t\n"
"	.ascii	\"cadd\\0\"\t\n"
".even\t\n"
".globl	_cadd\t\n"
"_cadd:\t\n"
"	fmoved	sp@(4),fp0\t\n"
"	faddd	sp@(20),fp0\t\n"
"	fmoved	sp@(12),fp1\t\n"
"	faddd	sp@(28),fp1\t\n"
"	movel	a1,d0		| pointer to result\t\n"
"	fmoved	fp0,a1@		| return z.real\t\n"
"	fmoved	fp1,a1@(8)	| return z.imag\t\n"
);	/* end asm	*/
#endif /* __M68881__ */

#ifdef	sfp004
__asm("

comm =	 -6
resp =	-16
zahl =	  0

.text
_funcname:
	.ascii	\"cadd\\0\"
.text
.even
.globl	_cadd
_cadd:
	lea	0xfffa50,a0
	movew	#0x5400,a0@(comm)	| z1.real -> fp0
	.long	0x0c688900, 0xfff067f8
	movel	a7@(4),a0@		| load arg_hi
	movel	a7@(8),a0@		| load arg_low

	movew	#0x5422,a0@(comm)	| fp0 += z2.real
	.long	0x0c688900, 0xfff067f8
	movel	a7@(20),a0@		| load arg_hi
	movel	a7@(24),a0@		| load arg_low

	movew	#0x5480,a0@(comm)	| z1.imag -> fp1
	.long	0x0c688900, 0xfff067f8
	movel	a7@(12),a0@		| load arg_hi
	movel	a7@(16),a0@		| load arg_low

	movew	#0x54a2,a0@(comm)	| fp1 += z2.imag
	movel	a1,d0			| pointer to result
	.long	0x0c688900, 0xfff067f8
	movel	a7@(28),a0@		| load arg_hi
	movel	a7@(32),a0@		| load arg_low

|	movew	#%0111 0101 0000 0000,a0@(comm)	| fetch fp0
	movew	#0x7400,a0@(comm)		| 
	.long	0x0c688900, 0xfff067f8
	movel	a0@(zahl),a1@
	movel	a0@(zahl),a1@(4)

|	movew	#%0111 0100 1000 0000,a0@(comm)	| fetch fp1
	movew	#0x7480,a0@(comm)		|
	.long	0x0c688900, 0xfff067f8
	movel	a0@(zahl),a1@(8)
	movel	a0@(zahl),a1@(12)
");	/* end asm	*/
#endif /* sfp004 */

#if defined (__M68881__) || defined (sfp004)
# ifdef ERROR_CHECK	/* no error checking for now	*/
__asm(
"	pea	_funcname\t\n"
"	jmp	c_err_check\t\n"
);	/* end asm	*/
# else /* ERROR_CHECK */

__asm("rts");

# endif /* ERROR_CHECK */
#endif /* defined (__M68881__) || defined (sfp004) */
