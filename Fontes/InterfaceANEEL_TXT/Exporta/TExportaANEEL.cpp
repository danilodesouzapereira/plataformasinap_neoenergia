//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaANEEL.h"
#include "TExportaEqpto.h"
#include "TLine.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\\DLL_Inc\Funcao.h"
#include "..\..\\DLL_Inc\Radial.h"
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
	radial        = DLL_NewObjRadial(apl);
   lines         = new TStringList();
   lisREDE       = new TList();
   lisEQP        = new TList();
	lisLIG        = new TList();
	//inicia dados
	Separador     = ';';
	}

//---------------------------------------------------------------------------
__fastcall TExportaANEEL::~TExportaANEEL(void)
   {
   //destrói objetos
	if (lines)  {delete lines;  lines  = NULL;}
	if (radial) {delete radial; radial = NULL;}
	if (exporta_eqpto) {delete exporta_eqpto; exporta_eqpto = NULL;}
	//destrói lista sem destruir objetos
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::ExportaRedes(AnsiString arq_txt, TDateTime data)
	{
	//variáveis locais
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//inicia objeto Radial
		if (! radial->Inicia(redes)) return(false);
		//define Extern_id
		IniciaObjAsIdTipos();
		IniciaObjAsIdArranjos();
		IniciaObjAsIdCurvasTipicas();
		IniciaObjAsIdEqptos();
		//define código das Barras com suas coordenadas
		IniciaCodigoBarraComCoordenadas();
		//reinicia lines
		lines->Clear();
		//configura ExportaEqpto
		exporta_eqpto->Separador = Separador;
		exporta_eqpto->Data      = data;
		//insere Bloco de versão
		if ( ! MontaBlocoVersao(arq_txt, data)) return(false);
		//dados de Arranjos
		if ( ! MontaPlanilhaCondutor())    return(false);
		//dados das redes MT
		if ( ! MontaPlanilhaTrafoAT_MT())   return(false);
		if ( ! MontaPlanilhaCircuito_MT())  return(false);
		if ( ! MontaPlanilhaSegmento_MT())  return(false);
		if ( ! MontaPlanilhaRegulador_MT()) return(false);
		if ( ! MontaPlanilhaChave_MT())     return(false);
		if ( ! MontaPlanilhaCarga_MT())     return(false);
		if ( ! MontaPlanilhaGerador_MT())   return(false);
		//dados das redes BT
		if ( ! MontaPlanilhaTrafoMT_BT())  return(false);
		if ( ! MontaPlanilhaSegmento_BT()) return(false);
		if ( ! MontaPlanilhaChave_BT())    return(false);
		if ( ! MontaPlanilhaRamal_BT())    return(false);
		if ( ! MontaPlanilhaCarga_BT())    return(false);
		if ( ! MontaPlanilhaGerador_BT())  return(false);
		//dados de coordenadas: dados não previstos pela ANEEL
		MontaPlanilhaCoordenadas();
		//insere Bloco de fim de dados
      if ( ! MontaBlocoFimDados()) return(false);
      //salva dados no arquivo
      lines->SaveToFile(arq_txt);
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
         if (! ExportaUmaRede(rede)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }
	return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaANEEL::IniciaCodigoBarraComCoordenadas(void)
	{
	//variáveis locais
	AnsiString codigo;
	VTBarra    *barra;
	VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList      *lisBARRA = redes->LisBarra();

	for (int nb = 0; nb < lisBARRA->Count; nb++)
		{
		barra = (VTBarra*)lisBARRA->Items[nb];
		codigo.sprintf("%d_%d_%05d", barra->utm.x, barra->utm.y, nb+1);
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
void __fastcall TExportaANEEL::InsereHeader(char *header)
   {
   lines->Add("//" + AnsiString::StringOfChar('-', 80));
   lines->Add(header + AnsiString(Separador));
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
bool __fastcall TExportaANEEL::MontaPlanilhaCondutor(void)
	{
	//variáveis locais
	AnsiString codigo;
	TList     *lisARRANJO;
	VTArranjo *arranjo;

	try{//insere header
		InsereHeader("Condutor");
		//obtém lista de Arranjos em uso na rede
		lisARRANJO = LisArranjo();
		//redefine código dos Arranjos
		for (int n = 0; n < lisARRANJO->Count; n++)
			{
			arranjo = (VTArranjo*)lisARRANJO->Items[n];
			codigo.sprintf("ArranjoANEEL_%04d", n+1);
			arranjo->Codigo = codigo;
			}
		//loop p/ todos Arranjos
		for (int n = 0; n < lisARRANJO->Count; n++)
			{
			arranjo = (VTArranjo*)lisARRANJO->Items[n];
			lines->Add(exporta_eqpto->MontaArranjo(arranjo));
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCoordenadas(void)
	{
	//variáveis locais
	TList   *lisBARRA;
	TList   *lisREDE;
	VTBarra *barra;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("Coordenada");
		//loop p/ todas Barras
		lisBARRA = redes->LisBarra();
		for (int n = 0; n < lisBARRA->Count; n++)
			{
			barra = (VTBarra*)lisBARRA->Items[n];
			//insere linha com dados de coordenadas
			lines->Add(exporta_eqpto->MontaCoordenada(barra));
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaPlanilhaCarga_BT(void)
	{
	//variáveis locais
	TList   *lisCARGA;
	TList   *lisREDE;
	VTCarga *carga;
	VTRede  *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("CargaBT");
		//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeSEC) continue;
			//obtém lista de Cargas da Rede
			lisCARGA = LisEqpto(rede, eqptoCARGA);
			//loop p/ todas Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				//descarta Cargas de IP
				//if (carga->classe->Tag == consIP) continue;
				//insere linha com dados da Carga
				lines->Add(exporta_eqpto->MontaCargaBT(rede, carga));
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
	TList   *lisCARGA;
	TList   *lisREDE;
	VTCarga *carga;
	VTRede  *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("CargaMT");
		//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Cargas da Rede
			lisCARGA = LisEqpto(rede, eqptoCARGA);
			//loop p/ todas Cargas
			for (int n = 0; n < lisCARGA->Count; n++)
				{
				carga = (VTCarga*)lisCARGA->Items[n];
				//insere linha com dados da Carga
				lines->Add(exporta_eqpto->MontaCargaMT(rede, carga));
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
	TList   *lisCHAVE;
	TList   *lisREDE;
	VTChave *chave;
	VTRede  *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("ChaveBT");
		//loop p/ todas Redes
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
				//insere linha com dados da Chave
				lines->Add(exporta_eqpto->MontaChaveBT(chave));
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
	TList   *lisCHAVE;
	TList   *lisREDE;
	VTChave *chave;
	VTRede  *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("ChaveMT");
		//loop p/ todas Redes
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
				//insere linha com dados da Chave
				lines->Add(exporta_eqpto->MontaChaveMT(chave));
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
	double       vnom_kv, vope_pu;
	TList        *lisREDE;
	VTPrimario   *primario;
	TList        *lisPRIMARIO = radial->LisPrimario();

	try{//insere header
		InsereHeader("Circuito");
		//loop p/ todos Primarios
		for (int n = 0; n < lisPRIMARIO->Count; n++)
			{
			primario = (VTPrimario*)lisPRIMARIO->Items[n];
			//proteção
			if (primario->Subestacao == NULL) continue;
			//determina tensão nominal do Primario
			if (! TensaoNominal(primario, vnom_kv)) continue;
			//determina tensão de operação do Primário
			if (! TensaoOperacao(primario, vope_pu)) continue;
			//insere linha com dados da Rede
			lines->Add(exporta_eqpto->MontaCircuitoMT(primario->Subestacao->Rede, primario->Rede, vnom_kv, vope_pu));
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
	TList     *lisGERADOR;
	TList     *lisREDE;
	VTGerador *gerador;
	VTRede    *rede;
	VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("GeradorBT");
		//loop p/ todas Redes
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
				//insere linha com dados do Gerador
				lines->Add(exporta_eqpto->MontaGeradorBT(rede, gerador));
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
	TList     *lisGERADOR;
	TList     *lisREDE;
	VTGerador *gerador;
	VTRede    *rede;
	VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("GeradorMT");
		//loop p/ todas Redes
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
				//insere linha com dados do Gerador
				lines->Add(exporta_eqpto->MontaGeradorMT(rede, gerador));
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
	TList       *lisTRECHO;
	TList       *lisREDE;
	VTTrecho    *trecho;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("RamalBT");
		//loop p/ todas Redes
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
					{//insere linha com dados do Ramal
					lines->Add(exporta_eqpto->MontaRamalBT(trecho));
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
	int         seq;
	TList       *lisREGULADOR;
	TList       *lisREDE;
	VTRegulador *regulador;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("ReguladorMT");
		//loop p/ todas Redes
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
				//verifica o tipo de ligacao
				switch(regulador->ligacao)
					{
					case lgMONOFASICO:   // 1 unidade monofásica
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, regulador->fases));
						break;
					case lgDELTA_ABERTO: //2 unidades monofásicas
						switch(regulador->fases & faseABC)
							{
							case faseA:
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB));
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseCA));
								break;
							case faseB:
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB));
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBC));
								break;
							case faseC:
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseBC));
								lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseCA));
								break;
							default:
								return(false);
							}
						break;
					case lgDELTA_FECHADO: //3 unidades monofásicas
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAB));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBC));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseCA));
						break;
					case lgEST_AT: //3 unidades monofásicas
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseAN));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseBN));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseCN));
						break;
					case lgEST_ISO:
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=1, faseA));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=2, faseB));
						lines->Add(exporta_eqpto->MontaReguladorMT(regulador, seq=3, faseC));
						break;
					default:
						return(false);
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
bool __fastcall TExportaANEEL::MontaPlanilhaSegmento_BT(void)
	{
	//variáveis locais
	TList       *lisTRECHO;
	TList       *lisREDE;
	VTTrecho    *trecho;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("SegmentoBT");
		//loop p/ todas Redes
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
				//verifica se Trecho não é um Ramal
				if (! trecho->RamalLigacao)
					{//insere linha com dados do Trecho
					lines->Add(exporta_eqpto->MontaSegmentoBT(trecho));
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
	TList       *lisTRECHO;
	TList       *lisREDE;
	VTTrecho    *trecho;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("SegmentoMT");
		//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede MT
			if (rede->TipoRede->Segmento != redePRI) continue;
			//obtém lista de Trechos da Rede
			lisTRECHO = LisEqpto(rede, eqptoTRECHO);
			//loop p/ todos Trechos
			for (int n = 0; n < lisTRECHO->Count; n++)
				{
				trecho = (VTTrecho*)lisTRECHO->Items[n];
				//insere linha com dados do Ramal
				lines->Add(exporta_eqpto->MontaSegmentoMT(trecho));
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
	TList       *lisTRAFO;
	TList       *lisREDE;
	VTTrafo     *trafo;
	VTRede      *rede;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//insere header
		InsereHeader("TrafoAT_MT");
		//loop p/ todas Redes
		lisREDE = redes->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede = (VTRede*)lisREDE->Items[nr];
			//verifica se é uma rede BT
			if (rede->TipoRede->Segmento != redeETD) continue;
			//obtém lista de Trafos da Rede
			lisTRAFO = LisEqpto(rede, eqptoTRAFO);
			//loop p/ todos Trafos
			for (int n = 0; n < lisTRAFO->Count; n++)
				{
				trafo = (VTTrafo*)lisTRAFO->Items[n];
				//insere linha com dados do Trafo
				lines->Add(exporta_eqpto->MontaTrafoAT_MT(rede, trafo, n+1));
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
	TList        *lisTRAFO;
	TList        *lisREDE;
	VTTrafo      *trafo;
	VTSecundario *secundario;
	TList        *lisSECUNDARIO = radial->LisSecundario();

	try{//insere header
		InsereHeader("TrafoMT_BT");
		//loop p/ todas Redes
		for (int nr = 0; nr < lisSECUNDARIO->Count; nr++)
			{
			secundario = (VTSecundario*)lisSECUNDARIO->Items[nr];
			//obtém lista de Trafos da Rede
			lisTRAFO = LisEqpto(secundario->Rede, eqptoTRAFO);
			//loop p/ todos Trafos
			for (int n = 0; n < lisTRAFO->Count; n++)
				{
				trafo = (VTTrafo*)lisTRAFO->Items[n];
				//insere linha com dados do Trafo
				lines->Add(exporta_eqpto->MontaTrafoMT_BT(secundario->Primario->Rede, trafo, n+1));
				}
			}
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::TensaoNominal(VTPrimario *primario, double &vnom_kv)
	{
	//variáveis locais
	VTBarra *barra;

	//assume tensão de 13.8 kV
	vnom_kv = 13.8;
	if ((barra = primario->BarraInicial) == NULL) return(false);
	vnom_kv = barra->vnom;
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::TensaoOperacao(VTPrimario *primario, double &vope_pu)
	{
	//variáveis locais
	VTSuprimento *suprimento;
	VTCurva      *curva;

	//assume tensão de 1 pu
	vope_pu = 1.0;
	//determina Suprimento da Subestacao
	lisEQP->Clear();
	primario->Subestacao->Rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
	if (lisEQP->Count == 0) return(false);
	suprimento = (VTSuprimento*)lisEQP->First();
	//determina tensão de operação média do Suprimento
	vope_pu = suprimento->curva->Media(indV);

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
			lines->Add(exporta_eqpto->MontaBarra((VTBarra*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaCapacitor((VTCapacitor*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaCapserie((VTCapserie*)lisEQP->Items[n]));
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
			lines->Add(exporta_eqpto->MontaCargaIP((VTCarga*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaCargaComMedicao((VTCarga*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaCargaSemMedicao((VTCarga*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaChave((VTChave*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaFiltro((VTFiltro*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaGerador((VTGerador*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaMutua((VTMutua*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaReator((VTReator*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaRede((VTRede*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaReguladorAutomatico((VTRegulador*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaReguladorTapFixo((VTRegulador*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaSuprimento((VTSuprimento*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaTrafoTrifasico2E((VTTrafo*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaTrafoTrifasico3E((VTTrafo3E*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaTrafoMonofasico((VTTrafo*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaTrafoZZ((VTTrafoZZ*)lisEQP->Items[n]));
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
			lines->Add(exporta_eqpto->MontaTrechoZ0Z1((VTTrecho*)lisEQP->Items[n]));
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
         lines->Add(exporta_eqpto->MontaTrechoArranjoTipico((VTTrecho*)lisEQP->Items[n]));
         }
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoFimDados(void)
   {
   //insere header
   InsereHeader("END");

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaANEEL::MontaBlocoVersao(AnsiString arq_txt, TDateTime data)
	{
	//insere header
	InsereHeader("Revisao");
	lines->Add(exporta_eqpto->MontaVersao());
	//insere nome do arquivo e data dos dados como comentários
	lines->Add("//Arquivo:      " + ExtractFileName(arq_txt));
	lines->Add("//Data da rede: " + DateToStr(data));

	return(true);
	}

//-----------------------------------------------------------------------------
// eof


