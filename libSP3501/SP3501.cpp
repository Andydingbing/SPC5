#include "SP3501.h"
#include "RegDef.h"

CSP3501::CSP3501()
{
    m_pS6 = new viPCIDev("s6");
}

CSP3501::~CSP3501()
{
    SAFE_DELETE(m_pS6);
}

CSP3501 &CSP3501::Instance()
{
	static CSP3501 g_SP3501;
	return g_SP3501;
}

int32_t CSP3501::Get_S6_Version(uint32_t &S6Ver)
{
	return 0;
}

int32_t CSP3501::OpenBoard()
{
	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	char szRsrcName[32];
	CGeneralIniFile IniFile(CONFIG_FILE_PATH);

	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));
	memset(szRsrcName,0,ARRAY_SIZE(szRsrcName));
	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);

	IniFile.GetConfigStringValue(szRsrcSection,"CXU_S6",szRsrcName);

    INT_CHECK(m_pS6->Init(szRsrcName,0x10ee,0x3501));
	return 0;
}

int32_t CSP3501::CloseBoard()
{
	return 0;
}

int32_t CSP3501::Vol9119(int16_t Val)
{
    CXU_S6_REG_DECLARE(0x000e);
    CXU_S6_REG_DECLARE(0x000f);
    CXU_S6_REG(0x000f).sign = 0;
    CXU_S6_REG(0x000f).val = (unsigned)Val;
    CXU_S6_W(0x000f);
    CXU_S6_OP(0x000e);
	return 0;
}

int32_t CSP3501::Vol9119(double Vol)
{
	return Vol9119(x9119vol2val(Vol));
}

int32_t CSP3501::SetBlower(int32_t iBlower,int32_t iSpeed)
{
    CXU_S6_REG_DECLARE(0x0012);
    CXU_S6_REG_DECLARE(0x0013);
	BlowerMap tBlowerMap[11] = {
        BlowerMap(0x0012,0x0032,0),
        BlowerMap(0x0012,0x0033,0),
        BlowerMap(0x0012,0x0033,1),
        BlowerMap(0x0012,0x0034,0),
        BlowerMap(0x0012,0x0034,1),
        BlowerMap(0x0013,0x0032,0),
        BlowerMap(0x0013,0x0032,1),
        BlowerMap(0x0013,0x0033,0),
        BlowerMap(0x0013,0x0033,1),
        BlowerMap(0x0013,0x0034,0),
        BlowerMap(0x0013,0x0034,1)
    };
    if (iBlower <= 4) {
        CXU_S6_REG(0x0012).speed = (unsigned)iSpeed;
        CXU_S6_REG(0x0012).addr = (unsigned)tBlowerMap[iBlower].m_iAddr;
        CXU_S6_REG(0x0012).blower = (unsigned)tBlowerMap[iBlower].m_iBlowerSel;
        CXU_S6_OP(0x0012);
        return 0;
    }
    else {
        CXU_S6_REG(0x0013).speed = (unsigned)iSpeed;
        CXU_S6_REG(0x0013).addr = (unsigned)tBlowerMap[iBlower].m_iAddr;
        CXU_S6_REG(0x0013).blower = (unsigned)tBlowerMap[iBlower].m_iBlowerSel;
        CXU_S6_OP(0x0013);
        return 0;
    }
    return 0;
}

int32_t CSP3501::SetBlower(int32_t iSpeed)
{
	for (int32_t i = 0;i < 11;i ++)
        INT_CHECK(SetBlower(i,iSpeed));
	return 0;
}
