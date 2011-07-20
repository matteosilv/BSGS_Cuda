#include <tommath.h>
int main()
{
	mp_int a,b,c,d;
	
	mp_init_multi(&a,&b,&c,&d,NULL);
	
	mp_set_int(&a,150);
	mp_set_int(&b,135);
	mp_set_int(&c,163);
	
	mp_mulmod_montgomery (&a,&b,&c,&d);
	
	printf("Result:%lu\n",mp_get_int(&d));
}