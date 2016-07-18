#ifndef __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__
#define __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__

#include <string>
#include <locale>
#include <cwchar>
#include <cstddef> 
#include "MemoryManager.h"

#define CODECVT_DO_LENGTH_CONST const

namespace Lynx
{
    class _PlatformExport utf8_codecvt_facet : public std::codecvt<wchar_t, char, std::mbstate_t>  
    {
    public:
        explicit utf8_codecvt_facet(std::size_t no_locale_manage=0)
            : std::codecvt<wchar_t, char, std::mbstate_t>(no_locale_manage) {}
    protected:
        virtual std::codecvt_base::result do_in(std::mbstate_t& state, 
            const char * from, const char * from_end, const char * & from_next,
            wchar_t * to, wchar_t * to_end, wchar_t*& to_next) const;

        virtual std::codecvt_base::result do_out(std::mbstate_t & state, const wchar_t * from,
            const wchar_t * from_end, const wchar_t*  & from_next,
            char * to, char * to_end, char * & to_next) const;

        bool invalid_continuing_octet(unsigned char octet_1) const 
        {
            return (octet_1 < 0x80|| 0xbf< octet_1);
        }

        bool invalid_leading_octet(unsigned char octet_1)   const 
        {
            return (0x7f < octet_1 && octet_1 < 0xc0) || (octet_1 > 0xfd);
        }

        // continuing octets = octets except for the leading octet
        static unsigned int get_cont_octet_count(unsigned   char lead_octet) 
        {
            return get_octet_count(lead_octet) - 1;
        }

        static unsigned int get_octet_count(unsigned char   lead_octet);

        // How many "continuing octets" will be needed for this word
        // ==   total octets - 1.
        int get_cont_octet_out_count(wchar_t word) const ;

        virtual bool do_always_noconv() const throw() { return false; }

        // UTF-8 isn't really stateful since we rewind on partial conversions
        virtual std::codecvt_base::result do_unshift(std::mbstate_t&,
            char * from, char * /*to*/, char * & next) const 
        {
            next = from;
            return ok;
        }

        virtual int do_encoding() const throw() 
        {
            const int variable_byte_external_encoding=0;
            return variable_byte_external_encoding;
        }

        // How many char objects can I process to get <= max_limit
        // wchar_t objects?
        virtual int do_length(CODECVT_DO_LENGTH_CONST std::mbstate_t &,
            const char * from, const char * from_end, std::size_t max_limit) const;

        // Largest possible value do_length(state,from,from_end,1) could return.
        virtual int do_max_length() const throw () 
        {
            return 6; // largest UTF-8 encoding of a UCS-4 character
        }
    };

    class _PlatformExport EncodingConvert
    {
    public:
        // ת���ַ������ANSI��UNICODE(wchar_t) 
        static BasicStringW   ansi2Unicode(const BasicString& s, 
            const std::codecvt<wchar_t, char, mbstate_t>& cvt);
        // ת���ַ������UNICODE��ANSI
        static BasicString    unicode2Ansi(const BasicStringW& s, 
            const std::codecvt<wchar_t, char, mbstate_t>& cvt);
        // ת���ַ������UTF-8��UNICODE(wchar_t)
        static BasicStringW   utf82Unicode(const BasicString& s);
        // ת���ַ������UNICODE��UTF-8
        static BasicString    unicode2Utf8(const BasicStringW& s);
        // ת���ַ������UNICODE��MBCS
        static BasicString    unicode2Mbcs(const BasicStringW& s, 
            const std::locale& loc = std::locale());
        // ת���ַ������MBCS��UNICODE
        static BasicStringW   mbcs2Unicode(const BasicString& s, 
            const std::locale& loc = std::locale());
        // ת���ַ������UTF-8��MBCS
        static BasicString    utf82Mbcs(const BasicString& s, 
            const std::locale& loc = std::locale());
        // ת���ַ������MBCS��UTF-8
        static BasicString    mbcs2Utf8(const BasicString& s, 
            const std::locale& loc = std::locale());
        // ת���ַ������ANSI��UTF-8
        static BasicString    ansi2Utf8(const BasicString& s,
            const std::locale& loc = std::locale());
        // ת���ַ������UTF-8��ANSI
        static BasicString    utf82Ansi(const BasicString& s,
            const std::locale& loc = std::locale());
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__

/***************************************************************************************************************************
ANSI/ASCII : ���������ұ�׼ίԱ��ָ�����ǻ���������ĸ��һ�׵��Ա���ϵͳ��
����Ҫ������ʾ�ִ�Ӣ���������ŷ���ԡ�
�����ֽ���ͨ�õĵ��ֽڱ���ϵͳ������ͬ�ڹ��ʱ�׼ISO 646����from wiki��

MBCS : ���ֽ��ַ�������һ����� Unicode ��֧���޷��õ��ֽڱ�ʾ���ַ����������ĺ����ģ��ķ�����
Ϊ�����г����ʱӦ����ʹ�� Unicode �� MBCS����ʹ�����ܹ�ͨ�����Ŀ���������֧�������ַ���֮һ�ĳ���
����� MBCS ʵ����˫�ֽ��ַ��� (DBCS)��һ����˵��Visual C++�������� MFC����ȫ֧�� DBCS����from msdn��

UNICODE : ��ҵ���һ�ֱ�׼��������ʹ���Ե��Գ�����������ʮ�ֵ�����ϵͳ��
Unicode �ǻ���ͨ����Ԫ����Universal Character Set���ı�׼����չ��
����ͬʱҲ���鱾����ʽ��The Unicode Standard��Ŀǰ�������Addison-Wesley Professional���棬ISBN-10: 0321480910�����ⷢ��
Unicode �����˳���ʮ�����Ԫ������Ԫ 2005 �꣬ Unicode �ĵ�ʮ�����Ԫ���������Ͽɳ�Ϊ��׼֮һ����
һ���������Ϊ�Ӿ��ο��Ĵ���ͼ��
һ�ױ��뷽����һ���׼��Ԫ���롢һ�װ������ϱ��֡��±��ֵ���Ԫ���Ե��оٵȡ�(from wiki)

UTF-8 : Ӣ��ȫ��Ϊ8-bit UCS/Unicode Transformation Format�������Unicode ��һ�ֿɱ䳤����Ԫ���롣
�����ƿ��Կ�����UTF-8��רΪUCS/Unicode��ƵĴ����ʽ��������������ʾ Unicode ��׼�е��κ���Ԫ��
��������봮���еĵ�һ��λԪ������ ASCII ���ݣ���ԭ������ ASCII �ַ�����������ֻ�������Ķ���
��ɼ���ʹ�á���ˣ����𽥳�Ϊ�����ʼ�����ҳ����������������ֵ�Ӧ���У����Ȳ��õı��롣(from wiki)

���������ʽ������ˣ���������ϸ�Ľ���һ��Charset/encoding���ַ���/���룩��
Charset�����׺�encoding��죬Ҳ�Ǹտ�ʼ�Ӵ��ַ����������������ױ��ε��ĸ��
�ַ����ĸ��ʵ���ϣ������������棬һ�������ַ��ļ��ϣ�����ν��Charset��
һ���Ǳ��뷽����Ҳ������ν��encoding��
��ν�ַ��ļ��ϣ��⼴һ���ַ������������������������з��ţ���ʵ���������ַ������ֵ��������塣
Ҳ����˵�������ϵ��ַ����������������뷽�����������Ƕ�������Щ������������ַ�����
���ǣ�ͨ����˵��һ���ַ����������������ַ����ϣ�ͬʱ������Ϊÿ�����Ŷ���һ�������Ʊ��룬
���ԣ��������ᵽGB2312��ʱ�����ǲ���������ָGB2312�ַ�����
ͬʱ��Ҳָ���˱��뷽����GB2312����Charset= GB2312,encoding=GB2312��
��˵�������ǵ��ĵ��в�����GB2312������ַ��������ǵĶ����Ʊ������GB2312�涨�ı��뷽ʽ��
�򵥵������ȷ��ˣ��ַ������ڱ��룬��������ַ�����

���ǣ�ͨ�������Ǹ��εģ�����һ�����⣬Unicode��
Unicode�ַ���������ı��뷽��ͨ����ΪUCS-2������һ����ͨ�õ����֣� UTF-16��
Ȼ��������UTF-16���ܺ����еĻ���ascii�ı��뷽�����ݣ��Ƚ��ص����������0x0��
�ڻ���ascii�ı��뷽���У�һ��8λ�� 0x0���Ǳ�ʾһ���ַ����Ľ����ģ�
��UTF-16��Ȼ������һ���ַ���
��ȫ�п����ڸ�8λ���ߵ�8λ�ϵ���0x0����ᵼ�ºܶ�Ӧ�ó������
�����������紫��Э�鵱�п��ܵ��´������ַ�������ضϡ�
���ǣ�����UTF-8��UTF-8�ṩ��һ����ascii���ݵ�unicode�ַ������뷽����
�����ϳ�����˵��˵UTF-8��1��3λ�䳤���룬���Ǵ���ģ�UTF-8��1��6λ�䳤���룬
3λ��˵����Դ�ڴ�������ú��ֱ�������3λ����ķ������ڣ�
�����⣬�����е�Unicode�淶������UTF-8ʵ������1��4λ�ı��룬
��Ϊ�ټ�����λ��������չ�ķ�Χ����Unicode��û�ж����κ��ַ��� 
UTF-8�ı��뷽�����ȱ�֤��127����׼ascii�ַ����ݣ�Ҳ����˵����UTF-8�����£�
Unicode��0x000000�C0x00007F��Χ���ַ������Ϊ0x0-0x7F��һ���ֽڵĶ����Ʊ��롣
��Σ�UTF-8��֤������0x7F���ϵ��ַ�,�ڱ�ת��ɶ��ֽ��ַ�ʱ��ÿ���ֽڵ����λһ��Ϊ1��
��ʵ����Ҳ�Ǵ����MBCS�����Ļ���ԭ��
����Ӧ�ó���û��ʶ����ֽ��ַ����ֽ���Ϸ�ʽ���߳��ִ����0x0��
�������ڣ�һ���DBCS˫�ֽڷ������Լ򵥵ĸ��ݸ�λ�Ƿ�Ϊ1���ж����ֽڻ���˫�ֽڣ�
��UTF-8�Ǳ䳤�ģ�Ӧ�ó�����Ҫ֪���������������ֽ����ݣ�
����UTF-8�Ĺ涨��������ߵ�һ���ֽ��⣬����������ֽھ���10��ͷ��������ֽڵĿ�ͷ��
110��ʾ����2λ��1110��ʾ����3λ��11110��ʾ����4λ��
��һ�������ַ���UTF-8����ֵ����һ���򵥵Ķ�Ӧ�㷨��UTF-16�õ�������Ͳ���ϸ������������google����
��ˣ����Ǳ�����ȷһ�����UTF-8�� unicode�ַ�����һ�����뷽����
��������˵��UTF-8�ַ���Unicode�ַ���ʱ��
��ĳЩ����£��������߼����ǵȼ۵ģ����ǣ����ǲ�����ͬһ��������
��ΪUnicode�ַ��ڶ������ϻ���һ��ѡ�����ԭ����UTF-16���롣

�ܽ�һ�£�Unicode�ַ����涨�ı�׼���뷽����UCS-2��UTF-16�����������ֽڱ�ʾһ��Unicode�ַ���
����ʵ�ϣ�UCS-4 ��UTF-32��Ҳ�Ѿ�������ˣ���4���ֽڱ�ʾһ��Unicode�ַ���Ȼ��
һ�����õ�Unicode���뷽����UTF-8,����1��4��6��λ�ı䳤�ֽ�����ʾһ��Unicode�ַ���
�����Դ�һ���򵥵�ת���㷨��UTF-16ֱ�ӵõ���
���������뷽����encoding������Ӧ��Unicode�ַ�����Charset����

Ȼ����Ҫ����һ��Codepage-����ҳ��codepageʵ������һ�ű�
ͨ����codepage��һ����unicode������mbcs��ת��������
����windows�ϳ��õ�MS936����ҳ��ʵ���Ͼ���GB2312��unicode��ת����
����֪����windows����ȫ���� unicode�ģ�
MS��Ӧ�ó���Ҳ����ǻ���unicode�����ģ����Ƕ�GB2312��֧�֣�
������Դ��codpage932��ͨ��cp932��ת����
Ӧ�ó��������unicode��gb2312֮������ת����
��Ҫ��һ������ǹ������ı��룬IBM��΢���ṩ�˼�����ҵ�ϳ��õ����ı���Ĵ���ҳ���������shift-jis��
IBM�Ĵ���ҳ��CCSID943��
��Ҳ��java��ת��ʱʹ�õĴ���ҳ����MS�Ĵ���ҳ��MS936���������߶������ shift-jis�Ĵ���ҳ��
�����������еĹ�ҵ��׼����ĳЩ�ַ���ת���������һ�£�
����ܻᵼ��Ӧ�ó������
�����������ĵ���������ͨ��java����shift-jis����ת��Ϊunicode�����VC��д�ĳ�����ת��Ϊshift-jis��
����������ת����ʹ���˲����ݵĴ���ҳ���������ݴ���
�Ⲣû��ʲô�ر�Ľ���취��ֻ���ǲ��������ַ��б������⴦��

����һ�����⣬������ν�Ļ���unicode��Ӧ�ó�����Ӧ�ó����ڲ��ַ���������ʲô��ʽ��
һ����˵��
����UCS-2Ҳ����UTF-16 �Ķ�������ʽ�����ģ�UTF-8һ��ֻ����Ϊ���ݴ����ʽ���ĵ������ʽ��
����һ�㣬
����java���룬javac�����������û��ָ��Դ�ļ����������£����ǽ�����Ϊ�Ǳ���ȱʡ���룬
�����ڼ�������windows�ϻᱻ��ʶΪGB2312��
��������windows�ϻᱻʶ��Ϊshift-jis��Ȼ��ʵ���ϣ��ڱ����ʱ��gb����jis�ַ����ᱻת���ɶ�Ӧ��UCS-2������ֵ��
������C++���򣬵���򵥵��� ���С� �������������õ���ʽ��ʱ��
��������򵥵Ľ���ʶ��Ϊ���ر��벢���ڱ����Ķ����ƴ����б����Ϊ���ر���Ķ�����ֵ
���Ҳ�֪��c++�������ܷ�ָ��Դ�ļ����롣�����Ҳ�̫��Ϥc++����
���ǣ�������� L���С� �����ı�׼c++��unicode�ַ����巽ʽ�������ַ�ʱ�����ᷢ����javac�������һ�������飬
�������������Ľ��ַ���ȷ���Ա��ر���ʶ��ת����UCS-2ֵ����Ϊ�����Ķ����ƴ��롣
C++�����У������L�궨���ַ�����
�����ڳ�����ֱ�ӵõ�Unicodeֵ����UTF-16����ֵ������ java�У�ʵ�������ǲ�����ֱ�ӵõ��ַ����Ķ�����ֵ��
��������ͨ��String��getbytes����ָ��UTF-16����õ������ڸ��ֱ���֮���ת��������Ͳ�׸���ˡ�

��󣬽���һ�¹���font�����⣬��ʵ���ܶ�ʱ���������������벢û�г��ֱ���ת������
ֻ������Ӧ�ó���ָ��������û�а�����Ӧ���ַ�ͼ����ѣ�
���������ͨ�����ǿ��Կ����в����ַ��ǿ���������ʾ�ġ�Font�ļ�ʵ����Ҳ��һ�������ļ���
һ��encoding��������λͼ��
���ڵ������ļ�ͨ��������������������һ��unicode��һ���������Ӧ���Եĳ��ñ��룬
Ӧ�ó�������ʾ�ַ���ʱ��ͨ�����Ҷ�Ӧ���������õ��ַ���λͼ��
******************************************************************************************************************************/



