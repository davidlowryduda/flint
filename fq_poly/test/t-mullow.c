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

    Copyright (C) 2012 Sebastian Pancratz 

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fq_poly.h"

#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("mullow... ");
    fflush(stdout);

    flint_randinit(state);

    /* Compare with truncated product of a and b */
    for (i = 0; i < 2000; i++)
    {
        fmpz_t p;
        long deg;
        fq_ctx_t ctx;

        fq_poly_t a, b, c, d;
        long n;

        fmpz_init(p);
        fmpz_set_ui(p, n_randprime(state, 2 + n_randint(state, 3), 1));
        deg = n_randint(state, 10) + 1;
        fq_ctx_init_conway(ctx, p, deg, "a");

        fq_poly_init(a);
        fq_poly_init(b);
        fq_poly_init(c);
        fq_poly_init(d);

        fq_poly_randtest(a, state, n_randint(state, 100), ctx);
        fq_poly_randtest(b, state, n_randint(state, 100), ctx);
        n = n_randint(state, 100);

        fq_poly_mullow(c, a, b, n, ctx);
        fq_poly_mul(d, a, b, ctx);
        fq_poly_truncate(d, n);

        result = (fq_poly_equal(c, d));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), fq_poly_print_pretty(a, "X", ctx), flint_printf("\n");
            flint_printf("b = "), fq_poly_print_pretty(b, "X", ctx), flint_printf("\n");
            flint_printf("c = "), fq_poly_print_pretty(c, "X", ctx), flint_printf("\n");
            flint_printf("d = "), fq_poly_print_pretty(d, "X", ctx), flint_printf("\n");
            abort();
        }

        fq_poly_clear(a);
        fq_poly_clear(b);
        fq_poly_clear(c);
        fq_poly_clear(d);

        fmpz_clear(p);
        fq_ctx_clear(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

