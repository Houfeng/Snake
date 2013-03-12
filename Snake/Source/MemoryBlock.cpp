#include "MemoryBlock.h"
 #ifdef _DEBUG
 //#define new DEBUG_NEW
 #undef THIS_FILE
 static char THIS_FILE[]=__FILE__;
 #endif
 
 //////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemoryBlock::~CMemoryBlock()
{
}
void * CMemoryBlock::operator new(size_t,USHORT nTypes, USHORT nUnitSize)
{
    return malloc(sizeof(CMemoryBlock) + nTypes * nUnitSize);
}
void  CMemoryBlock::operator delete(void *p,size_t)
{
    ::operator delete (p);
}
//初始化
CMemoryBlock::CMemoryBlock (USHORT nTypes , USHORT nUnitSize)
{
    nFree = nTypes - 1;
    pNext = NULL;
    nSize = nTypes * nUnitSize;
    nFirst = 1;

    char* pData=aData;
    for(USHORT i=1;i<nTypes;i++)
    {
        *(USHORT*)pData=i;
        pData+=nUnitSize;
    }
}