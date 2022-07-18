
#include "acb_theta.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("eld_interval....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 2000 * arb_test_multiplier(); iter++)
      {
	int a = n_randint(state, 2);
	slong prec = ARB_ELD_DEFAULT_PREC;
	slong mag_bits = n_randint(state, 5);
	int guaranteed_pt = iter % 2;
	
	slong min, max, mid;
	arb_t ctr, rad, tmax, tmin;
	arf_t pos;
	int fail;

	arb_init(ctr);
	arb_init(rad);
	arb_init(tmax);
	arb_init(tmin);
	arf_init(pos);

	arb_randtest(ctr, state, prec, mag_bits);
	arb_randtest(rad, state, prec, mag_bits);

	if (guaranteed_pt)
	  {
	    arf_set_si(arb_midref(ctr), a);
	    arb_abs(rad, rad);
	    arb_get_ubound_arf(pos, rad, prec);
	    arb_set_arf(rad, pos);
	  }

	arb_eld_interval(&min, &mid, &max, ctr, rad, a, prec);
	arb_set_si(tmax, max+3);
	arb_sub(tmax, tmax, rad, prec);
	arb_set_si(tmin, min-3);
	arb_add(tmin, tmin, rad, prec);
	
	fail = ((min > max) && guaranteed_pt)
	  || ((min <= max) &&
	      (FLINT_ABS(min) % 2 != a
	       || FLINT_ABS(mid) % 2 != a
	       || FLINT_ABS(max) % 2 != a
	       || mid < min
	       || mid > max
	       || !arb_gt(tmax, ctr)
	       || !arb_lt(tmin, ctr)));
	
	if (fail)
	  {
	    flint_printf("FAIL\n");
	    flint_printf("Center, radius:\n");
	    arb_printd(ctr, 10); flint_printf("\n");
	    arb_printd(rad, 10); flint_printf("\n");
	    flint_printf("a = %i, min = %wd, mid = %wd, max = %wd\n", a, min, mid, max);
	    fflush(stdout);
	    flint_abort();
	  }
	
	arb_clear(ctr);
	arb_clear(rad);
	arb_clear(tmax);
	arb_clear(tmin);
	arf_clear(pos);
      }
    
    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
