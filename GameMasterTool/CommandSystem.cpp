#include "CommandSystem.h"
#include "GameMasterTool.h"

using namespace Lynx;

const char* sCommandHelp =
"------------��������------------\r\n\r\n"
".Clear\r\n"
"------------��Ҳ���------------\r\n\r\n"
".SetPlayerExp  �������  ����ֵ\r\n"
".SetPlayerLevel  �������  �ȼ�ֵ\r\n"
".SetPlayerEnergy  �������  ����ֵ\r\n"
".SetPlayerGold  �������  ���ֵ\r\n"
".AddItem  �������  ģ��ID  ����\r\n"
".DelItem  �������  ����ID\r\n"
".AddEquip  �������  ģ��ID  ����\r\n"
".DelEquip  �������  װ��ID\r\n"
".AddRune  �������  ģ��ID  ����\r\n"
".DelRune  �������  ����ID\r\n"
".AddHeroDebris  �������  ģ��ID  ����\r\n"
".DelHeroDebris  �������  ��ƬID\r\n"
".AddHero  �������  ģ��ID  ����\r\n"
".DelHero  �������  ģ��ID  ����\r\n"
".AddPet  �������  ģ��ID  ����\r\n"
".DelPet  �������  ģ��ID  ����\r\n"
".AddPetDebris ������� ģ��ID   ����\r\r"
"\r\n------------Ӣ�۲���------------\r\n\r\n"
".SetHeroExp  �������  Ӣ��ID  ����ֵ\r\n"
".SetHeroLevel  �������  Ӣ��ID  �ȼ�ֵ\r\n"
".SetHeroStarLevel  �������  Ӣ��ID  �Ǽ�\r\n"
".setHeroAdvanced  �������  Ӣ��ID  �Ǽ�\r\n"
".SetHeroPresentPos  �������  Ӣ��ID  λ��(0������1��λ��1���󣩣�2��λ��2���У���3�� λ��3���ң���4���油1��5���油2)\r\n"
".SetHeroEquip  �������  װ��ID װ��λ��(0-25)\r\n"
".SetHeroRune  �������  ����ID  ����λ��(0-25)\r\n"
"\r\n------------װ������------------\r\n\r\n"
".SetEquipRefine  �������  װ��ID  �ȼ�ֵ\r\n"
"\r\n------------���Ĳ���------------\r\n\r\n"
".SetRuneRefine  �������  ����ID  �ȼ�ֵ\r\n"
".SetTollgate ������� �ؿ��½� �ؿ� �Ǽ� ����"
;

CommandSystem::CommandSystem() : mCurrentGMClient(NULL)
{

}

CommandSystem::~CommandSystem()
{

}

bool 
CommandSystem::initial()
{
    return true;
}

void 
CommandSystem::release()
{

}

void 
CommandSystem::execCommand(const String& str)
{
    if (str.empty())
    {
        return;
    }

    if (str[0] != '.')
    {
        execChat(str);
        return;
    }

    Vector<String> v;
    lynxStrSplit(str, " ", v, true);

    String cmd = v[0];
    lynxToLowerCase(cmd);

    mCurrentGMClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!mCurrentGMClient)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "ִ��ָ��ʧ��, ��ǰGMδ���ӷ�����";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    if (cmd == ".help")
    {
        execHelp(v);
    }
    else if (cmd == ".clear")
    {
        execClear(v);
    }
    else if (cmd == ".setplayerexp")
    {
        execSetPlayerExp(v);
    }
    else if (cmd == ".setplayerlevel")
    {
        execSetPlayerLevel(v);
    }
    else if (cmd == ".setplayerenergy")
    {
        execSetPlayerEnergy(v);
    }
    else if (cmd == ".setplayergold")
    {
        execSetPlayerGold(v);
    }
    else if (cmd == ".additem")
    {
        execAddItem(v);
    }
    else if (cmd == ".delitem")
    {
        execDelItem(v);
    }
    else if (cmd == ".addequip")
    {
        execAddEquip(v);
    }
    else if (cmd == ".delequip")
    {
        execDelEquip(v);
    }
    else if (cmd == ".addrune")
    {
        execAddRune(v);
    }
    else if (cmd == ".delrune")
    {
        execDelRune(v);
    }
    else if (cmd == ".addherodebris")
    {
        execAddHeroDebris(v);
    }
    else if (cmd == ".delherodebris")
    {
        execDelHeroDebris(v);
    }
    else if (cmd == ".addhero")
    {
        execAddHero(v);
    }
    else if (cmd == ".delhero")
    {
        execDelHero(v);
    }
    else if (cmd == ".addpet")
    {
        execAddPet(v);
    }
    else if (cmd == ".delpet")
    {
        execDelPet(v);
    }
    else if (cmd == ".setheroexp")
    {
        execSetHeroExp(v);
    }
    else if (cmd == ".setheroadvanced")
    {
        execSetHeroAdvanced(v);
    }
    else if (cmd == ".setherolevel")
    {
        execSetHeroLevel(v);
    }
    else if (cmd == ".setherostarlevel")
    {
        execSetHeroStarLevel(v);
    }
    else if (cmd == ".setheropresentpos")
    {
        execSetHeroPresentPos(v);
    }
    else if (cmd == ".setheroequip")
    {
        execSetHeroEquip(v);
    }
    else if (cmd == ".setherorune")
    {
        execSetHeroRune(v);
    }
    else if (cmd == ".setequiprefine")
    {
        execSetEquipRefine(v);
    }
    else if (cmd == ".setrunerefine")
    {
        execSetRuneRefine(v);
    }
    else if (cmd == ".settollgate")
    {
        execSetTollgate(v);
    }
    else if (cmd == ".addpetdebris")
    {
        execAddPetDebris(v);
    }
    else
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��Чָ�" + cmd;
        printEvent.mLogContents += "��";
        EventSystem::getSingleton().postEvent(printEvent);
    }
}

void 
CommandSystem::execChat(const String& cmd)
{

}

void 
CommandSystem::execHelp(const Vector<String>& v)
{
    WindowLogPrintEvent printEvent;
    printEvent.mLogContents = sCommandHelp;
    EventSystem::getSingleton().postEvent(printEvent);
}

void 
CommandSystem::execClear(const Vector<String>& v)
{
    WindowLogClearEvent ev;
    EventSystem::getSingleton().postEvent(ev);
}

void 
CommandSystem::execSetPlayerExp(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetPlayerExp �������  ����ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerExpReq req;
    req.mPlayerName = v[1];
    req.mLevelExp = lynxAtoi<UInt32>(v[2].c_str());
    
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetPlayerLevel(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetPlayerLevel  �������  �ȼ�ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerLevelReq req;
    req.mPlayerName = v[1];
    req.mLevel = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);

}

void 
CommandSystem::execSetPlayerEnergy(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetPlayerEnergy  �������  ����ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerEnergyReq req;
    req.mPlayerName = v[1];
    req.mEnergy = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);

}

void 
CommandSystem::execSetPlayerGold(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetPlayerGold  �������  ���ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerGoldReq req;
    req.mPlayerName = v[1];
    req.mGold = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddItem(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddItem  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddItemReq req;
    req.mPlayerName = v[1];
    req.mItemTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddPetDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddPetDebris  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddPetDebrisReq req;
    req.mPlayerName = v[1];
    req.mDebrisTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelItem(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelItem  �������  ����ID ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelItemReq req;
    req.mPlayerName = v[1];
    req.mItemGuid = lynxAtoi<Guid>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddEquip(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddEquip  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddEquipReq req;
    req.mPlayerName = v[1];
    req.mEquipTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelEquip(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelEquip  �������  װ��ID��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelEquipReq req;
    req.mPlayerName = v[1];
    req.mEquipGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddRune(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddRune  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddRuneReq req;
    req.mPlayerName = v[1];
    req.mRuneTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelRune(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelRune  �������  ����ID��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelRuneReq req;
    req.mPlayerName = v[1];
    req.mRuneGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddHeroDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddHeroDebris  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddHeroDebrisReq req;
    req.mPlayerName = v[1];
    req.mHeroDebrisTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelHeroDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelHeroDebris  �������  ��ƬID ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelHeroDebrisReq req;
    req.mPlayerName = v[1];
    req.mHeroDebrisGuid = lynxAtoi<Guid>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddHero(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddHero  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddHeroReq req;
    req.mPlayerName = v[1];
    req.mHeroTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelHero(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelHero  �������  Ӣ��ID��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelHeroReq req;
    req.mPlayerName = v[1];
    req.mHeroGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddPet(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.AddPet  �������  ģ��ID  ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddPetReq req;
    req.mPlayerName = v[1];
    req.mPetTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelPet(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.DelPet  �������  ����ID��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelPetReq req;
    req.mPlayerName = v[1];
    req.mPetGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}


void 
CommandSystem::execSetHeroExp(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroExp  �������  Ӣ��ID  ����ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroExpReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mLevelExp = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroAdvanced(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroAdvanced  �������  Ӣ��ID  ����ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroAdvancedReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mAdvanced = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroLevel(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroLevel  �������  Ӣ��ID  �ȼ�ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroLevelReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroStarLevel(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroStartLevel  �������  Ӣ��ID  �Ǽ�ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroStarLevelReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mStarLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroPresentPos(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroPresentPos  �������  Ӣ��ID  λ��(0������1��λ��1���󣩣�2��λ��2���У���3�� λ��3���ң���4���油1��5���油2)��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroPresentPosReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroEquip(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroEquip  �������   װ��ID  λ��(0-25)��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroEquipReq req;
    req.mPlayerName = v[1];
    //req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mEquipId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroRune(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetHeroRune  �������  ����ID  λ��(0-25)��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroRuneReq req;
    req.mPlayerName = v[1];
    //req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mRuneId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetEquipRefine(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetEquipRefine  �������  װ��ID  �ȼ�ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetEquipRefineReq req;
    req.mPlayerName = v[1];
    req.mEquipId = lynxAtoi<Guid>(v[2].c_str());
    req.mRefineLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetRuneRefine(const Vector<String> & v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetRuneRefine  �������  ����ID  �ȼ�ֵ��";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetRuneRefineReq req;
    req.mPlayerName = v[1];
    req.mRuneId = lynxAtoi<Guid>(v[2].c_str());
    req.mRefineLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetTollgate(const Vector<String> & v)
{
    if (v.size() != 6)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "��������, ��.SetTollgate ������� �ؿ��½� �ؿ� �Ǽ� ������";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetTollgateReq req;
    req.mPlayerName = v[1];
    req.mChapterId = lynxAtoi<UInt8>(v[2].c_str());
    req.mTollgateNum = lynxAtoi<UInt8>(v[3].c_str());
    req.mStarCount = lynxAtoi<UInt8>(v[4].c_str());
    req.mCount = lynxAtoi<UInt8>(v[5].c_str());
    mCurrentGMClient->sendMsg(req);
}

