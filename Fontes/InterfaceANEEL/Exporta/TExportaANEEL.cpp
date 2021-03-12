//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaANEEL.h"
#include "TExportaEqpto.h"
#include "..\BDaneel\VTBDaneel.h"
#include "..\Importa\VTExtrator.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Arranjo\VTCabo.h"
#include "..\..\Arranjo\VTSuporte.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Curvas\VTCurvas.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\ImportaRedeGIS\VTImportaRedeGIS.h"
#include "..\..\MontaZ\VTMontaZ.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCapserie.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTFiltro.h"
#include "..\..\Rede\VTMutua.h"
#include "..\..\Rede\VTReator.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTipoChave.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTipos.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Radial.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTExportaANEEL* NewObjExportaANEEL(VTApl *apl)
   {
	return(new TExportaANEEL(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaANEEL::TExportaANEEL(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
	//cria objetos
	exporta_eqpto = NewObjExportaEqpto(apl);
	BDaneel       = NewObjBDaneel(apl);
	radial        = DLL_NewObjRadial(apl);
	lisREDE       = new TList();
	lisEQP        = new TList();
	lisLIG        = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TExportaANEEL::~TExportaANEEL(void)
	{
   //destrói objetos
	if (radial)        {delete radial;        radial        = NULL;}
	if (BDaneel)       {delete BDaneel;       BDaneel       = NULL;}
	if (exporta_eqpto) {delete exporta_eqpto; exporta_eqpto = NULL;}
	//destrói lista sem destruir objetos
	if (lisREDE) {delete lisREDE; lisREDE = NULL;}
	if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
	if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::AtualizaCargaSemNeutro(TList *lisCARGA)
	{
	//variáveis locais
	VTCarga *carga;

	for (int n = 0; n < lisCARGA->Count; n++)
		{
		carga = (VTCarga*)lisCARGA->Items[n];
		//elimina neutro da Carga
		carga->Fases = carga->Fases & (faseABC | faseT);
		//verifica se Carga possui uma única fase
		if (fases->NumeroDeFases(carga->Fases) == 1)
			{//inclui faseT
			carga->Fases = carga->Fases | faseT;
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::AtualizaTrafoLigacaoDy1(TList *lisTRF)
	{
	//variáveis locais
	VTTrafo *trafo;

	for (int n = 0; n < lisTRF->Count; n++)
      {
		trafo = (VTTrafo*)lisTRF->Items[n];
      if (trafo->Monofasico) continue;
		//verifica se Trafo D/Y
		if ((trafo->pri.ligacao == lgTRIANG) && (trafo->sec.ligacao != lgTRIANG))
			{
			trafo->sec.defasagem = -(fabs(trafo->sec.defasagem));
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::AtualizaTrafoSemNeutro(TList *lisTRF)
	{
	//variáveis locais
	VTTrafo *trafo;

	for (int n = 0; n < lisTRF->Count; n++)
      {
		trafo = (VTTrafo*)lisTRF->Items[n];
		//elimina neutro do primário e secundário
		//trafo->pri.fase = trafo->pri.fase & (faseABC | faseT);
		//trafo->sec.fase = trafo->sec.fase & (faseABC | faseT);
		//verifica se o primário possui neutro
		if (trafo->pri.fase & faseN)
			{//substitui neutro por terra
			trafo->pri.fase = (trafo->pri.fase & faseABC) | faseT;
			}
		//verifica se o secundário possui neutro
		if (trafo->sec.fase & faseN)
			{//substitui neutro por terra
			trafo->sec.fase = (trafo->sec.fase & faseABC) | faseT;
			}
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::ExportaRedes(AnsiString filename, strOPCAO_ANEEL &opcao)
	{
	//variáveis locais
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//salva opções de exportação
		this->opcao = opcao;
		//inicia objeto Radial
		if (! radial->Inicia(redes)) return(false);
		//inicia conexão com a base de dados
//		if (! BDaneel->Conecta(filename)) return(false);
		//define Extern_id
		IniciaObjAsIdTipos();
		IniciaObjAsIdArranjos();
		IniciaObjAsIdCurvasTipicas();
		IniciaObjAsIdEqptos();
		//define código das Barras com suas coordenadas
		switch(opcao.fmt_codigo)
			{
			case 1:
				IniciaCodigoBarraComCoordenadas(false);
				break;
			case 2:
				IniciaCodigoBarraComCoordenadas(true);
				break;
			case 0:
			default:	//nada a fazer
				break;
			}
		//configura ExportaEqpto
		//exporta_eqpto->Data      = data;
		//Arranjos
		if ( ! MontaPlanilhaCondutor())    return(false);
		//cruvas típicas
//		if ( ! MontaPlanilhaCurvaTipica()) return(false);
		//dados das redes MT
//		if ( ! MontaPlanilhaTrafoAT_MT())  return(false);
//		if ( ! MontaPlanilhaCircuito_MT()) return(false);
//		if ( ! MontaPlanilhaSegmento_MT()) return(false);
//		if ( ! MontaPlanilhaRegulador_MT())return(false);
//		if ( ! MontaPlanilhaChave_MT())    return(false);
		if ( ! MontaPlanilhaCarga_MT())    return(false);
//		if ( ! MontaPlanilhaGerador_MT())  return(false);
		//dados das redes BT
//		if ( ! MontaPlanilhaTrafoMT_BT())  return(false);
//		if ( ! MontaPlanilhaSegmento_BT()) return(false);
//		if ( ! MontaPlanilhaChave_BT())    return(false);
//		if ( ! MontaPlanilhaRamal_BT())    return(false);
		if ( ! MontaPlanilhaCarga_BT())    return(false);
//		if ( ! MontaPlanilhaGerador_BT())  return(false);
		//fecha conexão com a base de dados
//		BDaneel->Desconecta();
		}catch(Exception &e)
         {
         return(false);
         }
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::ExportaRedes(void)
   {
	//variáveis locais
	VTRede  *rede;
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList   *lisREDE = redes->LisRede();

   try{//loop p/ todas Redes
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
			rede = (VTRede*)lisREDE->Items[nr];
         }
      }catch(Exception &e)
         {
         return(false);
         }
	return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaCodigoBarraComCoordenadas(bool xy_seq)
	{
	//variáveis locais
	AnsiString codigo;
	int        x1, y1, x2, y2;
	VTBarra    *barra;
	VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList      *lisBARRA = redes->LisBarra();

	for (int nb = 0; nb < lisBARRA->Count; nb++)
		{
		barra = (VTBarra*)lisBARRA->Items[nb];
		//obtém coordenadas utm da Batta
		if (! barra->CoordenadasUtm_cm(x1, y1))
			{//obtém coordenadas esquemático
			 barra->CoordenadasEsquematico(x1, y1, x2, y2);
         }
		//monta código da barra
		if (xy_seq) codigo.sprintf("%d_%d_%05d", x1, y1, nb+1);
		else        codigo.sprintf("%d_%d", x1, y1);
		barra->Codigo = codigo;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdArranjos(void)
	{
	//variáveis locais
	VTArranjo  *arranjo;
	VTCabo     *cabo;
	VTSuporte  *suporte;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	TList      *lisARR = arranjos->LisArranjo();
	TList      *lisCAB = arranjos->LisCabo();
	TList      *lisSUP = arranjos->LisSuporte();

	//inicia Extern_id dos Arranjos
	for (int n = 0, index = -1; n < lisARR->Count; n++, index--)
		{
		arranjo = (VTArranjo*)lisARR->Items[n];
      arranjo->Obj = (TObject*)index;
      }
   //inicia Extern_id dos Cabos
   for (int n = 0, index = -1; n < lisCAB->Count; n++, index--)
      {
      cabo = (VTCabo*)lisCAB->Items[n];
      cabo->Obj = (TObject*)index;
      }
   //inicia Extern_id dos Suportes
   for (int n = 0, index = -1; n < lisSUP->Count; n++, index--)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      suporte->Obj = (TObject*)index;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdCurvas(TList *lisCURVA)
   {
   //variáveis locais
   VTCurva  *curva;

    //inicia Extern_id das Curvas
   for (int n = 0, index = -1; n < lisCURVA->Count; n++, index--)
      {
      curva      = (VTCurva*)lisCURVA->Items[n];
      curva->Obj = (TObject*)index;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdCurvasTipicas(void)
   {
   //variáveis locais
   VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   IniciaObjAsIdCurvas(curvas->ListaCurvaTipicaCapacitor());
   IniciaObjAsIdCurvas(curvas->ListaCurvaTipicaCarga());
   IniciaObjAsIdCurvas(curvas->ListaCurvaTipicaGeradorFotovoltaico());
   IniciaObjAsIdCurvas(curvas->ListaCurvaTipicaIP());
   IniciaObjAsIdCurvas(curvas->ListaCurvaTipicaReator());
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdEqptos(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //monta lista com Redes, Barras, Eqbar e Ligacoes
   lisEQP->Clear();
   redes->LisRede(lisEQP);
	redes->LisBarra(lisEQP);
	redes->LisEqbar(lisEQP);
   redes->LisLigacao(lisEQP);
   //inicia Extern_id dos Eqptos
   IniciaObjAsIdEqptos(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdEqptos(TList *lisEQP)
   {
   //variáveis locais
   VTEqpto *eqpto;

   for (int n = 0, index = -1; n < lisEQP->Count; n++, index--)
      {
      eqpto      = (VTEqpto*)lisEQP->Items[n];
      eqpto->Obj = (TObject*)index;
      }

   }
//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdEqptos(VTRede *rede)
   {
   //monta lista com Rede, Barras, Eqbar e Ligacoes
   lisEQP->Clear();
   lisEQP->Add(rede);
   rede->LisBarra(lisEQP);
   rede->LisEqbar(lisEQP);
   rede->LisLigacao(lisEQP);
    //inicia Extern_id dos Eqptos
   IniciaObjAsIdEqptos(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaObjAsIdTipos(void)
   {
   //variáveis locais
   TList        *lisEXT;
   VTTipoChave  *tipochave;
   VTTipoRede   *tiporede;
   VTTipos      *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //inicia Extern_id dos TipoChave
   lisEXT = tipos->LisTipoChave();
   for (int n = 0, index = -1; n < lisEXT->Count; n++, index--)
      {
		tipochave      = (VTTipoChave*)lisEXT->Items[n];
      tipochave->Obj = (TObject*)index;
      }
	//inicia Extern_id dos TipoRede
   lisEXT = tipos->LisTipoRede();
   for (int n = 0, index = -1; n < lisEXT->Count; n++, index--)
      {
      tiporede      = (VTTipoRede*)lisEXT->Items[n];
      tiporede->Obj = (TObject*)index;
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TExportaANEEL::LisArranjo(void)
	{
	//variáveis
	VTTrecho *trecho;
	VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia lisEQP apenas com os Arranjos existentes na rede
	lisEQP->Clear();
	//obtém lista de Trechos da Rede
	lisLIG->Clear();
	redes->LisEqpto(lisLIG, eqptoTRECHO);
	for (int n = 0; n < lisLIG->Count; n++)
		{
		trecho =(VTTrecho*)lisLIG->Items[n];
		if (lisEQP->IndexOf(trecho->arranjo) < 0)
			{//insere Arranjo em lisEQP
			lisEQP->Add(trecho->arranjo);
			}
		}
	return(lisEQP);
	}

//---------------------------------------------------------------------------
TList* __fastcall TExportaANEEL::LisEqpto(TList *lisORIG, int tipo_eqpto)
	{
	//variáveis locais
   VTEqpto   *eqpto;
   VTLigacao *ligacao;
	VTRede    *rede;
	VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia lisEQP apenas com os Eqptos do tipo indicado
	lisEQP->Clear();
	for (int n = 0; n < lisORIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisORIG->Items[n];
      if (eqpto->Tipo() == tipo_eqpto) lisEQP->Add(eqpto);
      }
   //verifica se tipo_eqpto indica eqptoREDE
   if (tipo_eqpto == eqptoREDE)
      {//inclui na lista as Redes das Ligacoes
      for (int n = 0; n < lisORIG->Count; n++)
         {
         eqpto = (VTEqpto*)lisORIG->Items[n];
         if (eqpto->TipoLigacao())
            {
            ligacao = (VTLigacao*)eqpto;
            rede    = ligacao->rede;
            if (rede == redes->RedeIsolada()) continue;
            if (lisEQP->IndexOf(rede) < 0) lisEQP->Add(rede);
            }
         }
      }
   //verifica se tipo_eqpto indica eqptoBARRA
   else if (tipo_eqpto == eqptoBARRA)
      {//inclui na lista as Barras das Ligacoes
      for (int n = 0; n < lisORIG->Count; n++)
         {
         eqpto = (VTEqpto*)lisORIG->Items[n];
         if (eqpto->TipoLigacao())
            {
            ligacao = (VTLigacao*)eqpto;
            ligacao->LisBarra(lisEQP);
            }
         }
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaANEEL::LisLigacao(TList *lisORIG, TList *lisDEST, VTRede *rede)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   VTLigacao *ligacao;

   //insere em lisDEST todas as Ligacoes existentes em lisORIG que pertencem à Rede indicada
   for (int n = 0; n < lisORIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisORIG->Items[n];
      if (eqpto->TipoLigacao())
         {//verifica se a Ligacao pertence à Rede indicada
         ligacao = (VTLigacao*)eqpto;
         if ((rede == NULL)||(ligacao->rede == rede)) lisDEST->Add(eqpto);
         }
      }
   return(lisDEST->Count);
	}

//---------------------------------------------------------------------------
TList* __fastcall TExportaANEEL::LisEqpto(VTRede *rede, int tipo_eqpto)
   {
   //variáveis locais
   bool barra_isolada = true;

   //reinicia lisEQP apenas com os Eqptos do tipo indicado
	lisEQP->Clear();
   rede->LisEqpto(lisEQP, tipo_eqpto, barra_isolada);
   return(lisEQP);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCarga_BT()
	{
	//variáveis locais
	AnsiString   codigo;
	sdi_CARGA_BT sdi;
	TList        *lisCARGA;
	TList        *lisREDE;
	VTCarga      *carga;
	VTRede       *rede;
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Cargas da Rede
			lisCARGA = LisEqpto(rede, eqptoCARGA);
			//redefine código das Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				codigo.sprintf("%s_Carga%04d", rede->Codigo.c_str(), n+1);
				carga->Codigo = codigo;
				}
			//verifica se modelo de rede ANEEL (sem neutro)
			if (opcao.rede_aneel) AtualizaCargaSemNeutro(lisCARGA);
			//loop p/ todas Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				//inicia sdi_CARGA_BT
				if (! exporta_eqpto->MontaCargaBT(rede, carga, sdi)) return(false);
				//grava BDaneel
//				if (! BDaneel->WriteCargaBT(sdi)) return(false);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCarga_MT(void)
	{
	//variáveis locais
	AnsiString   codigo;
	sdi_CARGA_MT sdi;
	TList        *lisCARGA;
	TList        *lisREDE;
	VTCarga      *carga;
	VTRede       *rede;
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Cargas da Rede
			lisCARGA = LisEqpto(rede, eqptoCARGA);
			//redefine código das Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				codigo.sprintf("%s_Carga%04d", rede->Codigo.c_str(), n+1);
				carga->Codigo = codigo;
				}
			//verifica se modelo de rede ANEEL (sem neutro)
			if (opcao.rede_aneel) AtualizaCargaSemNeutro(lisCARGA);
			//loop p/ todas Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				//inicia sdi_CARGA_MT
				if (! exporta_eqpto->MontaCargaMT(rede, carga, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteCargaMT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaChave_BT(void)
	{
	//variáveis locais
	sdi_CHAVE_BT sdi;
	TList        *lisCHAVE;
	TList        *lisREDE;
	VTChave      *chave;
	VTRede       *rede;
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Chaves da Rede
			lisCHAVE = LisEqpto(rede, eqptoCHAVE);
			//loop p/ todas Chaves
			for (int n = 0; n < lisCHAVE->Count; n++)
				{
				chave = (VTChave*)lisCHAVE->Items[n];
				//inicia sdi_CHAVE_BT
				if (! exporta_eqpto->MontaChaveBT(chave, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteChaveBT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL:: MontaPlanilhaChave_MT(void)
	{
	//variáveis locais
	sdi_CHAVE_MT sdi;
	TList       *lisCHAVE;
	TList       *lisREDE;
	VTChave     *chave;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			lisCHAVE = LisEqpto(rede, eqptoCHAVE);
			//loop p/ todas Chaves
			for (int n = 0; n < lisCHAVE->Count; n++)
				{
				chave = (VTChave*)lisCHAVE->Items[n];
				//inicia sdi_CHAVE_MT
				if (! exporta_eqpto->MontaChaveMT(chave, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteChaveMT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCircuito_MT(void)
	{
	//variáveis locais
	sdi_CIRCUITO sdi;
	TList        *lisREDE;
	VTPrimario   *primario;
	TList        *lisPRIMARIO = radial->LisPrimario();

	try{//loop p/ todos Primarios
		for (int n = 0; n < lisPRIMARIO->Count; n++)
			{
			primario = (VTPrimario*)lisPRIMARIO->Items[n];
			//proteção
			if (primario->Subestacao == NULL)
				{//inicia sdi_CIRCUITO sem subestação
				if (! exporta_eqpto->MontaCircuitoMT(NULL, primario->Rede, sdi)) return(false);
				}
			else
				{//inicia sdi_CIRCUITO com subestação
				if (! exporta_eqpto->MontaCircuitoMT(primario->Subestacao->Rede, primario->Rede, sdi)) return(false);
				}
			//grava base de dados
//			BDaneel->WriteCircMT(sdi);
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCondutor(void)
	{
	//variáveis locais
	sdi_CONDUTOR sdi;
	AnsiString   codigo;
	double       comp_1km = 1.0;
	double       yc1_mho, yc0_mho;
	strIMP       z1, z0;
	TList        *lisARRANJO;
	VTArranjo    *arranjo;
	VTMontaZ     *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

	try{//obtém lista de Arranjos em uso na rede
		lisARRANJO = LisArranjo();
		//redefine Arranjos definido por impedância de sequência
		for (int n = 0; n < lisARRANJO->Count; n++)
			{
			arranjo = (VTArranjo*)lisARRANJO->Items[n];
			codigo.sprintf("ArranjoANEEL_%04d", n+1);
			arranjo->Codigo = codigo;
			//obtèm impedâncias em ohm/km
			arranjo->Z0_ohm(comp_1km, z0, yc0_mho);
			arranjo->Z1_ohm(comp_1km, z1, yc1_mho);
			//redefine o Arranjo
			arranjo->TipoCfg = arranjoZ0Z1;
			//não altera as fases do Arranjo
//			arranjo->Fases   = faseABC;
			//define Arranjo sem neutro
			arranjo->Fases   = arranjo->Fases & faseABC;
//			arranjo->z0      = z0;
			arranjo->z1      = z1;
			arranjo->z0      = z1;
			montaZ->Executa(arranjo);
			}
		//loop p/ todos Arranjos
		for (int n = 0; n < lisARRANJO->Count; n++)
			{
			arranjo = (VTArranjo*)lisARRANJO->Items[n];
			//inicia sdi_CONDUTOR
			if (! exporta_eqpto->MontaCondutor(arranjo, sdi)) return(false);
			//grava base de dados
//			BDaneel->WriteCodCondutor(sdi);
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCurvaTipica(void)
	{
	//variáveis locais
	sdi_CURVA sdi;
	TList     *lisCURVA;
	VTCurva   *curva;
	VTCurvas  *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	try{//loop p/ todas Curvas típicas de carga
		lisCURVA = curvas->ListaCurvaTipicaCarga();
		for (int n = 0; n < lisCURVA->Count; n++)
			{
			curva = (VTCurva*)lisCURVA->Items[n];
			//insere linha com dados da Curva
			if (! exporta_eqpto->MontaCurvaTipica(curva, sdi)) return(false);
			//grava base de dados
//			if (curva->Classe->Tag == consA4)
//				{BDaneel->WriteCrvCrgMT(sdi);}
//			else
//				{BDaneel->WriteCrvCrgBT(sdi);}
			}
		//loop p/ todas Curvas típicas de IP
		lisCURVA = curvas->ListaCurvaTipicaIP();
		for (int n = 0; n < lisCURVA->Count; n++)
			{
			curva = (VTCurva*)lisCURVA->Items[n];
			//inicia sdi_CURVA
			if (! exporta_eqpto->MontaCurvaTipica(curva, sdi)) return(false);
			//grava base de dados
//			BDaneel->WriteCrvCrgBT(sdi);
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaGerador_BT(void)
	{
	//variáveis locais
	sdi_GERADOR_BT sdi;
	TList          *lisGERADOR;
	TList          *lisREDE;
	VTGerador      *gerador;
	VTRede         *rede;
	VTRedes        *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Geradores da Rede
			lisGERADOR = LisEqpto(rede, eqptoGERADOR);
			//loop p/ todos Geradores
			for (int n = 0; n < lisGERADOR->Count; n++)
				{
				gerador = (VTGerador*)lisGERADOR->Items[n];
				//inicia sdi_GERADOR_BT
				if (! exporta_eqpto->MontaGeradorBT(rede, gerador, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteGeradorBT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaGerador_MT(void)
	{
	//variáveis locais
	sdi_GERADOR_MT sdi;
	TList          *lisGERADOR;
	TList          *lisREDE;
	VTGerador      *gerador;
	VTRede         *rede;
	VTRedes        *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Geradores da Rede
			lisGERADOR = LisEqpto(rede, eqptoGERADOR);
			//loop p/ todos Geradores
			for (int n = 0; n < lisGERADOR->Count; n++)
				{
				gerador = (VTGerador*)lisGERADOR->Items[n];
				//inicia sdi_GERADOR_MT
				if (! exporta_eqpto->MontaGeradorMT(rede, gerador, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteGeradorMT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaRamal_BT(void)
	{
	//variáveis locais
	sdi_RAMAL   sdi;
	TList       *lisTRECHO;
	TList       *lisREDE;
	VTTrecho    *trecho;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Trechos da Rede
			lisTRECHO = LisEqpto(rede, eqptoTRECHO);
			//loop p/ todos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				//verifica se o Trecho é um Ramal
				if (trecho->RamalLigacao)
					{//inicia sdi_RAMAL
					if (! exporta_eqpto->MontaRamalBT(trecho, sdi)) return(false);
					//grava base de dados
//					BDaneel->WriteRamal(sdi);
					}
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaRegulador_MT(void)
	{
	//variáveis locais
	sdi_REGULADOR sdi;
	int           seq;
	TList         *lisREGULADOR;
	TList         *lisREDE;
	VTRegulador   *regulador;
	VTRede        *rede;
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Reguladores da Rede
			lisREGULADOR = LisEqpto(rede, eqptoREGULADOR);
			//loop p/ todas Reguladores
			for (int n = 0; n < lisREGULADOR->Count; n++)
				{
				regulador = (VTRegulador*)lisREGULADOR->Items[n];
				//inicia sdi_REGULADOR em função tipo de ligacao
				switch(regulador->ligacao)
					{
					case lgMONOFASICO:   // 1 unidade monofásica
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, regulador->fases, sdi)) return(false);
						break;
					case lgDELTA_ABERTO: //2 unidades monofásicas
						switch(regulador->fases & faseABC)
							{
							case faseA:
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB, sdi)) return(false);
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseCA, sdi)) return(false);
								break;
							case faseB:
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB, sdi)) return(false);
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBC, sdi)) return(false);
								break;
							case faseC:
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseBC, sdi)) return(false);
								if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseCA, sdi)) return(false);
								break;
							default:
								return(false);
							}
						break;
					case lgDELTA_FECHADO: //3 unidades monofásicas
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBC, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseCA, sdi)) return(false);
						break;
					case lgEST_AT: //3 unidades monofásicas
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAN, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBN, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseCN, sdi)) return(false);
						break;
					case lgEST_ISO:
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseA, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseB, sdi)) return(false);
						if (! exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseC, sdi)) return(false);
						break;
					default:
						return(false);
					}
				//grava base de dados
//				BDaneel->WriteRegulador(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaSegmento_BT(void)
	{
	//variáveis locais
	AnsiString      codigo;
	sdi_SEGMENTO_BT sdi;
	TList           *lisTRECHO;
	TList           *lisREDE;
	VTTrecho        *trecho;
	VTRede          *rede;
	VTRedes         *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Trechos da Rede
			lisTRECHO = LisEqpto(rede, eqptoTRECHO);
			//redefine código dos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				codigo.sprintf("%s_T%04d", rede->Codigo.c_str(), n+1);
				trecho->Codigo = codigo;
				}
			//loop p/ todos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				//verifica se Trecho não é um Ramal
				if (! trecho->RamalLigacao)
					{//inicia sdi_SEGMENTO_BT
					if (! exporta_eqpto->MontaSegmentoBT(trecho, sdi)) return(false);
					//grava base de dados
//					BDaneel->WriteSegmentoBT(sdi);
					}
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaSegmento_MT(void)
	{
	//variáveis locais
	AnsiString   codigo;
	sdi_SEGMENTO_MT sdi;
	TList       *lisTRECHO;
	TList       *lisREDE;
	VTTrecho    *trecho;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Trechos da Rede
			lisTRECHO = LisEqpto(rede, eqptoTRECHO);
			//redefine código dos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				codigo.sprintf("%s_T%04d", rede->Codigo.c_str(), n+1);
				trecho->Codigo = codigo;
				}
			//loop p/ todos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				//inicia sdi_SEGMENTO_MT
				if (! exporta_eqpto->MontaSegmentoMT(trecho, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteSegmentoMT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaTrafoAT_MT(void)
	{
	//variáveis locais
	sdi_TRAFO_AT_MT sdi;
	TList           *lisTRAFO;
	TList           *lisREDE;
	VTTrafo         *trafo;
	VTRede          *rede;
	VTRedes         *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeETD) continue;
			//obtém lista de Trafos da Rede
			lisTRAFO = LisEqpto(rede, eqptoTRAFO);
			//acerta defasagem trado DY
			if (opcao.trafoDY == 1) AtualizaTrafoLigacaoDy1(lisTRAFO);
			//verifica se modelo de rede ANEEL (sem neutro)
			if (opcao.rede_aneel) AtualizaTrafoSemNeutro(lisTRAFO);
			//loop p/ todos Trafos
			for (int n = 0; n < lisTRAFO->Count; n++)
				{
				trafo = (VTTrafo*)lisTRAFO->Items[n];
				//inicia sdi_TRAFO_AT_MT
				if (! exporta_eqpto->MontaTrafoAT_MT(rede, trafo, n+1, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteTrafoATATATMT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaTrafoMT_BT(void)
	{
	//variáveis locais
	sdi_TRAFO_MT_BT sdi;
	TList           *lisTRAFO;
	TList           *lisREDE;
	VTTrafo         *trafo;
	VTSecundario    *secundario;
	TList           *lisSECUNDARIO = radial->LisSecundario();

	try{//loop p/ todas Redes
		for (int nr = 0; nr < lisSECUNDARIO->Count; nr++)
			{
			secundario = (VTSecundario*)lisSECUNDARIO->Items[nr];
			//obtém lista de Trafos da Rede
			lisTRAFO = LisEqpto(secundario->Rede, eqptoTRAFO);
			//acerta defasagem trado DY
			if (opcao.trafoDY == 1) AtualizaTrafoLigacaoDy1(lisTRAFO);
			//verifica se modelo de rede ANEEL (sem neutro)
			if (opcao.rede_aneel) AtualizaTrafoSemNeutro(lisTRAFO);
			//loop p/ todos Trafos
			for (int n = 0; n < lisTRAFO->Count; n++)
				{
				trafo = (VTTrafo*)lisTRAFO->Items[n];
				//inicia sdi_TRAFO_MT_BT
				if (! exporta_eqpto->MontaTrafoMT_BT(secundario->Primario->Rede, trafo, n+1, sdi)) return(false);
				//grava base de dados
//				BDaneel->WriteTrafoMTMTMTBT(sdi);
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

/*
//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoBarra(TList *lisEQP)
	{
	if (lisEQP->Count > 0)
		{//insere header
		InsereHeader("Barra");
		//loop p/ todos eqptos
		for (int n = 0; n < lisEQP->Count; n++)
			{
			if (! exporta_eqpto->MontaBarra((VTBarra*)lisEQP->Items[n]));
			}
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoCapacitor(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Capacitor");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaCapacitor((VTCapacitor*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoCapserie(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {///insere header
      InsereHeader("CapacitorSerie");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaCapserie((VTCapserie*)lisEQP->Items[n]));
         }
		}
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoCargaIP(TList *lisEQP)
   {
   //variáveis locais
   VTCarga *carga;

   //mantém em lisEQP somente as Cargas de IP
	for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      if (! carga->IP) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("CargaIP");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
			{
			if (! exporta_eqpto->MontaCargaIP((VTCarga*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoCargaComMedicao(TList *lisEQP)
   {
   //variáveis locais
   VTCarga *carga;

   //mantém em lisEQP somente as Cargas com curva própria
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      if (carga->curva->Tipica) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("CargaComMedicao");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaCargaComMedicao((VTCarga*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoCargaSemMedicao(TList *lisEQP)
   {
   //variáveis locais
   VTCarga *carga;

   //mantém em lisEQP somente as Cargas com curva típica
	for (int n = lisEQP->Count-1; n >= 0; n--)
		{
		carga = (VTCarga*)lisEQP->Items[n];
		if (! carga->curva->Tipica)lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("CargaSemMedicao");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaCargaSemMedicao((VTCarga*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoChave(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Chave");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaChave((VTChave*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoFiltro(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Filtro");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaFiltro((VTFiltro*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoGerador(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Gerador");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaGerador((VTGerador*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoMutua(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Mutua");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaMutua((VTMutua*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoReator(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Reator");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaReator((VTReator*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoRede(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("Rede");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaRede((VTRede*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoReguladorAutomatico(TList *lisEQP)
   {
   //variáveis locais
   VTRegulador *regulador;

   //mantém em lisEQP somente os Reguladores com ajsute automático
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      regulador = (VTRegulador*)lisEQP->Items[n];
      if (! regulador->ajuste_auto) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("ReguladorAutomatico");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaReguladorAutomatico((VTRegulador*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoReguladorTapFixo(TList *lisEQP)
   {
   //variáveis locais
   VTRegulador *regulador;

   //mantém em lisEQP somente os Reguladores com ajsute fixo
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      regulador = (VTRegulador*)lisEQP->Items[n];
      if (regulador->ajuste_auto) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("ReguladorTapFixo");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaReguladorTapFixo((VTRegulador*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoSuprimento(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {///insere header
      InsereHeader("Suprimento");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaSuprimento((VTSuprimento*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrafo(TList *lisEQP)
   {
   //variáveis locais
   VTTrafo *trafo;

   //mantém em lisEQP somente os trafos  trifásicos
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      if (trafo->Monofasico) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("TransformadorTrifasico2E");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaTrafoTrifasico2E((VTTrafo*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrafo3E(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("TransformadorTrifasico3E");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaTrafoTrifasico3E((VTTrafo3E*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrafoMonofasico(TList *lisEQP)
   {
   //variáveis locais
   VTTrafo *trafo;

   //mantém em lisEQP somente os trafos monofásicos
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      if (! trafo->Monofasico) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("TransformadorMonofasico");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaTrafoMonofasico((VTTrafo*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrafoZZ(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("TransformadorZigZag");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         if (! exporta_eqpto->MontaTrafoZZ((VTTrafoZZ*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrechoArranjoProprio(TList *lisEQP)
   {
   //variáveis locais
   VTTrecho *trecho;

   //filtra Trechos com Arranjo próprio
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      if (trecho->ArranjoTipico) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {//insere header
      InsereHeader("TrechoZ0Z1");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
			if (! exporta_eqpto->MontaTrechoZ0Z1((VTTrecho*)lisEQP->Items[n]));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoEqptoTrechoArranjoTipico(TList *lisEQP)
   {
   //variáveis locais
   VTTrecho *trecho;

   //filtra Trechos com Arranjo típico
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      if (! trecho->ArranjoTipico) lisEQP->Delete(n);
      }
   if (lisEQP->Count > 0)
      {///insere header
      InsereHeader("TrechoArranjoTipico");
      //loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
			if (! exporta_eqpto->MontaTrechoArranjoTipico((VTTrecho*)lisEQP->Items[n]));
         }
      }
   return(true);
   }
*/

//-----------------------------------------------------------------------------
// eof


