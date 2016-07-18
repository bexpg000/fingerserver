#ifndef __LYNX_GAME_SERVER_GAME_OBJECT_ATTRDATA_H__
#define __LYNX_GAME_SERVER_GAME_OBJECT_ATTRDATA_H__
#include "./CommonLib/Table.h"
#include <iostream>
using namespace std;

//���쳽

namespace Lynx
{
	class PlayerAttrData
	{
	public:
		PlayerAttrData():m_nMaxMp(0), m_nMaxHp(0), m_nMaxSp(0), m_nPA(0),m_nMA(0),m_nPF(0),m_nMF(0),m_nHPRecover(0),
			m_nMPRecover(0),m_nSPRecover(0),m_nSPDamage(0),m_nBaseCrit(0),m_fCritRate(0),m_nCritDef(0){};

		PlayerAttrData(UInt32 modelId, UInt32 level);
	
		PlayerAttrData(const PlayerAttrData& playerAttrData);
		
		void addAttrTypeValue(UInt32 attrType, float attrValue);

		PlayerAttrData & operator = (const PlayerAttrData & playerAttrData);

		UInt32 attrConverToPower();
		
		void attrConverToCapacity(UInt32 & fightCapacity,UInt32 attrType, UInt32 attrValue);

		//˫Ŀ��Ԫ����

		friend PlayerAttrData operator + (const PlayerAttrData & playerAttrData1, const PlayerAttrData & playerAttrData2)
		{
			PlayerAttrData playerAttrDataTemp;

			playerAttrDataTemp.m_nMaxHp = playerAttrData1.m_nMaxHp + playerAttrData2.m_nMaxHp;
			playerAttrDataTemp.m_nMaxMp = playerAttrData1.m_nMaxMp + playerAttrData2.m_nMaxMp;
			playerAttrDataTemp.m_nMaxSp = playerAttrData1.m_nMaxSp + playerAttrData2.m_nMaxSp;
			playerAttrDataTemp.m_nPA = playerAttrData1.m_nPA + playerAttrData2.m_nPA;
			playerAttrDataTemp.m_nMA = playerAttrData1.m_nMA + playerAttrData2.m_nMA;
			playerAttrDataTemp.m_nPF = playerAttrData1.m_nPF + playerAttrData2.m_nPF;
			playerAttrDataTemp.m_nMF = playerAttrData1.m_nMF + playerAttrData2.m_nMF;
			playerAttrDataTemp.m_nHPRecover = playerAttrData1.m_nHPRecover + playerAttrData2.m_nHPRecover;
			playerAttrDataTemp.m_nMPRecover = playerAttrData1.m_nMPRecover + playerAttrData2.m_nMPRecover;
			playerAttrDataTemp.m_nSPRecover = playerAttrData1.m_nSPRecover + playerAttrData2.m_nSPRecover;
			playerAttrDataTemp.m_nSPDamage = playerAttrData1.m_nSPDamage + playerAttrData2.m_nSPDamage;
			playerAttrDataTemp.m_nBaseCrit = playerAttrData1.m_nBaseCrit + playerAttrData2.m_nBaseCrit;
			playerAttrDataTemp.m_fCritRate = playerAttrData1.m_fCritRate + playerAttrData2.m_fCritRate;
			playerAttrDataTemp.m_nCritDef = playerAttrData1.m_nCritDef + playerAttrData2.m_nCritDef;

			return playerAttrDataTemp;
		}

		friend ostream& operator << (ostream & os, const PlayerAttrData& attrData)
		{
			os <<attrData.m_nMA<<endl<<attrData.m_nMaxMp;
			return os;
		}

		~PlayerAttrData(){}
	private:

		UInt32 m_nMaxHp;  //���hp
		UInt32 m_nMaxMp;  //���mp
		UInt32 m_nMaxSp;  //������ֵ
		UInt32 m_nPA; //������
		UInt32 m_nMA; //�������
		UInt32 m_nPF; //��������
		UInt32 m_nMF; //��������
		UInt32 m_nHPRecover; //HP�ظ�
		UInt32 m_nMPRecover; //MP�ظ�
		UInt32 m_nSPRecover; //SP�ظ�
		UInt32 m_nSPDamage; //sp�˺�
		UInt32 m_nBaseCrit; //����������
		float m_fCritRate; //��������
		UInt32 m_nCritDef; //����


	};

	
};

#endif //__LYNX_GAME_SERVER_GAME_OBJECT_ATTRDATA_H__