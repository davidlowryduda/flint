/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2008, 2009 William Hart
    Copyright (C) 2010 Fredrik Johansson

******************************************************************************/

#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_factor.h"

void
_fmpz_factor_fit_length(fmpz_factor_t factor, len_t len)
{
    if (len > factor->alloc)
    {
        if (len < 2 * factor->alloc)
            len = 2 * factor->alloc;

        factor->p = (fmpz *) flint_realloc(factor->p, len * sizeof(fmpz));
        factor->exp = flint_realloc(factor->exp, len * sizeof(len_t));

        if (len > factor->alloc)
        {
            flint_mpn_zero((mp_ptr)(factor->p + factor->alloc), len-factor->alloc);
            flint_mpn_zero((mp_ptr)(factor->exp + factor->alloc), len-factor->alloc);
        }

        factor->alloc = len;
    }
}
