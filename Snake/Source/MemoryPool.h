#ifndef __MemoryPool_h_
#define __MemoryPool_h_

#include "MemoryBlock.h"
#define MEMPOOL_ALIGNMENT 4


class CMemoryPool  
{
public:
    CMemoryPool(USHORT uUnitSize,USHORT uInitSize,USHORT uGrowSize); 
    virtual ~CMemoryPool();
    
    void* Alloc();
    void Free(void* pFree);
    
private:
    void FreeMemeoryBlock(CMemoryBlock* pMyBlock);
    CMemoryBlock*   pBlock;        //第一个block的指针 
    USHORT         nUnitSize;      //每个小内存块的字节数
    USHORT         nInitSize;      //初始的Block的内存块数目
    USHORT         nGrowSize;      //增加的Block的内存块数目
};

#endif