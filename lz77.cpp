// Lz77.cpp: implementation of the CLz77 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lz77.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLz77::CLz77()
{
	textsize = 0,	/* text size counter */
	codesize = 0,	/* code size counter */
	printcount = 0;	/* counter for reporting progress every 1K bytes */
}

CLz77::~CLz77()
{

}

void CLz77::InitTree(void)  /* initialize trees */
{
	int  i;

	/* For i = 0 to N - 1, rson[i] and lson[i] will be the right and
	   left children of node i.  These nodes need not be initialized.
	   Also, dad[i] is the parent of node i.  These are initialized to
	   NIL (= N), which stands for 'not used.'
	   For i = 0 to 255, rson[N + i + 1] is the root of the tree
	   for strings that begin with character i.  These are initialized
	   to NIL.  Note there are 256 trees. */

	for (i = N + 1; i <= N + 256; i++) rson[i] = NIL;
	for (i = 0; i < N; i++) dad[i] = NIL;
}

void CLz77::InsertNode(int r)
/* Inserts string of length F, text_buf[r..r+F-1], into one of the
	   trees (text_buf[r]'th tree) and returns the longest-match position
	   and length via the global variables match_position and match_length.
	   If match_length = F, then removes the old node in favor of the new
	   one, because the old one will be deleted sooner.
	   Note r plays double role, as tree node and position in buffer. */
{
	int  i, p, cmp;
	unsigned char  *key;

	cmp = 1;  key = &text_buf[r];  p = N + 1 + key[0];
	rson[r] = lson[r] = NIL;  match_length = 0;
	for ( ; ; ) {
		if (cmp >= 0) {
			if (rson[p] != NIL) p = rson[p];
			else {  rson[p] = r;  dad[r] = p;  return;  }
		} else {
			if (lson[p] != NIL) p = lson[p];
			else {  lson[p] = r;  dad[r] = p;  return;  }
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - text_buf[p + i]) != 0)  break;
		if (i > match_length) {
			match_position = p;
			if ((match_length = i) >= F)  break;
		}
	}
	dad[r] = dad[p];  lson[r] = lson[p];  rson[r] = rson[p];
	dad[lson[p]] = r;  dad[rson[p]] = r;
	if (rson[dad[p]] == p) rson[dad[p]] = r;
	else                   lson[dad[p]] = r;
	dad[p] = NIL;  /* remove p */
}

void CLz77::DeleteNode(int p)  /* deletes node p from tree */
{
	int  q;

	if (dad[p] == NIL) return;  /* not in tree */
	if (rson[p] == NIL) q = lson[p];
	else if (lson[p] == NIL) q = rson[p];
	else {
		q = lson[p];
		if (rson[q] != NIL) {
			do {  q = rson[q];  } while (rson[q] != NIL);
			rson[dad[q]] = lson[q];  dad[lson[q]] = dad[q];
			lson[q] = lson[p];  dad[lson[p]] = q;
		}
		rson[q] = rson[p];  dad[rson[p]] = q;
	}
	dad[q] = dad[p];
	if (rson[dad[p]] == p) rson[dad[p]] = q;  else lson[dad[p]] = q;
	dad[p] = NIL;
}

int CLz77::Encode(BYTE* inbuf,int insize,BYTE* outbuf , int outsize)
// function patched to produce gba bios decompression function processable data
{
	textsize = 0;	/* text size counter */
	codesize = 0;	/* code size counter */

	int inpos =0 , outpos = 0;
	int  i, c, len, r, s, last_match_length, code_buf_ptr;
	unsigned char  code_buf[17], mask;

	// write 32 bit header needed for GBA BIOS function
	// Bit 0-3   Reserved
	// Bit 4-7   Compressed type (must be 1 for LZ77)
	// Bit 8-31  Size of decompressed data
	u32 gbaheader = 0x10 + (insize<<8);
	unsigned char* tmp = (unsigned char*)&gbaheader;
	//printf("gba header: %x\n", gbaheader );
	for(i=0; i<4; i++) 
	{
		outbuf[outpos]=tmp[i];
		outpos++;
	}

	InitTree();  /* initialize trees */
	code_buf[0] = 0;  /* code_buf[1..16] saves eight units of code, and
		code_buf[0] works as eight flags, "0" representing that the unit
		is an unencoded letter (1 byte), "1" a position-and-length pair
		(2 bytes).  Thus, eight units require at most 16 bytes of code. */
	code_buf_ptr = 1;
	mask = 0x80;	// f黵 GBA fangen wir mit MSB an
	s = 0;
	r = N - F; 	// 4078

	for (i = s; i < r; i++) text_buf[i] = 0xff;  /* Clear the buffer with any character that will appear often. */
	for (len = 0; len < F && inpos<insize/*(c = getc(infile)) != EOF*/; len++)
	{
		c = inbuf[inpos++] ;
		text_buf[r + len] = c;  /* Read F bytes into the last F bytes of the buffer */
	}
	if ((textsize = len) == 0) return 0 ;  /* text of size zero */
	for (i = 1; i <= F; i++) InsertNode(r - i);  /* Insert the F strings,
		each of which begins with one or more 'space' characters.  Note
		the order in which these strings are inserted.  This way,
		degenerate trees will be less likely to occur. */
	InsertNode(r);  /* Finally, insert the whole string just read.  The
		global variables match_length and match_position are set. */


	// kompressions schleife

	do {
		if (match_length > len) match_length = len;  /* match_length
			may be spuriously long near the end of text. */

		// nicht komprimieren
		if (match_length <= THRESHOLD) {
			match_length = 1;  /* Not long enough match.  Send one byte. */
			// original: code_buf[0] |= mask;  /* 'send one byte' flag */
			code_buf[code_buf_ptr++] = text_buf[r];  /* Send uncoded. */
		} else
		// komprimieren
		{
			code_buf[0] |= mask;  // flag "komprimiert" setzen

			// Bit 0-3   Disp MSBs
			// Bit 4-7   Number of bytes to copy (minus 3)
			// Bit 8-15  Disp LSBs

			code_buf[code_buf_ptr++] = (unsigned char)
				(( (r-match_position-1)>>8) & 0x0f) |
				((match_length - (THRESHOLD + 1))<<4);

			code_buf[code_buf_ptr++] = (unsigned char) ((r-match_position-1) & 0xff);
			/* Send position and length pair. Note match_length > THRESHOLD. */
		}

		// mask shift
		if ((mask >>= 1) == 0) {  /* Shift mask right one bit. */
			for (i = 0; i < code_buf_ptr; i++)  /* Send at most 8 units of */
				outbuf[outpos++] = code_buf[i] ;
				/* putc(code_buf[i], outfile);     code together */
			codesize += code_buf_ptr;
			code_buf[0] = 0;  code_buf_ptr = 1;
			mask = 0x80;
		}

		last_match_length = match_length;
		for (i = 0; i < last_match_length &&
				/*(c = getc(infile)) != EOF*/inpos<insize; i++) {
			c=inbuf[inpos++];
			DeleteNode(s);		/* Delete old strings and */
			text_buf[s] = c;	/* read new bytes */
			if (s < F - 1) text_buf[s + N] = c;  /* If the position is
				near the end of buffer, extend the buffer to make
				string comparison easier. */
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
				/* Since this is a ring buffer, increment the position
				   modulo N. */
			InsertNode(r);	/* Register the string in text_buf[r..r+F-1] */
		}
		if ((textsize += i) > printcount) {
			printf("%12ld\r", textsize);  printcount += 1024;
				/* Reports progress each time the textsize exceeds
				   multiples of 1024. */
		}
		while (i++ < last_match_length) {	/* After the end of text, */
			DeleteNode(s);					/* no need to read, but */
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
			if (--len) InsertNode(r);		/* buffer may not be empty. */
		}
	} while (len > 0);	/* until length of string to be processed is zero */


	if (code_buf_ptr > 1) {		/* Send remaining code. */
		for (i = 0; i < code_buf_ptr; i++) //putc(code_buf[i], outfile);
			outbuf[outpos++] = code_buf[i] ;
		codesize += code_buf_ptr;
	}

	// pad output with zeros to make it a multiply of 4
	codesize += 4 ; //加上开始
	if(codesize%16)
	{
		for(i=0; i<16-(codesize%16); i++)
			outbuf[outpos++]=0;
			//putc(0x00, outfile);
		codesize += 16-(codesize%16);
	}
	ASSERT(outpos == codesize);
	return codesize ;
	//printf("In : %ld bytes\n", textsize);	/* Encoding is done. */
	//printf("Out: %ld bytes\n", codesize);
	//printf("Out/In: %.3f\n", (double)codesize / textsize);
}

void CLz77::Decode(BYTE* inbuf,int insize,BYTE* outbuf , int outsize)	/* Just the reverse of Encode(). */
// also fixed to decode in the way bios funktion works
{
	textsize = 0;	/* text size counter */
	codesize = 0;	/* code size counter */

	int inpos =0,outpos=0 ;
	int  i, j, k, r, c, z;
	unsigned int  flags;
	u32 decomp_size;	// i磎 using decomp_size and cur_size to make sure we dont "decompress" the padding data
	u32 cur_size=0;		// we added to the compressed data to keep it in 4 byte boundaries

	// discard gba header info
	u32 gbaheader;
	unsigned char* tmp = (unsigned char*)&gbaheader;
	for(i=0; i<4; i++) 
	{
		tmp[i] = inbuf[inpos++];
		//tmp[i] = getc(infile);
	}
	decomp_size = gbaheader>>8;
	//printf("gba header: %x, decompressed size: %d\n", gbaheader, decomp_size );

	for (i = 0; i < N - F; i++) text_buf[i] = 0xff;
	r = N - F;  flags = z = 7;
	for ( ; ; ) {
		flags <<= 1;
		z++;
		if (z == 8) {				// read new block flag
			if (inpos>=insize/*(c = getc(infile)) == EOF*/) break;
			c = inbuf[inpos++];
			flags = c;
			z = 0;				// reset counter
		}
		if (!(flags&0x80)) {			// flag bit zero => uncompressed
			if (inpos>=insize/*(c = getc(infile)) == EOF*/) break;
			c = inbuf[inpos++] ;
			if(cur_size<decomp_size) /*putc(c, outfile);*/
				outbuf[outpos++] = c ;	
			text_buf[r++] = c;  
			r &= (N - 1); 
			cur_size++;
		} else {
			if (inpos>=insize/*(i = getc(infile)) == EOF*/) break;
			i=inbuf[inpos++];
			if (inpos>=insize/*(j = getc(infile)) == EOF*/) break;
			j=inbuf[inpos++];
			j = j | ((i<<8)&0xf00);		// match offset
			i = ((i>>4)&0x0f)+THRESHOLD;	// match length
			for (k = 0; k <= i; k++) {
				c = text_buf[(r-j-1) & (N - 1)];
				if(cur_size<decomp_size) /*0putc(c, outfile);*/
					outbuf[outpos++] = c ;
				text_buf[r++] = c;  
				r &= (N - 1); 
				cur_size++;
			}
		}
	}
}



