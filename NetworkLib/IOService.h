#ifndef __LYNX_CORE_LIB_IOSERVICE_H__
#define __LYNX_CORE_LIB_IOSERVICE_H__

#include "ErrorCode.h"
#include "StreamBuffer.h"
#include "TimeUtil.h"

#define IOSERVICE_EPOLL_DRIVER                    0x00000001   // EPOOLģ������
#define IOSERVICE_IOCP_DRIVER                     0x00000010   // IOCPģ������
#define IOSERVICE_SELECT_DRIVER                   0x00000100   // SELECTģ������

#define IOSERVICE_TIMEOUT_EVENT                   0x01000000  // ��ʱ�¼�
#define IOSERVICE_READ_EVENT                      0x00000100  // �ɶ��¼�
#define IOSERVICE_WRITE_EVENT                     0x00001000  // ��д�¼�
#define IOSERVICE_SIGNAL_EVENT                    0x00000100  // �ź��¼�

namespace Lynx
{
    struct IOEvent;
    typedef void (*IOEventCallback)(IOEvent*, UInt32);

    //struct Overlapped 
    //{
    //    unsigned long* Internal;
    //    unsigned long* InternalHigh;
    //    union 
    //    {
    //        struct
    //        {
    //            unsigned long Offset;
    //            unsigned long OffsetHigh;
    //        };
    //
    //        void* Pointer;
    //    };
    //    void* hEvent;
    //};

    struct IOEvent
    {
        Int32                  mFd;             // �׽����ļ�������
        UInt32                 mErrno;          // IOEvent���¼������е�λ��
        UInt32                 mEventType;      // �¼�����
        bool                   mValid;          // �¼���Ч, ��ʾ����ӵ��¼�������
        IOEventCallback        mCallback;       // �¼���Ӧ�Ļص�����
        void*                  mOwner;          // �¼�����ָ��
        StreamBuffer::Node*    mData;           // �¼�����ָ��, ���������첽�շ�
        Int32                  mHeapIndex;      // ��С������, ������¼Timeoutʹ��
        Timeval                mTimeout;        // ��ʱʱ��
        Timeval                mDuration;       // ��ʱ���
                                                 // �����Ա��IOCPģ��ר��
        //Int32                  mListenFd;       // �����ļ�������
        //Overlapped             mOverlapped;     // �ص��ṹ����
        //UInt32                 mDstIp;          // Զ��IP��ַ, ͨ��inet_addr���ַ����õ�
        //UInt16                 mDstPort;        // Զ�˶˿ڵ�ַ
        //UInt8                  mPostConnect;    // ��ʶ�Ƿ�ΪͶ������, ��0��Ч
        //UInt8                  mPostAccept;     // ��ʶ�Ƿ�ΪͶ�ݽ���, ��0��Ч
        //UInt32                 mPostListen;     // ��ʶ�Ƿ�ΪͶ�ݼ���, ��0��Ч
        //UInt32                 mDataSize;       // ������¼����BUFFER�Ŀ��пռ��С

        IOEvent() : mFd(0), mErrno(0), mEventType(0), mCallback(0), mOwner(0)
                  , mData(0), mHeapIndex(-1), mValid(false)
                  //, mListenFd(-1), mDstIp(0), mDstPort(0), mPostConnect(0)
                  //, mPostAccept(0), mPostListen(0), mDataSize(0)
        {
            //memset(&mOverlapped, 0, sizeof(mOverlapped));
        }
    };

    class IOService;
    struct IOEventOperator
    {
        Int32                  mType;
        const char*            mName;
        void*                  (*mInitial)(IOService* ioService);
        void                   (*mRelease)(void* op);
        Int32                  (*mAddEvent)(IOEvent* ev, void* op);
        Int32                  (*mDelEvent)(IOEvent* ev, void* op);
        Int32                  (*mDispatch)(IOService* ioService, void* op, Timeval* tv);
    };

    struct IOEventMinHeap
    {
        IOEventMinHeap() : mHeap(0), mSize(0), mAllocNumber(0) {}
        ~IOEventMinHeap() { if (mHeap) free(mHeap); }

        inline bool empty() { return (mSize == 0u); }
        inline UInt32 size() { return mSize; }
        inline IOEvent* top() { return mSize ? *mHeap : 0; }
  
        Int32 elemGreater(IOEvent* a, IOEvent* b)
        {
            return TIMERCMP(&a->mTimeout, &b->mTimeout, >);
        }

        inline Int32 push(IOEvent* ev)
        {
            if (reserve(mSize + 1))
            {
                return -1;
            }
            shiftUp(mSize++, ev);
            return 0;
        }

        inline IOEvent* pop()
        {
            if (mSize)
            {
                IOEvent* ioEvent = *mHeap;
                shiftDown(0u, mHeap[--mSize]);
                ioEvent->mHeapIndex = -1;
                return ioEvent;
            }
            return 0;
        }

        inline Int32 erase(IOEvent* ev)
        {
            if (-1 != ev->mHeapIndex)
            {
                IOEvent* last = mHeap[--mSize];
                UInt32 parent = (ev->mHeapIndex - 1) / 2;
                // we replace e with the last element in the heap.  We might need to
                // shift it upward if it is less than its parent, or downward if it is
                // greater than one or both its children. Since the children are known
                // to be less than the parent, it can't need to shift both up and down.
                if (ev->mHeapIndex > 0 && elemGreater(mHeap[parent], last))
                {
                    shiftUp(ev->mHeapIndex, last);
                }
                else
                {
                    shiftDown(ev->mHeapIndex, last);
                }
                ev->mHeapIndex = -1;
                return 0;
            }
            return -1;
        }

        inline Int32 reserve(UInt32 n)
        {
            if (mAllocNumber < n)
            {
                UInt32 number = mAllocNumber ? mAllocNumber * 2 : 8;
                if (number < n)
                {
                    number = n;
                }
                IOEvent** ev = 0;
                if (!(ev = (IOEvent**)realloc(mHeap, number * sizeof(*ev))))
                {
                    return -1;
                }
                mHeap = ev;
                mAllocNumber = number;
            }
            return 0;
        }

        inline void shiftUp(UInt32 holeIndex, IOEvent* ev)
        {
            UInt32 parent = (holeIndex - 1) / 2;
            while (holeIndex && elemGreater(mHeap[parent], ev))
            {
                (mHeap[holeIndex] = mHeap[parent])->mHeapIndex = holeIndex;
                holeIndex = parent;
                parent = (holeIndex - 1) / 2;
            }
            (mHeap[holeIndex] = ev)->mHeapIndex = holeIndex;
        }

        inline void shiftDown(UInt32 holeIndex, IOEvent* ev)
        {
            unsigned minChild = 2 * (holeIndex + 1);
            while (minChild <= mSize)
            {
                minChild -= minChild == mSize || elemGreater(mHeap[minChild], mHeap[minChild - 1]);
                if (!(elemGreater(ev, mHeap[minChild])))
                {
                    break;
                }
                (mHeap[holeIndex] = mHeap[minChild])->mHeapIndex = holeIndex;
                holeIndex = minChild;
                minChild = 2 * (holeIndex + 1);
            }
            (mHeap[holeIndex] = ev)->mHeapIndex = holeIndex;
        }

    public:
        IOEvent**              mHeap;
        UInt32                 mSize;
        UInt32                 mAllocNumber;
    };

    class IOService
    {
    public:
        IOService(Int32 type = IOSERVICE_SELECT_DRIVER);
        virtual ~IOService();

        inline Int32 getType() { return mIOEventOperator->mType; }

        Int32 addEvent(IOEvent* ev);
        void  delEvent(IOEvent* ev);

        Int32 poll(UInt32 times = 1);
        Int32 run();

        void pause();
        void resume();
        void stop();
        
    private:
        bool                   mShutdown;           // ��ʶ�����Ƿ�ر�
        bool                   mPaused;             // ��ʶ�����Ƿ���ͣ
        IOEventOperator*       mIOEventOperator;    // �¼���������ָ��, ��ָ�������������
        void*                  mOperation;          // �¼�������������ָ��, Ӧ�ò�ֻ������,//selectģ�����ݣ�epollģ�����ݵ�
        IOEventMinHeap         mTimeoutHeap;        // ��ʱ������ʹ�õ���С��
        Timeval                mTimeFlag;           // ��ʱ������ʹ�õ�ʱ����, ��������ʱ��
    };
} // namespace Lynx

#endif // __LYNX_CORE_LIB_IOSERVICE_H__


