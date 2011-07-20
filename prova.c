#include <stdio.h>
#include <string.h>
#include <tommath.h>

int rsa_rng(unsigned char *dst, int len, void *dat)
{
	int x;
	for (x = 0; x < len; x++)
		dst[x] = rand() & 0xFF;
	return len;
}

int main(void) {
	
	mp_int a, b, c, prime, R;
	char str[310];
	int result;
	int size;
	mp_digit mp;

	if ((result = mp_init_multi(&a, &b, &c, &prime, &R, NULL)) != MP_OKAY) {
		printf("Error initializing the number. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}


	mp_set_int(&a, 261540040);
	mp_set_int(&b, 331232130);
	mp_prime_random_ex(&prime, 5, 34, LTM_PRIME_BBS, rsa_rng , NULL);
	
	printf("Numero primo generato: %lu\n" , mp_get_int(&prime));

	mp_radix_size (&prime, 10, &size);
	printf("Size: %d\n", size);
	mp_toradix (&prime, str, 10);
	printf("stringa: %s\n", str);


	/* get normalization */
	if ((result = mp_montgomery_calc_normalization(&R, &prime)) != MP_OKAY) {
		printf("Error getting norm. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}
	
	//mp_set_int(&R, 64);
	printf("Radice di Montgomery: %lu\n", mp_get_int(&R));

	/* get mp value */
	if ((result = mp_montgomery_setup(&prime, &mp)) != MP_OKAY) {
		printf("Error setting up montgomery. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}

	printf("a vale: %lu\n", mp_get_int(&a));
	printf("b vale: %lu\n", mp_get_int(&b));

	/* normalize ‘a’ so now a is equal to aR */
	if ((result = mp_mulmod(&a, &R, &prime, &a)) != MP_OKAY) {
		printf("Error computing aR. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}
	printf("ora aR vale: %lu\n", mp_get_int(&a));
	
	/* normalize ‘b’ so now a is equal to bR */
	if ((result = mp_mulmod(&b, &R, &prime, &b)) != MP_OKAY) {
		printf("Error computing aR. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}

	printf("ora bR vale: %lu\n", mp_get_int(&b));

	
	if ((result = mp_mul(&a, &b, &c)) != MP_OKAY) {
		printf("Error reducing. %s", mp_error_to_string(result));
		return EXIT_FAILURE;
	}
	printf("ora c vale: %lu\n", mp_get_int(&c));

	/* now reduce ‘c’ back down to c = a*b*R^2 * R^-1 == a*b*R */
	if ((result = mp_montgomery_reduce(&c, &prime, mp)) != MP_OKAY) {
		printf("Error reducing. %s", mp_error_to_string(result));
	        return EXIT_FAILURE;
	}
	

	/* now reduce ‘c’ back down to c = a*b*R * R^-1 == a*b */
	if ((result = mp_montgomery_reduce(&c, &prime, mp)) != MP_OKAY) {
		printf("Error reducing. %s", mp_error_to_string(result));
	        return EXIT_FAILURE;
	}


	printf("Numero finale: %lu\n", mp_get_int(&c));
	mp_toradix (&c, str, 10);
	printf("Numero finale ascii: %s\n", str);


	/* use the number */
	return EXIT_SUCCESS;

}

