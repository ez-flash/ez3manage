#include "std_defs2.h"

class rijndael : public AES
{
private:
	u4byte  k_len;
	u4byte  e_key[64];
	u4byte  d_key[64];

public:
	char *name(void);
	void set_key(const u1byte key[], const u4byte key_len);
	void encrypt(const u1byte in_blk[16], u1byte out_blk[16]);
	void decrypt(const u1byte in_blk[16], u1byte out_blk[16]);
};	 
