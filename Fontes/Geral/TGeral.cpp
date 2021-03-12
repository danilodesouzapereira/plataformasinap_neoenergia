//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TGeral.h"
#include "VTInfoset.h"
#include "VTPadraoRede.h"
#include "CfgFluxoH3\VTCfgFluxoH3.h"
#include "CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrid.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\BD.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGeral* __fastcall NewObjGeral(VTApl *apl)
   {
   VTGeral *geral = new TGeral(apl);
   return(geral);
   }

//-----------------------------------------------------------------------------
__fastcall TGeral::TGeral(VTApl *apl)
   {
   //variáveis locais
   VTGeral *geral_pai;

   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   grid             = NewObjGrid();
   PD.infoset       = NewObjInfoset(apl);
   PD.cfg_fluxoH3   = NewObjCfgFluxoH3(apl);
   PD.cfg_perda_tec = NewObjCfgPerdaTec(apl);
   lisPADRAOREDE    = new TList();
   //marca que não há um banco de dados selecionado
   BaseDado = SNPDBTYPE_INDEFINIDO;
   //verifica se há um objeto Geral em apl
   if ((geral_pai = (VTGeral*)apl->GetObject(__classid(VTGeral))) != NULL)
      {
      BaseDado = geral_pai->BaseDado;
      }
   //inicia valores default
   ReiniciaValoresDefault();
   //Le valores do infoset
   LeLimitesTensao();
   //seta flag indicando que objeto ainda não foi iniciado c/ dados da base
   Iniciado = false;
   }

//-----------------------------------------------------------------------------
__fastcall TGeral::~TGeral(void)
   {
   //destrói objetos
   if (grid)             {delete grid;             grid             = NULL; }
   if (PD.infoset)       {delete PD.infoset;       PD.infoset       = NULL; }
   if (PD.cfg_perda_tec) {delete PD.cfg_perda_tec; PD.cfg_perda_tec = NULL; }
   if (PD.cfg_fluxoH3)   {delete PD.cfg_fluxoH3;   PD.cfg_fluxoH3   = NULL; }
   //destrói lista e seus objetos
   if (lisPADRAOREDE) {LimpaTList(lisPADRAOREDE); delete lisPADRAOREDE; lisPADRAOREDE = NULL; }
   }

//-----------------------------------------------------------------------------
void __fastcall TGeral::CopiaAtributosDe(VTGeral &geral, bool somenteCfgFlow)
   {
   //variáveis locais
   TList        *lisEXT;
   VTPadraoRede *padrao;
   TList        *lisPADRAOTEMP;
   if(!somenteCfgFlow)
   {
	   //copia dados
	   AG       = geral.AG;
	   DIAG     = geral.DIAG;
	   DEMAN    = geral.DEMAN;
	   CURTO    = geral.CURTO;
	   FLOW     = geral.FLOW;
	   LOCAP    = geral.LOCAP;
	   REGU     = geral.REGU;
	   for (int n = 0; n < CONFIAB_MAX_EQPTO; n++) CONFIAB[n]  = geral.CONFIAB[n];
   }
   COR      = geral.COR;
   LIM_V	= geral.LIM_V; //FKM
   //copia objetos PadraoRede
   lisEXT = geral.LisPadraoRede();
   //elimina objetos RedePadrao
   if (lisPADRAOREDE != lisEXT)
   {
	   LimpaTList(lisPADRAOREDE);
	   for (int n = 0; n < lisEXT->Count; n++)
		  {
		  padrao = (VTPadraoRede*)lisEXT->Items[n];
		  lisPADRAOREDE->Add(padrao->Clone());
		  }
   }
   //copia demais dados
   BaseDado = geral.BaseDado;
   }

//---------------------------------------------------------------------------
VTPadraoRede* __fastcall TGeral::ExistePadraoRede(int padraorede_id)
   {
   //variáveis locais
   VTPadraoRede* padrao;

   //proteção
   if (!lisPADRAOREDE) return(NULL);

   for (int i = 0; i < lisPADRAOREDE->Count; i++)
      {
      padrao = (VTPadraoRede*)lisPADRAOREDE->Items[i];

      if (padrao->PadraoRedeId == padraorede_id) return(padrao);
      }
   return(NULL);
   } // ExistePadraoRede

//---------------------------------------------------------------------------
bool __fastcall TGeral::InserePadraoRede(VTPadraoRede* padrao)
   {
   //variáveis locais
   VTPadraoRede* padrao_aux;

   if (! padrao)        return(false);
   if (! lisPADRAOREDE) return(false);

   if ((padrao_aux = ExistePadraoRede(padrao->PadraoRedeId)) != NULL)
      {//retira e destrói Padrao anterior
      lisPADRAOREDE->Remove(padrao_aux);
      delete padrao_aux;
      }
   //insere o novo Padrao
   lisPADRAOREDE->Add(padrao);
   //redefine valores da estrutura FLOW
   if ((lisPADRAOREDE->Count == 1)||(padrao->PadraoRedeId == redePRI))
      {//salva valores FLOW.V[] e FLOW.I
//	  FLOW.V[padrao->PadraoRedeId][0] = padrao->V1;
//	  FLOW.V[padrao->PadraoRedeId][1] = padrao->V2;
//	  FLOW.V[padrao->PadraoRedeId][2] = padrao->V3;
//	  FLOW.V[padrao->PadraoRedeId][3] = padrao->V4;
	  /* TODO : fazer para BT tb */
	  //FLOW.V[4] = padrao->V5;
	  //FLOW.V[5] = padrao->V6;
	  //FLOW.V[6] = padrao->V7;
	  //FLOW.V[7] = padrao->V8;
      FLOW.I[0] = padrao->FPI1;
      FLOW.I[1] = padrao->FPI2;
      //intervalos de cargto de Trafo
      FLOW.S[0] = padrao->FPS1;
      FLOW.S[1] = padrao->FPS2;
      }
   return(true);
   } // InserePadraoRede

//---------------------------------------------------------------------------
bool __fastcall  TGeral::LeGeral(bool redeEmpresa)
{
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTBD * bd = DLL_NewObjBD(apl);
	bool sucesso = false;
	// guarda o arquivo de rede(.mdb) corrente
	AnsiString redeUsuario = path->ArqRede;

	try
	{
		if (redeEmpresa)
		{
			// define uso da base Access
			this->BaseDado = SNPDBTYPE_ACCESS;

			//
			// 1. desconecta da base atual
			//
			bd->Desconecta();


			//
			// 2. coloca o RedeEmpresa.mdb como o arquivo de rede atual
			//
			path->ArqRede = path->ArqRedeEmpresa();

			AnsiString dirRedes = path->DirRedes();
			AnsiString dirbase = path->DirBase;

			bd->Conecta();


			//
			// 3. salva o geral atual no redeEmpresa
			//
			sucesso = bd->Geral->LeGeral(this);

			//
			// 4. volta a base corrente para a base(rede) que o usuario estava utilizando
			//
			bd->Desconecta();
			path->ArqRede = redeUsuario;
			bd->Conecta();
		}
		else
		{
			bd->Conecta();
			sucesso = bd->Geral->LeGeral(this);
			bd->Desconecta();
		}

		delete bd;
	}
	catch(Exception &e)
	{
        return false;
    }
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGeral::LeLimitesTensao(void)
{
	int index = 0;

	//proteção
	if (PD.infoset == NULL) return (false);
	//abre arquivo com dados de configuração de perdas dos medidores
	if (! PD.infoset->LoadFromFile(ARQ_CFG_LIMITE_TENSAO))
	{
		LimitesTensaoDefault();
		return (false);
	}
	//verifica se há dados disponíveis
	if (PD.infoset->Count == 0) return (false);
	//lê todos os registros do Infoset
	//le nova linha no infoset
	if ( ! PD.infoset->SelectLine(index)) return (false);
	PD.infoset->GetInfo("limInfBT" ,  LIM_V.LIM_KV[0][0]);
	PD.infoset->GetInfo("limSupBT" ,  LIM_V.LIM_KV[0][1]);
	PD.infoset->GetInfo("bool_limInfBT" ,  LIM_V.LIM_ISABERTO[0][0]);
	PD.infoset->GetInfo("bool_limSupBT" ,  LIM_V.LIM_ISABERTO[0][1]);
	//incrementa index
	index++;
	//le nova linha no infoset
	if ( ! PD.infoset->SelectLine(index)) return (false);
	PD.infoset->GetInfo("limInfMT" ,  LIM_V.LIM_KV[1][0]);
	PD.infoset->GetInfo("limSupMT" ,  LIM_V.LIM_KV[1][1]);
	PD.infoset->GetInfo("bool_limInfMT" ,  LIM_V.LIM_ISABERTO[1][0]);
	PD.infoset->GetInfo("bool_limSupMT" ,  LIM_V.LIM_ISABERTO[1][1]);
	//incrementa index
	index++;
	//le nova linha no infoset
	if ( ! PD.infoset->InsertNewLine()) return (false);
	PD.infoset->GetInfo("limInfAT" ,  LIM_V.LIM_KV[2][0]);
	PD.infoset->GetInfo("limSupAT" ,  LIM_V.LIM_KV[2][1]);
	PD.infoset->GetInfo("bool_limInfAT" ,  LIM_V.LIM_ISABERTO[2][0]);
	PD.infoset->GetInfo("bool_limSupAT" ,  LIM_V.LIM_ISABERTO[2][1]);

	return (true);
}
//---------------------------------------------------------------------------
TList* __fastcall TGeral::LisPadraoRede(void)
   {
   return(lisPADRAOREDE);
   } // LisPadraoRede

//-----------------------------------------------------------------------------
bool __fastcall	TGeral::LimitesTensaoDefault(void)
{
	//reinicia os valores de tensão para os valores do prodist (2017.08.10)
   //BT
   LIM_V.LIM_KV[0][0] = 0.0;
   LIM_V.LIM_KV[0][1] = 1.0;
   LIM_V.LIM_ISABERTO[0][0] = true;
   LIM_V.LIM_ISABERTO[0][1] = false;
   //MT
   LIM_V.LIM_KV[1][0] = 1.0;
   LIM_V.LIM_KV[1][1] = 69.0;
   LIM_V.LIM_ISABERTO[1][0] = true;
   LIM_V.LIM_ISABERTO[1][1] = true;
   //AT
   LIM_V.LIM_KV[2][0] = 69.0;
   LIM_V.LIM_KV[2][1] = 230.0;
   LIM_V.LIM_ISABERTO[2][0] = false;
   LIM_V.LIM_ISABERTO[2][1] = true;

   return true;
}
//-----------------------------------------------------------------------------
bool __fastcall  TGeral::PadroesRedesDiferentes(VTGeral &geral)
{
	TList        *lisEXT;
	VTPadraoRede *padraoExt , *padrao;
	bool diferente = false;

	//1o verifica se os limites de carregamento sao diferentes
	if (!IsDoubleZero(geral.FLOW.I[0] - FLOW.I[0]))
	{
		return true;
	}
    else if (!IsDoubleZero(geral.FLOW.I[1] - FLOW.I[1]))
	{
		return true;
	}
	else if (!IsDoubleZero(geral.FLOW.S[0] - FLOW.S[0]))
	{
		return true;
	}
    else if (!IsDoubleZero(geral.FLOW.S[1] - FLOW.S[1]))
	{
		return true;
	}
	//copia objetos PadraoRede
   lisEXT = geral.LisPadraoRede();
   //percorre todos os padroes internos
   for(int np = 0; np < lisPADRAOREDE->Count; np++)
   {
		padrao = (VTPadraoRede*)lisPADRAOREDE->Items[np];
		//verifica se existe o padrao analisado no geral externo
		padraoExt = NULL;
		padraoExt = geral.ExistePadraoRede(padrao->PadraoRedeId);
		if (padraoExt == NULL)
		{
			diferente = true;
			break;
		}
		else
		{   //existindo compara limites
			if (!IsDoubleZero(padrao->V1 - padraoExt->V1))
			{
				diferente = true;
				break;
			}
			else if (!IsDoubleZero(padrao->V2 - padraoExt->V2))
			{
                diferente = true;
				break;
			}
			else if (!IsDoubleZero(padrao->V3 - padraoExt->V3))
			{
				diferente = true;
				break;
			}
            else if (!IsDoubleZero(padrao->V4 - padraoExt->V4))
			{
                diferente = true;
				break;
			}
        }
   }

   return diferente;
}
/*
//-----------------------------------------------------------------------------
void __fastcall TGeral::ReiniciaValoresDefault(void)
{//variáveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca *licenca = licencas->ExisteLicenca();

   if(licenca)
   {
		switch (licenca->Empresa)
		{
			case COPEL:
				ValoresDefaultCOPEL();
				break;
			default:
				ValoresDefault();
			;
		}
   }
}
*/
//JCG 2020.03.30 - ateração para compatibilidade com SigPerdas
//-----------------------------------------------------------------------------
void __fastcall TGeral::ReiniciaValoresDefault(void)
   {
   //variáveis locais
   VTLicenca *licenca;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //assume valores default
   ValoresDefault();
   //proteção
   if (licencas == NULL) return;
   if ((licenca = licencas->ExisteLicenca()) != NULL)
      {
		if (licenca->Empresa == COPEL)
		   {
         ValoresDefaultCOPEL();
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TGeral::RetiraPadraoRede(VTPadraoRede* padrao)
   {
   //proteção
   if (!lisPADRAOREDE) return(false);
   //verifica se deve retirar todos os PadraoRede
   if (padrao == NULL)
	  {//retira todos os PadraoRede
	  LimpaTList(lisPADRAOREDE);
	  }
   else
	  {
	  lisPADRAOREDE->Remove(padrao);
	  delete padrao;
      }

   return(true);
   } // RetiraPadraoRede

//---------------------------------------------------------------------------
bool __fastcall  TGeral::SalvaGeral(bool redeEmpresa)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTBD * bd = DLL_NewObjBD(apl);
	bool sucesso = false;
	// guarda o arquivo de rede(.mdb) corrente
	AnsiString redeUsuario = path->ArqRede;

	try
	{
		if (redeEmpresa)
		{
			// define uso da base Access
			this->BaseDado = SNPDBTYPE_ACCESS;

			//
			// 1. desconecta da base atual
			//
			bd->Desconecta();


			//
			// 2. coloca o RedeEmpresa.mdb como o arquivo de rede atual
			//
			path->ArqRede = path->ArqRedeEmpresa();
			bd->Conecta();


			//
			// 3. salva o geral atual no redeEmpresa
			//
			sucesso = bd->Geral->SalvaGeral(this);

			//
			// 4. volta a base corrente para a base(rede) que o usuario estava utilizando
			//
			bd->Desconecta();
			path->ArqRede = redeUsuario;
			bd->Conecta();
			sucesso = (sucesso && bd->Geral->LeGeral(this));
		}
		else
		{
			bd->Conecta();
			sucesso = bd->Geral->SalvaGeral(this);
			bd->Desconecta();
		}
		delete bd;
	}
	catch(Exception &e)
	{
		return false;
	}
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall	TGeral::SalvaLimitesTensao(void)
{
	//proteção
   if (PD.infoset == NULL) return (false);
   PD.infoset->Clear();
   //salva todos os objetos em Infoset
   //insere nova linha no infoset
   if ( ! PD.infoset->InsertNewLine()) return(false);
   PD.infoset->AddInfo("limInfBT" ,  LIM_V.LIM_KV[0][0]);
   PD.infoset->AddInfo("limSupBT" ,  LIM_V.LIM_KV[0][1]);
   PD.infoset->AddInfo("bool_limInfBT" ,  LIM_V.LIM_ISABERTO[0][0]);
   PD.infoset->AddInfo("bool_limSupBT" ,  LIM_V.LIM_ISABERTO[0][1]);
	//insere nova linha no infoset
   if ( ! PD.infoset->InsertNewLine()) return(false);
   PD.infoset->AddInfo("limInfMT" ,  LIM_V.LIM_KV[1][0]);
   PD.infoset->AddInfo("limSupMT" ,  LIM_V.LIM_KV[1][1]);
   PD.infoset->AddInfo("bool_limInfMT" ,  LIM_V.LIM_ISABERTO[1][0]);
   PD.infoset->AddInfo("bool_limSupMT" ,  LIM_V.LIM_ISABERTO[1][1]);
   //insere nova linha no infoset
   if ( ! PD.infoset->InsertNewLine()) return(false);
   PD.infoset->AddInfo("limInfAT" ,  LIM_V.LIM_KV[2][0]);
   PD.infoset->AddInfo("limSupAT" ,  LIM_V.LIM_KV[2][1]);
   PD.infoset->AddInfo("bool_limInfAT" ,  LIM_V.LIM_ISABERTO[2][0]);
   PD.infoset->AddInfo("bool_limSupAT" ,  LIM_V.LIM_ISABERTO[2][1]);

   //salva arquivo com limites e tipos do intervalo
   PD.infoset->SaveToFile(ARQ_CFG_LIMITE_TENSAO);

	return(true);
}
//---------------------------------------------------------------------------
int __fastcall  TGeral::TipoSegmentoNivelTensao(VTBarra *barra)
{
	//int tipoSegmento = redeINDEF;
	//verifica em qual tipo de segmento a barra se encaixa
	if(barra->vnom < LIM_V.LIM_KV[0][0])
		{return redeINDEF;}
	//verifica se é maior que o limite superior da AT
	if(barra->vnom > LIM_V.LIM_KV[2][1])
		{return redeTRANS;}
	//verifica se é maior que o limite inferior da AT
	else if (barra->vnom > LIM_V.LIM_KV[2][0])
	{
		//verifica se é igual ao limite superior da AT
		if(IsDoubleZero(barra->vnom - LIM_V.LIM_KV[2][1]))
		{   //sendo igual, depende se o intervalo é aberto ou fechado
			if(LIM_V.LIM_ISABERTO[2][1] ==  true)
				{return redeTRANS;}
			else
				{return redeSUB;}
		}
		else //nao sendo igual ao limite superior, é subtransmissao
			{return redeSUB;}
	}
	//verifica se é maior que o limite inferior da MT
	else if (barra->vnom > LIM_V.LIM_KV[1][0])
	{
		//verifica se é igual ao limite superior da MT
		if(IsDoubleZero(barra->vnom - LIM_V.LIM_KV[1][1]))
		{   //sendo igual, depende se o intervalo é aberto ou fechado
			if(LIM_V.LIM_ISABERTO[1][1] ==  true)
				{return redeSUB;}
			else
				{return redePRI;}
		}
		else //nao sendo igual ao limite superior, é subtransmissao
			{return redePRI;}
	}
	//verifica se é maior que o limite inferior da BT
	else if (barra->vnom > LIM_V.LIM_KV[0][0])
	{
		//verifica se é igual ao limite superior da BT
		if(IsDoubleZero(barra->vnom - LIM_V.LIM_KV[0][1]))
		{   //sendo igual, depende se o intervalo é aberto ou fechado
			if(LIM_V.LIM_ISABERTO[0][1] ==  true)
				{return redePRI;}
			else
				{return redeSEC;}
		}
		else //nao sendo igual ao limite superior, é subtransmissao
			{return redeSEC;}
	}

	return redeINDEF;
}
//---------------------------------------------------------------------------
void __fastcall  TGeral::ValoresDefault(void)
   {
   //variáveis locais
   int          vet_tiporede[] = {redeTRANS, redeSUB, redePRI, redeSEC};
   VTPadraoRede *padrao_rede;

   //define patamar de ponta
   for (int np = 0; np < MAX_PAT; np++) PAT.PONTA[np] = false;
   PAT.PONTA[MAX_PAT-2] = true;
   //TEMPORARIO: = 04/11/2004 - define gráficos do AG
   AG.visu[0].exibeIndiv  = agINDIV_ALFA;
   AG.visu[0].tipoGrafico = agGRAF_TOPO;
   AG.visu[0].npatExibe   = 2;
   //
   AG.visu[1].exibeIndiv  = agINDIV_ALFA;
   AG.visu[1].tipoGrafico = agGRAF_FLOW;
   AG.visu[1].npatExibe   = 2;
   //
   AG.visu[2].exibeIndiv  = agINDIV_ALL;
   AG.visu[2].tipoGrafico = agGRAF_TOPO;
   AG.visu[2].npatExibe   = 2;
   //
   AG.visu[3].exibeIndiv  = agINDIV_ALL;
   AG.visu[3].tipoGrafico = agGRAF_FLOW;
   AG.visu[3].npatExibe   = 2;
   //cores p/ diagnóstico dos eqptos
   COR.Faixa[0] = TColor(8454016);
   COR.Faixa[1] = TColor(65535);
   COR.Faixa[2] = TColor(255);
   COR.Faixa[3] = TColor(8421504);
   //cores p/ redes
   COR.Circ[0] = TColor(8388863);
   COR.Circ[1] = TColor(16744448);
   COR.Circ[2] = TColor(16744448);
   COR.Circ[3] = TColor(33023);
   COR.Circ[4] = TColor(12615935);
   COR.Circ[5] = TColor(8454143);
   COR.Circ[6] = TColor(8421440);
   COR.Circ[7] = TColor(16777088);
   COR.Circ[8] = TColor(32896);
   COR.Circ[9] = TColor(16744703);

   //impedância de defeito para curto fase/terra
   CURTO.R_DEF = 40.;
   CURTO.X_DEF =  0.;
   //reinicia objetos PadraoRede
   LimpaTList(lisPADRAOREDE);
   for (int n = 0; n < sizeof(vet_tiporede)/sizeof(int); n++)
      {//cria um novo Padraorede
      lisPADRAOREDE->Add(padrao_rede = NewObjPadraoRede());
      padrao_rede->PadraoRedeId = vet_tiporede[n];
	  switch (padrao_rede->PadraoRedeId)
		 {
         case redeTRANS:
            padrao_rede->Descricao = "Transmissão";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 230.;
            padrao_rede->VFN  = 230./sqrt(3.);
			padrao_rede->V1   = 0.93;
            padrao_rede->V2   = 0.95;
            padrao_rede->V3   = 1.05;
            padrao_rede->V4   = 1.07;
            padrao_rede->FPI1 = 0.7;
            padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.8;
			padrao_rede->FPS2 = 1.0;
			break;
         case redeSUB:
            padrao_rede->Descricao = "Subtransmissão";
            padrao_rede->NumFases  = 3;
			padrao_rede->VFF  = 88.;
            padrao_rede->VFN  = 88./sqrt(3.);
			padrao_rede->V1   = 0.90;
            padrao_rede->V2   = 0.95;
            padrao_rede->V3   = 1.05;
            padrao_rede->V4   = 1.07;
			padrao_rede->FPI1 = 0.7;
            padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.8;
			padrao_rede->FPS2 = 1.0;
			break;
         case redePRI:
			padrao_rede->Descricao = "Dist.Primária";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 13.8;
            padrao_rede->VFN  = 13.8/sqrt(3.);
			padrao_rede->V1   = 0.90;
            padrao_rede->V2   = 0.93;
			padrao_rede->V3   = 1.05;
            padrao_rede->V4   = 1.05;
            padrao_rede->FPI1 = 0.7;
			padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.8;
			padrao_rede->FPS2 = 1.0;
			//intervalos de cargto de Cabo
			FLOW.I[0] = padrao_rede->FPI1;
			FLOW.I[1] = padrao_rede->FPI2;
			//intervalos de cargto de Trafo
			FLOW.S[0] = padrao_rede->FPS1;
			FLOW.S[1] = padrao_rede->FPS2;
			break;
		 case redeSEC:
            padrao_rede->Descricao = "Dist.Secundária";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 0.220;
            padrao_rede->VFN  = 0.220/sqrt(3.);
			padrao_rede->V1   = 0.86;           //189 V
			padrao_rede->V2   = 0.91;           //201 V
			padrao_rede->V3   = 1.05;           //231 V
			padrao_rede->V4   = 1.06;           //233 V
			padrao_rede->FPI1 = 0.7;
			padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.8;
			padrao_rede->FPS2 = 1.0;
			break;
		 default:
			break;
		 }
	  }
   //Poe valores padrao do prodist
   LimitesTensaoDefault();
   }
//---------------------------------------------------------------------------
void __fastcall  TGeral::ValoresDefaultCOPEL(void)
   {
   //variáveis locais
   int          vet_tiporede[] = {redeTRANS, redeSUB, redePRI, redeSEC};
   VTPadraoRede *padrao_rede;

   //define patamar de ponta
   for (int np = 0; np < MAX_PAT; np++) PAT.PONTA[np] = false;
   PAT.PONTA[MAX_PAT-2] = true;
   //TEMPORARIO: = 04/11/2004 - define gráficos do AG
   AG.visu[0].exibeIndiv  = agINDIV_ALFA;
   AG.visu[0].tipoGrafico = agGRAF_TOPO;
   AG.visu[0].npatExibe   = 2;
   //
   AG.visu[1].exibeIndiv  = agINDIV_ALFA;
   AG.visu[1].tipoGrafico = agGRAF_FLOW;
   AG.visu[1].npatExibe   = 2;
   //
   AG.visu[2].exibeIndiv  = agINDIV_ALL;
   AG.visu[2].tipoGrafico = agGRAF_TOPO;
   AG.visu[2].npatExibe   = 2;
   //
   AG.visu[3].exibeIndiv  = agINDIV_ALL;
   AG.visu[3].tipoGrafico = agGRAF_FLOW;
   AG.visu[3].npatExibe   = 2;
   //cores p/ diagnóstico dos eqptos
//   COR.Faixa[0] = TColor(8454016);
   COR.Faixa[0] = clBlue;
   COR.Faixa[1] = TColor(65535);
   COR.Faixa[2] = TColor(255);
   COR.Faixa[3] = TColor(8421504);
   //cores p/ redes
   COR.Circ[0] = TColor(8388863);
   COR.Circ[1] = TColor(16744448);
   COR.Circ[2] = TColor(16744448);
   COR.Circ[3] = TColor(33023);
   COR.Circ[4] = TColor(12615935);
   COR.Circ[5] = TColor(8454143);
   COR.Circ[6] = TColor(8421440);
   COR.Circ[7] = TColor(16777088);
   COR.Circ[8] = TColor(32896);
   COR.Circ[9] = TColor(16744703);

   //impedância de defeito para curto fase/terra
   CURTO.R_DEF = 40.;
   CURTO.X_DEF =  0.;
   //reinicia objetos PadraoRede
   LimpaTList(lisPADRAOREDE);
   for (int n = 0; n < sizeof(vet_tiporede)/sizeof(int); n++)
      {//cria um novo Padraorede
      lisPADRAOREDE->Add(padrao_rede = NewObjPadraoRede());
      padrao_rede->PadraoRedeId = vet_tiporede[n];
	  switch (padrao_rede->PadraoRedeId)
		 {
         case redeTRANS:
            padrao_rede->Descricao = "Transmissão";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 230.;
            padrao_rede->VFN  = 230./sqrt(3.);
			padrao_rede->V1   = 0.93;
            padrao_rede->V2   = 0.95;
            padrao_rede->V3   = 1.05;
            padrao_rede->V4   = 1.07;
			padrao_rede->FPI1 = 0.6;
            padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.9;
			padrao_rede->FPS2 = 1.0;
			break;
         case redeSUB:
            padrao_rede->Descricao = "Subtransmissão";
            padrao_rede->NumFases  = 3;
			padrao_rede->VFF  = 88.;
            padrao_rede->VFN  = 88./sqrt(3.);
			padrao_rede->V1   = 0.90;
            padrao_rede->V2   = 0.95;
            padrao_rede->V3   = 1.05;
            padrao_rede->V4   = 1.07;
			padrao_rede->FPI1 = 0.6;
            padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.9;
			padrao_rede->FPS2 = 1.0;
			break;
         case redePRI:
			padrao_rede->Descricao = "Dist.Primária";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 13.8;
            padrao_rede->VFN  = 13.8/sqrt(3.);
			padrao_rede->V1   = 0.915;
			padrao_rede->V2   = 0.935;
			padrao_rede->V3   = 1.00;
			padrao_rede->V4   = 1.00;
			padrao_rede->FPI1 = 0.6;
			padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.9;
			padrao_rede->FPS2 = 1.0;
			//intervalos de cargto de Cabo
			FLOW.I[0] = padrao_rede->FPI1;
			FLOW.I[1] = padrao_rede->FPI2;
			//intervalos de cargto de Trafo
			FLOW.S[0] = padrao_rede->FPS1;
			FLOW.S[1] = padrao_rede->FPS2;
			break;
		 case redeSEC:
            padrao_rede->Descricao = "Dist.Secundária";
            padrao_rede->NumFases  = 3;
            padrao_rede->VFF  = 0.220;
            padrao_rede->VFN  = 0.220/sqrt(3.);
			padrao_rede->V1   = 0.86;           //189 V
			padrao_rede->V2   = 0.91;           //201 V
			padrao_rede->V3   = 1.05;           //231 V
			padrao_rede->V4   = 1.06;           //233 V
			padrao_rede->FPI1 = 0.6;
			padrao_rede->FPI2 = 1.0;
			padrao_rede->FPS1 = 0.9;
			padrao_rede->FPS2 = 1.0;
			break;
		 default:
			break;
		 }
	  }
   //Poe valores padrao do prodist
   LimitesTensaoDefault();
   }
//---------------------------------------------------------------------------
//eof