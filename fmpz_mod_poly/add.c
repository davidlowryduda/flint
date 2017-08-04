/*
    Copyright (C) 2011 Sebastian Pancratz
    Copyright (C) 2008, 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "fmpz_mod_poly.h"

void _fmpz_mod_poly_add(fmpz *res, const fmpz *poly1, slong len1, 
                                   const fmpz *poly2, slong len2, const fmpz_t p)
{
    slong i, len = FLINT_MAX(len1, len2);
	fmpz_t diff;

    _fmpz_poly_add(res, poly1, len1, poly2, len2);

	fmpz_init(diff);
	
    for (i = 0; i < len; i++)
    {
        fmpz_sub(diff, res + i, p);
		if (fmpz_sgn(diff) >= 0)
            fmpz_swap(res + i, diff);
    }
	
	fmpz_clear(diff);
}

void fmpz_mod_poly_add(fmpz_mod_poly_t res, 
                       const fmpz_mod_poly_t poly1, const fmpz_mod_poly_t poly2)
{
    slong max = FLINT_MAX(poly1->length, poly2->length);

    fmpz_mod_poly_fit_length(res, max);

    _fmpz_mod_poly_add(res->coeffs, poly1->coeffs, poly1->length, 
                                    poly2->coeffs, poly2->length, &(res->p));

    _fmpz_mod_poly_set_length(res, max);
    _fmpz_mod_poly_normalise(res);
}

