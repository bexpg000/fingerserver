#if defined(WIN32)
# include <windows.h>
#else
# include <pthread.h>
# include <errno.h>
# include <sys/time.h>
#endif

#include "Mutex.h"
#include "TimeUtil.h"
#include "Condition.h"
#include "Logger.h"

using namespace Lynx;

Condition::Condition()
{
#ifndef WIN32
    int ret = pthread_cond_init(&mId,0);
    ASSERT(ret == 0);
#else
    mBlocked = 0;
    mGone = 0;
    mWaiting = 0;
    mGate = reinterpret_cast<void*>(CreateSemaphore(0, 1, 1, 0));
    mQueue = reinterpret_cast<void*>(CreateSemaphore(0, 0, LONG_MAX, 0));
    mMutex = reinterpret_cast<void*>(CreateMutex(0, 0, 0));

    if (!mGate || !mQueue || !mMutex)
    {
       int res = 0;
       if (mGate)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(mGate));
           ASSERT(res);
       }
       if (mQueue)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(mQueue));
           ASSERT(res);
       }
       if (mMutex)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(mMutex));
           ASSERT(res);
       }

       ASSERT(0);
   }
#endif
}

Condition::~Condition ()
{
#ifndef WIN32
    int ret = pthread_cond_destroy(&mId);
    ASSERT(ret != EBUSY);
#else
    int res = 0;
    res = CloseHandle(reinterpret_cast<HANDLE>(mGate));
    ASSERT(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(mQueue));
    ASSERT(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(mMutex));
    ASSERT(res);
#endif
}

#if defined(WIN32)
void
Condition::enterWait ()
{
    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mGate), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);
    ++mBlocked;
    res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
    ASSERT(res);
}
#endif

void
Condition::wait(Mutex& mutex)
{
#ifndef WIN32
    int ret = pthread_cond_wait(&mId, mutex.getId());
    ASSERT(ret == 0);
#else
    enterWait();

    mutex.unlock();

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mQueue), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);

    unsigned wasWaiting = 0;
    unsigned wasGone = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mMutex), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);
    wasWaiting = mWaiting;
    wasGone = mGone;
    if (wasWaiting != 0)
    {
        if (--mWaiting == 0)
        {
            if (mBlocked != 0)
            {
                res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0); // open mGate
                ASSERT(res);
                wasWaiting = 0;
            }
            else if (mGone != 0)
            {
                mGone = 0;
            }
        }
    }
    else if (++mGone == (ULONG_MAX / 2))
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(mGate), INFINITE);
        ASSERT(res == WAIT_OBJECT_0);
        mBlocked -= mGone;
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
        ASSERT(res);
        mGone = 0;
    }
    res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
    ASSERT(res);

    if (wasWaiting == 1)
    {
        for (; wasGone; --wasGone)
        {
            res = WaitForSingleObject(reinterpret_cast<HANDLE>(mQueue), INFINITE);
            ASSERT(res == WAIT_OBJECT_0);
        }
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
        ASSERT(res);
    }

    mutex.lock();
#endif
}

void
Condition::wait(Mutex* mutex)
{
    wait(*mutex);
}

bool
Condition::wait(Mutex& mutex, unsigned int ms)
{
    if (ms == 0)
    {
        wait(mutex);
        return true;
    }
#ifndef WIN32
    uint64_t expires64 = TimeUtil::getLocalTimeMilliSec() + ms;
    timespec expiresTS;
    expiresTS.tv_sec = expires64 / 1000;
    expiresTS.tv_nsec = (expires64 % 1000) * 1000000L;

    ASSERT(expiresTS.tv_nsec < 1000000000L);

    int ret = pthread_cond_timedwait(&mId, mutex.getId(), &expiresTS);

    if (ret == EINTR || ret == ETIMEDOUT) 
    {
        return false;
    }
    else
    {
        ASSERT(ret == 0);
        return true;
    }
#else
    enterWait();

    mutex.unlock();

    bool ret = false;
    unsigned int res = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mQueue),ms);
    ASSERT(res != WAIT_FAILED && res != WAIT_ABANDONED);
    ret = (res == WAIT_OBJECT_0);

    unsigned wasWaiting = 0;
    unsigned wasGone = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mMutex), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);
    wasWaiting = mWaiting;
    wasGone = mGone;
    if (wasWaiting != 0)
    {
        if (!ret) 
        {
            if (mBlocked != 0)
                --mBlocked;
            else
                ++mGone; 
        }
        if (--mWaiting == 0)
        {
            if (mBlocked != 0)
            {
                res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0); // open mGate
                ASSERT(res);
                wasWaiting = 0;
            }
            else if (mGone != 0)
                mGone = 0;
        }
    }
    else if (++mGone == (ULONG_MAX / 2))
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(mGate), INFINITE);
        ASSERT(res == WAIT_OBJECT_0);
        mBlocked -= mGone;
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
        ASSERT(res);
        mGone = 0;
    }
    res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
    ASSERT(res);

    if (wasWaiting == 1)
    {
        for (; wasGone; --wasGone)
        {
            res = WaitForSingleObject(reinterpret_cast<HANDLE>(mQueue), INFINITE);
            assert(res ==  WAIT_OBJECT_0);
        }
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
        ASSERT(res);
    }

    mutex.lock();

    return ret;
#endif
}

bool
Condition::wait (Mutex* mutex, unsigned int ms)
{
    return wait(*mutex, ms);
}

void
Condition::signal ()
{
#ifndef WIN32
    int ret = pthread_cond_signal(&mId);
    ASSERT(ret == 0);
#else
    unsigned signals = 0;

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mMutex), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);

    if (mWaiting != 0) 
    {
        if (mBlocked == 0)
        {
            res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
            ASSERT(res);
            return;
        }

        ++mWaiting;
        --mBlocked;
        signals = 1;
    }
    else
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(mGate), INFINITE);
        ASSERT(res == WAIT_OBJECT_0);
        if (mBlocked > mGone)
        {
            if (mGone != 0)
            {
                mBlocked -= mGone;
                mGone = 0;
            }
            signals = mWaiting = 1;
            --mBlocked;
        }
        else
        {
            res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
            ASSERT(res);
        }
    }

    res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
    ASSERT(res);

    if (signals)
    {
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mQueue), signals, 0);
        ASSERT(res);
    }
#endif
}

void
Condition::broadcast()
{
#ifndef WIN32
    pthread_cond_broadcast(&mId);
#else
    unsigned signals = 0;

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(mMutex), INFINITE);
    ASSERT(res == WAIT_OBJECT_0);

    if (mWaiting != 0) 
    {
        if (mBlocked == 0)
        {
            res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
            ASSERT(res);
            return;
        }

        mWaiting += (signals = mBlocked);
        mBlocked = 0;
    }
    else
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(mGate), INFINITE);
        ASSERT(res == WAIT_OBJECT_0);
        if (mBlocked > mGone)
        {
            if (mGone != 0)
            {
                mBlocked -= mGone;
                mGone = 0;
            }
            signals = mWaiting = mBlocked;
            mBlocked = 0;
        }
        else
        {
            res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mGate), 1, 0);
            ASSERT(res);
        }
    }

    res = ReleaseMutex(reinterpret_cast<HANDLE>(mMutex));
    ASSERT(res);

    if (signals)
    {
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(mQueue), signals, 0);
        ASSERT(res);
    }
#endif
}

