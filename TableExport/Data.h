#ifndef __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__

#include "PlatformLib.h"
#include "NetworkLib.h"
#include "MsgDefine.h"


namespace Lynx
{
    typedef StringMap<String> FilePathMap;
    typedef StringMap<bool>        FileSelectedMap;

    class Data
    {
    public:
        Data();
        virtual ~Data();

        bool Initial();
        void Release();
        void Update();
        void RegisterMsgHandler();

        // ����·�������ļ����ֺͺ�׺.xlsx��
        bool Export(String& filePath);

        // ��������ѡ���ļ�
        void ExportAllSelected();

        void ConvertCsvToDbc(const String& filePath);
        void DeleteFileByStr(const String& path, const String& del);
        void PromptInfo(void);

        // true:ȫ��ѡ�� false:ȫ��ȡ��ѡ��
        void SelectAllExcel(bool selected = true);

    private:
        // �ļ�·�������Ժ����·�����ɣ�
        String GetFullPath(const String& path);
        void        InitXL(void);
        void        ReleaseXL(void);

    public:
        FilePathMap        m_ExcelFileMap;
        FileSelectedMap    m_ExcelSelectedMap;
        String        m_FieldTableName;
        String        m_FieldTablePath;
        String        m_DictionaryName;
        String        m_DictionaryPath;
        
        unsigned long      m_StringSheetCount;
    };
} // namespace Lynx

#endif // __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__

