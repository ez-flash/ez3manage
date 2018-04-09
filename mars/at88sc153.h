
// Copyright (C) Elva, 1998

class CGpa 
{
  private :
	// state var	
	unsigned char R[7];
	unsigned char S[7];
	unsigned char T[5];

	// key var
	unsigned char Crypto[8];	// cryptogram
	unsigned char GC[8];
	unsigned char Q0[8];
	unsigned char Q1[8];
	unsigned char Q2[8];


	void calculState(unsigned char Ri, unsigned char Ti, unsigned char Si);
	unsigned char calcul(void);

  public :
	CGpa();
	~CGpa();
	unsigned char calcul(unsigned char nbre);
	void calculKey(unsigned char key);
	void SetInit(unsigned char *crypto,unsigned char *graine,unsigned char *host);
	void Authenticate(unsigned char *q1,unsigned char *q2);
};

