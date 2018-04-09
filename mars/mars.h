#include "std_defs2.h"

class mars : public AES
{
private:
	u4byte   l_key[40];
public:
	char *name(void);
	void set_key(const u1byte key[], const u4byte key_bits);
	void encrypt(const u1byte in_blk[16], u1byte out_blk[16]);
	void decrypt(const u1byte in_blk[16], u1byte out_blk[16]);
};	 
