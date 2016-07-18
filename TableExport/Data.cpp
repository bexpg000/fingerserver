#include "XLHandler.h"
#include "Data.h"
#include "TableExport.h"
#include "DBCMaker.h"
#include <stdio.h> 
#include "HelpHandler.h"
#include "ResourceGroupManager.h"

using namespace Lynx;

//const String g_DictionaryTemplateName = "DictionaryTemplate.xlsx";

static unsigned int g_ConvertCount = 0;
static unsigned int g_SuccessCount = 0;
static std::string g_ConvertPrompt = "";

static XLHandler s_XLHanlder;

// ��¼��CSVת��ΪDBC��ת��״̬
#define PROMPT_CONVERT(param1)    do{                                   \
    String fileName;                                               \
    if (param1 == 1)                                                    \
    {                                                                   \
        g_ConvertPrompt += "\nת���ļ�: ";                              \
        fileName = lynxGetFileName(iter->mValue.c_str());                  \
        g_ConvertPrompt += fileName;                                    \
        g_ConvertCount++;                                               \
    }                                                                   \
    else if (param1 == 2)                                               \
    {                                                                   \
        g_SuccessCount++;                                               \
        g_ConvertPrompt += "\nת���ɹ�:";                               \
    }                                                                   \
    else if (param1 == 3)                                               \
    {                                                                   \
        g_ConvertPrompt += "\nת��ʧ��:";                               \
    }                                                                   \
}while(0)

Data::Data() : m_StringSheetCount(0)
{
    m_FieldTableName.clear();
    //m_FieldTablePath.clear();
    m_DictionaryName.clear();
    m_DictionaryPath.clear();
}

Data::~Data()
{
    Release();
}

bool 
Data::Initial()
{
    m_FieldTableName = CONFIG_MANAGER().getAsString("TableName");
    //m_FieldTablePath = CONFIG_MANAGER().getAsString("TablePath");

    m_DictionaryName = "DictionaryTemplate.xlsx";

    ResourceGroup dataResourceGroup;
    if (!ResourceGroupManager::getSingleton().getResourceGroup("Table", dataResourceGroup))
    {
        LOG_WARN("Failed to get Data resource group.");
        return false;
    }

    // ��������excel�ļ�
    for (ResourceGroup::Iter* iterData = dataResourceGroup.begin();
        iterData != dataResourceGroup.end(); iterData = dataResourceGroup.next(iterData))
    {
        FileUtil::findFiles(iterData->mKey, "*.xlsx", m_ExcelFileMap);
    }
#if 0
    // ����m_FieldTablePath��m_FieldTableName
    FilePathMap::Iter* iter = m_ExcelFileMap.find(m_FieldTableName);
    if (iter != m_ExcelFileMap.end())
    {
        m_FieldTablePath = iter->mValue;
        m_ExcelFileMap.erase(iter);
    }
    // ת��m_FieldTablePathΪ����·��
    if (!m_FieldTablePath.empty())
    {
        m_FieldTablePath = GetFullPath(m_FieldTablePath);
    }
    else
    {
        ::MessageBox(NULL, _T("�滻�ֶα����·��Ϊ��!"), _T("������ʾ"), MB_OK);
        return false;
    }
#endif
    // �����ֵ�ģ��
    //FilePathMap::Iter* iter_dictionary = m_ExcelFileMap.find(m_DictionaryName);
    //if (iter_dictionary != m_ExcelFileMap.end())
    //{
    //    m_DictionaryPath = iter_dictionary->mValue;
    //    m_ExcelFileMap.erase(iter_dictionary);
    //}

    //// ȡ�ֵ��ľ���·��
    //if (!m_DictionaryPath.empty())
    //{
    //    m_DictionaryPath = GetFullPath(m_DictionaryPath);
    //}
    //else
    //{
    //    ::MessageBox(NULL, _T("�ֵ�����·��Ϊ��!"), _T("������ʾ"), MB_OK);
    //    return false;
    //}

    // ��ʼ��excel�ļ�ѡ��map��Ĭ�϶�Ϊfalse
    for(FilePathMap::Iter* iterExcel = m_ExcelFileMap.begin(); iterExcel != m_ExcelFileMap.end();
        iterExcel = m_ExcelFileMap.next(iterExcel))
    {
        if (!iterExcel)
        {
            continue;
        }
        m_ExcelSelectedMap.insert(iterExcel->mKey, false);
    }
    RegisterMsgHandler();

    // EXCEL��ʼ��
    InitXL();

    return true;
}

void 
Data::Release()
{
    ReleaseXL();
}

void 
Data::Update()
{

}

void 
Data::RegisterMsgHandler()
{

}

bool 
Data::Export(String& filePath)
{
    const std::wstring wName = HelpHandler::s2ws(filePath);
    const CString name = wName.c_str();
    CString tmpName = s_XLHanlder.SaveToExcel(filePath);    

    s_XLHanlder.SetSheetCount(0);
    s_XLHanlder.Proc(tmpName);
    s_XLHanlder.RenameFile(filePath);    

    return true;
}

void 
Data::ExportAllSelected()
{
    for (FileSelectedMap::Iter* iterSelect = m_ExcelSelectedMap.begin(); iterSelect != m_ExcelSelectedMap.end();
        iterSelect = m_ExcelSelectedMap.next(iterSelect))
    {
        if (!iterSelect || !(iterSelect->mValue))
        {
            continue;
        }

        FilePathMap::Iter* iterFile = m_ExcelFileMap.find(iterSelect->mKey);
        if (!iterFile)
        {
            continue;
        }

        String absolutePath = GetFullPath(iterFile->mValue);
        String csvFilePath = HelpHandler::Replace(iterFile->mValue, ".xlsx", ".csv");

        int posReplace = csvFilePath.find("Table");
        csvFilePath.replace(posReplace, 5, "Data", 0, 4);

        int pos = csvFilePath.find("StringTemplate.csv");
        if (pos != String::npos 
            && csvFilePath.substr(pos).compare("StringTemplate.csv") == 0)
        { // stringtemplate �����Ҫ���⴦��
            ResourceGroup resourceGroup;
            ResourceGroupManager::getSingleton().getResourceGroup("Data", resourceGroup);
            String path = resourceGroup.begin()->mKey;
            List<String> csvFileList;
            FileUtil::findFiles(path, "StringTemplate*.csv", csvFileList, false);
            for (List<String>::Iter* iter = csvFileList.begin();
                iter != csvFileList.end(); iter = csvFileList.next(iter))
            {
                const std::wstring wCsvFilePath= HelpHandler::s2ws(iter->mValue.c_str());
                ::DeleteFile(wCsvFilePath.c_str());
            }
        }
        else
        {
            const std::wstring wCsvFilePath= HelpHandler::s2ws(csvFilePath);
            ::DeleteFile(wCsvFilePath.c_str());
        }        
		
        Export(absolutePath);
        ConvertCsvToDbc(csvFilePath);
    }

    PromptInfo();
}

void 
Data::SelectAllExcel(bool selected)
{
    for (FileSelectedMap::Iter* iter = m_ExcelSelectedMap.begin(); iter != m_ExcelSelectedMap.end();
        iter = m_ExcelSelectedMap.next(iter))
    {
        if (!iter || iter->mValue == selected)
        {
            continue;
        }
        iter->mValue = selected;
    }

    // ˢ��MainPanel��������
    ReBuildMainPanelNotify notify;
    MSG_QUEUE().SendMsg(notify);
}

String 
Data::GetFullPath(const String& path)
{
    wchar_t buffSource[1024] = {0};
    wchar_t buffDest[1024] = {0};
    char buff[1024] = {0};
    DWORD dwNum; 

    dwNum = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, buffSource, dwNum);
    GetFullPathName(buffSource, 1024, buffDest, NULL);
    dwNum = WideCharToMultiByte(CP_OEMCP,NULL, buffDest, -1, NULL, 0, NULL, FALSE);
    WideCharToMultiByte(CP_OEMCP,NULL, buffDest, -1, buff, dwNum, NULL, FALSE);

    String temp(buff);
    return temp;
}

void
Data::InitXL(void)
{
    ::CoInitialize(NULL);
    s_XLHanlder.Init();
    s_XLHanlder.Open();
    //s_XLHanlder.LoadFieldTable(m_FieldTablePath);
    //s_XLHanlder.LoadDictionnaryTable(m_DictionaryPath);
}

void        
Data::ReleaseXL(void)
{
    ::CoUninitialize();
}

String GetNumInString(const char* str, int size)
{
    String result;
    for (int i = 0; i < size; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result += str[i];
        }
    }
    return result;
}

void 
Data::ConvertCsvToDbc(const String& filePath)
{
    if (!ResourceGroupManager::getSingleton().initial("./ResourceConfig.xml", true))
    {
        LOG_ERROR("Failed to initial resource config file.");
        return;
    }

    ResourceGroup resourceGroup;
    ResourceGroupManager::getSingleton().getResourceGroup("Data", resourceGroup);
    String path = resourceGroup.begin()->mKey;
    printf("����Ŀ¼%s\n", path.c_str());

    DBCMaker maker;
    maker.Initial(path);

    List<String> csvFileList;
    FileUtil::findFiles(path, "*.csv", csvFileList, false);

    // ������������
    for (List<String>::Iter* iter = csvFileList.begin();
        iter != csvFileList.end(); iter = csvFileList.next(iter))
    {      
        if (iter->mValue.compare(filePath) != 0)
        {
            continue;
        }
    
        PROMPT_CONVERT(1);
        
        const String& fileName = lynxGetFileName(iter->mValue.c_str());
        if (maker.Convert(fileName))
        {
            PROMPT_CONVERT(2);
        }
        else
        {
            //PROMPT_CONVERT(3);
            g_ConvertPrompt += "\nת��ʧ��";                         
            //if (gRepeatKeyInt)
            //{
            //    g_ConvertPrompt += "   �ظ���key: ";
            //    char key[255] = {0};
            //    sprintf(key, "%d", gRepeatKeyInt);
            //    g_ConvertPrompt += key;
            //}
            //if (gRepeatKeyString.size())
            //{
            //    g_ConvertPrompt += "   �ظ���key: ";
            //    g_ConvertPrompt += gRepeatKeyString;
            //}

            break;
        }
    }

    // ������������
    // stringTemplate
    bool isClearMap = true;
    List<String> csvStringTemplateFileList;
    FileUtil::findFiles(path, "StringTemplate*.csv", csvStringTemplateFileList, false);
    for(List<String>::Iter* iter = csvStringTemplateFileList.begin(); 
        iter != csvStringTemplateFileList.end(); iter = csvStringTemplateFileList.next(iter))
    {
        if (filePath.find("StringTemplate", 0) != String::npos)
        {
            PROMPT_CONVERT(1);
            String no = GetNumInString(iter->mValue.c_str(), iter->mValue.size());

            Int32 repeatKey = 0;
            if (maker.ConvertString(lynxGetFileName(iter->mValue.c_str()), atoi(no.c_str()), isClearMap))
            {
                if (isClearMap = true)
                {
                    PROMPT_CONVERT(2);
                    isClearMap = false;
                }
            }
            else
            {
                g_ConvertPrompt += "\nת��ʧ��";                         
                //if (gRepeatKeyInt)
                //{
                //    g_ConvertPrompt += "   �ظ���key: ";
                //    char key[255] = {0};
                //    sprintf(key, "%d", gRepeatKeyInt);
                //    g_ConvertPrompt += key;
                //}
                //if (gRepeatKeyString.size())
                //{
                //    g_ConvertPrompt += "   �ظ���key: ";
                //    g_ConvertPrompt += gRepeatKeyString;
                //}
                PROMPT_CONVERT(3);
            }
        }
    }

/* ���� 2013-4-15 û����
    // questInfoTemplate
    isClearMap = true;
    List<String> csvQuestInfoTemplateFileList;
    FileUtil::findFiles(path, "QuestInfoTemplate*.csv", csvQuestInfoTemplateFileList, false);
    for(List<String>::Iter* iter = csvQuestInfoTemplateFileList.begin(); 
        iter != csvQuestInfoTemplateFileList.end(); iter = csvQuestInfoTemplateFileList.next(iter))
    {
        if (filePath.find("QuestInfoTemplate", 0) != String::npos)
        {
            PROMPT_CONVERT(1);
            String no = GetNumInString(iter->mValue.c_str(), iter->mValue.size());
            if (maker.ConvertQuestInfo(lynxGetFileName(iter->mValue.c_str()), atoi(no.c_str()), isClearMap))
            {
                PROMPT_CONVERT(2);
                isClearMap = false;
            }
            else
            {
                PROMPT_CONVERT(3);
            }
        }
    }
*/

    // ɾ�����ɵ���ʱ�ļ�
    DeleteFileByStr(path, "*_tmp.xlsx");
    DeleteFileByStr(path, "*_tmp.csv");
}

void
Data::DeleteFileByStr(const String& path, const String& del)
{
    List<String> DelCsvFileList;
    FileUtil::findFiles(path, del, DelCsvFileList, false);

    for (List<String>::Iter* iter = DelCsvFileList.begin();
        iter != DelCsvFileList.end(); iter = DelCsvFileList.next(iter))
    {
        std::wstring stemp = HelpHandler::s2ws(iter->mValue);
        LPCWSTR result = stemp.c_str();
        ::DeleteFile(result);
    }
}

void 
Data::PromptInfo(void)
{
    char convertCount[100] = {0};
    char successCount[100] = {0};

    String convert;
    sprintf(convertCount, "\n\nת������: %d\n", g_ConvertCount);
    sprintf(successCount, "ת���ɹ�����: %d", g_SuccessCount);

	//String temp = "";

	//temp += convertCount;
	//temp += successCount;
    
    g_ConvertPrompt += convertCount;
    g_ConvertPrompt += successCount;
    
    wxMessageBox(g_ConvertPrompt.c_str());

    g_ConvertCount = 0;

	

    g_ConvertPrompt.clear();
}