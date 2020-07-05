#ifndef _FIFOLIB_H_
#define _FIFOLIB_H_

#include "BaseType.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef int (*DATA_RELEASE)(void * pData);

typedef struct data_fifo
{
	P2PLock m_Lock;
	int m_iBase;			// fifo baseָ��
	int m_iTop;             // fifo topָ��
	int m_iMaxCnt;          //���ɻ������
	int m_iFlag;         
	int m_iPod;      	
	int m_iByteSzie;
	DATA_RELEASE m_ReleaseFun;
	void *m_pData[0];
} Data_FIFO,*PData_FIFO;


DLL_DECLARE Data_FIFO *Make_FIFO(int _iSize,DATA_RELEASE _Release);

DLL_DECLARE void FIFOClear(Data_FIFO *_ptFifo);

DLL_DECLARE INT32  P2PPushDataToFiFo(void *_pData, Data_FIFO *ptFifo);

DLL_DECLARE void *GetDataFromFiFo(Data_FIFO *_ptFifo);

DLL_DECLARE int GetFifoByteNum( Data_FIFO *_ptFifo);


#ifdef __cplusplus
}
#endif

#endif 


