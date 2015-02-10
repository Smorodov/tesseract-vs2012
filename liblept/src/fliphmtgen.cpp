/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

/*
 *    fliphmtgen.c
 *
 *       DWA implementation of hit-miss transforms with auto-generated sels
 *       for pixOrientDetectDwa() and pixUpDownDetectDwa() in flipdetect.c
 *
 *            PIX             *pixFlipFHMTGen()
 *              static l_int32   flipfhmtgen_low()  -- dispatcher
 *                static void      fhmt_1_0()
 *                static void      fhmt_1_1()
 *                static void      fhmt_1_2()
 *                static void      fhmt_1_3()
 *
 *       The code (rearranged) was generated by prog/flipselgen.c
 */

#include <string.h>
#include "allheaders.h"

static l_int32   NUM_SELS_GENERATED = 4;
static char  SEL_NAMES[][10] = {"flipsel1",
                                "flipsel2",
                                "flipsel3",
                                "flipsel4"
                               };

static l_int32 flipfhmtgen_low(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32, l_int32);

static void  fhmt_1_0(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fhmt_1_1(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fhmt_1_2(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fhmt_1_3(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);


/*---------------------------------------------------------------------*
 *                          Top-level hmt functions                    *
 *---------------------------------------------------------------------*/
/*
 *  pixFlipFHMTGen()
 *
 *     Input:  pixd (usual 3 choices: null, == pixs, != pixs)
 *             pixs
 *             sel name (one of four defined in SEL_NAMES[])
 *     Return: pixd
 *
 *     Action: hit-miss transform on pixs by the sel
 *     N.B.: the sel must have at least one hit, and it
 *           can have any number of misses.
 */
PIX *
pixFlipFHMTGen(PIX   *pixd,
               PIX   *pixs,
               char  *selname)
{
	l_int32    i, index, found, w, h, wpls, wpld;
	l_uint32  *datad, *datas, *datat;
	PIX       *pixt;
	PROCNAME("pixFlipFHMTGen");
	if (!pixs)
	{
		return (PIX *)ERROR_PTR("pixs not defined", procName, pixd);
	}
	if (pixGetDepth(pixs) != 1)
	{
		return (PIX *)ERROR_PTR("pixs must be 1 bpp", procName, pixd);
	}
	found = FALSE;
	for (i = 0; i < NUM_SELS_GENERATED; i++)
	{
		if (strcmp(selname, SEL_NAMES[i]) == 0)
		{
			found = TRUE;
			index = i;
			break;
		}
	}
	if (found == FALSE)
	{
		return (PIX *)ERROR_PTR("sel index not found", procName, pixd);
	}
	if (pixd)
	{
		if (!pixSizesEqual(pixs, pixd))
		{
			return (PIX *)ERROR_PTR("sizes not equal", procName, pixd);
		}
	}
	else
	{
		if ((pixd = pixCreateTemplate(pixs)) == NULL)
		{
			return (PIX *)ERROR_PTR("pixd not made", procName, NULL);
		}
	}
	wpls = pixGetWpl(pixs);
	wpld = pixGetWpl(pixd);
	/*  The images must be surrounded with ADDED_BORDER white pixels,
	 *  that we'll read from.  We fabricate a "proper"
	 *  image as the subimage within the border, having the
	 *  following parameters:  */
	w = pixGetWidth(pixs) - 2 * ADDED_BORDER;
	h = pixGetHeight(pixs) - 2 * ADDED_BORDER;
	datas = pixGetData(pixs) + ADDED_BORDER * wpls + ADDED_BORDER / 32;
	datad = pixGetData(pixd) + ADDED_BORDER * wpld + ADDED_BORDER / 32;
	if (pixd == pixs)    /* need temp image if in-place */
	{
		if ((pixt = pixCopy(NULL, pixs)) == NULL)
		{
			return (PIX *)ERROR_PTR("pixt not made", procName, pixd);
		}
		datat = pixGetData(pixt) + ADDED_BORDER * wpls + ADDED_BORDER / 32;
		flipfhmtgen_low(datad, w, h, wpld, datat, wpls, index);
		pixDestroy(&pixt);
	}
	else      /* simple and not in-place */
	{
		flipfhmtgen_low(datad, w, h, wpld, datas, wpls, index);
	}
	return pixd;
}


/*---------------------------------------------------------------------*
 *                           Fast hmt dispatcher                       *
 *---------------------------------------------------------------------*/
/*
 *  flipfhmtgen_low()
 *
 *       A dispatcher to appropriate low-level code for flip hmt ops
 */
static l_int32
flipfhmtgen_low(l_uint32  *datad,
                l_int32    w,
                l_int32    h,
                l_int32    wpld,
                l_uint32  *datas,
                l_int32    wpls,
                l_int32    index)
{
	switch (index)
	{
		case 0:
			fhmt_1_0(datad, w, h, wpld, datas, wpls);
			break;
		case 1:
			fhmt_1_1(datad, w, h, wpld, datas, wpls);
			break;
		case 2:
			fhmt_1_2(datad, w, h, wpld, datas, wpls);
			break;
		case 3:
			fhmt_1_3(datad, w, h, wpld, datas, wpls);
			break;
	}
	return 0;
}


/*--------------------------------------------------------------------------*
 *                  Low-level auto-generated hmt routines                   *
 *--------------------------------------------------------------------------*/
/*
 *  N.B.  in all the low-level routines, the part of the image
 *        that is accessed has been clipped by ADDED_BORDER pixels
 *        on all four sides.  This is done in the higher level
 *        code by redefining w and h smaller and by moving the
 *        start-of-image pointers up to the beginning of this
 *        interior rectangle.
 */

static void
fhmt_1_0(l_uint32  *datad,
         l_int32    w,
         l_int32    h,
         l_int32    wpld,
         l_uint32  *datas,
         l_int32    wpls)
{
	l_int32              i;
	register l_int32     j, pwpls;
	register l_uint32   *sptr, *dptr;
	l_int32              wpls2, wpls3;
	wpls2 = 2 * wpls;
	wpls3 = 3 * wpls;
	pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */
	for (i = 0; i < h; i++)
	{
		sptr = datas + i * wpls;
		dptr = datad + i * wpld;
		for (j = 0; j < pwpls; j++, sptr++, dptr++)
		{
			*dptr = ((*(sptr - wpls) >> 3) | (*(sptr - wpls - 1) << 29)) &
			        (~*(sptr - wpls)) &
			        ((~*(sptr - wpls) << 1) | (~*(sptr - wpls + 1) >> 31)) &
			        ((*(sptr) >> 3) | (*(sptr - 1) << 29)) &
			        ((~*(sptr) >> 1) | (~*(sptr - 1) << 31)) &
			        (~*sptr) &
			        ((~*(sptr) << 1) | (~*(sptr + 1) >> 31)) &
			        ((*(sptr + wpls) >> 3) | (*(sptr + wpls - 1) << 29)) &
			        (~*(sptr + wpls)) &
			        ((*(sptr + wpls2) >> 3) | (*(sptr + wpls2 - 1) << 29)) &
			        ((*(sptr + wpls3) >> 3) | (*(sptr + wpls3 - 1) << 29)) &
			        ((*(sptr + wpls3) >> 2) | (*(sptr + wpls3 - 1) << 30)) &
			        ((*(sptr + wpls3) >> 1) | (*(sptr + wpls3 - 1) << 31)) &
			        (*(sptr + wpls3)) &
			        ((*(sptr + wpls3) << 1) | (*(sptr + wpls3 + 1) >> 31)) &
			        ((*(sptr + wpls3) << 2) | (*(sptr + wpls3 + 1) >> 30));
		}
	}
}


static void
fhmt_1_1(l_uint32  *datad,
         l_int32    w,
         l_int32    h,
         l_int32    wpld,
         l_uint32  *datas,
         l_int32    wpls)
{
	l_int32              i;
	register l_int32     j, pwpls;
	register l_uint32   *sptr, *dptr;
	l_int32              wpls2, wpls3;
	wpls2 = 2 * wpls;
	wpls3 = 3 * wpls;
	pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */
	for (i = 0; i < h; i++)
	{
		sptr = datas + i * wpls;
		dptr = datad + i * wpld;
		for (j = 0; j < pwpls; j++, sptr++, dptr++)
		{
			*dptr = ((~*(sptr - wpls) >> 1) | (~*(sptr - wpls - 1) << 31)) &
			        (~*(sptr - wpls)) &
			        ((*(sptr - wpls) << 3) | (*(sptr - wpls + 1) >> 29)) &
			        ((~*(sptr) >> 1) | (~*(sptr - 1) << 31)) &
			        (~*sptr) &
			        ((~*(sptr) << 1) | (~*(sptr + 1) >> 31)) &
			        ((*(sptr) << 3) | (*(sptr + 1) >> 29)) &
			        (~*(sptr + wpls)) &
			        ((*(sptr + wpls) << 3) | (*(sptr + wpls + 1) >> 29)) &
			        ((*(sptr + wpls2) << 3) | (*(sptr + wpls2 + 1) >> 29)) &
			        ((*(sptr + wpls3) >> 2) | (*(sptr + wpls3 - 1) << 30)) &
			        ((*(sptr + wpls3) >> 1) | (*(sptr + wpls3 - 1) << 31)) &
			        (*(sptr + wpls3)) &
			        ((*(sptr + wpls3) << 1) | (*(sptr + wpls3 + 1) >> 31)) &
			        ((*(sptr + wpls3) << 2) | (*(sptr + wpls3 + 1) >> 30)) &
			        ((*(sptr + wpls3) << 3) | (*(sptr + wpls3 + 1) >> 29));
		}
	}
}


static void
fhmt_1_2(l_uint32  *datad,
         l_int32    w,
         l_int32    h,
         l_int32    wpld,
         l_uint32  *datas,
         l_int32    wpls)
{
	l_int32              i;
	register l_int32     j, pwpls;
	register l_uint32   *sptr, *dptr;
	l_int32              wpls2, wpls3;
	wpls2 = 2 * wpls;
	wpls3 = 3 * wpls;
	pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */
	for (i = 0; i < h; i++)
	{
		sptr = datas + i * wpls;
		dptr = datad + i * wpld;
		for (j = 0; j < pwpls; j++, sptr++, dptr++)
		{
			*dptr = ((*(sptr - wpls3) >> 3) | (*(sptr - wpls3 - 1) << 29)) &
			        ((*(sptr - wpls3) >> 2) | (*(sptr - wpls3 - 1) << 30)) &
			        ((*(sptr - wpls3) >> 1) | (*(sptr - wpls3 - 1) << 31)) &
			        (*(sptr - wpls3)) &
			        ((*(sptr - wpls3) << 1) | (*(sptr - wpls3 + 1) >> 31)) &
			        ((*(sptr - wpls3) << 2) | (*(sptr - wpls3 + 1) >> 30)) &
			        ((*(sptr - wpls2) >> 3) | (*(sptr - wpls2 - 1) << 29)) &
			        ((*(sptr - wpls) >> 3) | (*(sptr - wpls - 1) << 29)) &
			        (~*(sptr - wpls)) &
			        ((*(sptr) >> 3) | (*(sptr - 1) << 29)) &
			        ((~*(sptr) >> 1) | (~*(sptr - 1) << 31)) &
			        (~*sptr) &
			        ((~*(sptr) << 1) | (~*(sptr + 1) >> 31)) &
			        ((*(sptr + wpls) >> 3) | (*(sptr + wpls - 1) << 29)) &
			        (~*(sptr + wpls)) &
			        ((~*(sptr + wpls) << 1) | (~*(sptr + wpls + 1) >> 31));
		}
	}
}


static void
fhmt_1_3(l_uint32  *datad,
         l_int32    w,
         l_int32    h,
         l_int32    wpld,
         l_uint32  *datas,
         l_int32    wpls)
{
	l_int32              i;
	register l_int32     j, pwpls;
	register l_uint32   *sptr, *dptr;
	l_int32              wpls2, wpls3;
	wpls2 = 2 * wpls;
	wpls3 = 3 * wpls;
	pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */
	for (i = 0; i < h; i++)
	{
		sptr = datas + i * wpls;
		dptr = datad + i * wpld;
		for (j = 0; j < pwpls; j++, sptr++, dptr++)
		{
			*dptr = ((*(sptr - wpls3) >> 2) | (*(sptr - wpls3 - 1) << 30)) &
			        ((*(sptr - wpls3) >> 1) | (*(sptr - wpls3 - 1) << 31)) &
			        (*(sptr - wpls3)) &
			        ((*(sptr - wpls3) << 1) | (*(sptr - wpls3 + 1) >> 31)) &
			        ((*(sptr - wpls3) << 2) | (*(sptr - wpls3 + 1) >> 30)) &
			        ((*(sptr - wpls3) << 3) | (*(sptr - wpls3 + 1) >> 29)) &
			        ((*(sptr - wpls2) << 3) | (*(sptr - wpls2 + 1) >> 29)) &
			        (~*(sptr - wpls)) &
			        ((*(sptr - wpls) << 3) | (*(sptr - wpls + 1) >> 29)) &
			        ((~*(sptr) >> 1) | (~*(sptr - 1) << 31)) &
			        (~*sptr) &
			        ((~*(sptr) << 1) | (~*(sptr + 1) >> 31)) &
			        ((*(sptr) << 3) | (*(sptr + 1) >> 29)) &
			        ((~*(sptr + wpls) >> 1) | (~*(sptr + wpls - 1) << 31)) &
			        (~*(sptr + wpls)) &
			        ((*(sptr + wpls) << 3) | (*(sptr + wpls + 1) >> 29));
		}
	}
}