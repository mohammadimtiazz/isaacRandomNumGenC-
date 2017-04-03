// isaacTest01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#ifdef _MSC_VER
  typedef unsigned __int32 uint32_t;
#else
  #include <stdint.h>
#endif
 using namespace std;
/* a ub4 is an unsigned 4-byte quantity */
typedef  uint32_t  ub4;
 
/* external results */
ub4 randrsl[1280], randrslX[1280], randcnt;
 
/* internal state */
static    ub4 mm[1280];
static    ub4 aa=0, bb=0, cc=0;

/* if (flag!=0), then use the contents of randrsl[] to initialize mm[]. */
#define mix(a,b,c,d,e,f,g,h) \
{ \
   a^=b<<11; d+=a; b+=c; \
   b^=c>>2;  e+=b; c+=d; \
   c^=d<<8;  f+=c; d+=e; \
   d^=e>>16; g+=d; e+=f; \
   e^=f<<10; h+=e; f+=g; \
   f^=g>>4;  a+=f; g+=h; \
   g^=h<<8;  b+=g; h+=a; \
   h^=a>>9;  c+=h; a+=b; \
}





void isaac()
{
   register ub4 i,x,y;
 
   cc = cc + 1;    /* cc just gets incremented once per 256 results */
   bb = bb + cc;   /* then combined with bb */
 
   for (i=0; i<1280; ++i)
   {
     x = mm[i];
     switch (i%4)
     {
     case 0: aa = aa^(aa<<13); break;
     case 1: aa = aa^(aa>>6); break;
     case 2: aa = aa^(aa<<2); break;
     case 3: aa = aa^(aa>>16); break;
     }
     aa              = mm[(i+128)%1280] + aa;
     mm[i]      = y  = mm[(x>>2)%1280] + aa + bb;
     randrsl[i] = bb = mm[(y>>10)%1280] + x;
   }

   


   for (i = 0; i < 1280; i++)
   {
	   randrslX[i] = randrsl[i]%1280;
   }

   for (i = 0; i < 1280; i++) cout << randrslX[i] << "  ";





   // not in original readable.c
   randcnt = 0;
}


void randinit(int flag)
{
   register int i;
   ub4 a,b,c,d,e,f,g,h;
   aa=bb=cc=0;
   a=b=c=d=e=f=g=h=0x9e3779b9;  /* the golden ratio */
 
   for (i=0; i<4; ++i)          /* scramble it */
   {
     mix(a,b,c,d,e,f,g,h);
   }
 
   for (i=0; i<1280; i+=8)   /* fill in mm[] with messy stuff */
   {
     if (flag)                  /* use all the information in the seed */
	 {
       a+=randrsl[i  ]; b+=randrsl[i+1]; c+=randrsl[i+2]; d+=randrsl[i+3];
       e+=randrsl[i+4]; f+=randrsl[i+5]; g+=randrsl[i+6]; h+=randrsl[i+7];
     }
     mix(a,b,c,d,e,f,g,h);
     mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
     mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
   }
 
   if (flag)
   {        /* do a second pass to make all of the seed affect all of mm */
	 for (i=0; i<1280; i+=8)
     {
       a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
       e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
       mix(a,b,c,d,e,f,g,h);
       mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
       mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
     }
   }
   for (i = 0; i < 256; i++) cout << mm[i] << "  ";
   cout << endl;
 
   isaac();            /* fill in the first set of results */
   randcnt=0;        /* prepare to use the first set of results */
}


void iSeed(int *seed, int flag, unsigned int mX)
{
	register ub4 i,m;
	for (i=0; i<1280; i++) mm[i]=0;
	m = mX;
	//m = sizeof(seed)/sizeof(seed[0]);
	//m = strlen(seed);
	for (i=0; i<1280; i++)
	{
	// in case seed has less than 256 elements
        if (i>m) randrsl[i]=0;  else randrsl[i] = seed[i];
	}
	// initialize ISAAC with seed
	randinit(flag);
}


// maximum length of message
#define MAXMSG 4096
#define MOD 95
#define START 32

int main()
{
	register ub4 n,l;
	// input: message and key
	char *msg = "a Top Secret secret";
	int key[] = {1,2,3};


	// Vernam ciphertext & plaintext
	char vctx[MAXMSG], vptx[MAXMSG];


	// Caesar ciphertext & plaintext
	char cctx[MAXMSG], cptx[MAXMSG];
	l = strlen(msg);


	// Encrypt: Vernam XOR
	iSeed(key,1, 3);
}
