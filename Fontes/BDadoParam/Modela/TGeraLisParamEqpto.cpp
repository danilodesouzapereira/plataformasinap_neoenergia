//---------------------------------------------------------------------------
#include <algorithm>
#pragma hdrstop
#include "TGeraLisParamEqpto.h"
#include "VTBarraOc.h"

//#include <ProjetoEDP\Fontes\ExtratorDados\Dados\VTOcorr.h>
//#include <ProjetoEDP\Fontes\ExtratorDados\Dados\VTEqptoOc.h>
//#include <ProjetoEDP\Fontes\ExtratorDados\Dados\VTCircuito.h>

//hk201612: compatibilização extrator geral #include "VTOcorr.h"
//hk201808: nao utilizado: #include "VTOcorrGeral.h"
//hk201705 : alterações provisórias para alocação IF
#include <ModulosOpcionais\Fontes\ExtratorOcorrencias\Dados\VTOcorr.h>
#include "VTEqptoOc.h"
#include "VTCircuito.h"

#include "..\Dados\VTParamConf.h" //hk201808: indisp
#include "..\..\CalcIndCont\VTBuscaProt.h"
#include "..\..\CalcIndCont\VTBlocoRad.h"
#include "..\..\CalcIndCont\VTCalcIndCont.h"
#include "..\..\CalcIndCont\VTVisRede.h"

#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>

#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "..\..\..\DLL_Inc\CalcIndCont.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//COMENTÁRIOS
//hk201607 Virou função externa, acrescimo de flag : PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg = false)
//hk201607 Virou função externa, acrescimo de flag : PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg = false)
//hk201608: ((VTChave*)paramBloco->eqpto)->rede->Codigo; : chave->rede->Codigo;//
//hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
//hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede: bool flagEliminaReg = true;
//hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg)
//hk201610: ajustes de paramCarga em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg)
//hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		//if(paramBloco->nFalhas < 0) //retira paramBloco nao mapeado por lisParamEqpto
		//  {
		//  lisParamBloco->Delete(np);
		//  delete paramBloco;
		//  continue;
		//  }
//hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : paramCarga->codRede = paramEqpto->rede->Codigo;

//hk201611: mudanca dos parametros void __fastcall TGeraLisParamEqpto::CalculaDadosParamRede(VTParamConf *paramConf)//(TList *lisParamEqpto, TList *lisParamRede)
//para: void __fastcall TGeraLisParamEqpto::CalculaDadosParamRede(TList *lisParamEqpto, TList *lisParamRede, strDadoParamGerais *paramGeral)//(TList *lisParamEqpto, TList *lisParamRede)

//hk201703: adaptações para planTurmas: acréscimo de fator tma
//hk201703: Correção do tempo de atendimento: acréscimo de paramEqpto->tat: apresentação no módulo de gerenciamento de ocorrencias
//pedro20170425- correção da contagem do número de dias
//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
//---------------------------------------------------------------------------
VTGeraLisParamEqpto* __fastcall NewObjGeraLisParamEqpto(VTApl *apl_owner)
	{
	try{//cria objeto
		return(new TGeraLisParamEqpto(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}
//---------------------------------------------------------------------------
__fastcall TGeraLisParamEqpto::TGeraLisParamEqpto(VTApl *apl_owner)
	{
	//salva ponteiro dos objetos
	this->apl = NewObjApl(NULL, apl_owner);


	lisBarraOc = new TList();
	//for(int n = 0; n < 24; n++) distFalhas[n] = 0.0;


	lisEqpto = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TGeraLisParamEqpto::~TGeraLisParamEqpto(void)
	{
	if(apl) {delete apl; apl = NULL;}
	if(lisBarraOc) {LimpaTList(lisBarraOc); delete lisBarraOc; lisBarraOc = NULL;}
	if(lisEqpto) {delete lisEqpto; lisEqpto = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::CalculaConsBlocoEJusante(TList *lisParamBlocoExterno)
{
	//variaveis locais
	strDadoParamBloco *paramBloco;
	VTBlocoRad *blocoRad;
	VTBloco *bloco;
	TList *lisCargas;

	//cria lista
	lisCargas = new TList();
	for (int nb = 0; nb < lisParamBlocoExterno->Count; nb++)
	{
		paramBloco = (strDadoParamBloco*)lisParamBlocoExterno->Items[nb];
		//determina ncons do bloco
		lisCargas->Clear();
		paramBloco->bloco->LisEqbar(lisCargas, eqptoCARGA);
		paramBloco->nconsBloco = DeterminaQtdClientes(lisCargas);
		//inicia nconsJus do paramBloco
		paramBloco->nconsJus = 0;
		//determina blocos a jusante
		blocoRad = buscaProt->IdentificaBlocoRad(paramBloco->bloco);
		for (int nbl = 0; nbl < blocoRad->LisBlocoJus->Count; nbl++)
		{
			bloco = (VTBloco*)blocoRad->LisBlocoJus->Items[nbl];
			lisCargas->Clear();
			bloco->LisEqbar(lisCargas, eqptoCARGA);
			//soma consumidores para blocos a jusante
			paramBloco->nconsJus += DeterminaQtdClientes(lisCargas);
		}
	}
	//destroi lista
	delete lisCargas;
}

// -----------------------------------------------------------------------------
int __fastcall TGeraLisParamEqpto::DeterminaQtdClientes(TList *lisCARGA)
{
	// variáveis locais
	int numClientes = 0;
	VTCarga *carga;

	// determina cargas
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{ // soma consumidores total de cada carga
		carga = (VTCarga*)lisCARGA->Items[nc];
		numClientes += carga->NumConsTotal;
	}
	return (numClientes);
}

// ---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaParamConfRede(VTParamConf *paramConf, VTBlocos *blocos)
	{   //chamada de calcIndCont->LeArqMdb(AnsiString arqParam)
		 //apos        bdadoParam->LeDado(paramConf);
	bool flagBuscaProt = false;

	buscaProt = NULL;
	if(blocos != NULL)
	  {
	  apl->Add(blocos); //apl filho de aplOwner
	  //Cria objeto buscaprot com blocos externos
	  buscaProt = DLL_NewBuscaProt(apl, true);
	  flagBuscaProt = true;
	  //Inicia buscaProt Ordenacao de chaves e identifica chaves VIS
	  if(!buscaProt->Inicia()) //ver se na nova inicialização faz corretamente
		  {Aviso("Problemas na inicialização: verificar dados de entrada");//OrdenaChaves();
			return;
		  }
	  }
	else
	  buscaProt  = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));

	//1.PARAMCIRC atribui ponteiro de rede
	//paramRede->rede, paramRede->compRede
	PreparaLisParamRede(paramConf->LisParamRede);
	//2.atribui ponteiros a lisParamEqpto, eqpto/ carga e gera distribuição de falhas
	//e gera paramEqpto->nfalhasEquip, durTotal
	//                 ->nfEquip[nm], durTot[nm]
   //e paramConf->DistFalhasPat[nh]
	PreparaLisParamEqptoRede(paramConf->LisParamEqpto, paramConf->DistFalhasPat);
	//3.Redistribui falhas para somar 1.0, soma distFalhasPat[0->23] = 1.0
	RedistribuiFalhasHora(paramConf->DistFalhasPat);
	//4.Atribui ponteiro paramBloco->eqpto e paramBloco->bloco, paramBloco->compBloco
	bool flagEliminaReg = true;  		//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede: bool flagEliminaReg = true;
	PreparaLisParamBlocoRede(paramConf->LisParamBloco, flagEliminaReg); //flagEliminaReg: se elimina parambloco não encontrados
	//5.Atribui ponteiro paramCarga->rede (MT), paramCarga->carga  = carga e paramCarga->nconsJus = NCons(carga);
	PreparaLisParamCargaRede(paramConf->LisParamCarga, flagEliminaReg); //flagEliminaReg: se elimina paramCarga não encontrados
	//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede

	//6.PARAMCHAVESTELECOM atribui ponteiro chvTelecom->chave de chave existente na rede
	PreparaLisChavesTelecom(paramConf->LisChavesTelecom);
	//7.TIPOEQPROT atribui ponteiros dadoTipoEqProt->tipoChave de tipos de chave
	PreparaLisTipoEqProt(paramConf->LisTipoEqProt);
   //8.equipamentos IF atribui ponteiro de chave associada a IF hk201808: reformulação
	PreparaLisChavesIF(paramConf->LisChavesIF);

   //9.indisponibilidade por equipamento: hk201808: indisp
	PreparaEqptoIndisp(paramConf->LisEqptoIndisp);

	if(flagBuscaProt)
	  {
	  apl->Remove(blocos);
	  delete buscaProt; buscaProt = NULL;
	  }
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisTipoEqProt(TList *lisTipoEqProtExterno)
	{
	VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList   *lisTipoChave = tipos->LisTipoChave();
	VTTipoChave  *tipoChave;
	strDadoTipoEqProt *dadoTipoEqProt;
	AnsiString codAux;


	for(int n = 0; n < lisTipoEqProtExterno->Count; n++)
		{
		dadoTipoEqProt = (strDadoTipoEqProt*)lisTipoEqProtExterno->Items[n];
		//Busca tipoChave com codigo = codTipoChave
		tipoChave = NULL;
		for(int n = 0; n < lisTipoChave->Count; n++)
			 {
			 tipoChave = (VTTipoChave*)lisTipoChave->Items[n];
			 codAux = tipoChave->Codigo;
			 if(codAux.AnsiCompare(dadoTipoEqProt->codTipoEqProt) == 0) break;
			 tipoChave = NULL;
			 }
		dadoTipoEqProt->tipoChave = tipoChave;
		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisChavesTelecom(TList *lisChavesTelecomExterno)
	{
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	strDadoChaveTelecom *chvTelecom;
	TList  *lisChaves;
	VTChave *chave;
	//Inicia buscaProt->LisChvAuto e cria lista de chaves clone

	//Inicia lista com chaves automáticas e telecomandadas buscaProt->LisChvAuto
	//ReiniciaChavesAuto();//IniciaLisChavesAuto();

	lisChaves = new TList();
	redes->LisEqpto(lisChaves, eqptoCHAVE);
	for(int n = 0; n < lisChavesTelecomExterno->Count; n++)
		{
		chvTelecom = (strDadoChaveTelecom*)lisChavesTelecomExterno->Items[n];
		if((chvTelecom->chave = BuscaChave(chvTelecom->codChave, lisChaves)) == NULL) continue;


		//----------Insere chave em lisChavesAuto--------------
		//-e altera o tipo da chave para chaveRA, religadora---
		//e cria chave clone para retorno a configuração original
		//InsereChaveAuto(chave);
		}

	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisChavesIF(TList *lisChavesIFExterno)
	{ //hk201808: reformulação
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	TList  *lisChaves;
	VTChave *chave;
	strDadoChaveIF *chaveIF;

	lisChaves = new TList();
	redes->LisEqpto(lisChaves, eqptoCHAVE);
	for(int n = 0; n < lisChavesIFExterno->Count; n++)
		{
		chaveIF = (strDadoChaveIF*)lisChavesIFExterno->Items[n];
		chaveIF->chave = BuscaChave(chaveIF->codChave, lisChaves);
		}
   delete lisChaves;
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaEqptoIndisp(TList *lisEqptoIndisp)
	{ //hk201808: indisp
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	TList  *lisEqpto;
	strDadoEqptoIndisp *dadoEqptoIndisp;

	lisEqpto = new TList();
	redes->LisEqpto(lisEqpto, eqptoCHAVE);
	for(int n = 0; n < lisEqptoIndisp->Count; n++)
		{
		dadoEqptoIndisp = (strDadoEqptoIndisp*)lisEqptoIndisp->Items[n];
      dadoEqptoIndisp->eqpto = BuscaChave(dadoEqptoIndisp->cod_eqpto, lisEqpto);
		}

   lisEqpto->Clear();
   redes->LisEqpto(lisEqpto, eqptoBARRA);
   VTBarra *barra;
	for(int n = 0; n < lisEqptoIndisp->Count; n++)
		{
		dadoEqptoIndisp = (strDadoEqptoIndisp*)lisEqptoIndisp->Items[n];
      if(dadoEqptoIndisp->eqpto != NULL) continue;
      barra = NULL;
      for(int neq = 0; neq < lisEqpto->Count; neq++)
         {
         barra = (VTBarra*)lisEqpto->Items[neq];
         if(barra->Codigo.AnsiCompare(dadoEqptoIndisp->cod_eqpto) == 0)
            {
            dadoEqptoIndisp->eqpto = barra;
            break;
            }
         }

      }
   delete lisEqpto;
	}
//---------------------------------------------------------------------------
VTChave* __fastcall TGeraLisParamEqpto::BuscaChave(AnsiString codChave, TList *lisChave)
	{
	VTChave *chave;
	chave = NULL;
	for(int n = 0; n < lisChave->Count; n++)
		{
		chave = (VTChave*)lisChave->Items[n];
		if(chave->Codigo.AnsiCompare(codChave) == 0) break;
		chave = NULL;
		}
	return(chave);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg)  //hkmuda1
	{
	//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg)
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	strDadoParamBloco *paramBloco;
	TList             *lisChaves;



	//protecao
	if(buscaProt == NULL) return;

	lisChaves   = new TList();
	redes->LisEqpto(lisChaves, eqptoCHAVE);

	for(int nb = lisParamBlocoExterno->Count - 1; nb >= 0; nb--)
		{
		paramBloco = (strDadoParamBloco*)lisParamBlocoExterno->Items[nb];
      //hk201802a
      //paramBloco->codRede;
      //paramBloco->codChave;
      //paramBloco->codBarra;
		paramBloco->eqpto = BuscaChave(paramBloco->codChave, lisChaves);


		if(paramBloco->eqpto == NULL)
		  {
		  if(flagEliminaReg == true)
			 {
			 lisParamBlocoExterno->Delete(nb);
			 delete paramBloco;
			 continue;
			 }
		  paramBloco->bloco     = NULL;
		  paramBloco->compBloco = 0.0;
		  }
		else
		  {
        //hk201802a: identificação da barra, paramBloco->eqptoBarra
        VTBarra *barra1, *barra2;
        barra1 = ((VTChave*)paramBloco->eqpto)->pbarra1;
        barra2 = ((VTChave*)paramBloco->eqpto)->pbarra2;
        paramBloco->eqptoBarra = NULL;
        if(barra1->Codigo.AnsiCompare(paramBloco->codBarra) == 0)
            paramBloco->eqptoBarra = barra1;
        if(barra2->Codigo.AnsiCompare(paramBloco->codBarra) == 0)
            paramBloco->eqptoBarra = barra2;
                                     //Verif201701
        //hk201802a: identificação do bloco baseado em chave e barra
        if(paramBloco->eqptoBarra == NULL)
		     paramBloco->bloco     = buscaProt->IdentificaBloco(paramBloco->eqpto);
        else
           {
           if(((VTChave*)paramBloco->eqpto)->Estado == chvFECHADA)
             {
             VTBlocoRad *blocoRad;
             blocoRad = buscaProt->IdentificaBlocoRad(paramBloco->eqpto);
				 //gh201809: proteção
				 if (blocoRad == NULL)
					{
					continue;
					}
             if(blocoRad->BarraChave == paramBloco->eqptoBarra)
               paramBloco->bloco = blocoRad->Bloco;
             else
               paramBloco->bloco = blocoRad->BlocoRadMon->Bloco;
             }
           else
             {
             VTVisRede *visRede;
             visRede = buscaProt->IdentificaVisRede((VTChave*)paramBloco->eqpto);
             //gh201808: proteção para blocos definidos por chaves NA
				 if (visRede == NULL)
					{
					continue;
				 	}
             if(visRede->pbarra1 == paramBloco->eqptoBarra)
               paramBloco->bloco = visRede->bloco1;
             else
               paramBloco->bloco = visRede->bloco2;
             }
           }
		  paramBloco->codRede   = ((VTChave*)paramBloco->eqpto)->rede->Codigo;//hkv201608: ((VTChave*)paramBloco->eqpto)->rede->Codigo; : chave->rede->Codigo;//
		  paramBloco->compBloco = ComprimentoBloco(paramBloco->bloco);
		  }
		//Identifica blocos formados por chaves novas que dividem paramBloco->bloco
		//ppppppaaaaaarrrrroooooouuuu
		}
	/*
	for(int nb = lisParamBlocoExterno->Count - 1; nb >= 0; nb--)
		{
		paramBloco = (strDadoParamBloco*)lisParamBlocoExterno->Items[nb];
		if(((chave = BuscaChave(paramBloco->codChave, lisChaves)) == NULL)&&(flagEliminaReg ==true))//hkv201608 acréscimo de flag
		  {
		  lisParamBlocoExterno->Delete(nb);
		  delete paramBloco;
		  continue;
		  }
		paramBloco->eqpto     = chave;
		//Identifica bloco alimentado pela chave
		if(chave == NULL)
		  {paramBloco->bloco     = NULL;
			paramBloco->compBloco = 0.0;
		  }
		else //hkv201608-bug chave = NULL
		  {
		  paramBloco->bloco     = buscaProt->IdentificaBloco(chave);
		  paramBloco->codRede   = chave->rede->Codigo;//hkv201608
		  paramBloco->compBloco = ComprimentoBloco(paramBloco->bloco);
		  }
		//Identifica blocos formados por chaves novas que dividem paramBloco->bloco
		//ppppppaaaaaarrrrroooooouuuu
		}

	*/
	delete lisChaves;
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg)  //hkmuda1
	{
	//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg)
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList   *lisCargas, *lisAux;
	strDadoParamCarga *paramCarga;

	lisCargas = new TList();
	lisAux    = new TList();
	redes->LisEqbar(lisCargas, eqptoCARGA);


	for(int nc = lisParamCargaExterno->Count - 1; nc >= 0; nc--)
		{
		paramCarga = (strDadoParamCarga*)lisParamCargaExterno->Items[nc];
		paramCarga->carga  = BuscaCarga(paramCarga->codCarga, lisCargas);

		if(paramCarga->carga == NULL)
		  {
		  if(flagEliminaReg == true)
			 {
			 lisParamCargaExterno->Delete(nc);
			 delete paramCarga;
			 continue;
			 }
		  paramCarga->rede    = NULL;
		  paramCarga->carga   = NULL;
		  paramCarga->nconsJus= 0;
		  }
		else
		  {
		  paramCarga->rede   = BuscaRedeMT_CargaBT(paramCarga->carga, lisAux);
		  if(paramCarga->rede != NULL) paramCarga->codRede = paramCarga->rede->Codigo;
		  paramCarga->nconsJus = NCons(paramCarga->carga);
		  }
		}

	/*
	for(int nc = lisParamCargaExterno->Count - 1; nc >= 0; nc--)
		{
		paramCarga = (strDadoParamCarga*)lisParamCargaExterno->Items[nc];
		if(((carga = BuscaCarga(paramCarga->codCarga, lisCargas)) == NULL)&&(flagEliminaReg ==true))//hk201608 acréscimo de flag
		  {
		  lisParamCargaExterno->Delete(nc);
		  delete paramCarga;
		  continue;
		  }
		if(carga == NULL)
		  {
		  paramCarga->rede    = NULL;
		  paramCarga->carga   = NULL;
		  paramCarga->nconsJus= 0;
		  }
		else  //hk201608-bug carga = NULL
		  {
		  paramCarga->rede   = BuscaRedeMT_CargaBT(carga, lisAux);
		  paramCarga->carga  = carga;
		  paramCarga->nconsJus = NCons(carga);
		  }
		}
	*/
	delete lisCargas;
	delete lisAux;
	}
//---------------------------------------------------------------------------
int __fastcall TGeraLisParamEqpto::NCons(VTCarga *carga)
	{
	int naux  = 0;

	naux += carga->NumConsResidencial;
	naux += carga->NumConsComercial;
	naux += carga->NumConsIndustrial;
	naux += carga->NumConsRural;
	naux += carga->NumConsOutros; //MISTURADO COM IP!!!!
	naux += carga->NumConsA4;
	//Valor mínimo de consumidores
	if(naux == 0) naux = 1;

	return(naux);
	}
//---------------------------------------------------------------------------
VTCarga* __fastcall TGeraLisParamEqpto::BuscaCarga(AnsiString codCarga, TList *lisCarga)
	{
	VTCarga *carga;
	carga = NULL;
	for(int n = 0; n < lisCarga->Count; n++)
		{
		carga = (VTCarga*)lisCarga->Items[n];
		if(carga->Codigo.AnsiCompare(codCarga) == 0) break;
		carga = NULL;
		}
	return(carga);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::RedistribuiFalhasHora(double *distFalhasPat) //ssai
	{
	double dTot = 0.0;
	int    numPat = 24;

	for(int np = 0; np < numPat; np++)
		dTot += distFalhasPat[np];

	if(IsDoubleZero(dTot))
	  {
	  for(int np = 0; np < numPat; np++)
			distFalhasPat[np] = 1.0 / numPat;
	  }
	else
	  {
	  for(int np = 0; np < numPat; np++)
			distFalhasPat[np] = distFalhasPat[np] / dTot;
	  }
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::PreparaLisParamRede(TList *lisParamRede)
	{
	strDadoParamRede *paramRede;
	VTRede           *rede;

	for(int n = 0; n < lisParamRede->Count; n++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		//Busca rede entre redes carregadas
		rede = BuscaRede(paramRede->codRede);
		//	{
		//	lisParamRede->Delete(n);
		//	delete paramRede;
		//	}
		paramRede->rede = rede;
		paramRede->compRede = 0.0;
		if(rede != NULL) paramRede->compRede = ComprimentoRede(rede);
		}

	}
//------------------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18006360
void __fastcall TGeraLisParamEqpto::PreparaLisParamEqptoRede(TList *lisParamEqptoExterno, double *distFalhas){
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	TList     *lisCirc;
	strDadoParamEqpto  *paramEqpto;
	VTEqpto            *eqpto;
	TList              *lisAux, *lisCargas;
	double             nconsBT;
	VTCarga            *carga;

	//Elimina elementos de lisParamEqpto nao encontrados
	//e gera as informações históricas e de cadastro para a simulação
		//Cadastro
		//paramEqpto->tipoRede = BT(carga !=NULL) ou MT(eqpto
		//eqpto =  BuscaEqpto(paramEqpto->codEqpto, paramEqpto->codRede);
		//eqpto = NULL --> delete lisParamEqpto->paramEqpto
		//eqpto !=NULL -->
		//paramEqpto->eqpto  =  (eqpto->tipo = eqptoCARGA) ? NULL : eqpto
		//paramEqpto->carga  =  (eqpto->tipo = eqptoCARGA) ? (VTCarga*)eqpto : NULL
		//paramEqpto->rede   =  (eqpto->tipo = eqptoCARGA) ? BuscaRedeMT_CargaBT((VTCarga*)eqpto, lisAux) : ((VTChave*)eqpto)->rede
		//paramEqpto->nconsJus =  (eqptoCARGA)  ? NCons(paramEqpto->carga) : 0.0
		//paramEqpto->carga->Obj  =  (eqptoCARGA)  ? paramEqpto: NULL
		//paramEqpto->codRede

		//Estatisticas de falhas e tempos
		//paramEqpto->nFalhasEquip = paramEqpto->nf + paramEqpto->nfr / (nconsBT = paramEqpto->nconsJus)
		//paramEqpto->nFEquip[nm] = paramEqpto->nfmes[nm] + paramEqpto->nfrmes[nm] / (nconsBT  = paramEqpto->nconsJus)
		//paramEqpto->durTotal = paramEqpto->td * paramEqpto->nFalhasEquip;
		//paramEqpto->durTot[nm] = paramEqpto->tdmes[nm] * paramEqpto->nFEquip[nm];

		//distribuição de falhas por hora
		//distFalhas[np] += paramEqpto->nfhora[np]

		//Resumo:
		//paramEqpto->nFalhasEquip, paramEqpto->nFEquip[nm]
		//paramEqpto->durTotal    , paramEqpto->durTot[nm]
		//e gera distribuicao falhas/ hora: distFalhas[np]

	lisCargas = new TList();
	redes->LisEqbar(lisCargas, eqptoCARGA);
	for(int nc = 0; nc < lisCargas->Count; nc++){
		carga = (VTCarga*)lisCargas->Items[nc];
		carga->Obj = NULL;
	}
	delete lisCargas;

	lisAux = new TList();
	//Inicia distribuição de falhas
	if(distFalhas != NULL) {
	  for(int np = 0; np < 24; np++)
	  	distFalhas[np] = 0.0;
  }

	for(int nb = lisParamEqptoExterno->Count - 1; nb >= 0; nb--){
		paramEqpto = (strDadoParamEqpto*)lisParamEqptoExterno->Items[nb];
		//Busca ponteiro eqpto baseado na rede
		eqpto = BuscaEqpto(paramEqpto->codEqpto, paramEqpto->codRede);
		if(eqpto == NULL){
			lisParamEqptoExterno->Delete(nb); //será ? Acho que sim por que depois não
			delete paramEqpto;                //       será gravado em paramConf.mdb
			continue;
		}
		//Verifica se existe outro paramEqpto com ponteiro para mesmo eqpto; if(paramEqpto->carga == eqptoOcorr) OU if(paramEqpto->eqpto == eqptoOcorr)
		if(BuscaParamEqpto(eqpto, lisParamEqptoExterno) != NULL){
			lisParamEqptoExterno->Delete(nb);
			delete paramEqpto;
			continue;
		}

		if(eqpto->Tipo() == eqptoCARGA){
			//Características de paramEqpto
			paramEqpto->tipoRede  = "BT";
			paramEqpto->eqpto = NULL;
			paramEqpto->carga = (VTCarga*)eqpto;

			paramEqpto->rede = BuscaRedeMT_CargaBT((VTCarga*)eqpto, lisAux);  //Verif201701
			//Calcula número de consumidores BT hknova
			paramEqpto->nconsJus = nconsBT = NCons(paramEqpto->carga);
			paramEqpto->carga->Obj = paramEqpto;
		} else {
			nconsBT = 0;
			paramEqpto->tipoRede  = "MT";
			paramEqpto->eqpto = eqpto;
			paramEqpto->carga = NULL; //Verif201701: caso trafo
			paramEqpto->rede  = ((VTChave*)eqpto)->rede;
			paramEqpto->nconsJus = 0;
		}

//		if(paramEqpto->codRede.Trim().AnsiCompare(paramEqpto->rede->Codigo) != 0) {
//			paramEqpto->codRede = paramEqpto->rede->Codigo;
//			//lisParamEqpto->Delete(nb);
//			//delete paramEqpto;
//			//continue;
//		}
        // proteção contra equipamentos em barras isoladas (sem rede)
        if(paramEqpto->rede)
            paramEqpto->codRede = paramEqpto->rede->Codigo;

		paramEqpto->nFalhasEquip = paramEqpto->nf;//hhhparamEqpto->nFalhasNRamal;

		for(int nm = 0; nm < 12; nm++)
			paramEqpto->nFEquip[nm] = paramEqpto->nfmes[nm];

		if(nconsBT > 0) {
			paramEqpto->nFalhasEquip += (paramEqpto->nfr / nconsBT);//hhh (paramEqpto->nFalhasRamal / nconsBT);
			for(int nm = 0; nm < 12; nm++)
				paramEqpto->nFEquip[nm] += (paramEqpto->nfrmes[nm] / nconsBT);
		}

		paramEqpto->durTotal = paramEqpto->td * paramEqpto->nFalhasEquip;//hhh paramEqpto->tdeslMedio * paramEqpto->nFalhasEquip;
		for(int nm = 0; nm < 12; nm++)
			paramEqpto->durTot[nm] = paramEqpto->tdmes[nm] * paramEqpto->nFEquip[nm];

		//Acrescentar falhas a distribuição de falhas por hora
		if(distFalhas != NULL) {
			for(int np = 0; np < 24; np++)
				distFalhas[np] += paramEqpto->nfhora[np];
		}
	}
	delete lisAux;

	//Arredondamento de dados
	//ParamEqptoArredonda(lisParamEqptoExterno, 1000.0);
	//ImprimeLisParamEqpto("lisParamEqpto222.txt");
}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ParamEqptoArredonda(TList *lisParamEqptoExterno, double iprec)
	{
	strDadoParamEqpto *paramEqpto;

	for(int n = 0; n < lisParamEqptoExterno->Count; n++)
		{
		paramEqpto = (strDadoParamEqpto*)lisParamEqptoExterno->Items[n];
		paramEqpto->nFalhasEquip = ((int)(paramEqpto->nFalhasEquip*iprec)) / iprec;
		paramEqpto->durTotal     = ((int)(paramEqpto->durTotal*iprec)) / iprec;
		for(int nm = 0; nm < 12; nm++)
			{
			paramEqpto->nFEquip[nm] = ((int)(paramEqpto->nFEquip[nm]*iprec)) / iprec;
			paramEqpto->durTot[nm]  = ((int)(paramEqpto->durTot[nm]*iprec)) / iprec;
			}
		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ReiniciaLisParamBlocoRede(TList       *lisParamEqptoEntrada,
                                                              TList       *lisTipoEqProt,
                                                              TList       *lisParamBlocoExterno,
                                                              TList       *lisParamCargaExterno,
                                                              VTBlocos    *blocos)
	{
	bool flagBuscaProt = false;

	buscaProt = NULL;
	if(blocos != NULL)
	  {
	  apl->Add(blocos);
	  //Cria objeto buscaprot com blocos externos
	  buscaProt = DLL_NewBuscaProt(apl, true);
	  flagBuscaProt = true;
	  //Inicia buscaProt Ordenacao de chaves e identifica chaves VIS
	  if(!buscaProt->Inicia(false)) //ver se na nova inicialização faz corretamente Inicia(bool flagExeBlocos = true)
		  {Aviso("Problemas na inicialização: verificar dados de entrada");//OrdenaChaves();
			return;
		  }
	  }
	else
	  buscaProt  = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));

	buscaProt->LisTipoEqProt->Clear();
	buscaProt->LisTipoEqProt->Assign(lisTipoEqProt, laCopy);

	//PreparaLisParamEqptoRede(lisParamEqptoEntrada, NULL);

	//Gera lisParamEqpto->paramEqpto->(lambda, trep) = f(paramEqpto->nFalhasEquip, durTotal, ndiasObs)
	//Calcula
	//paramEqpto->lambda, trep
	//em funcao de:
	  //paramEqpto->eqpto
	  //paramEqpto->carga
	  //paramEqpto->durTotal
	  //paramEqpto->nFalhasEquip
	  //paramEqpto->ndiasObs
	ReiniciaDadosParamEqpto(lisParamEqptoEntrada);
	//Reinicia lisParamBloco->paramBloco->(lambda, trep) e reconstroi lisParamCarga;
	//e lisParamCarga
	//Funcao de:
		//paramEqpto->eqpto
		//paramEqpto->rede
		//paramEqpto->lambda
		//paramEqpto->trep
	//Reconstroi lisParamCarga->strDadoParamCarga funcao de
	//	 paramCarga->rede  = paramEqpto->rede;
	//  paramCarga->carga = paramEqpto->carga;
	//	 paramCarga->lambda= paramEqpto->lambda;
	//	 paramCarga->trep  = paramEqpto->trep;
	//	 paramCarga->nconsJus = NCons(paramEqpto->carga);
	//Funcao de:
		//paramEqpto->carga
		//paramEqpto->rede
		//paramEqpto->lambda
		//paramEqpto->trep
	// GH - 04/05/2018 - correção para que no módulo de ocorrências o cálculo de taxas de falha permaneça não
	// considerando falhas transitórias
    RedistribuiFalhasCirc(lisParamEqptoEntrada,
                          lisParamBlocoExterno,
                          lisParamCargaExterno,
                          NULL,
                          buscaProt->IProfReliga);

	//calcula quantidade de consumidores do bloco e dos blocos a jusante
	CalculaConsBlocoEJusante(lisParamBlocoExterno);

	if(flagBuscaProt)
	  {
	  apl->Remove(blocos);
	  delete buscaProt; buscaProt = NULL;
	  }
	}

//---------------------------------------------------------------------------
//hk201611: mudanca dos parametros void __fastcall TGeraLisParamEqpto::CalculaDadosParamRede(VTParamConf *paramConf)//(TList *lisParamEqpto, TList *lisParamRede)
void __fastcall TGeraLisParamEqpto::CalculaDadosParamRede(TList *lisParamEqpto, TList *lisParamRede, strDadoParamGerais *paramGeral)//(TList *lisParamEqpto, TList *lisParamRede)
	{
	//hk201611 TList *lisParamRede  = paramConf->LisParamRede;
	//hk201611 TList *lisParamEqpto = paramConf->LisParamEqpto;
	//hk201611 strDadoParamGerais *paramGeral = paramConf->ParamGerais;
	strDadoParamEqpto  *paramEqpto;
	strDadoParamRede   *paramRede;
   double daux;

	//Calculo de paramRede e paramGeral
		//->nFalhasEquip
		//->durTotal
		//->ndiasObs
		//->nFEquip[nm]
		//->durTot[nm]
	//Funcao de lisParamEqpto->
		//paramEqpto->tipoRede
		//paramEqpto->rede
		//paramEqpto->nFalhasEquip;
		//paramEqpto->durTotal;
		//paramEqpto->ndiasObs;

	if(paramGeral != NULL)
	{
	//Inicia paramGeral
	paramGeral->nFalhasEquip = 0.0;
	paramGeral->durTotal     = 0.0;
   paramGeral->ptpre        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   paramGeral->ptdlo        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   paramGeral->ptexe        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
	paramGeral->ndiasObs     = 0;
	paramGeral->compRede     = 0.0;
	paramGeral->cli          = 0.0;
	paramGeral->clih         = 0.0;
	for(int nm = 0; nm < 12; nm++)
		{
		paramGeral->nFEquip[nm] = 0.0;
		paramGeral->durTot[nm]  = 0.0;
		paramGeral->climes[nm]  = 0.0;//hkmar
		paramGeral->clihmes[nm] = 0.0;
		}
	for(int nh = 0; nh < 24; nh++) paramGeral->nfhora[nh] = 0.0;
	}

	//Inicia lisParamRede->paramRede
	for(int n = 0; n < lisParamRede->Count; n++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		paramRede->nFalhasEquip = 0.0;
		paramRede->durTotal     = 0.0;
      paramRede->ptpre        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
      paramRede->ptdlo        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
      paramRede->ptexe        = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
		paramRede->ndiasObs     = 0;
		paramRede->cli          = 0.0;
		paramRede->clih         = 0.0;
		for(int nm = 0; nm < 12; nm++)
			{
			paramRede->nFEquip[nm] = 0.0;
			paramRede->durTot[nm]  = 0.0;
			paramRede->climes[nm]  = 0.0;
			paramRede->clihmes[nm] = 0.0;
			}
		}
	//Reinicializa nFalhasEquip, durTotal e ndiasObs
	//Considerando paramEqpto->tipoRede = MT
	//Considerando paramEqpto->rede != NULL
	for(int n = 0; n < lisParamEqpto->Count; n++)
		{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		if(paramEqpto->rede == NULL) continue;
		if((paramRede = ParametroConf(paramEqpto->rede, lisParamRede)) == NULL)
			  {lisParamRede->Add(paramRede = new strDadoParamRede);
				paramRede->codRede = paramEqpto->rede->Codigo;
				paramRede->rede    = paramEqpto->rede;
				paramRede->compRede = ComprimentoRede(paramEqpto->rede);
				paramRede->lambda = 0.0;
				paramRede->trep   = 0.0;
				paramRede->nFalhasEquip = 0.0;
				paramRede->durTotal     = 0.0;
            //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
            paramRede->ptpre        = 0.0;
            paramRede->ptdlo        = 0.0;
            paramRede->ptexe        = 0.0;
				paramRede->ndiasObs     = 0;
				paramRede->cli          = 0.0;
				paramRede->clih         = 0.0;
				for(int nm = 0; nm < 12; nm++) {paramRede->nFEquip[nm]  = 0.0;
														  paramRede->durTot[nm]   = 0.0;
														  paramRede->climes[nm]   = 0.0;
														  paramRede->clihmes[nm]  = 0.0;
														 }
			  }

		//hkfev if(paramEqpto->tipoRede != "MT") continue;
		if(paramEqpto->tipoRede == "MT")
		  {
		  paramRede->nFalhasEquip += paramEqpto->nFalhasEquip;
		  paramRede->durTotal     += paramEqpto->durTotal;
        paramRede->ptpre        += (paramEqpto->durTotal * paramEqpto->ptpre);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
        paramRede->ptdlo        += (paramEqpto->durTotal * paramEqpto->ptdlo);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
        paramRede->ptexe        += (paramEqpto->durTotal * paramEqpto->ptexe);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
		  paramRede->ndiasObs     =  paramEqpto->ndiasObs;
		  paramRede->cli          += paramEqpto->cli;
		  paramRede->clih         += paramEqpto->clih;
		  paramRede->qtdOcorr	 	= paramEqpto->qtdOcorr;

		  if(paramGeral != NULL)
			 {
			 paramGeral->nFalhasEquip += paramEqpto->nFalhasEquip;
			 paramGeral->durTotal     += paramEqpto->durTotal;
          paramGeral->ptpre        += (paramEqpto->durTotal * paramEqpto->ptpre);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          paramGeral->ptdlo        += (paramEqpto->durTotal * paramEqpto->ptdlo);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          paramGeral->ptexe        += (paramEqpto->durTotal * paramEqpto->ptexe);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
			 paramGeral->ndiasObs     =  paramEqpto->ndiasObs;
			 paramGeral->cli          += paramEqpto->cli;
			 paramGeral->clih         += paramEqpto->clih;
			 }

		  for(int nm = 0; nm < 12; nm++)
			{
			paramRede->nFEquip[nm]  += paramEqpto->nFEquip[nm];
			paramRede->durTot[nm]   += paramEqpto->durTot[nm];
			paramRede->climes[nm]   += paramEqpto->climes[nm];
			paramRede->clihmes[nm]  += paramEqpto->clihmes[nm];

			if(paramGeral != NULL)
			  {
			  paramGeral->nFEquip[nm] += paramEqpto->nFEquip[nm];
			  paramGeral->durTot[nm]  += paramEqpto->durTot[nm];
			  paramGeral->climes[nm]  += paramEqpto->climes[nm];
			  paramGeral->clihmes[nm] += paramEqpto->clihmes[nm];
			  }
			}
		  }
		if(paramGeral != NULL)
		{
		for(int nh = 0; nh < 24; nh++)
			{
			paramGeral->nfhora[nh] += paramEqpto->nfhora[nh];
			}
		}
		//else
		//{//para paramEqpto->tipoRede = "BT"
		//  paramRede->nFalhasEquipBT += paramEqpto->nFalhasEquip;
		//  paramRede->durTotalBT     += paramEqpto->durTotal;
		//  paramRede->ndiasObsBT      = paramEqpto->ndiasObs;
		//  paramGeral->nFalhasEquipBT+= paramEqpto->nFalhasEquip;
		//  paramGeral->durTotalBT    += paramEqpto->durTotal;
		//  paramGeral->ndiasObsBT     = paramEqpto->ndiasObs;
		//  for(int nm = 0; nm < 12; nm++)
		//	{
		//	paramRede->nFEquipBT[nm] += paramEqpto->nFEquip[nm];
		//	paramRede->durTotBT[nm]  += paramEqpto->durTot[nm];
		//	paramGeral->nFEquipBT[nm] += paramEqpto->nFEquip[nm];
		//	paramGeral->durTotBT[nm]  += paramEqpto->durTot[nm];
		//	}
		//}
		}



	//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
	for(int n = 0; n < lisParamRede->Count; n++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
      daux = paramRede->ptpre + paramRede->ptdlo + paramRede->ptexe;
      if(IsDoubleZero(daux)) continue;
      paramRede->ptpre = paramRede->ptpre/daux;
      paramRede->ptdlo = paramRede->ptdlo/daux;
      paramRede->ptexe = paramRede->ptexe/daux;
      }


	if(paramGeral != NULL)
	  {
	  for(int n = 0; n < lisParamRede->Count; n++)
		  {
		  paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		  if(paramRede->rede == NULL) continue;
		  paramGeral->compRede += paramRede->compRede;
		  }
	  //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
     daux = paramGeral->ptpre + paramGeral->ptdlo + paramGeral->ptexe;
     if(!IsDoubleZero(daux))
       {paramGeral->ptpre /= daux;
        paramGeral->ptdlo /= daux;
        paramGeral->ptexe /= daux;
       }

	  }
	}
//---------------------------------------------------------------------------
strDadoParamRede* __fastcall TGeraLisParamEqpto::ParametroConf(AnsiString codRede, TList *lisParamRede)
	{
	strDadoParamRede* paramRede = NULL;
	for(int np = 0; np < lisParamRede->Count; np++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if(paramRede->codRede.AnsiCompare(codRede.Trim()) == 0) break;
		paramRede = NULL;
		}
	return(paramRede);
	}
//---------------------------------------------------------------------------
strDadoParamRede* __fastcall TGeraLisParamEqpto::ParametroConf(VTRede *rede, TList *lisParamRede)
	{
	strDadoParamRede* paramRede = NULL;

	if(rede == NULL) return(NULL);

	for(int np = 0; np < lisParamRede->Count; np++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if(paramRede->rede == rede) break;
		paramRede = NULL;
		}
	return(paramRede);
	}
//---------------------------------------------------------------------------
int __fastcall TGeraLisParamEqpto::RedistribuiFalhasCircMes(VTParamConf *paramConf)
	{
	TList *lisParamEqpto = paramConf->LisParamEqpto;
	TList *lisParamBloco = paramConf->LisParamBloco;
	TList *lisParamCarga = paramConf->LisParamCarga;
	TList *lisParamRede  = paramConf->LisParamRede;
	strDadoParamGerais *paramGeral = paramConf->ParamGerais;

	VTChave       *chave;
	VTBloco       *bloco;
	TList   *lisRede, *lisBlocosJusDef;
	strDadoParamEqpto *paramEqpto;
	strDadoParamBloco *paramBloco;
	strDadoParamRede  *paramRede;
	double        compBlocos, taxaFalha, treparo, naux0, naux1;
	double        compBloco;
	double        daux;
	TList         *lisAux;
	int           icontaMes;
	//Gera paramGeral, paramRede
	//paramBloco e paramCarga->lamb_pu[nm], tr_pu[nm] para modular taxas e tempos ao longo do ano
	//função de paramGeral, paramRede, paramEqpto,
	//paramGeral->nFEquip[nm] / paramGeral->nFalhasEquip;
	//(paramGeral->clihmes[nm] / paramGeral->climes[nm]) * (paramGeral->cli/ paramGeral->clih) :versao anterior paramEqpto->durTot[nm];
	//---
	//paramRede->nFEquip[nm] / paramRede->nFalhasEquip;
	//(paramRede->clihmes[nm] / paramRede->climes[nm]) * (paramRede->cli/ paramRede->clih) :versao anterior paramEqpto->durTot[nm];
	//---
	//paramEqpto->nFEquip[nm] / paramRede->nFalhasEquip;
	//(paramEqpto->clihmes[nm] / paramRede->climes[nm]) * (paramRede->cli/ paramRede->clih) :versao anterior paramEqpto->durTot[nm];

	double trep;
	double soma;

	//paramGeral
	for(int nm = 0; nm < 12; nm++) {paramGeral->lamb_pu[nm] = 0.0; paramGeral->tr_pu[nm] = 0.0;}
	icontaMes = 0;
	if((paramGeral->nFalhasEquip > 0)&&(paramGeral->clih > 0))
	  {
	  for(int nm = 0; nm < 12; nm++)
		  {
			if(paramGeral->nFEquip[nm] > 0) icontaMes++;

			paramGeral->lamb_pu[nm]  = paramGeral->nFEquip[nm] / paramGeral->nFalhasEquip;// * 12 * 365 /paramGeral->ndiasObs;//em falhas/ano/km
			if(paramGeral->climes[nm] > 0)
				paramGeral->tr_pu[nm] = (paramGeral->clihmes[nm] / paramGeral->climes[nm]) * (paramGeral->cli/ paramGeral->clih);
		  }
	  }


	//lisParamRede
	for(int nr = 0; nr < lisParamRede->Count; nr++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[nr];
		for(int nm = 0; nm < 12; nm++)
			{
			paramRede->tr_pu[nm]  = 0.0;
			paramRede->lamb_pu[nm]= 0.0;
			}
		if((!(paramRede->nFalhasEquip > 0))||(!(paramRede->clih > 0))) continue;
		for(int nm = 0; nm < 12; nm++)
		  {paramRede->lamb_pu[nm]  = paramRede->nFEquip[nm] / paramRede->nFalhasEquip;// * 12 * 365 /paramGeral->ndiasObs;//em falhas/ano/km
			if(paramRede->climes[nm] > 0)
				paramRede->tr_pu[nm] = (paramRede->clihmes[nm] / paramRede->climes[nm]) * (paramRede->cli/ paramRede->clih);
		  }
		}

	//lisParamBloco
	for(int nb = 0; nb < lisParamBloco->Count; nb++)
		{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[nb];
		paramBloco->nFalhas = 0.0;
		paramBloco->duraTot = 0.0;
		for(int nm = 0; nm < 12; nm++)
			{
			paramBloco->lamb_pu[nm] = 0.0;
			paramBloco->tr_pu[nm]   = 0.0;
			}
		}

	lisAux = new TList();
	lisAux->Assign(lisParamEqpto, laOr);
	lisBlocosJusDef = new TList();
	for(int np = lisAux->Count - 1; np >= 0; np--)
		{
		paramEqpto = (strDadoParamEqpto*)lisAux->Items[np];
		if(paramEqpto->eqpto == NULL) continue; //eqpto encontrado
		if(paramEqpto->eqpto->Tipo() != eqptoCHAVE) continue;

		//Remove da lista
		lisAux->Delete(np);
		chave = (VTChave*)paramEqpto->eqpto;
		lisBlocosJusDef->Clear();
		//Preenche lista com blocos da zona de protecao da chave
		buscaProt->ListaZonaProtJus(chave, lisBlocosJusDef);
		//Protecao no caso de reconfiguracao de rede
		if(lisBlocosJusDef->Count == 0) continue;
		//Calcula o comprimento dos blocos da zona de protecao
		compBlocos = ComprimentoBlocos(lisBlocosJusDef);

		//Calcula o comprimento da zona de protecao
		for(int nb = 0; nb < lisBlocosJusDef->Count; nb++)
			{
			bloco = (VTBloco*)lisBlocosJusDef->Items[nb];
			if((paramBloco = ParametroConfBloco(bloco, lisParamBloco)) == NULL) continue;

			if((!(paramEqpto->nFalhasEquip > 0))||(!(paramEqpto->clih > 0))) continue;
			for(int nm = 0; nm < 12; nm++)
				{paramBloco->lamb_pu[nm]  = paramEqpto->nFEquip[nm] / paramEqpto->nFalhasEquip;// * 12 * 365 /paramGeral->ndiasObs;//em falhas/ano/km
				 if(paramEqpto->climes[nm] > 0)
					paramBloco->tr_pu[nm] = (paramEqpto->clihmes[nm] / paramEqpto->climes[nm]) * (paramEqpto->cli/ paramEqpto->clih);
				}

			//if(IsDoubleZero(compBlocos))
			//  daux = 1.0;
			//else
			//  daux = (paramBloco->compBloco/ compBlocos);
			//for(int nm = 0; nm < 12; nm++)
			//	{
			//	paramBloco->lamb_pu[nm] += (paramEqpto->nFEquip[nm] * daux);
			//	paramBloco->nFalhas     += (paramEqpto->nFEquip[nm] * daux);
			//	trep = 0.0;
			//	if(paramEqpto->climes[nm] > 0.0) trep = paramEqpto->clihmes[nm] / paramEqpto->climes[nm];
			//	paramBloco->tr_pu[nm]   += (paramEqpto->nFEquip[nm] * trep * daux); //hkmar
				//hkmar paramBloco->duraTot     += paramEqpto->durTot[nm];
			//	}
			}

		}
	//for(int nb = 0; nb < lisParamBloco->Count; nb++)
	//	{
	//	paramBloco = (strDadoParamBloco*)lisParamBloco->Items[nb];
	//	if(IsDoubleZero(paramBloco->nFalhas)) continue;
	//	for(int nm = 0; nm < 12; nm++)
	//		paramBloco->lamb_pu[nm] = paramBloco->lamb_pu[nm] / paramBloco->nFalhas;
	//	}

	if(lisParamCarga != NULL)
	  {
	  strDadoParamCarga *paramCarga;
	  for(int np = 0; np < lisParamCarga->Count; np++)
		  {
		  paramCarga = (strDadoParamCarga*)lisParamCarga->Items[np];
		  paramCarga->nFalhas = 0.0;
		  paramCarga->duraTot = 0.0;
		  for(int nm = 0; nm < 12; nm++)
			  {
			  paramCarga->lamb_pu[nm] = 0.0;
			  paramCarga->tr_pu[nm]   = 0.0;
			  }

		  paramEqpto = BuscaParamEqpto(paramCarga->carga, lisAux);
		  if(paramEqpto == NULL) continue;

		  //Remove da lista
		  lisAux->Remove(paramEqpto);

		  if((!(paramEqpto->nFalhasEquip > 0))||(!(paramEqpto->clih > 0))) continue;
		  for(int nm = 0; nm < 12; nm++)
			  {paramCarga->lamb_pu[nm]  = paramEqpto->nFEquip[nm] / paramEqpto->nFalhasEquip;// * 12 * 365 /paramGeral->ndiasObs;//em falhas/ano/km
				if(paramEqpto->climes[nm] > 0)
					paramCarga->tr_pu[nm] = (paramEqpto->clihmes[nm] / paramEqpto->climes[nm]) * (paramEqpto->cli/ paramEqpto->clih);
			  }
		  }
	  }
	delete lisAux;
	delete lisBlocosJusDef;

	//Checagem
	//lisParamEqpto --> nfalhasEquip, nfEquip[nm], durTot[nm];
	//lisParamBloco --> bloco, chave, neqProt, chaveProt, nFalhas, lamb_pu[nm], tr_pu[nm]
	//lisParamCarga --> carga, nfalhas, lamb_pu[nm], tr_pu[nm]
	//VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov
	//FILE      *fout;
	//AnsiString arq;
	//arq = path->DirTmp() + "\\ParamConfMes.txt";
	//paramConf->ImprimeParametrosMes(arq);

	return(icontaMes);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::RedistribuiFalhasCirc(VTParamConf *paramConf)
	{
	RedistribuiFalhasCirc(paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga, paramConf->ParamGerais->percDefTrans, paramConf->ParamGerais->iProfReliga);

	//VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov
	//FILE      *fout;
	//AnsiString arq;
	//arq = path->DirTmp() + "\\ParamSimula_RedistribuiFalhasCirc.txt";
	//paramConf->ImprimeParametrosSimula(arq);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::RedistribuiFalhasCirc(TList *lisParamEqpto, TList *lisParamBloco, TList *lisParamCarga, double percDefTrans, int iProfReliga)
	{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede  *rede;
	TList   *lisRede;
	strDadoParamEqpto *paramEqpto;
	strDadoParamBloco *paramBloco;
	TList         *lisBlocosJusDef;
	TList         *lisTrec;
	VTChave       *chave;
	VTBloco       *bloco;
	double        compBlocos, taxaFalha, treparo, naux0, naux1;
	double        compBloco;//nocorrLoc, nocorrNLoc;
	double        daux, dfalhas;
	int iconta, ncons;
	VTBlocoRad    *blocoRad;
	bool				flagProt;

	lisChavesProtMont = new TList();
	lisChavesMont = new TList();

	//Reinicia lista de parametros por bloco (redes de MT, eqptoCHAVE)
	//////////////////////////////////////////////////
	//Funcao que gera parametros por bloco de carga// hk201710IF: Acréscimo de ptpre, ptdlo, ptdexe
	//lisParamBloco-->  strDadoParamBloco->(lambda, trep, ptpre, ptdlo, ptdexe) = f(lisParamEqpto)//
	//lisParamCarga-->  strDadoParamCarga->(lambda, trep, ptpre, ptdlo, ptdexe) = f(lisParamEqpto)
	/////////////////////////////////////////////////
		//paramEqpto->eqpto
		//paramEqpto->rede
		//paramEqpto->lambda
		//paramEqpto->trep
	//Reinicia lisParamCarga funcao de
		//paramEqpto->carga
		//paramEqpto->rede
		//paramEqpto->lambda
		//paramEqpto->trep


	naux0 = naux1 = 0;
	LimpaTList(lisParamBloco);
	//for(int np = 0; np < lisParamBloco->Count; np++)
	//	{
	//	paramBloco = (strDadoParamBloco*)lisParamBloco->Items[np];
	//	paramBloco->nFalhas = -1.0; //caso a variavel permaneca negativa entao não esta mapeada por lisParamEqpto
	//	paramBloco->duraTot = 0.0;
	//	}
	lisBlocosJusDef = new TList();
	lisRede = redes->LisRede();
	iconta = 0;
	for(int nr = 0; nr < lisRede->Count; nr++)
		{
		rede = (VTRede*)lisRede->Items[nr];//ordena por rede existente
		for(int np = 0; np < lisParamEqpto->Count; np++)
			{
			paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
			if(paramEqpto->eqpto == NULL) continue; //eqpto encontrado
			if(paramEqpto->rede != rede) continue;
			if(paramEqpto->eqpto->Tipo() != eqptoCHAVE) continue;

			iconta++;
			chave = (VTChave*)paramEqpto->eqpto;
			lisBlocosJusDef->Clear();
			//Preenche lista com blocos da zona de protecao da chave
			//hk201802a: buscaProt->ListaZonaProtJus(chave, lisBlocosJusDef);
         buscaProt->ListaBlocoRadZonaProtJus(chave, lisBlocosJusDef);
			//Protecao no caso de reconfiguracao de rede
			if(lisBlocosJusDef->Count == 0) continue;
			//Calcula o comprimento da zona de protecao
			compBlocos = 0.0;
			for(int nb = 0; nb < lisBlocosJusDef->Count; nb++)
				{//hk201802a
				//bloco = (VTBloco*)lisBlocosJusDef->Items[nb];
				blocoRad = (VTBlocoRad*)lisBlocosJusDef->Items[nb];
				compBlocos += ComprimentoBloco(blocoRad->Bloco);
				//bloco = (VTBloco*)lisBlocosJusDef->Items[nb];
				//compBlocos += ComprimentoBloco(bloco);
				}
			//hkpc paramEqpto->compSim = compBlocos;

			for(int nb = 0; nb < lisBlocosJusDef->Count; nb++)
				{
				//hk201802a
				//bloco = (VTBloco*)lisBlocosJusDef->Items[nb];
            blocoRad = (VTBlocoRad*)lisBlocosJusDef->Items[nb];
            bloco = blocoRad->Bloco;
				if((paramBloco = ParametroConfBloco(bloco, lisParamBloco)) == NULL)
				  {lisParamBloco->Add(paramBloco = new strDadoParamBloco);
					paramBloco->fatTMA= 1.0;//hk201703: adaptações para planTurmas: acréscimo de fator tma
					paramBloco->bloco = bloco;
					paramBloco->fase = GetFaseFromBloco(bloco);
					paramBloco->compBloco = ComprimentoBloco(bloco);
					//hk201802a: blocoRad = buscaProt->IdentificaBlocoRad(bloco);
					paramBloco->eqpto     = NULL;
					paramBloco->codChave  = "";
					paramBloco->codRede   = "";
					paramBloco->lisObrasBloco = NULL;
					if(blocoRad != NULL)
					  {
					  paramBloco->eqpto    = blocoRad->Chave;
                 paramBloco->eqptoBarra = blocoRad->BarraChave; //hk201802a

				      if(blocoRad->Chave != NULL) paramEqpto->codRede  = blocoRad->Chave->rede->Codigo;
					  paramBloco->codChave = paramBloco->eqpto->Codigo;
                 paramBloco->codBarra = blocoRad->BarraChave->Codigo;//hk201802a
					  paramBloco->codRede  = blocoRad->Chave->rede->Codigo;
					  }
					paramBloco->compZonaProt = compBlocos;
					paramBloco->nFalhas = 0.0;
					paramBloco->duraTot = 0.0;
               paramBloco->ptpre   = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
               paramBloco->ptdlo   = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
               paramBloco->ptexe   = 0.0;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
					//for(int nm = 0; nm < 12; nm++)
					//	{
					//	paramBloco->lamb_pu[nm] = 0.0;
					//	paramBloco->tr_pu[nm]   = 0.0;
					//	}
				  }
				//paramBloco->codChave += ("-" +  paramEqpto->eqpto->Codigo);
				if(IsDoubleZero(compBlocos))
				  daux = 1.0;
				else
				  daux = paramBloco->compBloco / compBlocos;

				dfalhas = paramEqpto->lambda * daux;
				if(paramBloco->nFalhas < 0) paramBloco->nFalhas = 0.0;

				paramBloco->nFalhas += dfalhas;
				paramBloco->duraTot += (daux = (dfalhas * paramEqpto->trep));//paramBloco->duraTot += (dfalhas * paramEqpto->trep);

            paramBloco->ptpre   += (daux * paramEqpto->ptpre);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
            paramBloco->ptdlo   += (daux * paramEqpto->ptdlo);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
            paramBloco->ptexe   += (daux * paramEqpto->ptexe);//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
				//for(int nm = 0; nm < 12; nm++)
				//	{
				//	dfalhas = paramEqpto->lamb[nm] * daux;
				//	paramBloco->lamb_pu[nm] += dfalhas;
				//	paramBloco->tr_pu[nm]   += (dfalhas * paramEqpto->tr[12]);
				//	}

				}

			}
		}
	for(int np = lisParamBloco->Count - 1; np >= 0; np--)
		{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[np];
		//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		//if(paramBloco->nFalhas < 0) //retira paramBloco nao mapeado por lisParamEqpto
		//  {
		//  lisParamBloco->Delete(np);
		//  delete paramBloco;
		//  continue;
		//  }


		if(IsDoubleZero(paramBloco->compBloco)||IsDoubleZero(paramBloco->nFalhas))
		  {
		  paramBloco->lambda = 0.0;
		  paramBloco->trep   = 0.0;
		  }
		else
		  {
		  paramBloco->lambda = paramBloco->nFalhas / paramBloco->compBloco;
		  paramBloco->trep   = paramBloco->duraTot / paramBloco->nFalhas;

        daux = (paramBloco->ptpre + paramBloco->ptdlo + paramBloco->ptexe);
        if(!IsDoubleZero(daux))          //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          {
          paramBloco->ptpre /= daux;     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          paramBloco->ptdlo /= daux;     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          paramBloco->ptexe /= daux;     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
          }
		  //lista de chaves de proteção a montante do ponto de defeito
		  if (buscaProt != NULL)
				{
				buscaProt->IdentificaChvMont(paramBloco->bloco, this->lisChavesProtMont, flagProt = true);
				//lista de chaves a montante do ponto de defeito até o suprimento
				buscaProt->IdentificaChvMont(paramBloco->bloco, this->lisChavesMont, flagProt = false);
				}

		  switch (iProfReliga)
		  {
		  case 0: // somente o bloco imediatamente a jusante
		  if (paramBloco->eqpto != NULL)
				{
				chave = (VTChave*)paramBloco->eqpto;
				if (chave->TipoChave->Tipo == chaveRELIGADORA || chave->TipoChave->Tipo == chaveDISJUNTOR)
					{
					paramBloco->lambda = paramBloco->lambda / (1 - percDefTrans);
					break;
					}
				}
				break;

			case 1: // até o próximo religador
			for (int n = 0; n < this->lisChavesProtMont->Count; n++)
				{
				chave = (VTChave*)this->lisChavesProtMont->Items[n];
				if (chave->TipoChave->Tipo == chaveRELIGADORA || chave->TipoChave->Tipo == chaveDISJUNTOR)
					 {
					 paramBloco->lambda = paramBloco->lambda / (1 - percDefTrans);
					 break;
					 }

				chave = NULL;
				}
			break;
			}
		  }
		}


	if(lisParamCarga != NULL)
	  {
	  strDadoParamCarga *paramCarga;
	  LimpaTList(lisParamCarga);
	  for(int nr = 0; nr < lisRede->Count; nr++)
		  {
		  rede = (VTRede*)lisRede->Items[nr];//ordena por rede existente
		  for(int np = 0; np < lisParamEqpto->Count; np++)
			  {
			  paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
			  if(paramEqpto->carga == NULL) continue; //eqpto encontrado
			  if(paramEqpto->rede != rede) continue;
			  lisParamCarga->Add(paramCarga = new strDadoParamCarga);
			  paramCarga->rede     = paramEqpto->rede;
			  paramCarga->carga    = paramEqpto->carga;
			  paramCarga->codRede  = paramEqpto->rede->Codigo; //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : paramCarga->codRede = paramEqpto->rede->Codigo;
			  paramCarga->codCarga = paramCarga->carga->Codigo;
			  paramCarga->lambda   = paramEqpto->lambda;
			  paramCarga->trep     = paramEqpto->trep;

           paramCarga->ptpre    = paramEqpto->ptpre;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
           paramCarga->ptdlo    = paramEqpto->ptdlo;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
           paramCarga->ptexe    = paramEqpto->ptexe;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)

			  paramCarga->fatTMA   = 1.0;//hk201703: adaptações para planTurmas: acréscimo de fator tma
			  paramCarga->nconsJus = NCons(paramEqpto->carga);
			  }
		  }
	  }

	delete lisBlocosJusDef;
	DestroiTObject(lisChavesProtMont);
	DestroiTObject(lisChavesMont);
	}
//---------------------------------------------------------------------------
strDadoParamBloco* __fastcall TGeraLisParamEqpto::ParametroConfBloco(VTBloco *bloco, TList *lisParamBloco)
	{
	strDadoParamBloco *paramBloco = NULL;
	for(int np = 0; np < lisParamBloco->Count; np++)
		{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[np];
		if(paramBloco->bloco == bloco) break;
		paramBloco = NULL;
		}
	return(paramBloco);
	}
//---------------------------------------------------------------------------
double __fastcall TGeraLisParamEqpto::ComprimentoBlocos(TList *lisBlocos)
	{
	VTBloco *bloco;
	double    comp_km = 0.0;
	for(int n = 0; n < lisBlocos->Count; n++)
		{
		bloco = (VTBloco*)lisBlocos->Items[n];
		comp_km += ComprimentoBloco(bloco);
		}
	return(comp_km);
	}
//---------------------------------------------------------------------------
double __fastcall TGeraLisParamEqpto::ComprimentoBloco(VTBloco *bloco)
	{
	TList *lisLiga;
	VTEqpto *pliga;
	double    comp_km = 0.0;

	if(bloco == NULL) return(0.0);

	lisLiga = bloco->LisLigacao();

	for(int n = 0; n < lisLiga->Count; n++)
		{
		pliga = (VTEqpto*)lisLiga->Items[n];
		if(pliga->Tipo() != eqptoTRECHO) continue;
		comp_km += ((VTTrecho*)pliga)->Comprimento_km;
		}
	return(comp_km);
	}
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
void __fastcall TGeraLisParamEqpto::ReiniciaDadosParamRede(TList *lisParamRede)
	{
	//TList *lisParamRede = paramConf->LisParamRede;
	strDadoParamRede *paramRede;
	//Calcula paramRede->lambda e paramRede->trep
	//em funcao de:
	//paramRede->durTotal
	//paramRede->nFalhasEquip
	//paramRede->ndiasObs

	//Calcula paramRede->lambda e paramRede->trep
	for(int n = 0; n < lisParamRede->Count; n++)
		{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		paramRede->lambda = 0.0;
		paramRede->trep   = 0.0;

		if(paramRede->rede == NULL) continue;
		if(!paramRede->rede->Carregada) continue;

		if((!IsDoubleZero(paramRede->compRede)) &&
           (!IsDoubleZero(paramRede->ndiasObs)))
		  {
			paramRede->lambda = paramRede->nFalhasEquip *  365 /paramRede->ndiasObs;
			paramRede->lambda /= paramRede->compRede;
			if(paramRede->cli > 0.0)
				 paramRede->trep   = paramRede->clih * 60.0/ paramRede->cli;
			//hkmar paramRede->trep   = paramRede->durTotal/ paramRede->nFalhasEquip;
		  }
		}
	}
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
void __fastcall TGeraLisParamEqpto::ReiniciaDadosParamGeral(strDadoParamGerais *paramGeral)
	{
	//Calcula paramGeral->lambda e paramGeral->trep
	//com as funcoes:
	//paramGeral->trep   = paramGeral->clih * 60.0/ paramGeral->cli: versao anterior paramGeral->durTotal
	//paramGeral->lambda = paramGeral->nFalhasEquip *  365 /paramGeral->ndiasObs/ paramGeral->compRede

	paramGeral->lambda = 0.0;
	paramGeral->trep   = 0.0;

	if((!IsDoubleZero(paramGeral->compRede)) &&
       (!IsDoubleZero(paramGeral->ndiasObs)))
	  {
	  paramGeral->lambda = paramGeral->nFalhasEquip *  365 /paramGeral->ndiasObs;
	  paramGeral->lambda /= paramGeral->compRede;
	  //hkmar paramGeral->trep   = paramGeral->durTotal/ paramGeral->nFalhasEquip;
	  if(paramGeral->cli > 0.0)
			paramGeral->trep   = paramGeral->clih * 60.0/ paramGeral->cli;
	  }
	}
    
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ReiniciaDistribHora(strDadoParamGerais *paramGeral, double *distFalhasPat)
	{


	for(int nh = 0; nh < 24; nh++)
		{
		distFalhasPat[nh] = paramGeral->nfhora[nh];
		}
	//Redistribui falhas para somar 1.0, soma distFalhasPat[0->23] = 1.0
	RedistribuiFalhasHora(distFalhasPat);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ReiniciaDadosParamEqpto(TList *lisParamEqpto)
	{
	//TList *lisParamEqpto = paramConf->LisParamEqpto;
	strDadoParamEqpto* paramEqpto;
	//Calcula
	//paramEqpto->lambda e paramEqpto->trep
	//em funcao de:
	//paramEqpto->eqpto
	//paramEqpto->carga
	//paramEqpto->clih, paramEqpto->cli - versão anterior: paramEqpto->durTotall
	//paramEqpto->nFalhasEquip
	//paramEqpto->ndiasObs

	//Calcula taxas e treparo por paramEqpto
	for(int np = 0; np < lisParamEqpto->Count; np++)
		{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if((paramEqpto->eqpto == NULL)&&(paramEqpto->carga == NULL)) continue;
		//hkpc if(paramEqpto->flagEncontrado == false) continue;
		//hkmar if(paramEqpto->eqpto != NULL)
		//hkmar   {if(paramEqpto->eqpto->Tipo() != eqptoCHAVE) continue;
		//hkmar   }

		if(IsDoubleZero(paramEqpto->ndiasObs)) //hkmar ||IsDoubleZero(paramEqpto->nFalhasEquip))
		  {
		  paramEqpto->lambda = 0.0;
		  paramEqpto->trep   = 0.0;
		  }
		else
		  {
		  paramEqpto->lambda = (paramEqpto->nFalhasEquip *  365 /paramEqpto->ndiasObs);
		  //hkmar if(paramEqpto->nFalhasEquip > 0)
		  //hkmar 			 paramEqpto->trep   = paramEqpto->durTotal/ paramEqpto->nFalhasEquip;
		  if(paramEqpto->cli > 0)  //hkmar
						paramEqpto->trep   = paramEqpto->clih * 60.0/ paramEqpto->cli;
		  }
		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ImprimeLisOc(void)
	{
	VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
//hk201808: não utilizado	VTOcorrGeral   *ocorr; //VTOcorr   *ocorr; hk201612: compatibilização extrator geral
	//FILE      *fout;
	AnsiString arq;

//		__property VTAno*           Ano              = {read=PM_GetAno,              write=PM_SetAno};
//		__property AnsiString       Area             = {read=PM_GetArea,             write=PM_SetArea};
//		__property VTCausa*         Causa            = {read=PM_GetCausa,            write=PM_SetCausa};
//		__property VTCircuito*      Circuito         = {read=PM_GetCircuito,         write=PM_SetCircuito};
//		__property VTClasse*        Classe           = {read=PM_GetClasse,           write=PM_SetClasse};
//		__property VTClasseCliente* ClasseCliente    = {read=PM_GetClasseCliente,    write=PM_SetClasseCliente};
//		__property int              Clientes         = {read=PM_GetClientes,         write=PM_SetClientes};
//		__property AnsiString       Codigo           = {read=PM_GetCodigo,           write=PM_SetCodigo};
//		__property int              Conjunto         = {read=PM_GetConjunto,         write=PM_SetConjunto};
//		__property TDateTime        DH_Interrupcao   = {read=PM_GetDataInterrupcao,  write=PM_SetDataInterrupcao};
//		__property TDateTime        DH_Restauracao   = {read=PM_GetDataRestauracao,  write=PM_SetDataRestauracao};
//		__property int              Duracao          = {read=PM_GetDuracao,          write=PM_SetDuracao};
//		__property VTEqptoOc*       EqptoOc          = {read=PM_GetEqptoOc,          write=PM_SetEqptoOc};
//		__property AnsiString       Instalacao       = {read=PM_GetInstalacao,       write=PM_SetInstalacao};
//		__property AnsiString       InstalacaoInterr = {read=PM_GetInstalacaoInterr, write=PM_SetInstalacaoInterr};
//		__property int              Municipio        = {read=PM_GetMunicipio,        write=PM_SetMunicipio};
//		__property int              Ordem            = {read=PM_GetOrdem,            write=PM_SetOrdem};
//		__property VTTipoOcorr*     Tipo             = {read=PM_GetTipo,             write=PM_SetTipo};
//	for(int n = 0; n< lisOc->Count; n++)
//		{
//		ocorr = (VTOcorr*)lisOc->Items[n];
//		ocorr->
//		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::ImprimeLisParamEqpto(TList *lisParamEqpto, AnsiString nomeArq)
	{
	VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
	FILE      *fout;
	AnsiString arq;
	strDadoParamEqpto *paramEqpto;

	arq = path->DirExporta() + "\\" + nomeArq;
	fout = fopen(arq.c_str(), "wt");

//			AnsiString codEqpto;     //Equipamento
//			AnsiString codRede;      //Código Rede
//			AnsiString tipoRede;     //Rede (MT, BT, AT)
//			double     nFalhasEquip; //Falhas por ano (do equipamento localizado)
//			double     nFalhasRamal; //Falhas por ano em ramal de BT se codEqpto é uma ET hknova
//			double     nFalhasNRamal;//Falhas por ano em ET ou ao longo da rede MT hknova
//			double     tdeslMedio;   //tempo de desligamento médio
//			double     durTotal;     //Duracao [min.] por ano das falhas no equipamento localizado
//			double     ndiasObs;     //Número de dias entre dataIni e dataFim
//			Parametros e variáveis auxiliares obtidos a partir das redes abertas
//			VTRede  *rede;
//			VTEqpto *eqpto;
//			VTCarga *carga;
//			TObject *paramOrig;
//       TObject *barraOcOrig;
//			int     nconsJus; //número de consumidores a jusante do trafo de distrib. - carga BT
//			double  lambda, trep; //taxa de falha e tempo de reparo
	fprintf(fout, "codEqpto\t ");
	fprintf(fout, "codRede\t ");
	fprintf(fout, "nFalhasEquip\t ");
	fprintf(fout, "durTotal\t ");

	fprintf(fout, "nf\t ");
	fprintf(fout, "nfr\t ");
	fprintf(fout, "tdeslMedio\t ");
	for(int nm = 0; nm < 12; nm++)
		{fprintf(fout, "nfmes[%i]\t ", nm);
		 fprintf(fout, "nfrmes[%i]\t ", nm);
		 fprintf(fout, "tdmes[%i]\t ", nm);
		}

	fprintf(fout, "ndiasObs\n");
	for(int n = 0; n < lisParamEqpto->Count; n++)
		{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		fprintf(fout, "%s\t ", paramEqpto->codEqpto.c_str());
		fprintf(fout, "%s\t ", paramEqpto->codRede.c_str());
		fprintf(fout, "%6.4f\t ", paramEqpto->nFalhasEquip);
		fprintf(fout, "%6.4f\t ", paramEqpto->durTotal);

		fprintf(fout, "%6.4f\t ", paramEqpto->nf);
		fprintf(fout, "%6.4f\t ", paramEqpto->nfr);
		fprintf(fout, "%6.4f\t ", paramEqpto->td);
		for(int nm = 0; nm < 12; nm++)
			{fprintf(fout, "%6.4f\t ", paramEqpto->nfmes[nm]);
			 fprintf(fout, "%6.4f\t ", paramEqpto->nfrmes[nm]);
			 fprintf(fout, "%6.4f\t ", paramEqpto->tdmes[nm]);
			}

		fprintf(fout, "%6.4f\n", paramEqpto->ndiasObs);
		}

	fclose(fout);
	}
//---------------------------------------------------------------------------
int __fastcall TGeraLisParamEqpto::DefineNDiasObs(void)
	{
	VTOcorrGeral   *ocorr; //VTOcorr   *ocorr; hk201612: compatibilização extrator geral
	double diaMin, diaMax, dia;
	int    ndiasObs = 0;
	//ETAPA 3: PARAMOCORRENCIAS: Define período de observacao
	diaMin = diaMax = -1.0;

	//pedro20170425- correção da contagem do número de dias

	if(lisOc->Count == 0)
		return(0);

	for(int n = 0; n< lisOc->Count; n++) {
		ocorr = (VTOcorrGeral*)lisOc->Items[n]; //ocorr = (VTOcorr*)lisOc->Items[n]; hk201612: compatibilização extrator geral

		//hk201612: compatibilização extrator geral dia = ocorr->DH_Interrupcao.operator double();
		dia = ocorr->DhInicial.operator double();
		AnsiString dhInicial = ocorr->DhInicial.FormatString("dd/mm/yyyy hh:mm:ss");
		if((diaMin < 0)||(dia < diaMin)){
			diaMin = ocorr->DhInicial;
		}
		if((diaMax < 0)||(dia > diaMax)){
			diaMax = ocorr->DhInicial;
		}
	}

	return(ndiasObs);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraLisParamEqpto::GeraDistribFalhasPatamar(void)
	{
	TList *lisAux;
	VTBarraOc *barraOc;
	VTOcorrGeral   *ocorr, *ocorrAux; //VTOcorr   *ocorr, *ocorrAux; hk201612: compatibilização extrator geral
	unsigned short hour, min, sec, msec;
	unsigned short year, month, day;
	AnsiString aux;
	double nconsBT, nconsRamal, daux;


//	VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
//	FILE      *fout;
//	AnsiString arq;
//	arq = path->DirExporta() + "\\barraOc30181.txt";
//	fout = fopen(arq.c_str(), "wt");

	lisAux = new TList();
	//ETAPA 5: PARAMOCORRENCIAS: GERA DISTRIBUIÇÃO DE FALHAS POR PATAMAR HORÁRIO
	//e PREENCHE barraOc->LisOcorrF (uma ocorrencia por horário de inicio no equipamento)
	//Gera barraOc->NOcorPond
	//barraOc->NFRMes[month], barraOc->NFMes[month]
	//Gera ocorrencias com o início e na mesma barra
	//barraOc->LisOcorrF = f(barraOc->LisTrataOcorr)
	//
	//Calcula
	//		barraOc->LisOcorrF->ocorr
	//		barraOc->NOcorRamal : ramal de ligacao no cliente
	//		barraOc->NOcorNRamal: não ramal de ligacao - chave MT ou trafo BT
	//    barraOc->NFHora[hour]
	//    barraOc->NFMes[month]
	//    barraOc->NFRMes[month]
	//Funcao de
	//    lisBarraOc->barraOc->LisTrataOcorr
	//		ocorr->FlagRamal
	//		ocorr->Clientes
	//		ocorr->DH_Interrupcao



	//Inicia distribuição de falhas
//	for(int np = 0; np < 24; np++) distFalhas[np] = 0.0;
	//Define o número de ocorrencias por barra
	for(int nb = 0; nb < lisBarraOc->Count; nb++)
		{
		barraOc = (VTBarraOc*)lisBarraOc->Items[nb];

		//Preenche lista de ocorrencias com mesma dada
		lisAux->Clear();
		lisAux->Assign(barraOc->LisTrataOcorr, laOr); //barraOc->LisTrataOcorr = eqptoOc->ListaOcorr()
		while(lisAux->Count > 0)
			{
			//Em lisOcorrF: ocorrencias com o início e na mesma barra
			//barraOc->LisOcorrF->Add(ocorr = (VTOcorr*)lisAux->First());//hk201612: compatibilização extrator geral
			barraOc->LisOcorrF->Add(ocorr = (VTOcorrGeral*)lisAux->First());
			//hk201612: compatibilização extrator geral dia = ocorr->DH_Interrupcao.operator double();
			ocorr->DhInicial.DecodeTime(&hour, &min, &sec, &msec); //ocorr->DH_Interrupcao.DecodeTime(&hour, &min, &sec, &msec);
			ocorr->DhInicial.DecodeDate(&year, &month, &day);      //ocorr->DH_Interrupcao.DecodeDate(&year, &month, &day);

			//hkeq if(barraOc->EqptoOcorr->Tipo() == eqptoCARGA)

			//if(barraOc->EqptoOc->Eqpto->Tipo() == eqptoCARGA)
			//  {
			  //hkmuda aux = ocorr->InstalacaoInterr.Trim().SubString(1,1);
			  //hkmuda if(aux.AnsiCompare("B") == 0)
			  //if(((sdi_ocorr.InstalacaoInterr.Trim()).SubString(1,1)).AnsiCompare("B") == 0)
			  //	flagRamal = true;
			  if(ocorr->FlagRamal)
				 {
				 //nconsBT = barraOc->NConsBT;

				 nconsRamal = ocorr->Cliente; //hk201612: compatibilização extrator geral nconsRamal = ocorr->Clientes;
				 //if((nconsBT > 0)&&(!(nconsRamal > nconsBT)))
					 {
					 daux = barraOc->NOcorRamal + nconsRamal;
					 barraOc->NOcorRamal = daux;
					 daux = barraOc->NFRMes[month - 1]  + nconsRamal;
					 barraOc->NFRMes[month - 1] = daux;
					 }
				 }
			  else
				 {daux = barraOc->NOcorNRamal + 1;
				  barraOc->NOcorNRamal = daux;
				  daux = barraOc->NFMes[month - 1]  + 1;
				  barraOc->NFMes[month - 1] = daux;
				 }
			//  }
			//else
			//  {
			//  if(!ocorr->FlagRamal)
			//	 {
			//	 daux = barraOc->NOcorNRamal + 1;
			//	 barraOc->NOcorNRamal = daux;
			//	 }
			//  }


			//Soma numero de ocorrencias ponderadas da hora da ocorrencia em distFalhas
//			distFalhas[hour] += 1;//daux;
			barraOc->NFHora[hour] += 1;

//			fprintf(fout, "%s\t %s\t %i\t %i\n", barraOc->CodigoRede, barraOc->Codigo, (int)hour, (int)min );

			//Elimina de lisAux as ocorrencias de mesmo instante, mesmo DH_Interrupcao
			for(int noc = lisAux->Count - 1; noc >= 0; noc--)
				{
				//hk201612: compatibilização extrator geral ocorrAux = (VTOcorr*)lisAux->Items[noc];
				ocorrAux = (VTOcorrGeral*)lisAux->Items[noc];
				if(ocorrAux->DhInicial != ocorr->DhInicial) continue;//hk201612: compatibilização extrator geral if(ocorrAux->DH_Interrupcao != ocorr->DH_Interrupcao) continue;
				lisAux->Delete(noc);
				}
			}
		}
	delete lisAux;
//	fclose(fout);
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraLisParamEqpto::GeraParamEqptoBdado(void)
	{
	VTBarraOc *barraOc;
	strDadoParamEqpto *paramEqpto;
	VTCircuito        *circuito;

	double        nfalhasEquip, durTotal;
	int           ncons;

	//Popula lisParamEqpto->paramEqpto = f(lisBarraOc->barraOc)
	//paramEqpto->barraOcOrig = (TObject*)barraOc;
	//paramEqpto->codEqpto     = barraOc->Codigo;
	//paramEqpto->codRede      = circuito->CodigoRede;
	//paramEqpto->ndiasObs     = barraOc->NDiasObs
	//paramEqpto->nf = barraOc->NOcorNRamal;
	//paramEqpto->nfr= barraOc->NOcorRamal;
	//paramEqpto->td = barraOc->TdeslMedio;
	//paramEqpto->cli = barraOc->Cli;
	//paramEqpto->clih= barraOc->Clih;
	//paramEqpto->nfhora[np] = barraOc->NFHora[np];
	//paramEqpto->nfrhora[np]= 0
	//paramEqpto->nfmes[nm]  = barraOc->NFMes[nm];
	//paramEqpto->nfrmes[nm] = barraOc->NFRMes[nm];
	//paramEqpto->tdmes[nm]  = barraOc->TDMes[nm];
	//paramEqpto->climes[nm] = barraOc->CLIMes[nm];  //hkmar
	//paramEqpto->clihmes[nm]= barraOc->CLIHMes[nm]; //hkmar
	//
	//paramEqpto->rede         = NULL
	//paramEqpto->tipoRede     = ""
	//paramEqpto->eqpto        = NULL
	//paramEqpto->carga        = NULL;
	//paramEqpto->nFalhasEquip = 0.0;
	//paramEqpto->durTotal     = 0.0;



	LimpaTList(lisParamEqpto);

	//Inicia a quantidade de us do polo
	for(int nb = 0; nb < lisBarraOc->Count; nb++)
		{
		barraOc = (VTBarraOc*)lisBarraOc->Items[nb];
		//Gera um parametro ocorrencia por circuito em
		//que o equipamento de barraOc foi alocado
		for(int nc = 0; nc < barraOc->LisCircuitos->Count; nc++)
			{
			circuito = (VTCircuito*)barraOc->LisCircuitos->Items[nc];
			//barraOc->ParEqExt = (TObject*)(paramEqpto = new strDadoParamEqpto);
			paramEqpto = new strDadoParamEqpto;
			paramEqpto->barraOcOrig = (TObject*)barraOc;
			lisParamEqpto->Add(paramEqpto);
			//Parametros básicos para serem gravados em base
			paramEqpto->codEqpto     = barraOc->Codigo;
			paramEqpto->codRede      = circuito->CodigoRede;//barraOc->CodigoRede;

			if(barraOc->EqptoOc->Eqpto != NULL){

				if(barraOc->EqptoOc->Eqpto->Tipo() == eqptoCHAVE){
            	VTChave *chave = (VTChave*)barraOc->EqptoOc->Eqpto;
					paramEqpto->tipoEquipamento = chave->TipoChave->Codigo;
				} else {
					paramEqpto->tipoEquipamento = barraOc->EqptoOc->Eqpto->TipoAsString();
				}

			}

			paramEqpto->ndiasObs     = barraOc->NDiasObs;//poloEstudo->NDiasObs;//lisMES->Count * 30;
			//Parametros a serem definidos com a rede carregada em tempo de cálculo de indicadores
			paramEqpto->rede         = NULL;
			paramEqpto->tipoRede     = "";
			paramEqpto->eqpto        = NULL;
			paramEqpto->carga        = NULL;
			paramEqpto->nFalhasEquip = 0.0;
			paramEqpto->durTotal     = 0.0;


			//paramEqpto->nFalhasRamal = barraOc->NOcorRamal;
			//paramEqpto->nFalhasNRamal= barraOc->NOcorNRamal;
			//paramEqpto->tdeslMedio   = barraOc->TdeslMedio;
			paramEqpto->nfgeral = barraOc->LisOcorrF->Count; //hk201611: nfgeral: total de falhas associadas ao equipamento

			paramEqpto->nf = barraOc->NOcorNRamal;
			paramEqpto->nfr= barraOc->NOcorRamal;
			paramEqpto->td = barraOc->TdeslMedio;  // barraOc->TdeslMedio = (barraOc->Clih* 60/ barraOc->Cli)
			paramEqpto->cli = barraOc->Cli;
			paramEqpto->clih= barraOc->Clih;
			paramEqpto->tat = barraOc->Tat; //hk201703: Correção do tempo de atendimento: apresentação no módulo de gerenciamento de ocorrencias

         //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
         paramEqpto->ptpre = barraOc->Ptpre;
         paramEqpto->ptdlo = barraOc->Ptdlo;
         paramEqpto->ptexe = barraOc->Ptexe;

			for(int np = 0; np < 24; np++)
				{paramEqpto->nfhora[np] = barraOc->NFHora[np];
				 paramEqpto->nfrhora[np]= 0;
				}

			for(int nm = 0; nm < 12; nm++)
				{
				paramEqpto->nfmes[nm]  = barraOc->NFMes[nm];
				paramEqpto->nfrmes[nm] = barraOc->NFRMes[nm];
				paramEqpto->tdmes[nm]  = barraOc->TDMes[nm];
				paramEqpto->climes[nm] = barraOc->CLIMes[nm];  //hkmar
				paramEqpto->clihmes[nm]= barraOc->CLIHMes[nm]; //hkmar
				}
			//double     nfmes[12], nfrmes[12], tdmes[12];
			}
		}
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraLisParamEqpto::GeraLisBarraOcBdado(TList *lisEqptoOc, TList *lisBarraOc)//hk201702: integração simTurmas
	{
	//ETAPA 4: PARAMOCORRENCIAS: CRIA LISTA DE BARRAOC, LISBARRAOC
	//Preenche lisBarraOc->barraOc = NewObjBarraOc
	//barraOc->Codigo   = eqptoOc->EqptoCodBase
	//barraOc->EqptoOc  = eqptoOc
	//barraOc->ndiasObs = ndiasObs
	//barraOc->LisTrataOcorr <--eqptoOc->ListaOcorr()
	//barraOc->LisCircuitos  <--eqptoOc->ListaCircuitos()
	//---------------------------------
	//Definidos a posteriori
	//barraOc->EqptoOcorr
	//barraOc->RedePri
	//barraOc->NConsBT


	//Funcao de
		//lisEqptoOc->eqptoOc


	TList      *lisOcorrEqpto, *lisOcorrBarraOc, *lisCircuito;
	VTEqptoOc  *eqptoOc;
	VTCircuito *circuito;

	VTBarraOc *barraOc;

	LimpaTList(lisBarraOc);
	for(int n = 0; n < lisEqptoOc->Count; n++)
		{
		eqptoOc = (VTEqptoOc*)lisEqptoOc->Items[n];

		lisBarraOc->Add(barraOc = NewObjBarraOc());

		//
		barraOc->EqptoOc = eqptoOc;

		//Atribui o numero de dias de observacao
		barraOc->NDiasObs   = ndiasObs;
		//Atribui o codigo do equipamento
		barraOc->Codigo = eqptoOc->EqptoCodBase;

		//busca o primeiro circuito onde o equipamento está alocado

		lisCircuito = eqptoOc->ListaCircuitos();
		if(lisCircuito->Count == 0)
		{
			continue;
		}
		circuito = (VTCircuito*)lisCircuito->First();
		//Atribui o codigo do circuito, primeiro circuito
		barraOc->CodigoRede = circuito->CodigoRede;

		lisOcorrEqpto   = eqptoOc->ListaOcorr();
		lisOcorrBarraOc = barraOc->LisTrataOcorr;
		lisOcorrBarraOc->Clear();
		//Adiciona as ocorrencias
		for(int noc = 0; noc < lisOcorrEqpto->Count; noc++)
			{
			lisOcorrBarraOc->Add(lisOcorrEqpto->Items[noc]);
			}
		//Copia lista de eqptoOc->LisCircuitos
		barraOc->LisCircuitos->Assign(eqptoOc->ListaCircuitos(), laCopy);
		}

	return(true);
	}
//---------------------------------------------------------------------------
VTRede* __fastcall TGeraLisParamEqpto::BuscaRede(AnsiString CodigoCircuito)
	{
	TList     *lisRede;
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTRede    *rede;
	lisRede = redes->LisRede();

	rede = NULL;
	for(int nr = 0; nr < lisRede->Count; nr++)
		{
		rede = (VTRede*)lisRede->Items[nr];
		if(rede->Carregada)
		  {if(rede->Codigo.AnsiCompare(CodigoCircuito.Trim()) == 0) break;
		  }
		rede = NULL;
		}
	return(rede);
	}
//---------------------------------------------------------------------------
VTEqpto* __fastcall TGeraLisParamEqpto::BuscaEqpto(AnsiString EqptoCodBase, AnsiString CodigoCircuito)
	{
	VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	VTEqpto *eqpto;
	VTCarga *carga;
	TList  *lisRede;

	lisRede = redes->LisRede();
	//Busca ponteiro eqppto
	//Se CodCircuito != "",
	//		Busca rede MT , se rede != NULL entao {busca chave, busca carga}
	//Se não encontrou ou se CodCircuito == "",
	//		Busca chave, busca carga nas redes carregadas


	//Busca rede por CodigoCircuito
	if(CodigoCircuito != "")
	{
	rede = BuscaRede(CodigoCircuito);

	if(rede == NULL)
	  {
	  return(NULL);
	  }

	lisEqpto->Clear();
	rede->LisEqpto(lisEqpto, eqptoCHAVE);
	eqpto = NULL;
	for(int neq = 0; neq < lisEqpto->Count; neq++)
		{
		eqpto = (VTEqpto*)lisEqpto->Items[neq];
		if(eqpto->Codigo.AnsiCompare(EqptoCodBase.Trim()) == 0) break;
		eqpto = NULL;
		}

	if(eqpto != NULL)
		 {
			return(eqpto);
		 }

	lisEqpto->Clear();
	rede->LisEqbar(lisEqpto, eqptoCARGA);
	carga = NULL;
	for(int nc = 0; nc < lisEqpto->Count; nc++)
		  {
		  carga = (VTCarga*)lisEqpto->Items[nc];
		  if(carga->Codigo.AnsiCompare(EqptoCodBase.Trim()) == 0) break;
		  carga = NULL;
		  }
	if(carga != NULL)
		 {
		  return((VTEqpto*)carga);
		 }
	}

	////////////////////////////////////////////////////
	//Busca em todas as redes caso nao encontre em codigoCircuito
	//no caso de transferencia - pode ser bom?
	lisEqpto->Clear();
	redes->LisEqpto(lisEqpto, eqptoCHAVE);
	eqpto = NULL;
	for(int neq = 0; neq < lisEqpto->Count; neq++)
		{
		eqpto = (VTEqpto*)lisEqpto->Items[neq];
		if(eqpto->Codigo.AnsiCompare(EqptoCodBase.Trim()) == 0) break;
		eqpto = NULL;
		}

	if(eqpto != NULL)
		 {
			return(eqpto);
		 }

	lisEqpto->Clear();
	redes->LisEqbar(lisEqpto, eqptoCARGA);
	carga = NULL;
	for(int nc = 0; nc < lisEqpto->Count; nc++)
		  {
		  carga = (VTCarga*)lisEqpto->Items[nc];
		  if(carga->Codigo.AnsiCompare(EqptoCodBase.Trim()) == 0) break;
		  carga = NULL;
		  }
	if(carga != NULL)
		 {
		  return((VTEqpto*)carga);
		 }

	return(NULL);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraLisParamEqpto::GeraTemposBarraOc(void)
	{
	VTBarraOc *barraOc;

	//VTOcorrGeral   *ocorr;//VTOcorr   *ocorr; hk201612: compatibilização extrator geral
   VTOcorr *ocorr; //hk201705 : alterações provisórias para alocação IF
	unsigned short year, month, day;
	double daux, taux;

   double dconta, tpre, tdlo, texe; //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
   double ptpre, ptdlo, ptexe; //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
   double dh1, dh2, dh3, dh4;


	//ETAPA 6: PARAMOCORRENCIAS: GERA TEMPOS MÉDIOS DE ATENDIMENTO POR EQUIPAMENTO
	//Define a duracao dos atendimentos por equipamento
	// barraOc->Clih = Soma(barraOc->LisTrataOcorr->ocorr->Clientes * ocorr->Duracao);
	// barraOc->Cli  = Soma(barraOc->LisTrataOcorr->ocorr->Clientes);
	// barraOc->CLIHMes[month - 1] = SomaMonth(barraOc->LisTrataOcorr->ocorr->Clientes * ocorr->Duracao);
	// barraOc->CLIMes[month - 1]  = SomaMonth(barraOc->LisTrataOcorr->ocorr->Clientes);
	// barraOc->TdeslMedio = (barraOc->Clih* 60/ barraOc->Cli)
	// barraOc->DuraTotal =  (barraOc->NOcorNRamal + barraOc->NOcorRamal) * barraOc->TdeslMedio;
	// barraOc->TDMes[nm] = (barraOc->CLIHMes[nm]* 60/ barraOc->CLIMes[nm])


	double nOcorrEquipamento, cli;
	for(int nb = 0; nb < lisBarraOc->Count; nb++)
		{
		barraOc = (VTBarraOc*)lisBarraOc->Items[nb];
		barraOc->Cli = 0.0;
		barraOc->Clih= 0.0;
		for(int nm = 0; nm < 12; nm++)
			{barraOc->CLIHMes[nm] = 0.0;
			 barraOc->CLIMes[nm]  = 0.0;
			 barraOc->TDMes[nm]   = 0.0;
			}

		nOcorrEquipamento = 0.0;
		cli = 0.0;
		taux = 0.0; //hk201703: Correção do tempo de atendimento: acréscimo de paramEqpto->tat: apresentação no módulo de gerenciamento de ocorrencias
      dconta = tpre  = tdlo  = texe = 0.0; //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
      ptpre  = ptdlo = ptexe = 0.0; //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)

		//Gera o tempo médio em todas as etapas da ocorrencia
		for(int noc = 0; noc < barraOc->LisTrataOcorr->Count; noc++) ////barraOc->LisTrataOcorr <--eqptoOc->ListaOcorr() - copia exata
			{
			//hk201612: compatibilização extrator geral ocorr = (VTOcorr*)barraOc->LisTrataOcorr->Items[noc];
         //hk201705 : alterações provisórias para alocação IF
			//ocorr = (VTOcorrGeral*)barraOc->LisTrataOcorr->Items[noc];
         ocorr = (VTOcorr*)barraOc->LisTrataOcorr->Items[noc];

			cli += ocorr->Cliente;//hk201612: compatibilização extrator geral cli += ocorr->Clientes;
			//hkmar Busca o número máximo de clientes de barraOc
			//hkmar if(ocorr->Clientes > barraOc->Cli) barraOc->Cli = ocorr->Clientes;
			//hk201612: compatibilização extrator geral
			//barraOc->Clih += (ocorr->Clientes * ocorr->Duracao);
			daux = ocorr->ClienteHora * 60;
			barraOc->Clih += daux;
			ocorr->DhInicial.DecodeDate(&year, &month, &day);//hk201612: compatibilização extrator geral ocorr->DH_Interrupcao.DecodeDate(&year, &month, &day);
			barraOc->CLIHMes[month - 1] += daux; //hk201612: compatibilização extrator geral barraOc->CLIHMes[month - 1] += (ocorr->Clientes * ocorr->Duracao);
			barraOc->CLIMes[month - 1]  += ocorr->Cliente;//hk201612: compatibilização extrator geral barraOc->CLIMes[month - 1]  += ocorr->Clientes;
			if(barraOc->LisOcorrF->IndexOf(ocorr) != -1)  //hk201703: Correção do tempo de atendimento: acréscimo de paramEqpto->tat: apresentação no módulo de gerenciamento de ocorrencias
			  {
			  if(ocorr->Cliente > 0)
				  {taux += (ocorr->ClienteHora / (double)ocorr->Cliente);
               //hk201705 : alterações provisórias para alocação IF
               dh1 = (double)(ocorr->DhInicial);
               dh2 = (double)(ocorr->DhAlocacao);
               dh3 = (double)(ocorr->DhLocalizacao);
               dh4 = (double)(ocorr->DhFinal);
               if((!IsDoubleZero(dh1))&&(!IsDoubleZero(dh2))&&(!IsDoubleZero(dh3))&&(!IsDoubleZero(dh4)))
                 {
                 dconta += 1.0;
                 daux = max(((double)(ocorr->DhAlocacao    - ocorr->DhInicial)) * 24 * 60, 0.0);
                 tpre += daux;
                 daux = max(((double)(ocorr->DhLocalizacao - ocorr->DhAlocacao))    * 24 * 60, 0.0);
                 tdlo += daux;
                 daux = max(((double)(ocorr->DhFinal       - ocorr->DhLocalizacao)) * 24 * 60, 0.0);
                 texe += daux;
                 }
               }
			  }
			nOcorrEquipamento++;
			}
		if(barraOc->LisOcorrF->Count > 0) barraOc->Tat = (taux / (double)barraOc->LisOcorrF->Count); //hk201703: Correção do tempo de atendimento: acréscimo de paramEqpto->tat: apresentação no módulo de gerenciamento de ocorrencias
      //hk201705 : alterações provisórias para alocação IF
      if(dconta > 0)
        {
        barraOc->Ptpre = (tpre/ dconta);
        barraOc->Ptdlo = (tdlo/ dconta);
        barraOc->Ptexe = (texe/ dconta);
        daux = barraOc->Ptpre + barraOc->Ptdlo + barraOc->Ptexe;
        if(daux > 0)
          {
          barraOc->Ptpre /= daux;
          barraOc->Ptdlo /= daux;
          barraOc->Ptexe /= daux;
          }
        }
		barraOc->Cli  = cli;//hkmar
		barraOc->Clih /= 60.0;

		barraOc->EqptoOc->QtdOcorrEqpto = nOcorrEquipamento;


		for(int nm = 0; nm < 12; nm++) barraOc->CLIHMes[nm] /= 60.0;

		barraOc->TdeslMedio= 0.0;
		barraOc->DuraTotal = 0.0;
		if(cli > 0)
		  {barraOc->TdeslMedio = (barraOc->Clih* 60/ cli);
			barraOc->DuraTotal =  (barraOc->NOcorNRamal + barraOc->NOcorRamal) * barraOc->TdeslMedio;//hkmar
			//hkmar barraOc->DuraTotal  =  barraOc->NOcorPond * barraOc->TdeslMedio;//barraOc->NOcorPond * barraOc->TdeslMedio;
			for(int nm = 0; nm < 12; nm++)
				{
				if(barraOc->CLIMes[nm] > 0)
					barraOc->TDMes[nm] = (barraOc->CLIHMes[nm]* 60/ barraOc->CLIMes[nm]);
				}
		  }
		  //barraOc->DuraTotal = barraOc->NOcor * (barraOc->Clih / cli);
			barraOc->EqptoOc->QtdOcorr = lisOc->Count;

		}



	return(true);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TGeraLisParamEqpto::GetFaseFromBloco(VTBloco *bloco)
{
	/*
	blocos tem lista de ligações.
	Ligações podem ser trechos e trafos (são os objetos que eu preciso pra checar as fases)
	CASE trecho: trecho->Arranjo->fase (olhar VTArranjo). pegar VTFases do APL.
		 		 Fazer fases->numeroDeFases(trecho->Arranjo->fase) (retorna um INT, 1, 2 ou 3) contar quantos de cada existem.
	CASE trafo: trafo->monofasico (retorna bool) contar quantos monofásicos e quantos não-monofásicos existem.
				(Não-monofásicos são trifásicos)
	Se trifásicos > 0, então botar trifásico no resultado. (RETURN aqui)
	Se bifásico > 0, então botar bifásico no resultado. (RETURN aqui)
	Se monofasico > 0, então botar monofasico no resultado. (RETURN aqui)
	Se der ruim total, retornar "Indefinido"
	*/
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	int monofasicoCount = 0;
	int bifasicoCount = 0;
	int trifasicoCount = 0;

	for(int i = 0; i < bloco->LisLigacao()->Count; i++)
	{
		VTLigacao *ligacao = (VTLigacao*)bloco->LisLigacao()->Items[i];
		if(ligacao->Tipo() == eqptoTRECHO)
		{
			VTTrecho *trecho = (VTTrecho*)ligacao;
			int fase = fases->NumeroDeFases(trecho->arranjo->Fases);
			if(fase == 1)
			{
				monofasicoCount++;
			}
			if(fase == 2)
			{
				bifasicoCount++;
			}
			if(fase == 3)
			{
				trifasicoCount++;
			}
		}
		if(ligacao->Tipo() == eqptoTRAFO)
		{
			VTTrafo *trafo = (VTTrafo*)ligacao;
			if(trafo->Monofasico)
			{
				monofasicoCount++;
			}
			else
			{
            	trifasicoCount++;
			}
		}
	}

	if(trifasicoCount > 0)
	{
		return "Trifásico";
	}
	if(bifasicoCount > 0)
	{
		return "Bifásico";
	}
	if(monofasicoCount > 0)
	{
		return "Monofásico";
	}

	return "Indefinida";

}

//---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TGeraLisParamEqpto::BuscaParamEqpto(VTEqpto *eqptoOcorr, TList *lisParamEqptoExterno)
	{
	strDadoParamEqpto *paramEqpto;

	paramEqpto = NULL;
	for(int n = 0; n < lisParamEqptoExterno->Count; n++)
		{
		paramEqpto = (strDadoParamEqpto*)lisParamEqptoExterno->Items[n];
		if(paramEqpto->carga == eqptoOcorr) break;
		if(paramEqpto->eqpto == eqptoOcorr) break;
		paramEqpto = NULL;
		}
	return(paramEqpto);
	}
//---------------------------------------------------------------------------
VTRede* __fastcall TGeraLisParamEqpto::BuscaRedeMT_CargaBT(VTCarga *cargaBT, TList *lisAux)
{
    VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
    VTLigacao *ligacao;
    lisAux->Clear();
    redes->LisLigacao(lisAux, cargaBT->pbarra);
    if(lisAux->Count == 0)
        return(NULL);

    return(((VTLigacao*)lisAux->First())->rede);
}
//---------------------------------------------------------------------------
bool __fastcall TGeraLisParamEqpto::GeraLisParamEqptoBdado(TList *lisOc, TList *lisEqptoOc, TList *lisParamEqpto, int ndiasObsExt)
	{
	this->lisParamEqpto = lisParamEqpto;
	this->lisEqptoOc    = lisEqptoOc;
	this->lisOc         = lisOc;
	//this->lisCircuitos  = lisCircuitos;


	LimpaTList(lisParamEqpto);

	//ImprimeLisOc();

	//Etapa 1: define número de dias observados em lisOc
	//funcao de lisOc->ocorr
	//diaMin   = Min(ocorr->DH_Interrupcao)
	//diaMax   = Max(ocorr->DH_Interrupcao)
	//ndiasObs = diaMax - diaMin;

	if(ndiasObsExt < 0)
		ndiasObs = DefineNDiasObs();
   else

		ndiasObs = ndiasObsExt;
	//Etapa 2: Mapeia EqptoOc e eqpto s da rede, lisEqptoOc->EqptoOc->Eqpto
	//GeraEqptoRede();
	//ETAPA 4: PARAMOCORRENCIAS: CRIA LISTA DE BARRAOC, LISBARRAOC
	//EM FUNCAO DE lisEqptoOc
	//Preenche lisBarraOc->barraOc = NewObjBarraOc, para cada lisEqptoOc->eqptoOc
	//barraOc->Codigo   = eqptoOc->EqptoCodBase
	//barraOc->EqptoOc  = eqptoOc
	//barraOc->ndiasObs = ndiasObs
	//barraOc->LisTrataOcorr <--eqptoOc->ListaOcorr() - copia exata
	//barraOc->LisCircuitos  <--eqptoOc->ListaCircuitos() - copia exata
	GeraLisBarraOcBdado(lisEqptoOc, lisBarraOc);   //hk201702: integração simTurmas
	//ETAPA 5: PARAMOCORRENCIAS: GERA DISTRIBUIÇÃO DE FALHAS POR PATAMAR HORÁRIO
	//Gera ocorrencias com o início e na mesma barra
	//barraOc->LisOcorrF = f(barraOc->LisTrataOcorr)
	//Gera barraOc->NFHora[h] e barraOc->NFMes[m] e barraOc->NFRMes[m]  distFalhas[hour]
	//Calcula
	//		barraOc->LisOcorrF->ocorr : lista de ocorrencias sem repeticao - fechadas, 1 ocorrencia = 1 evento falha
	//		barraOc->NOcorRamal : ramal de ligacao no cliente
	//		barraOc->NOcorNRamal: não ramal de ligacao - chave MT ou trafo BT
	//    barraOc->NFHora[hour]
	//    barraOc->NFMes[month]
	//    barraOc->NFRMes[month]
	//Funcao de
	//    lisBarraOc->barraOc->LisTrataOcorr
	//		ocorr->FlagRamal
	//		ocorr->Clientes
	//		ocorr->DH_Interrupcao
	GeraDistribFalhasPatamar();//Bdado();

	//ETAPA 8:Gera barraOc->NOcorPond
	//GeraNFalhasBarraOc();

	//ETAPA 6: PARAMOCORRENCIAS: GERA TEMPOS MÉDIOS DE ATENDIMENTO POR EQUIPAMENTO
	//Define a duracao dos atendimentos por equipamento
	// barraOc->Clih = Soma(barraOc->LisTrataOcorr->ocorr->Clientes * ocorr->Duracao);
	// barraOc->Cli  = Soma(barraOc->LisTrataOcorr->ocorr->Clientes);
	// barraOc->CLIHMes[month - 1] = SomaMonth(barraOc->LisTrataOcorr->ocorr->Clientes * ocorr->Duracao);
	// barraOc->CLIMes[month - 1]  = SomaMonth(barraOc->LisTrataOcorr->ocorr->Clientes);
	// barraOc->TdeslMedio = (barraOc->Clih* 60/ barraOc->Cli)
	// barraOc->DuraTotal =  (barraOc->NOcorNRamal + barraOc->NOcorRamal) * barraOc->TdeslMedio;
	// barraOc->TDMes[nm] = (barraOc->CLIHMes[nm]* 60/ barraOc->CLIMes[nm])
	GeraTemposBarraOc();


	//ETAPA 7: Inicia lisParamEqpto = f(lisBarraOc)
	//Geração de lisParamEqpto
	//lisParamEqpto->strDadoParamEqpto->nfalhas, durTotal, ndiasObs
	//GERA:
	//lisParamEqpto->paramEqpto = f(lisBarraOc->barraOc)
	//ONDE:
	//paramEqpto->barraOcOrig = (TObject*)barraOc;
	//paramEqpto->codEqpto     = barraOc->Codigo;
	//paramEqpto->codRede      = circuito->CodigoRede;
	//paramEqpto->ndiasObs     = barraOc->NDiasObs
	//paramEqpto->nf = barraOc->NOcorNRamal;
	//paramEqpto->nfr= barraOc->NOcorRamal;
	//paramEqpto->td = barraOc->TdeslMedio;
	//paramEqpto->cli = barraOc->Cli;
	//paramEqpto->clih= barraOc->Clih;
	//paramEqpto->nfhora[np] = barraOc->NFHora[np];
	//paramEqpto->nfrhora[np]= 0
	//paramEqpto->nfmes[nm]  = barraOc->NFMes[nm];
	//paramEqpto->nfrmes[nm] = barraOc->NFRMes[nm];
	//paramEqpto->tdmes[nm]  = barraOc->TDMes[nm];
	//paramEqpto->climes[nm] = barraOc->CLIMes[nm];  //hkmar
	//paramEqpto->clihmes[nm]= barraOc->CLIHMes[nm]; //hkmar
	//
	//paramEqpto->rede         = NULL
	//paramEqpto->tipoRede     = ""
	//paramEqpto->eqpto        = NULL
	//paramEqpto->carga        = NULL;
	//paramEqpto->nFalhasEquip = 0.0;
	//paramEqpto->durTotal     = 0.0;
	GeraParamEqptoBdado();


	//ETAPA 8: Inicia lisParamRede = f(lisParamEqpto) e
	//Geração de lisParamRede->(nfalhas, durTotal, ndiasObs)
	//if(lisParamRede != NULL)
	//		  GeraParamRedeBdado();

	//teste para geração de lisParamEqpto->paramEqpto para simulação
	//PreparaLisParamEqptoRede(lisParamEqpto);

	//ImprimeLisParamEqpto("lisParamEqpto000.txt");

	return(true);
	}
//---------------------------------------------------------------------------
double __fastcall TGeraLisParamEqpto::ComprimentoRede(VTRede *rede)//ssai
	{
	TList *lisLiga;
	VTEqpto *pliga;
	double    comp_km = 0.0;

	if(rede == NULL) return(0.0);

	lisLiga = rede->LisLigacao();

	for(int n = 0; n < lisLiga->Count; n++)
		{
		pliga = (VTEqpto*)lisLiga->Items[n];
		if(pliga->Tipo() != eqptoTRECHO) continue;
		comp_km += ((VTTrecho*)pliga)->Comprimento_km;
		}
	return(comp_km);
	}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall TGeraLisParamEqpto::GeraNFalhasBarraOc(void)
	{
	//Gera
	//		lisBarraOc->
	//		barraOc->NOcorPond
	//		barraOc->RedePri
	//		barraOc->NConsBT
	//Funcao de
	//		barraOc->EqptoOcorr
	//		barraOc->NOcorNRamal
	//		barraOc->NOcorRamal


	VTBarraOc *barraOc;
	double    nconsBT;
	TList     *lisAux;

	lisAux = new TList();
	for(int nb = 0; nb < lisBarraOc->Count; nb++)
		{
		barraOc = (VTBarraOc*)lisBarraOc->Items[nb];
		barraOc->NOcorPond = 0;

		if(barraOc->EqptoOc->Eqpto == NULL) continue;

		//Busca rede primaria carregada
		if(barraOc->EqptoOc->Eqpto->Tipo() == eqptoCHAVE)
			  barraOc->RedePri = ((VTChave*)barraOc->EqptoOc->Eqpto)->rede;
		else
			 {barraOc->RedePri = BuscaRedeMT_CargaBT((VTCarga*)barraOc->EqptoOc->Eqpto, lisAux);
			  //Calcula número de consumidores BT hknova
			  nconsBT = ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsA4;
			  nconsBT += ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsComercial;
			  nconsBT += ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsIndustrial;
			  nconsBT += ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsOutros;
			  nconsBT += ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsResidencial;
			  nconsBT += ((VTCarga*)barraOc->EqptoOc->Eqpto)->NumConsRural;
			  barraOc->NConsBT = nconsBT;
			  }

		nconsBT = barraOc->NConsBT;
		barraOc->NOcorPond = barraOc->NOcorNRamal;
		if((nconsBT > 0)&&(barraOc->NOcorRamal > 0))
		  barraOc->NOcorPond += (barraOc->NOcorRamal / nconsBT);
		}
	delete lisAux;
	}
*/
