
// Copyright (C) Elva, 1998

#include "stdafx.h"
#include "AT88SC153.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGpa::CGpa()
{
}

CGpa::~CGpa()
{
}


void CGpa::SetInit(unsigned char *crypto, //ci
				   unsigned char *graine, //gc
				   unsigned char *host)   //q0
{
unsigned char i;

	for (i=0;i<8;i++)
	{
		Crypto[i] = crypto[i];
		GC[i] = graine[i];
		Q0[i] = host[i];
	}

	// Init R and S
	for (i=0;i<7;i++)
	{
		R[i] = 0;
		S[i] = 0;
	}

	// Init T
	for (i=0;i<5;i++)
	{
		T[i] = 0;
	}
}

void CGpa::Authenticate(unsigned char *q1,unsigned char *q2)
{
int i;

	for (i=0;i<4;i++)
	{
		calculKey(Crypto[2*i]);
		calculKey(Crypto[2*i+1]);
		calculKey(Q0[i]);
	}
	for (i=0;i<4;i++)
	{
		calculKey(GC[2*i]);
		calculKey(GC[2*i+1]);
		calculKey(Q0[i+4]);
	}


	for (i=0;i<8;i++)
	{
		Q2[i] = calcul(2);
		Q2[i] <<= 4;
		Q2[i] |= calcul(2);
		Q1[i] = calcul(2);
		Q1[i] <<= 4;
		Q1[i] |= calcul(2);

		q2[i] = Q2[i];
		q1[i] = Q1[i];

	}

}

void CGpa::calculKey(unsigned char key)
{
unsigned char Ri,Ti,Si;

	// calcul des variables intermediaires
	Ri = key & 0x1F;
	Si = ((key << 3) & 0x78) + ((key >> 5) & 0x07);
	Ti = (key >> 3) & 0x1F;

	calculState(Ri,Ti,Si);
}

void CGpa::calculState(unsigned char Ri, unsigned char Ti, unsigned char Si)
{
unsigned char temp,bit0;

	bit0 = (R[6] >> 4) & 1;
	temp = (2*R[6] | bit0) & 0x1F;
	R[6] = R[5];
	R[5] = R[4];
	R[4] = R[3];
	R[3] = R[2] ^ Ri;
	R[2] = R[1];
	R[1] = R[0];
	R[0] = (R[4] + temp);
	if ( R[0] > 31 )
	{
		R[0] %= 31;
		if ( R[0] == 0 ) R[0] = 31;
	}

	bit0 = (S[6] >> 6) & 1;
	temp = (2*S[6] | bit0) & 0x7F;
	S[6] = S[5];
	S[5] = S[4] ^ Si;
	S[4] = S[3];
	S[3] = S[2];
	S[2] = S[1];
	S[1] = S[0];
	S[0] = (S[6] + temp) ;
	if ( S[0] > 127 )
	{
		S[0] %= 127;
		if ( S[0] == 0 ) S[0] = 127;
	}

	temp = T[4];
	T[4] = T[3];
	T[3] = T[2];
	T[2] = T[1] ^ Ti;
	T[1] = T[0];
	T[0] = (T[3] + temp);
	if ( T[0] > 31 )
	{
		T[0] %= 31;
		if ( T[0] == 0 ) T[0] = 31;
	}
}

unsigned char CGpa::calcul(void)
{
unsigned char Ri,Ti,Si_,Si;
unsigned char lastCalc = 0;

	calculState(0,0,0);
	Ri = (R[0] ^ R[4]) & 0x1F;
	Ti = (T[0] ^ T[3]) & 0x1F;
	Si  = S[0];
	Si_ = ~S[0];

	lastCalc = ( (Ri & Si_) | (Si & Ti) ) & 0xF;
	return lastCalc;
}

unsigned char CGpa::calcul(unsigned char nbre)
{
unsigned char i;
unsigned char calc;

	for (i=0;i<nbre;i++)
	{
		calc = calcul();
	}
	return calc;
}


