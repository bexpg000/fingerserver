#ifndef __LYNX_PLATFROM_LIB_LRU_CACHE_H__
#define __LYNX_PLATFROM_LIB_LRU_CACHE_H__

#include "PlatformLib.h"

namespace Lynx
{
    // �ص���������Ϊ�٣� ��ʾ�˶�����ɾ���� ��������Ϊ��Դ����ʹ��
    // LruCacheӦ�ø��¸ö�����ʱ�䣬 ����Map�аѶ����Ƴ�
    typedef Delegate<bool (void* dataPointer)> LruCacheRemoveCallback;

    struct LruElement
    {
        UInt32 mOperateTime;
        void* mData;
    };

    template <typename PointerType>
    class LruCache
    {
    public:
        LruCache(LruCacheRemoveCallback removeCallback, UInt32 cacheTime/*second*/) 
            : mCacheTime(cacheTime), mLastUpdateTime(0)
        {
            mRemoveCallback = removeCallback;
            mUpdateInterval = cacheTime / 2;
        }
        ~LruCache() { clear(); }

        void clear()
        {
            for (StringMap<LruElement>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end(); iter = mElementMap.next(iter))
            {
                // �˴������жϻص���������ֵ�� ֱ��ɾ����
                mRemoveCallback(iter->mValue.mData);
            }
            mElementMap.clear();
        }

        UInt32 size() const
        {
            return mElementMap.size();
        }

        void add(const String& name, PointerType pointer)
        {
            LruElement elem;
            elem.mData = pointer;
            elem.mOperateTime = TimeUtil::getLocalTimeSec();
            mElementMap.insert(name, elem);
        }

        //void remove(const String& name)
        //{
        //    StringMap<LruElement>::Iter* iter = mElementMap.find(name);
        //    if (iter != mElementMap.end())
        //    {
        //        // �˴������жϻص���������ֵ�� ֱ��ɾ����
        //        mRemoveCallback(iter->mValue.mData);
        //        mElementMap.erase(iter);
        //    }
        //}

        PointerType get(const String& name)
        {
            StringMap<LruElement>::Iter* iter = mElementMap.find(name);
            if (iter != mElementMap.end())
            {
                iter->mValue.mOperateTime = TimeUtil::getLocalTimeSec();
                return (PointerType)iter->mValue.mData;
            }
            return NULL;
        }

        void update()
        {
            UInt32 currTime = TimeUtil::getLocalTimeSec();
            if (currTime - mLastUpdateTime < mUpdateInterval)
            {
                return;
            }
                
            LruElement* elem = NULL;
            for (StringMap<LruElement>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end();)
            {
                if (iter->mValue.mOperateTime + mCacheTime <= currTime)
                {
                    // �ص���������Ϊ�٣� ��ʾ�˶�����ɾ���� ��������Ϊ��Դ����ʹ��
                    // LruCacheӦ�ø��¸ö�����ʱ�䣬 ����Map�аѶ����Ƴ�
                    if (!mRemoveCallback(iter->mValue.mData))
                    {
                        iter->mValue.mOperateTime = TimeUtil::getLocalTimeSec();
                        iter = mElementMap.next(iter);
                    }
                    else
                    {
                        iter = mElementMap.erase(iter);
                    }
                }
                else
                {
                    iter = mElementMap.next(iter);
                }
            }
        }
    private:
        StringMap<LruElement> mElementMap;
        LruCacheRemoveCallback mRemoveCallback;
        UInt32 mCacheTime;
        UInt32 mUpdateInterval;
        UInt32 mLastUpdateTime;
    };
} // namespace Lynx


#endif // __LYNX_PLATFROM_LIB_LRU_CACHE_H__

