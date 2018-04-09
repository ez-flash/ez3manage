// ECC_Check.h: interface for the CECC_Check class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECC_CHECK_H__C330281E_3445_4651_8972_7828BD330E8F__INCLUDED_)
#define AFX_ECC_CHECK_H__C330281E_3445_4651_8972_7828BD330E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	XMODE	8

/*****************************************************************************/
/* Integer Types                                                             */
/*****************************************************************************/

typedef unsigned long		uint32_t;			/* unsigned 4 byte integer */
typedef signed long			int32_t;			/* signed 4 byte integer */
typedef unsigned short		uint16_t;			/* unsigned 2 byte integer */
typedef signed short		int16_t;			/* signed 2 byte integer */
typedef unsigned char		uint8_t;			/* unsigned 1 byte integer */
typedef signed char			int8_t;				/* signed 1 byte integer */
typedef enum {
	ECC_NO_ERROR			= 0,		/* no error */
	ECC_CORRECTABLE_ERROR	= 1,		/* one bit data error */
	ECC_ECC_ERROR			= 2,		/* one bit ECC error */
	ECC_UNCORRECTABLE_ERROR	= 3			/* uncorrectable error */
} eccdiff_t;
/*****************************************************************************/
/* Address Types                                                             */
/*****************************************************************************/

typedef unsigned char *		address_t;			/* address (pointer) */
typedef unsigned long		address_value_t;	/* address (for calculation) */


class CECC_Check  
{
protected:


public:
	CECC_Check();
	virtual ~CECC_Check();

#if (XMODE == 8)
	void make_ecc_512(uint8_t * ecc_buf, uint8_t * data_buf);
	eccdiff_t compare_ecc_512(uint8_t *iEccdata1, uint8_t *iEccdata2, 
			  uint8_t *pPagedata, int32_t *pOffset, uint8_t *pCorrected);
#else //mode16
	void make_ecc_512(uint16_t * ecc_buf, uint16_t * data_buf);
	eccdiff_t compare_ecc_512(uint16_t *iEccdata1, uint16_t *iEccdata2, 
			  uint16_t *pPagedata, int32_t *pOffset, uint16_t *pCorrected);
#endif

};

#endif // !defined(AFX_ECC_CHECK_H__C330281E_3445_4651_8972_7828BD330E8F__INCLUDED_)
