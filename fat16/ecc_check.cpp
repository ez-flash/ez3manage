// ECC_Check.cpp: implementation of the CECC_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ECC_Check.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECC_Check::CECC_Check()
{

}

CECC_Check::~CECC_Check()
{

}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*		make_ecc_512                                                         */
/* DESCRIPTION                                                               */
/*		This function generates 3 byte ECC for 512 byte data.                */
/*      (Software ECC)                                                       */
/* PARAMETERS                                                                */
/*		ecc_buf			the location where ECC should be stored              */
/*		data_buf		given data                                           */
/* RETURN VALUES                                                             */
/*		none                                                                 */
/*                                                                           */
/*****************************************************************************/
#if (XMODE == 8)
void CECC_Check::make_ecc_512(uint8_t * ecc_buf, uint8_t * data_buf)
#else
void CECC_Check::make_ecc_512(uint16_t * ecc_buf, uint16_t * data_buf)
#endif
{
	
    uint32_t	i, ALIGN_FACTOR; 
	uint32_t	tmp;
	uint32_t	uiparity = 0;
	uint32_t	parityCol, ecc = 0;
	uint32_t	parityCol4321 = 0, parityCol4343 = 0, parityCol4242 = 0, parityColTot = 0;
	uint32_t	*Data;
	uint32_t	Xorbit=0;

	ALIGN_FACTOR = (uint32_t)data_buf % 4 ;
	Data = (uint32_t *)(data_buf + ALIGN_FACTOR);

	for( i = 0; i < 16; i++)
	{
		parityCol = *Data++; 
		tmp = *Data++; parityCol ^= tmp; parityCol4242 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4343 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4242 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4321 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4321 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
		tmp = *Data++; parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;

		parityColTot ^= parityCol;

		tmp = (parityCol >> 16) ^ parityCol;
		tmp = (tmp >> 8) ^ tmp;
		tmp = (tmp >> 4) ^ tmp;
		tmp = ((tmp >> 2) ^ tmp) & 0x03;
		if ((tmp == 0x01) || (tmp == 0x02))
		{
			uiparity ^= i;
			Xorbit ^= 0x01;
		}
	}

#if (XMODE == 8)
	tmp = (parityCol4321 >> 16) ^ parityCol4321;
	tmp = (tmp << 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x200;	// p128
#else
	tmp = (parityCol4321 >> 16) ^ parityCol4321;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x80;	// p128
#endif
#if (XMODE == 8)
	tmp = (parityCol4343 >> 16) ^ parityCol4343;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x80;	// p64
#else
	tmp = (parityCol4343 >> 16) ^ parityCol4343;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x20;	// p64
#endif
#if (XMODE == 8)
	tmp = (parityCol4242 >> 16) ^ parityCol4242;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x20;	// p32
#else
	tmp = (parityCol4242 >> 16) ^ parityCol4242;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x08;	// p32
#endif
#if (XMODE == 8)
	tmp = parityColTot & 0xFFFF0000;
	tmp = tmp >> 16;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x08;	// p16
#else
	tmp = parityColTot & 0xFFFF0000;
	tmp = tmp >> 16;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x02;	// p16
#endif
#if (XMODE == 8)
	tmp = parityColTot & 0xFF00FF00;
	tmp = (tmp >> 16) ^ tmp;
	tmp = (tmp >> 8);
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x02;	// p8
#else
	tmp = parityColTot & 0xFF00FF00;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8);
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x800000;	// p8
#endif
#if (XMODE == 8)
	tmp = parityColTot & 0xF0F0F0F0 ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x800000;	// p4
#else
	tmp = parityColTot & 0xF0F0F0F0 ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x200000;	// p4
#endif
#if (XMODE == 8)
	tmp = parityColTot & 0xCCCCCCCC ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp >> 2);
	ecc |= ((tmp << 1) ^ tmp) & 0x200000;	// p2
#else
	tmp = parityColTot & 0xCCCCCCCC ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x80000;	// p2
#endif
#if (XMODE == 8)
	tmp = parityColTot & 0xAAAAAAAA ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= (tmp & 0x80000);	// p1
#else
	tmp = parityColTot & 0xAAAAAAAA ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= (tmp & 0x20000);	// p1
#endif
#if (XMODE == 8)
	ecc |= (uiparity & 0x01) <<11;	
	ecc |= (uiparity & 0x02) <<12;	
	ecc |= (uiparity & 0x04) <<13;
	ecc |= (uiparity & 0x08) <<14;
#else
	ecc |= (uiparity & 0x01) <<9;
	ecc |= (uiparity & 0x02) <<10;
	ecc |= (uiparity & 0x04) <<11;
	ecc |= (uiparity & 0x08) <<12;
#endif

	if (Xorbit)
	{
		ecc |= (ecc ^ 0x00AAAAAA)>>1;
	}
	else
	{
		ecc |= (ecc >> 1);
	}
#if (XMODE == 8)
	ecc = ~ecc;
	*(ecc_buf + 2) = (uint8_t) (ecc >> 16);
	*(ecc_buf + 1) = (uint8_t) (ecc >> 8);
	*(ecc_buf + 0) = (uint8_t) (ecc);
#else	// X16
	ecc = ( ~ecc ) | 0xFF000000;
	*(ecc_buf + 1) = (uint16_t) (ecc >> 16);
	*(ecc_buf + 0) = (uint16_t) (ecc);
#endif
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*		compare_ecc_512                                                      */
/* DESCRIPTION                                                               */
/*		This function compares two ECCs and indicates if there is an error.  */
/* PARAMETERS                                                                */
/*		ecc_data1		one ECC to be compared                               */
/*		ecc_data2		the other ECC to be compared                         */
/*		page_data		content of data page                                 */
/*		offset			where the error occurred                             */
/*		corrected		correct data                                         */
/* RETURN VALUES                                                             */
/*		Upon successful completion, compare_ecc returns SSR_SUCCESS.         */
/*      Otherwise, corresponding error code is returned.                     */
/*                                                                           */
/*****************************************************************************/
#if (XMODE == 8)
eccdiff_t CECC_Check::compare_ecc_512(uint8_t *iEccdata1, uint8_t *iEccdata2, 
          uint8_t *pPagedata, int32_t *pOffset, uint8_t *pCorrected)
#else	// X16
eccdiff_t CECC_Check::compare_ecc_512(uint16_t *iEccdata1, uint16_t *iEccdata2, 
          uint16_t *pPagedata, int32_t *pOffset, uint16_t *pCorrected)
#endif
{

	uint32_t  iCompecc = 0, iEccsum = 0;
    uint32_t  iFindbyte   = 0;
    uint32_t  iIndex;
    uint32_t  nT1 = 0, nT2 =0;

#if (XMODE == 8)
	uint8_t   iNewvalue;
    uint8_t   iFindbit    = 0;

    uint8_t   *pEcc1 = (uint8_t *)iEccdata1;
    uint8_t   *pEcc2 = (uint8_t *)iEccdata2;

	for ( iIndex = 0; iIndex <2; iIndex++)
    {
        nT1 ^= (((*pEcc1) >> iIndex) & 0x01);
        nT2 ^= (((*pEcc2) >> iIndex) & 0x01);
    }

    for (iIndex = 0; iIndex < 3; iIndex++)
        iCompecc |= ((~(*pEcc1++) ^ ~(*pEcc2++)) << iIndex * 8);
    
    for(iIndex = 0; iIndex < 24; iIndex++) {
        iEccsum += ((iCompecc >> iIndex) & 0x01);
    }

#else	// X16
	uint16_t   iNewvalue;
    uint16_t   iFindbit    = 0;
	
	uint16_t   *pEcc1 = (uint16_t *)iEccdata1;
    uint16_t   *pEcc2 = (uint16_t *)iEccdata2;

	for ( iIndex = 0; iIndex <2; iIndex++)
    {
        nT1 ^= (((*pEcc1) >> iIndex) & 0x01);
        nT2 ^= (((*pEcc2) >> iIndex) & 0x01);
    }

	for (iIndex = 0; iIndex < 2; iIndex++)		// 2 word of ECC data
		iCompecc |= (((~*pEcc1++) ^ (~*pEcc2++)) << iIndex * 16);
	
	for(iIndex = 0; iIndex < 24; iIndex++) {
		iEccsum += ((iCompecc >> iIndex) & 0x01);
	}
#endif
    
    switch (iEccsum) {
    case 0 :
		printf("RESULT : no error\n");
        return ECC_NO_ERROR;

	case 1 :
		printf("RESULT : ECC code 1 bit fail\n");
        return ECC_ECC_ERROR;

    case 12 :
        if (nT1 != nT2)
        {
#if (XMODE == 8)
            iFindbyte = ((iCompecc >> 17 & 1) << 8) + ((iCompecc >> 15 & 1) << 7) + ((iCompecc >> 13 & 1) << 6)
                      + ((iCompecc >> 11 & 1) << 5) + ((iCompecc >> 9 & 1) << 4) + ((iCompecc >> 7 & 1) << 3)
                      + ((iCompecc >> 5 & 1) << 2) + ((iCompecc >> 3 & 1) << 1) + (iCompecc >> 1 & 1);
            iFindbit =  (uint8_t)(((iCompecc >> 23 & 1) << 2) + ((iCompecc >> 21 & 1) << 1) + (iCompecc >> 19 & 1));
            iNewvalue = (uint8_t)(pPagedata[iFindbyte] ^ (1 << iFindbit));
#else // CASE_X16
            iFindbyte = ((iCompecc >> 15 & 1) << 7) + ((iCompecc >> 13 & 1) << 6)
                      + ((iCompecc >> 11 & 1) << 5) + ((iCompecc >> 9 & 1) << 4) + ((iCompecc >> 7 & 1) << 3)
                      + ((iCompecc >> 5 & 1) << 2) + ((iCompecc >> 3 & 1) << 1) + (iCompecc >> 1 & 1) ;
            iFindbit =  (uint16_t)(((iCompecc >> 23 & 1) << 3) + ((iCompecc >> 21 & 1) << 2) + ((iCompecc >> 19 & 1) << 1) 
                      + (iCompecc >> 17 & 1) );
			iNewvalue = (uint16_t)(pPagedata[iFindbyte] ^ (1 << iFindbit));
#endif
			printf("iCompecc = %d\n",iCompecc);
            printf("RESULT : one bit error\r\n");
            printf("byte = %d, bit = %d\r\n", iFindbyte, iFindbit);
            printf("corrupted = %x, corrected = %x\r\n", pPagedata[iFindbyte], iNewvalue);

            if (pOffset != NULL) {
                *pOffset = iFindbyte;
            }
            if (pCorrected != NULL) {
                *pCorrected = iNewvalue;
            }
            
            return ECC_CORRECTABLE_ERROR;
        }
        else
            return ECC_UNCORRECTABLE_ERROR;

    default :
		printf("RESULT : unrecoverable error\n");
        return ECC_UNCORRECTABLE_ERROR;
    }   
}
