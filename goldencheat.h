// GoldenCheat.h: interface for the CGoldenCheat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GOLDENCHEAT_H__A7598603_1881_4CD3_A416_80DE900A4211__INCLUDED_)
#define AFX_GOLDENCHEAT_H__A7598603_1881_4CD3_A416_80DE900A4211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGoldenCheat    
{
public:
	BOOL CheatEditGame(BYTE *pLoader,/*大小1Mbit*/
					   BYTE *pXCode ,/*仅xcode数据*/
					   BYTE *pCheatList,/*金手指列表*/
					   int xcodeIndex/*游戏的索引*/,
					   int cheatgame/*有金手指游戏的个数*/);
						
	BOOL CheatAddNewGame(BYTE *pLoader ,/*大小1Mbit*/
						   BYTE	*pGame , /*游戏数据*/
						   BYTE *pXCode ,/*仅xcode数据*/
						   BYTE *pCheatList,/*金手指列表*/
						   BYTE	entryIndex, /*入口索引*/
						   int xcodeIndex/*游戏的索引*/);
	BOOL IsXCodeValid(void* pXCode,BYTE* pGameData,UINT size );
	CGoldenCheat();
	virtual ~CGoldenCheat();

protected:
	DWORD	CountOffset(DWORD dwAddress,WORD uiNum);
	BYTE	GetResetFromXCode(void *pXCode);
	BYTE	GetReturnRegisterFromGame(DWORD dwAddress,void *pGameData);
	void	Set_BX_Reg(WORD *pBX , BYTE reg);
	void	Set_LDR_Reg(WORD *pBX , BYTE reg);
	void	Set_POP_Reg(WORD *pBX , BYTE reg);
	void	Set_MOV_Reg(WORD *pBX , BYTE reg);
};

#endif // !defined(AFX_GOLDENCHEAT_H__A7598603_1881_4CD3_A416_80DE900A4211__INCLUDED_)
