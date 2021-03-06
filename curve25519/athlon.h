/*
curve25519_athlon.h version 20050915
D. J. Bernstein
Public domain.
*/

#ifndef CURVE25519_ATHLON
#define CURVE25519_ATHLON

extern void curve25519_athlon(unsigned char *, const unsigned char *, const unsigned char *);

/* internal functions, exposed purely for testing */
extern void curve25519_athlon_init(void);
extern void curve25519_athlon_mainloop(double *, const unsigned char *);
extern void curve25519_athlon_recip(double *, const double *);
extern void curve25519_athlon_square(double *, const double *);
extern void curve25519_athlon_mult(double *, const double *, const double *);
extern void curve25519_athlon_todouble(double *, const unsigned char *);
extern void curve25519_athlon_fromdouble(unsigned char *, const double *);

#ifndef curve25519_implementation
#define curve25519_implementation "curve25519_athlon"
#define curve25519 curve25519_athlon
#define curve25519_init curve25519_athlon_init
#define curve25519_mainloop curve25519_athlon_mainloop
#define curve25519_recip curve25519_athlon_recip
#define curve25519_square curve25519_athlon_square
#define curve25519_mult curve25519_athlon_mult
#define curve25519_todouble curve25519_athlon_todouble
#define curve25519_fromdouble curve25519_athlon_fromdouble
#endif

#endif

#define mult curve25519_mult
#define square curve25519_square

void curve25519_athlon_recip(double out[10], const double z[10])
{
	double z2[10];
	double z9[10];
	double z11[10];
	double z2_5_0[10];
	double z2_10_0[10];
	double z2_20_0[10];
	double z2_50_0[10];
	double z2_100_0[10];
	double t0[10];
	double t1[10];
	int i;

	/* 2 */ square(z2, z);
	/* 4 */ square(t1, z2);
	/* 8 */ square(t0, t1);
	/* 9 */ mult(z9, t0, z);
	/* 11 */ mult(z11, z9, z2);
	/* 22 */ square(t0, z11);
	/* 2^5 - 2^0 = 31 */ mult(z2_5_0, t0, z9);

	/* 2^6 - 2^1 */ square(t0, z2_5_0);
	/* 2^7 - 2^2 */ square(t1, t0);
	/* 2^8 - 2^3 */ square(t0, t1);
	/* 2^9 - 2^4 */ square(t1, t0);
	/* 2^10 - 2^5 */ square(t0, t1);
	/* 2^10 - 2^0 */ mult(z2_10_0, t0, z2_5_0);

	/* 2^11 - 2^1 */ square(t0, z2_10_0);
	/* 2^12 - 2^2 */ square(t1, t0);
	/* 2^20 - 2^10 */ for (i = 2; i < 10; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^20 - 2^0 */ mult(z2_20_0, t1, z2_10_0);

	/* 2^21 - 2^1 */ square(t0, z2_20_0);
	/* 2^22 - 2^2 */ square(t1, t0);
	/* 2^40 - 2^20 */ for (i = 2; i < 20; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^40 - 2^0 */ mult(t0, t1, z2_20_0);

	/* 2^41 - 2^1 */ square(t1, t0);
	/* 2^42 - 2^2 */ square(t0, t1);
	/* 2^50 - 2^10 */ for (i = 2; i < 10; i += 2) { square(t1, t0); square(t0, t1); }
	/* 2^50 - 2^0 */ mult(z2_50_0, t0, z2_10_0);

	/* 2^51 - 2^1 */ square(t0, z2_50_0);
	/* 2^52 - 2^2 */ square(t1, t0);
	/* 2^100 - 2^50 */ for (i = 2; i < 50; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^100 - 2^0 */ mult(z2_100_0, t1, z2_50_0);

	/* 2^101 - 2^1 */ square(t1, z2_100_0);
	/* 2^102 - 2^2 */ square(t0, t1);
	/* 2^200 - 2^100 */ for (i = 2; i < 100; i += 2) { square(t1, t0); square(t0, t1); }
	/* 2^200 - 2^0 */ mult(t1, t0, z2_100_0);

	/* 2^201 - 2^1 */ square(t0, t1);
	/* 2^202 - 2^2 */ square(t1, t0);
	/* 2^250 - 2^50 */ for (i = 2; i < 50; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^250 - 2^0 */ mult(t0, t1, z2_50_0);

	/* 2^251 - 2^1 */ square(t1, t0);
	/* 2^252 - 2^2 */ square(t0, t1);
	/* 2^253 - 2^3 */ square(t1, t0);
	/* 2^254 - 2^4 */ square(t0, t1);
	/* 2^255 - 2^5 */ square(t1, t0);
	/* 2^255 - 21 */ mult(out, t1, z11);
}

void curve25519_athlon(unsigned char ek[32],
	const unsigned char e[32],
	const unsigned char k[32])
{
	double work[30];
	curve25519_athlon_init();
	curve25519_athlon_todouble(work, k);
	curve25519_athlon_mainloop(work, e);
	curve25519_athlon_recip(work + 10, work + 10);
	mult(work + 20, work, work + 10);
	curve25519_athlon_fromdouble(ek, work + 20);
}
