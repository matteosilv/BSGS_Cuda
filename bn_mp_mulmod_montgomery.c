#include <tommath.h>
/* Multiple precision modular multiplication using
 * Montgomery reduction.
 *
 * Credits to Tom St Denis, tomstdenis@gmail.com, http://libtom.org
 * for using his "libtommath" library. 
 */


/* d = a * b (mod c) */
int mp_mulmod_montgomery (mp_int * a, mp_int * b, mp_int * c, mp_int * d)
{
	int res;
	mp_int R;
	mp_digit mp;
	
	/* initialize R */
	if ((res = mp_init(&R)) != MP_OKAY) {
		printf("Error initializing the number. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* get normalizzation */
	if ((res = mp_montgomery_calc_normalization(&R,c)) != MP_OKAY) {
		printf("Error getting norm. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* get mp value */
	if ((res = mp_montgomery_setup(c, &mp)) != MP_OKAY) {
		printf("Error setting up montgomery. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* normalize ‘a’ so now a is equal to aR */
	if ((res = mp_mulmod(a, &R, c, a)) != MP_OKAY) {
		printf("Error computing aR. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* normalize ‘b’ so now a is equal to bR */
	if ((res = mp_mulmod(b, &R, c, b)) != MP_OKAY) {
		printf("Error computing aR. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* compute a * b in Montgomery domain */
	if ((res = mp_mul(a, b, d)) != MP_OKAY) {
		printf("Error multiplying. %s", mp_error_to_string(res));
		return EXIT_FAILURE;
	}
	
	/* now reduce ‘d’ back down to d = a*b*R^2 * R^-1 == a*b*R */
	if ((res = mp_montgomery_reduce(d, c, mp)) != MP_OKAY) {
		printf("Error reducing. %s", mp_error_to_string(res));
	        return EXIT_FAILURE;
	}
	
	/* now reduce ‘d’ back down to d = a*b*R * R^-1 == a*b */
	if ((res = mp_montgomery_reduce(d, c, mp)) != MP_OKAY) {
		printf("Error reducing. %s", mp_error_to_string(res));
	        return EXIT_FAILURE;
	}
	
	mp_clear (&R);
	
	return res;
}
