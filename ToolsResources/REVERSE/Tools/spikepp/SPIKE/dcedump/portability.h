/*
portability.h

This should handle differences in archetectures and stuff and
provide 32 bit unsigned values portably and stuff

*/

#ifndef PORTABILITY
#define PORTABILITY

typedef unsigned int uint32;

/*used in twofish3.c*/
#define byte(x,n)   ((u1byte)((x) >> (8 * n)))
#define rotl(x,n)   (((x) << ((int)(n))) | ((x) >> (32 - (int)(n))))
#define rotr(x,n)   (((x) >> ((int)(n))) | ((x) << (32 - (int)(n))))


#ifndef u1byte
typedef unsigned char u1byte;	/* an 8 bit unsigned character type */
typedef unsigned short u2byte;	/* a 16 bit unsigned integer type   */
typedef uint32 u4byte;	/* a 32 bit unsigned integer type   */
typedef char s1byte;	/* an 8 bit signed character type   */
typedef short s2byte;	/* a 16 bit signed integer type     */
typedef long s4byte;	/* a 32 bit signed integer type     */
#endif




#endif
