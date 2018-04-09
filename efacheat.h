#pragma once
class CEfaCheat
{
public:
	CEfaCheat(void);
	~CEfaCheat(void);
	BOOL	IsXCodeValid(void* pXCode,BYTE* pGameData ,UINT size );
	BOOL	CheatAddNewGame(BYTE *pGame,/*游戏指针，名称为Loader，但实际上是游戏*/
							BYTE *pPosition,/*置放的位置，应该在pGame之内*/
							BYTE *pXCode ,/*仅xcode数据*/
							BYTE *pCheatList,/*金手指列表*/
							int  cheatsize);
	BOOL	InstanceAddNewGame(BYTE *pGame,/*游戏指针，名称为Loader，但实际上是游戏*/
							BYTE *pPosition,/*置放的位置，应该在pGame之内*/
							BYTE *pXCode ,/*仅xcode数据*/
							BYTE *pCheatList,/*金手指列表*/
							int  cheatsize,
							DWORD	dwNandAddress /*nand Flash 存档位置的起始地址*/
							);

	//bool	CheatEditGame(BYTE *pXCode ,/*仅xcode数据*/
	//						  BYTE *pCheatList,/*金手指列表*/
	//						  BYTE *pPostion/*游戏金手指的位置*/);
private:
	DWORD	CountOffset(DWORD dwAddress,WORD uiNum);
	BYTE	GetResetFromXCode(void *pXCode);
	BYTE	GetReturnRegisterFromGame(DWORD dwAddress,void *pGameData);
	void	Set_BX_Reg(WORD *pBX , BYTE reg);
	void	Set_LDR_Reg(WORD *pBX , BYTE reg);
	void	Set_POP_Reg(WORD *pBX , BYTE reg);
	void	Set_MOV_Reg(WORD *pBX , BYTE reg);

};
