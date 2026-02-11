//=============================================================================
// FileName: ItemAttrType.h
// Creater: ZhangXuedong
// Date: 2004.12.28
// Comment: Item Attribute type
//=============================================================================

#ifndef ITEMATTRTYPE_H
#define ITEMATTRTYPE_H

// ������ע�⣬�������Ա�ű������0���������Ϣ��Լ����
// ������ע�⣬�������Ա�ŵ�˳��Ҫ�ͽ�ɫ���Ա�ŵ�˳��һ��

const long	ITEMATTR_COUNT_BASE0    = 0;
const long	ITEMATTR_COE_STR        = ITEMATTR_COUNT_BASE0 + 1; // ����ϵ���ӳɣ�strength coefficient��
const long	ITEMATTR_COE_AGI        = ITEMATTR_COUNT_BASE0 + 2; // ����ϵ���ӳ�
const long	ITEMATTR_COE_DEX        = ITEMATTR_COUNT_BASE0 + 3; // רעϵ���ӳ�
const long	ITEMATTR_COE_CON        = ITEMATTR_COUNT_BASE0 + 4; // ����ϵ���ӳ�
const long	ITEMATTR_COE_STA        = ITEMATTR_COUNT_BASE0 + 5; // ����ϵ���ӳ�
const long	ITEMATTR_COE_LUK        = ITEMATTR_COUNT_BASE0 + 6; // ����ϵ���ӳ�
const long	ITEMATTR_COE_ASPD       = ITEMATTR_COUNT_BASE0 + 7; // ����Ƶ��ϵ���ӳ�
const long	ITEMATTR_COE_ADIS       = ITEMATTR_COUNT_BASE0 + 8; // ��������ϵ���ӳ�
const long	ITEMATTR_COE_MNATK      = ITEMATTR_COUNT_BASE0 + 9; // ��С������ϵ���ӳ�
const long	ITEMATTR_COE_MXATK      = ITEMATTR_COUNT_BASE0 + 10; // ��󹥻���ϵ���ӳ�
const long	ITEMATTR_COE_DEF        = ITEMATTR_COUNT_BASE0 + 11; // ����ϵ���ӳ�
const long	ITEMATTR_COE_MXHP       = ITEMATTR_COUNT_BASE0 + 12; // ���Hpϵ���ӳ�
const long	ITEMATTR_COE_MXSP       = ITEMATTR_COUNT_BASE0 + 13; // ���Spϵ���ӳ�
const long	ITEMATTR_COE_FLEE       = ITEMATTR_COUNT_BASE0 + 14; // ������ϵ���ӳ�
const long	ITEMATTR_COE_HIT        = ITEMATTR_COUNT_BASE0 + 15; // ������ϵ���ӳ�
const long	ITEMATTR_COE_CRT        = ITEMATTR_COUNT_BASE0 + 16; // ������ϵ���ӳ�
const long	ITEMATTR_COE_MF         = ITEMATTR_COUNT_BASE0 + 17; // Ѱ����ϵ���ӳ�
const long	ITEMATTR_COE_HREC       = ITEMATTR_COUNT_BASE0 + 18; // hp�ָ��ٶ�ϵ���ӳ�
const long	ITEMATTR_COE_SREC       = ITEMATTR_COUNT_BASE0 + 19; // sp�ָ��ٶ�ϵ���ӳ�
const long	ITEMATTR_COE_MSPD       = ITEMATTR_COUNT_BASE0 + 20; // �ƶ��ٶ�ϵ���ӳ�
const long	ITEMATTR_COE_COL        = ITEMATTR_COUNT_BASE0 + 21; // ��Դ�ɼ��ٶ�ϵ���ӳ�
const long	ITEMATTR_COE_PDEF       = ITEMATTR_COUNT_BASE0 + 22; // �����ֿ�ϵ���ӳ�

const long	ITEMATTR_COUNT_BASE1    = 25;
const long	ITEMATTR_VAL_STR        = ITEMATTR_COUNT_BASE1 + 1; // ���������ӳɣ�strength value��
const long	ITEMATTR_VAL_AGI        = ITEMATTR_COUNT_BASE1 + 2; // ���ݳ����ӳ�
const long	ITEMATTR_VAL_DEX        = ITEMATTR_COUNT_BASE1 + 3; // רע�����ӳ�
const long	ITEMATTR_VAL_CON        = ITEMATTR_COUNT_BASE1 + 4; // ���ʳ����ӳ�
const long	ITEMATTR_VAL_STA        = ITEMATTR_COUNT_BASE1 + 5; // ���������ӳ�
const long	ITEMATTR_VAL_LUK        = ITEMATTR_COUNT_BASE1 + 6; // ���˳����ӳ�
const long	ITEMATTR_VAL_ASPD       = ITEMATTR_COUNT_BASE1 + 7; // ����Ƶ�ʳ����ӳ�
const long	ITEMATTR_VAL_ADIS       = ITEMATTR_COUNT_BASE1 + 8; // �������볣���ӳ�
const long	ITEMATTR_VAL_MNATK      = ITEMATTR_COUNT_BASE1 + 9; // ��С�����������ӳ�
const long	ITEMATTR_VAL_MXATK      = ITEMATTR_COUNT_BASE1 + 10; // ��󹥻��������ӳ�
const long	ITEMATTR_VAL_DEF        = ITEMATTR_COUNT_BASE1 + 11; // ���������ӳ�
const long	ITEMATTR_VAL_MXHP       = ITEMATTR_COUNT_BASE1 + 12; // ���Hp�����ӳ�
const long	ITEMATTR_VAL_MXSP       = ITEMATTR_COUNT_BASE1 + 13; // ���Sp�����ӳ�
const long	ITEMATTR_VAL_FLEE       = ITEMATTR_COUNT_BASE1 + 14; // �����ʳ����ӳ�
const long	ITEMATTR_VAL_HIT        = ITEMATTR_COUNT_BASE1 + 15; // �����ʳ����ӳ�
const long	ITEMATTR_VAL_CRT        = ITEMATTR_COUNT_BASE1 + 16; // �����ʳ����ӳ�
const long	ITEMATTR_VAL_MF         = ITEMATTR_COUNT_BASE1 + 17; // Ѱ���ʳ����ӳ�
const long	ITEMATTR_VAL_HREC       = ITEMATTR_COUNT_BASE1 + 18; // hp�ָ��ٶȳ����ӳ�
const long	ITEMATTR_VAL_SREC       = ITEMATTR_COUNT_BASE1 + 19; // sp�ָ��ٶȳ����ӳ�
const long	ITEMATTR_VAL_MSPD       = ITEMATTR_COUNT_BASE1 + 20; // �ƶ��ٶȳ����ӳ�
const long	ITEMATTR_VAL_COL        = ITEMATTR_COUNT_BASE1 + 21; // ��Դ�ɼ��ٶȳ����ӳ�
const long	ITEMATTR_VAL_PDEF       = ITEMATTR_COUNT_BASE1 + 22; // �����ֿ������ӳ�

const long	ITEMATTR_COUNT_BASE2    = 49;
const long	ITEMATTR_LHAND_VAL      = ITEMATTR_COUNT_BASE2 + 1; // �������ּӳ�
const long	ITEMATTR_MAXURE	        = ITEMATTR_COUNT_BASE2 + 2; // ����;ö�
const long	ITEMATTR_MAXFORGE       = ITEMATTR_COUNT_BASE2 + 3; // ������ȼ�
const long	ITEMATTR_MAXENERGY      = ITEMATTR_COUNT_BASE2 + 4; // �������
const long	ITEMATTR_URE            = ITEMATTR_COUNT_BASE2 + 5; // ��ǰ�;ö�
const long	ITEMATTR_FORGE          = ITEMATTR_COUNT_BASE2 + 6; // ��ǰ�����ȼ�
const long	ITEMATTR_ENERGY         = ITEMATTR_COUNT_BASE2 + 7; // ��ǰ����


const long	ITEMATTR_MAX_NUM        = 58;
const long  ITEMATTR_CLIENT_MAX     = ITEMATTR_VAL_PDEF + 1; // �ͻ������ڶ�ȡ�����ã���Ϊ��󼸸����Բ���Ҫ��ʾ

const long  ITEMATTR_COUNT_BASE3    = 180;
const long  ITEMATTR_VAL_PARAM1		= ITEMATTR_COUNT_BASE3 + 1;	// ���߸�����Ϣһ
const long  ITEMATTR_VAL_PARAM2		= ITEMATTR_COUNT_BASE3 + 2;	// ���߸�����Ϣ��
const long  ITEMATTR_VAL_LEVEL		= ITEMATTR_COUNT_BASE3 + 3; // ����װ���ȼ���Ϣ
const long  ITEMATTR_VAL_FUSIONID   = ITEMATTR_COUNT_BASE3 + 4; // ����װ���ۺ���ϢID

// Extra attibutes by Mdr

const long  ITEMATTR_TRADABLE       = ITEMATTR_COUNT_BASE2 + 8;
const long  ITEMATTR_EXPIRATION     = ITEMATTR_COUNT_BASE3 + 5;

#endif // ITEMATTRTYPE_H
