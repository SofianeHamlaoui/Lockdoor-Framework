
extern void E_P16(unsigned char *p14,unsigned char *p16);
extern void E_P24(unsigned char *p21, unsigned char *c8, unsigned char *p24);
extern void D_P16(unsigned char *p14, unsigned char *in, unsigned char *out);
extern void E_old_pw_hash( unsigned char *p14, unsigned char *in, unsigned char *out);
extern void cred_hash1(unsigned char *out,unsigned char *in,unsigned char *key);
extern void cred_hash2(unsigned char *out,unsigned char *in,unsigned char *key);
extern void cred_hash3(unsigned char *out,unsigned char *in,unsigned char *key, int forw);
extern void SamOEMhash( unsigned char *data, unsigned char *key, int val);
