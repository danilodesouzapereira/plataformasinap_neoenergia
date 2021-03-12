// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TInfoZccManager.h"
// ---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTInfoZccManager* __fastcall NewObjInfoZccManager(VTApl *apl)
{
	try
	{
		return (new TInfoZccManager(apl));
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TInfoZccManager::TInfoZccManager(VTApl *apl)
{
	this->apl = apl;

	lisINFOZCC = new TList;
}

// ---------------------------------------------------------------------------
__fastcall TInfoZccManager::~TInfoZccManager(void)
{
	if (lisINFOZCC)
	{
		delete lisINFOZCC;
		lisINFOZCC = NULL;
	}
}
// ---------------------------------------------------------------------------
strInfoZcc* __fastcall TInfoZccManager::ExisteInfoZcc(int externIdRede)
{
	strInfoZcc *infoZcc = NULL;

	for (int ni = 0; ni  < lisINFOZCC->Count; ni++)
	{
		infoZcc = (strInfoZcc*)lisINFOZCC->Items[ni];
		if (infoZcc->externIdRede == externIdRede)
			{break;}
        infoZcc = NULL;
	}
	return infoZcc;
}
// ---------------------------------------------------------------------------
strInfoZcc* __fastcall TInfoZccManager::InsereInfoZcc(void)
{
	strInfoZcc *nova;

	lisINFOZCC->Add(nova = new strInfoZcc);
	//seta valores default
	nova->externIdRede = -1;
	nova->pcc3f = 0;
	nova->pcc1f = 0;
	nova->acc3f = 0;
	nova->acc1f = 0;
	nova->sBase_mva = 100;
	nova->vBase_kva = 0;
	nova->z0_pu.r = 0;
	nova->z0_pu.x = 0;
	nova->z1_pu.r = 0;
	nova->z1_pu.x = 0;
	nova->ref_enr_trafo = 2;

	return nova;
}
// ---------------------------------------------------------------------------
bool __fastcall TInfoZccManager::LeInfoZcc(void)
{	// variáveis locais
	AnsiString txt;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	bool sucesso = false;
	strInfoZcc *infoZcc;

	try
	{
		if (geral)
		{
			geral->Infoset->Clear();
			// lê parâmetros para conexão
			if (geral->Infoset->LoadFromFile(IZ_ZCCFILE))
			{
				for (int nl = 0; nl < geral->Infoset->Count; nl++)
				{
					geral->Infoset->SelectLine(nl);
					infoZcc = InsereInfoZcc();
					//obtem o id da rede
					geral->Infoset->GetInfo(izID, txt);
					infoZcc->externIdRede = StrToInt(txt);
					//obtem a potencia de curto 3F
					geral->Infoset->GetInfo(izPCC3F, txt);
					infoZcc->pcc3f = StrToDouble(txt);
					//obtem a potencia de curto 1F
					geral->Infoset->GetInfo(izPCC1F, txt);
					infoZcc->pcc1f = StrToDouble(txt);
					//obtem a ??? 3F
					geral->Infoset->GetInfo(izACC3F, txt);
					infoZcc->acc3f = StrToDouble(txt);
					//obtem a ??? 1F
					geral->Infoset->GetInfo(izACC1F, txt);
					infoZcc->acc1f = StrToDouble(txt);
					//obtem o s de base utilizado em mva
					geral->Infoset->GetInfo(izSBASE, txt);
					infoZcc->sBase_mva = StrToDouble(txt);
					//obtem o v de base utilizado em kv
					geral->Infoset->GetInfo(izVBASE, txt);
					infoZcc->vBase_kva = StrToDouble(txt);
					//obtem o r0 em pu
					geral->Infoset->GetInfo(izR0, txt);
					infoZcc->z0_pu.r = StrToDouble(txt);
					//obtem o x0 em pu
					geral->Infoset->GetInfo(izX0, txt);
					infoZcc->z0_pu.x = StrToDouble(txt);
					//obtem o r1 em pu
					geral->Infoset->GetInfo(izR1, txt);
					infoZcc->z1_pu.r = StrToDouble(txt);
					//obtem o x1 em pu
					geral->Infoset->GetInfo(izX1, txt);
					infoZcc->z1_pu.x = StrToDouble(txt);
					//obtem o enrolamento de referencia
					geral->Infoset->GetInfo(izREF, txt);
					infoZcc->ref_enr_trafo = StrToInt(txt);
				}
			}
		}
        sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return sucesso;

}
// ---------------------------------------------------------------------------
bool __fastcall TInfoZccManager::SalvaInfoZcc(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	strInfoZcc *sdi;
	bool sucesso = false;

	try
	{
		geral->Infoset->Clear();
		for (int ni = 0; ni < lisINFOZCC->Count; ni++)
		{
			sdi = (strInfoZcc*)lisINFOZCC->Items[ni];
			geral->Infoset->AddInfo(izID, sdi->externIdRede);
			geral->Infoset->AddInfo(izPCC3F,sdi->pcc3f );
			geral->Infoset->AddInfo(izPCC1F, sdi->pcc1f);
			geral->Infoset->AddInfo(izACC3F, sdi->acc3f);
			geral->Infoset->AddInfo(izACC1F, sdi->acc1f);
			geral->Infoset->AddInfo(izSBASE, sdi->sBase_mva);
			geral->Infoset->AddInfo(izVBASE, sdi->vBase_kva);
			geral->Infoset->AddInfo(izR0, sdi->z0_pu.r);
			geral->Infoset->AddInfo(izX0, sdi->z0_pu.x);
			geral->Infoset->AddInfo(izR1, sdi->z1_pu.r);
			geral->Infoset->AddInfo(izX1, sdi->z1_pu.x);
			geral->Infoset->AddInfo(izREF,sdi->ref_enr_trafo);
			// grava no arquivo
			geral->Infoset->InsertNewLine();
		}
		geral->Infoset->SaveToFile(IZ_ZCCFILE);
		sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
// eof
