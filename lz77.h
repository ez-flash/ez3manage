// Lz77.h: interface for the CLz77 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LZ77_H__2A5F37F7_8DB4_426E_B3C2_09AEB5E2EE46__INCLUDED_)
#define AFX_LZ77_H__2A5F37F7_8DB4_426E_B3C2_09AEB5E2EE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

#define N		 4096	/* size of ring buffer */
#define F		   18	/* upper limit for match_length */
#define THRESHOLD	2  /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */
typedef unsigned long u32;
class CLz77  
{
public:
	CLz77();
	virtual ~CLz77();
public:
//	struct stat finfo;
	unsigned long int
		textsize,	/* text size counter */
		codesize,	/* code size counter */
		printcount;	/* counter for reporting progress every 1K bytes */
	unsigned char
		text_buf[N + F - 1];	/* ring buffer of size N,
			with extra F-1 bytes to facilitate string comparison */
	int		match_position, match_length,  /* of longest match.  These are
			set by the InsertNode() procedure. */
		lson[N + 1], rson[N + 257], dad[N + 1];  /* left & right children &
			parents -- These constitute binary search trees. */
	//FILE	*infile, *outfile;  /* input & output files */
private:
	void InitTree(void);  /* initialize trees */
	void InsertNode(int r);
	void DeleteNode(int p);  /* deletes node p from tree */
public:
	void Decode(BYTE* inbuf,int insize,BYTE* outbuf , int outsize);	/* Just the reverse of Encode(). */
	int Encode(BYTE* inbuf,int insize,BYTE* outbuf , int outsize);
	//void Encode(void);
	//void Decode(void);	/* Just the reverse of Encode(). */

};

#endif // !defined(AFX_LZ77_H__2A5F37F7_8DB4_426E_B3C2_09AEB5E2EE46__INCLUDED_)
