#ifndef __MemoryBlock_h_
#define __MemoryBlock_h_

#include "System.h"
#define USHORT int
#define ULONG long
class CMemoryBlock  
 {
public: 
    CMemoryBlock (USHORT nTypes = 1, USHORT nUnitSize = 0);    
    virtual ~CMemoryBlock();
    
    void  operator delete(void *p,size_t);
    void * operator new(size_t,USHORT nTypes, USHORT nUnitSize);    
    
public:
    USHORT          nSize;              //内存块的大小  每个小块所占内存*小块个数
    USHORT          nFree;              //空闲块数
    USHORT          nFirst;             //第一个空闲块
    //USHORT          nDummyAlign1;        
    CMemoryBlock*  pNext;               //下一个Block
    char            aData[1];           //数据的初始位置

};

#endif