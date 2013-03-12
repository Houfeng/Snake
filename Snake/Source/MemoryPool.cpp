#include "MemoryPool.h"
 #ifdef _DEBUG
 #undef THIS_FILE
 static char THIS_FILE[]=__FILE__;
 //#define new DEBUG_NEW
 #endif
 //////////////////////////////////////////////////////////////////////
 // Construction/Destruction
 //////////////////////////////////////////////////////////////////////

CMemoryPool::~CMemoryPool()
{
    CMemoryBlock* pMyBlock = pBlock;
    if(pBlock)
        FreeMemeoryBlock(pMyBlock);
}

 //递归释放内存
 void CMemoryPool::FreeMemeoryBlock(CMemoryBlock *pMyBlock)
 {
     if(pMyBlock->pNext)
         FreeMemeoryBlock(pMyBlock->pNext);
 
     delete pMyBlock;
}
 
 
CMemoryPool::CMemoryPool(USHORT _uUnitSize,USHORT _uInitSize,USHORT _uGrowSize)
 {
    pBlock=NULL;
     nInitSize=_uInitSize;
     nGrowSize=_uGrowSize;
     
     if(_uUnitSize<=2)
         nUnitSize=2;
     else if(_uUnitSize>2 && _uUnitSize<=4)
         nUnitSize=4;
     else 
     {
         if(_uUnitSize % MEMPOOL_ALIGNMENT ==0)
             nUnitSize=_uUnitSize;
         else
             nUnitSize=(_uUnitSize/MEMPOOL_ALIGNMENT+1)*MEMPOOL_ALIGNMENT;
     }
 }
 
 //申请内存
 void* CMemoryPool::Alloc()
 {
     if(!pBlock)
     {
         pBlock=new(nInitSize,nUnitSize) CMemoryBlock(nInitSize,nUnitSize);
         return pBlock->aData;
     }
 
     CMemoryBlock * pMyBlock=pBlock;
     while(pMyBlock && !pMyBlock->nFree)
         pMyBlock = pMyBlock->pNext;
 
     //接下来的两种情况：1.找到有空闲块的block 2.找不到
     void * retval;
     if(pMyBlock)
     {
         pMyBlock->nFree--;
         retval = pMyBlock->aData + nUnitSize * pMyBlock->nFirst;
         pMyBlock->nFirst = *((USHORT*)retval);
         return retval;
     }
     else
     {
         if(!nGrowSize)
             return NULL;
         CMemoryBlock* newBlock = new(nGrowSize,nUnitSize) CMemoryBlock(nGrowSize,nUnitSize);
         if(!newBlock)
             return NULL;
         newBlock->pNext = pBlock;
         pBlock = newBlock;
         return (void*)(newBlock->aData);
     }
     
     return NULL;
 }
 
 void CMemoryPool::Free(void* pFree)
 {
     CMemoryBlock* pMyBlock = pBlock;
     CMemoryBlock* pPreBlock = NULL;    //pMyBlock指向Block的前一个Block，用于设置pNext
     while(
		 (ULONG)pFree < (ULONG)pMyBlock->aData 
		 || (ULONG)pFree > (ULONG)(pMyBlock->aData + pMyBlock->nSize)
		 )
     {
         pPreBlock = pMyBlock;
         pMyBlock = pMyBlock->pNext;
 
         if(!pMyBlock)
             return;
     }
     if(pMyBlock)
     {
        pMyBlock->nFree++;
        *((USHORT*)pFree) = pMyBlock->nFirst;
        pMyBlock->nFirst =( (USHORT)( (ULONG)pFree - (ULONG)pMyBlock->aData)/nUnitSize );
        
        if(pMyBlock->nFree * nUnitSize == pMyBlock->nSize)   //如果全是自由块
        {
            if(!pMyBlock->pNext)  //如果这是最后一个block,则将其释放
            {
                delete pMyBlock;  //释放
                if(pPreBlock)
                    pPreBlock->pNext = NULL;  //设置上一块的pNext指向NULL
                else
                    pBlock = NULL;
            }
                
        }
    }
}
