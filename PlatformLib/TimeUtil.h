#ifndef __LYNX_PLATFORM_LIB_TIME_H__
#define __LYNX_PLATFORM_LIB_TIME_H__

#include "MemoryManager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEGAL_TIME(t)       (t >= 0 && t <=2713881599u)

#define TIMERCMP(tvp, uvp, cmp)                                 \
        (((tvp)->tv_sec == (uvp)->tv_sec) ?                     \
        ((tvp)->tv_usec cmp (uvp)->tv_usec) :                   \
        ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define TIMERADD(tvp, uvp, vvp)                                 \
do {                                                            \
        (vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;          \
        (vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
        if ((vvp)->tv_usec >= 1000000) {                        \
                (vvp)->tv_sec++;                                \
                (vvp)->tv_usec -= 1000000;                      \
        }                                                       \
} while (0)

#define TIMERSUB(tvp, uvp, vvp)                                 \
do {                                                            \
        (vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;          \
        (vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;       \
        if ((vvp)->tv_usec < 0) {                               \
                (vvp)->tv_sec--;                                \
                (vvp)->tv_usec += 1000000;                      \
        }                                                       \
} while (0)

struct Timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
};

struct Timezone {
        int     tz_minuteswest; /* minutes west of Greenwich */
        int     tz_dsttime;     /* type of dst correction */
};

#ifdef __cplusplus
}
#endif

namespace Lynx
{
    class _PlatformExport TimeUtil
    {
    public:
        static void SetTimeOffset(Int64 timeOffset);

        /********** 
         *��������* 
         **********/

        // ȡ����UTC Epoch��ȥ��ȥ�������൱�ڵ�ǰʱ��������, _tȱʡʱ��ʾ��ǰʱ��
        // _tȡֵ��Χ: [0, 2713881599] ([1970.1.1 08:00:00, 2055.12.31 23:59:59])
        static bool localTime(struct tm& tm_time, time_t _t = -1);

        // ȥ��ǰʱ��ĳ�����ڶ�Ӧ����UTC Epoch��ȥ�˶�����
        // _tmȡֵ��Χ: [1971.1.1 00:00:00, 2055.12.31 23:59:59]
        static time_t makeTime(struct tm& _tm);

        // ȡ��ǰʱ�̾�UTC Epoch��ȥ��ȥ�˶�����
        static time_t time();

        // ����/����/΢��(UTC)
        static UInt64 getTimeMicroSec();
        static UInt64 getTimeMilliSec();
        static UInt32 getTimeSec();

        static UInt64 getLocalTimeMilliSec();
        static UInt32 getLocalTimeSec();

        
        /********** 
         *�����޹�* 
         **********/

        // ���º��� _t ��ȡֵ��Χ��localTime��ͬ
        // ȡ��ǰʱ������00:00:00ʱ�̾�UTC Epoch��ȥ��ȥ�˶�����, 0��ʾ��Чֵ
        static UInt32 getTodayBaseSec(time_t _t = -1);

        // ȡ��ǰʱ�����վ�UTC Epoch��ȥ��ȥ�˶�����, 0��ʾ��Чֵ
        static UInt32 getDateNum(time_t _t = -1);

        // ȡ��ǰʱ����ǰʱ��Ϊ��ǰʱ�����յڼ���, -1��ʾ��Чֵ
        static Int32 getSecNum(time_t _t = -1);

        // ����ǰʱ����ǰʱ������λ������ʽ��ʾ, ��17:34 -> 1734, 08:02 -> 802, -1��ʾ��Чֵ
        static Int32 getTime(time_t _t = -1);


        /********** 
         *�������* 
         **********/

        // ���º��� _t ��ȡֵ��Χ��localTime��ͬ
        // ȡ�ַ�����ʽʱ���
        static bool getTimestamp(char* buf, Int32 size, UInt64 millisec);

        // ȡ���ָ�ʽʱ���(����������ʱ��), 0��ʾ��Чֵ
        static UInt64 getTimestamp64(time_t _t = -1);

        // ȡ�ַ�����ʽ����(Mon,Tue,Wed,Thu,Fri,Sat,Sun), Err��ʾ��Чֵ
        static String getDay(time_t _t = -1);

        // ȡ���ָ�ʽ����(0-6, 0Ϊ����), -1��ʾ��Чֵ
        static Int32 getDayNum(time_t _t = -1);

		// ȡ���ָ�ʽ����(1-7), -1��ʾ��Чֵ
		static Int32
		getSevenDayNum(time_t _t/* = -1*/);

        // ȡ��λ���ָ�ʽ����(����), -1��ʾ��Чֵ
        static Int32 getDate(time_t _t = -1);


        /********** 
         *  ����  * 
         **********/
        static UInt32 getTimeOfDay(Timeval* tv, Timezone* tz);
        static void sleep(UInt64 millisec);

		static Int32 getSecOfToday();
		//���쳽��������ת��Ϊ���������

		static	Int32 getSecOfToThatday(UInt32 timeSec);
		static Int32 convertSecToTodaySec(time_t  seconds);

		//���쳽, ������ת��Ϊ�����Сʱ
		static Int32 convertSecToTodayHour(time_t  seconds);

		//���ʱ���ַ���
		static bool getTimeStr(char* buf, Int32 size, UInt64 ms);

#if defined (_WIN32)
        static UInt32 getTickCount();
#endif
        static UInt64 getSystemTime();
	private:
		static Int64 sTimeOffset;
    };

	class Tick
    {
    public:
        Tick();
        virtual ~Tick();

        bool initial();
        void release();
        void update();

        // ��ȡ��һ֡���ѵ�ʱ��
        inline float getDeltaTime() const { return mDeltaTime; }
        // �õ��ۼ�ʱ��, �ӳ���������ʼ����
        inline float getAccumulateTime() const { return mAccumulateTime; }
        // ���֡��
        inline UInt32 getFps() const { return (UInt32)mFps; }
        // �õ�֡��
        inline UInt64 getFrameCount() const { return mFrameCount; }
        
    private:
		// ��������ʱ���¼
		UInt64 mStartTime;
        // ��ǰʱ��(����, ��������ʼ�ۼ�)
        UInt32 mCurrentTime;
        // ���20ms����ÿ֡ƽ�����ѵ�ʱ��
        float mLoopTime;
        // ��һ֡���ѵ�ʱ��
        float mDeltaTime;
        // ��ǰ֡��
        float mFps;
        // ��ǰ��֡��
        UInt64 mFrameCount;
        // ��һ֡ʱ��(����)
        UInt32 mLastTime;
        // ֡�ۼƼ���
        UInt32 mLoopCnt;
        // ʱ���ۼƼ���
        UInt32 mLoopHop;
        // �����������ۼ�ʱ��
        float mAccumulateTime;
    };

} // namespace KGame 

#endif // __LYNX_PLATFORM_LIB_TIME_H__


