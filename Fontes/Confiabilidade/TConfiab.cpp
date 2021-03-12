//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplConfiab.h"
#include "TConfiab.h"
#include "TDataEqpto.h"
#include "TDataRede.h"
#include "TEqptoMutua.h"
#include "VTCfgGrafConfiab.h"
#include "VTDadoConfiab.h"
#include "GeraCorte\TGeraCorte.h"
#include "GeraCorte\TCorteConf.h"
#include "CalcIndices\VTCalcIndCont.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Diretorio\VTPath.h"
#include "..\Diretorio\DirArq.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\GeraPath\VTGeraPath.h"
#include "..\GeraPath\VTArvore.h"
#include "..\GeraPath\VTArvores.h"
//#include "..\GeraPath\VTEqptoPath.h"
#include "..\GeraPath\TEqptoPath.h"
#include "..\GeraRede\VTGeraRede.h"
#include "..\GeraRede\TLigaSerie.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTConfiab* __fastcall NewObjConfiab(VTApl *apl_owner)
	{
	return(new TConfiab(apl_owner));
	}
//---------------------------------------------------------------------------
__fastcall TConfiab::TConfiab(VTApl *apl_owner)
	{
	//salva ponteiros
	this->apl   = new TAplConfiab(NULL, apl_owner);
	dataConfiab = (VTDadoConfiab*) apl->GetObject(__classid(VTDadoConfiab));
	gerapath    = (VTGeraPath*)    apl->GetObject(__classid(VTGeraPath));

	//Lista que armazena arvores
	lisArv  = new TList();
	//Lista com objetos com parametros por equipamento
	lisLiga = new TList();

	//lista auxiliar
	lisEqpto = new TList();

	lisGrupoCargas = new TList();//Lista com agrupamentos de cargas
	lisSUP         = new TList();//Lista com suprimentos

	//Inicia barra de progresso
	prog = NULL;

	//Inicia parametros de confiabilidade
	//IniciaParamConf(this->dataConfiab);
	LeArquivos("");
	}
//---------------------------------------------------------------------------
__fastcall TConfiab::~TConfiab(void)
	{
	GravaArqParam();

	if (apl)           {delete apl; apl = NULL;}
	if(lisArv)         {LimpaTList(lisArv); delete lisArv; lisArv = NULL;}
	if(lisEqpto)       {delete lisEqpto; lisEqpto = NULL;}
	if(lisGrupoCargas) {LimpaTList(lisGrupoCargas); delete lisGrupoCargas; lisGrupoCargas = NULL;}
	if(lisSUP)         {delete lisSUP; lisSUP = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::AddLigaExc(VTLigacao* pliga, bool flag_12)
	{
	gerapath->AddLigaExc(pliga, flag_12);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::AgregaRes(RES_CONF &res_conf, VTEqpto* eqpto)
	{
	VTCalcIndCont* calcind =  (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	TList    *lisAux, *lisAux1;
	int      iaux;
	bool     resposta;


	lisAux = new TList();
	iaux = lisEqpto->IndexOf(eqpto);
	if(!(iaux < 0)) lisAux->Add(lisArv->Items[iaux]);
	resposta = calcind->AgregaRes(res_conf, lisAux);
	delete lisAux;
	return(resposta);
   }
//---------------------------------------------------------------------------
bool __fastcall TConfiab::AgregaRes(RES_CONF &res_conf, TList* lisCargas)
   {
	VTCalcIndCont* calcind =  (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
   TList* lisAux;
   VTEqpto* eqpto;
   int      iaux;
	bool     resposta;
	VTArvores* arvs;//VTArvore* arv;

	lisAux = new TList();
	for(int n = 0; n < lisCargas->Count; n++)
		{
		iaux = lisEqpto->IndexOf(lisCargas->Items[n]);
		if(iaux < 0) continue;
		arvs = (VTArvores*)lisArv->Items[iaux];//arv = (VTArvore*)lisArv->Items[iaux];

		if(arvs->EqRaiz() != lisCargas->Items[n])
			int jjj = 0;


		lisAux->Add(lisArv->Items[iaux]);
		}
	resposta = calcind->AgregaRes(res_conf, lisAux);
	delete lisAux;
	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::AgregaRes(RES_CONF &res_conf, VTBloco* bloco)
	{
	VTCalcIndCont* calcind =  (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	TList* lisAux;
	VTEqpto* eqpto;
	int      iaux;

	bool     resposta;
	TList    *lisBarBl;
	TList    *lisCargas;
	VTCarga  *carga;

	lisAux = new TList();
	bloco->LisEqbar(lisAux, eqptoCARGA);

	lisBarBl = bloco->LisBarra();
	for(int n = 0; n < lisGrupoCargas->Count; n++)
		{
		lisCargas = (TList*)lisGrupoCargas->Items[n];
		for(int nc = 0; nc < lisCargas->Count; nc++)
			{
			carga = (VTCarga*)lisCargas->Items[nc];
			if(lisAux->IndexOf(carga) != -1) continue;
			if(lisBarBl->IndexOf(carga->pbarra) == -1) continue;
			lisAux->Add(carga);
			}
		}

	resposta = AgregaRes(res_conf, lisAux);
	delete lisAux;

	return(resposta);
	}
/*//---------------------------------------------------------------------------
void __fastcall TConfiab::DefSuprimento(VTEqpto* eqpto)
	{

	//Popula gerapath->lisFontes
	if(!eqpto)
	  gerapath->AddFonte(NULL);
	else
	  gerapath->AddFonte(eqpto);
	}
*/
//---------------------------------------------------------------------------
VTDadoConfiab* __fastcall TConfiab::DadoConfiab(void)
	{
	return(dataConfiab);
	}
//---------------------------------------------------------------------------
VTDadoConfiab* __fastcall TConfiab::DadoConfiabNovo(void)
	{
	VTDadoConfiab* dataNovo;
	dataNovo = NewObjDadoConfiab();
	dataNovo->CopiaDados(dataConfiab);//hknnn
	return(dataNovo);
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::DefDadoConfiab(VTDadoConfiab* dataExt)
	{
	dataConfiab->CopiaDados(dataExt);
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::DefParEqpto(PAR_CONF pconf, VTEqpto* eqpto)
	{
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));

	dataConfiab->DefParamEqpto(pconf, eqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::DefProgresso(VTProgresso  *prog)
	{
	this->prog = prog;
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::DefineCargas(void)
	{
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	VTRedes*      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCarga*      carga;
	TList        *lisAux, *lisAux2, *lisBloco;
	VTBlocos     *blocos;
	VTBloco      *bloco;
	VTDadoConfiab *dataAtual;

	dataAtual = dataConfiab;//confiab->DadoConfiab();
	LimpaTList(lisGrupoCargas);
	if(dataAtual->flag_BLOCO)
	  {
	  //Cria objeto blocos
	  blocos = DLL_NewObjBlocos();
	  blocos->Executa(redes);
	  lisBloco = blocos->LisBloco();
	  for(int nb = 0; nb < lisBloco->Count; nb++)
		  {
		  bloco = (VTBloco*)lisBloco->Items[nb];
		  lisGrupoCargas->Add(lisAux2 = new TList());
		  bloco->LisEqbar(lisAux2, eqptoCARGA);
		  if(lisAux2->Count > 0) continue;
		  lisGrupoCargas->Remove(lisAux2);
		  delete lisAux2;
		  }
	  }
	else
	  {
	  lisAux  = new TList();
	  redes->LisEqpto(lisAux, eqptoCARGA);
	  for(int nc = 0; nc < lisAux->Count; nc++)
			{
			carga = (VTCarga*)lisAux->Items[nc];
			lisGrupoCargas->Add(lisAux2 = new TList());
			lisAux2->Add(carga);
			}
	  delete lisAux;
	  }
	}

//---------------------------------------------------------------------------
void __fastcall TConfiab::DefineSuprimentos(void)
	{
	//variáveis locais
	VTConsulta   *consulta = DLL_NewObjConsulta(apl);
	VTRedes       *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTReducao     *redu;
	TList         *lisFontes, *lisBarra, *lisRedu;

	//Inicia lista de suprimentos
	lisSUP->Clear();

	lisFontes = consulta->LisEqptoRede(redes, eqptoGERADOR);
	lisSUP->Assign(lisFontes, laOr);
	lisFontes = consulta->LisEqptoRede(redes, eqptoSUPRIMENTO);
	lisSUP->Assign(lisFontes, laOr);
	//REDUÇÕES COM SUPRIMENTO
	lisRedu = new TList();
	redes->LisReducao(lisRedu);
	for(int nr = 0; nr < lisRedu->Count; nr++)
		{
		redu = (VTReducao*)lisRedu->Items[nr];
		if(!redu->ExisteGeracao()) continue;
		lisBarra = redu->LisBarra();
		for(int nbar = 0; nbar < lisBarra->Count; nbar++)
			lisSUP->Add(lisBarra->Items[nbar]);
		}
	if(lisRedu) delete lisRedu;
	if(consulta) delete consulta;
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::ExecutaAjParamRede(double prec)
	{
	TDataRede *dataRede;
	TList     *lisAux;
	VTEqpto   *eqpto;
	RES_CONF  presconf;
	PAR_CONF  pconf;
	double    fat_fec, fat_dec;
	bool      resposta = true;



	lisAux = new TList();
	for(int n = 0; n < dataConfiab->LisDataAjusteRede->Count; n++)
		{
		dataRede = (TDataRede*)dataConfiab->LisDataAjusteRede->Items[n];
		//lisAux->Clear();
		//dataRede->rede->LisEqpto(lisAux, eqptoLIGACAO);

		/*FILE *fp;
		AnsiString arq;
		VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
		VTEqpto *eqpto_aux;
		PAR_CONF par_aux;
		arq = path->DirTmp() + "\\param.txt";
		fp = fopen(arq.c_str(), "wt");
		for(int neq =0; neq < lisAux->Count; neq++)
			{
			eqpto = (VTEqpto*)lisAux->Items[neq];
			fprintf(fp, "---------------------------------------\n");
			fprintf(fp, "Eqpto: %s\n", eqpto->Codigo);
			fprintf(fp, "---------------------------------------\n");
			dataConfiab->ParamEqpto(pconf, eqpto);
			for(int nl = 0; nl < lisLiga->Count; nl++)
				{eqpto_aux = (VTEqpto*)lisLiga->Items[nl];
				 dataConfiab->ParamEqpto(par_aux, eqpto_aux);
				 fprintf(fp, "%s\t%6.2f\t%6.2f\t", eqpto_aux->Codigo, par_aux.tx_falha, par_aux.treparo);
				 fprintf(fp, "%6.2f\t%6.2f\n", par_aux.tx_interr, par_aux.tman);
				}
			fprintf(fp, "---------------------------------------\n");
			fprintf(fp, "---------------------------------------\n");

			dataConfiab->DefParamEqpto(pconf, eqpto);
			for(int nl = 0; nl < lisLiga->Count; nl++)
				{eqpto_aux = (VTEqpto*)lisLiga->Items[nl];
				 dataConfiab->ParamEqpto(par_aux, eqpto_aux);
				 fprintf(fp, "%s\t%6.2f\t%6.2f\t", eqpto_aux->Codigo, par_aux.tx_falha, par_aux.treparo);
				 fprintf(fp, "%6.2f\t%6.2f\n", par_aux.tx_interr, par_aux.tman);
				}
			}

		fclose(fp);
		break;
		*/

		lisAux->Clear();
		dataRede->rede->LisEqpto(lisAux, eqptoCARGA);
		AgregaRes(presconf, lisAux);
		//Ajusta parametros de falha e interrupção dos equipamentos da rede
		fat_fec = dataRede->pres.fec / presconf.fec;
		fat_dec = dataRede->pres.dec / (presconf.dec * fat_fec);

		if(fabs(fat_fec -1.) > prec) resposta = false;
		if(fabs(fat_dec -1.) > prec) resposta = false;
		//resposta = false;
		//if(resposta == true) continue;

		dataRede->pres.fat_dec *= fat_dec;
		dataRede->pres.fat_fec *= fat_fec;

		lisAux->Clear();
		dataRede->rede->LisEqpto(lisAux, eqptoLIGACAO);
		for(int neq =0; neq < lisAux->Count; neq++)
			{
			eqpto = (VTEqpto*)lisAux->Items[neq];
			dataConfiab->ParamEqpto(pconf, eqpto);
			pconf.tx_falha *= fat_fec;
			pconf.tx_interr*= fat_fec;
			pconf.treparo  *= fat_dec;
			pconf.tman     *= fat_dec;
			dataConfiab->DefParamEqpto(pconf, eqpto);
			}

		}
	delete lisAux;
	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::ExecutaCortesSC(void)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	TList     *lisCargas;
	TList     *lisTipoProt;
	VTEqpto   *eqpto;
	AnsiString      aux;
	int iaux;

	VTArvores *arvs, *arvs_aux;


	//Inicia DESTINOS(Cargas) dos caminhos
	gerapath->AddFonte(NULL);//DefSuprimento(NULL);
	for(int n = 0; n < lisGrupoCargas->Count; n++)
		 {
		 lisCargas = (TList*)lisGrupoCargas->Items[n];
		 //Gera arvores para o primeira carga do grupo
		 eqpto = (VTEqpto*)lisCargas->First();
       //AddFonte(eqpto): lisFontes->Add(eqpto)
		 gerapath->AddFonte(eqpto);//DefSuprimento(eqpto);
		 }

	if(prog) prog->Start(progTEXTO);
	if(prog) prog->Add("Gerando cortes mínimos");
	//Inicia lista com tipos de protecao
	lisTipoProt = gerapath->LisTipoProt();
	lisTipoProt->Clear();
	lisTipoProt->Assign(dataConfiab->lisTipoChv);

	//com inversao dos arcos excluidos flag_INV = true
	gerapath->GeraArvore(lisArv, lisSUP, true);



   //hk201711mutua: criar arvores para equipamentos mutua
   //TEqptoMutua *eqptoMutua;
   //eqptoMutua->eqpto1;
   //eqptoMutua->eqpto2;


   //adiciona a arvs->LisEqpto
   //adiciona a arvs->LisEqptoPath
   //arvs->LisPath(false, eqpto1);
   //arvs->LisPathProt(eqpto1)
   //iaux = arvs-->lisEqpto->IndexOf(eqpto);
   //eqpath = (TEqptoPath*)arvs-->lisEqptoPath->Items[iaux];
   TList     *lisEqpto, *lisEqptoPath, *lisPath, *lisPathProt;
   TList     *lisPath1, *lisPath2;
   TEqptoMutua *eqptoMutua;
   //VTEqptoPath *eqptoPath;
   TEqptoPath *eqptoPath;
   VTEqpto     *eqpto1, *eqpto2;
   for(int nm = 0; nm < dataConfiab->LisEqptoMutua->Count; nm++)
      {
      eqptoMutua = (TEqptoMutua*)dataConfiab->LisEqptoMutua->Items[nm];
      eqpto1 = eqptoMutua->eqpto1;
      eqpto2 = eqptoMutua->eqpto2;
	   for(int n = 0; n < lisArv->Count; n++)
		   {
		   arvs = (VTArvores*)lisArv->Items[n];
		   if(arvs->arvs_ori != NULL) continue;
         lisEqpto = arvs->LisEqpto();
		 //arvs->EqRaiz()->Codigo;
         //if(!((lisEqpto->IndexOf(eqpto1) != -1)&&(lisEqpto->IndexOf(eqpto2) != -1))) continue;
         if((lisEqpto->IndexOf(eqpto1) == -1)&&(lisEqpto->IndexOf(eqpto2) == -1)) continue;

         lisEqpto->Add(eqptoMutua);
         lisEqptoPath = arvs->LisEqptoPath();
         //lisEqptoPath->Add(eqptoPath = gerapath->NovoEqptoPath(eqptoMutua));
         lisEqptoPath->Add(eqptoPath = new TEqptoPath(eqptoMutua));

         lisPath = eqptoPath->LisPath(false);
         if((lisPath1 = arvs->LisPath(false, eqpto1)) != NULL)
            lisPath->Assign(lisPath1, laOr);
         if((lisPath2 = arvs->LisPath(false, eqpto2)) != NULL)
            lisPath->Assign(lisPath2, laOr);

         lisPathProt = eqptoPath->LisPathProt();
         if((lisPath1 = arvs->LisPathProt(eqpto1)) != NULL)
            lisPathProt->Assign(lisPath1, laOr);
         if((lisPath2 = arvs->LisPathProt(eqpto2)) != NULL)
            lisPathProt->Assign(lisPath2, laOr);
		   }
      }

   //hk201711mutua: acrescenta

	//Executa cortes minimos
	geracorte->ExecutaCorteA(lisArv, dataConfiab->icont);
	//Copia arvores para cargas de lisGrupocargas
	GeraCopiaArvores(lisArv);
	if(prog) prog->Stop();

	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::ExecutaSC(void)
	{
	VTArvores *arvs;
	VTCalcIndCont *calcind =  (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTGeraCorte   *geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));

	//Prepara os cortes a partir dos suprimentos
	ExecutaCortesSC();
	//Executa calculo de indicadores
	ExecutaCalcIndices();

	if(prog) prog->Stop();
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::LeArquivos(AnsiString pathParam)
	{
	AnsiString arqParam;
	bool       resposta = true;

	if(pathParam.AnsiCompare("") == 0)
	  {this->pathParam = LePathParam();} //leitura de tmp\dirConfMalha.tmp
	else
	  {this->pathParam = pathParam;
	  }
	//Leitura de dados gerais
	arqParam = this->pathParam + "parConfRecMalha.txt";
	if(!LeArqParam(arqParam)) resposta = false;

	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::GravaArqParam(void)
	{
	VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TDataEqpto *dataEqpto;
	VTTipoChave* tipoChave;
	AnsiString arq;
	FILE    *fmed;
	int     iaux;

	try
		{
		if(pathParam.AnsiCompare("") == 0)
			arq = path->DirTmp() + "\\parConfRecMalha.txt";
		else
			arq = pathParam + "\\parConfRecMalha.txt";

		//Dados gerais: parâmetros
		fmed = fopen(arq.c_str(), "wt");
		fprintf(fmed, "CONFIABILIDADE_MALHA\n");
		fprintf(fmed, "%s\n", dataConfiab->descricao.c_str());
		fprintf(fmed, "%i; ", dataConfiab->icont);
		iaux = (dataConfiab->flag_BLOCO) ? 1 : 0;
		fprintf(fmed, "%i; ", iaux);
		iaux = (dataConfiab->flag_NA) ? 1 : 0;
		fprintf(fmed, "%i\n", iaux);

		ImprimeDataEqpto(fmed, dataConfiab->DataGeralEqpto);
		ImprimeDataEqpto(fmed, dataConfiab->DataGeralTrecho);


		//Dados gerais: tipos de equipamentos de protecao
		for(int nc = 0; nc < dataConfiab->lisTipoChv->Count; nc++)
			{
			tipoChave = (VTTipoChave*)dataConfiab->lisTipoChv->Items[nc];
			fprintf(fmed, "%s\n", tipoChave->Codigo.c_str());
			}
		fprintf(fmed, "TIPOEQUIPAMENTOPROTECAO\n");

		for(int n = 0; n < dataConfiab->LisDataEqpto->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataEqpto->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATAEQPTO\n");
		for(int n = 0; n < dataConfiab->LisDataRede->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataRede->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATAREDE\n");
		for(int n = 0; n < dataConfiab->LisDataTipo->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataTipo->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATATIPO\n");
		for(int n = 0; n < dataConfiab->LisDataTipoTrecho->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataTipoTrecho->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATATIPOTRECHO\n");
		for(int n = 0; n < dataConfiab->LisDataTipoChave->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataTipoChave->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATATIPOCHAVE\n");
		for(int n = 0; n < dataConfiab->LisDataTipoGerador->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataTipoGerador->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATATIPOGERADOR\n");
		for(int n = 0; n < dataConfiab->LisDataTipoTrafo->Count; n++)
			{
			dataEqpto = (TDataEqpto*)dataConfiab->LisDataTipoTrafo->Items[n];
			ImprimeDataEqpto(fmed, dataEqpto);
			}
		fprintf(fmed, "LISDATATIPOTRAFO\n");


		fclose(fmed);

		//Grava arquivo com diretório para o próximo acesso
		arq = path->DirTmp() + "\\dirConfMalha.tmp";
		fmed = fopen(arq.c_str(), "wt");
		fprintf(fmed, "%s", pathParam.c_str());
		fclose(fmed);
		}
	catch(Exception &e)
		{
		return false;
		}
	return true;
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::ImprimeDataEqpto(FILE *fout, VTDataEqpto *dataEqpto)
	{
	AnsiString aux;

	fprintf(fout, "DataEqpto; ");
	aux = "INDEF";
	if(dataEqpto->eqpto != NULL) aux = ((VTEqpto*)dataEqpto->eqpto)->Codigo;
	fprintf(fout, "%s; ", aux.c_str());
	aux = "INDEF";
	if(dataEqpto->eqpto_aux != NULL) aux = ((VTEqpto*)dataEqpto->eqpto_aux)->Codigo;
	fprintf(fout, "%s; ", aux.c_str());
	fprintf(fout, "%i; ", dataEqpto->itipo);
	fprintf(fout, "%i; ", dataEqpto->itipo_aux);
	fprintf(fout, "%s; ", dataEqpto->codigo.c_str());
	fprintf(fout, "%6.2f; ", dataEqpto->vmin);
	fprintf(fout, "%6.2f; ", dataEqpto->vmax);
	fprintf(fout, "%6.2f; ", dataEqpto->pconf.tx_falha);
	fprintf(fout, "%6.2f; ", dataEqpto->pconf.tx_interr);
	fprintf(fout, "%6.2f; ", dataEqpto->pconf.treparo);
	fprintf(fout, "%6.2f; ", dataEqpto->pconf.tman);
	fprintf(fout, "%6.2f; ", dataEqpto->pconf.comp);
	fprintf(fout, "%6.2f\n", dataEqpto->pconf.fat_isola);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::LeArqParam(AnsiString arqParam)
	{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	bool   resposta = true;
	FILE *fmed;
	AnsiString aux, codTipoChave;
	VTTipoChave *tipoChave;
	bool    flagEOF = false;
	VTDataEqpto *dataEqpto;

	//Reinicia dados
	dataConfiab->Inicia();

	if(arqParam.AnsiCompare("") == 0)
	  {arqParam = path->DirTmp() + "\\parConfRecMalha.txt";
		if(!FileExists(arqParam)) return(false);
	  }
	else
	  {
	  if(!FileExists(arqParam)) return(false);
	  }

	fmed = fopen(arqParam.c_str(), "r");
	if((aux = LeDado(fmed, flagEOF)).AnsiCompare("CONFIABILIDADE_MALHA") != 0)
		{fclose(fmed); return(false);}

	try
	{
	dataConfiab->descricao = LeDado(fmed, flagEOF);
	dataConfiab->icont     = StrToInt(LeDado(fmed, flagEOF));
	dataConfiab->flag_BLOCO= (StrToInt(LeDado(fmed, flagEOF)) == 1);
	dataConfiab->flag_NA   = (StrToInt(LeDado(fmed, flagEOF)) == 1);

	LeDado(fmed, flagEOF); LeDataEqpto(fmed, dataConfiab->DataGeralEqpto, "GERAL");
	LeDado(fmed, flagEOF); LeDataEqpto(fmed, dataConfiab->DataGeralTrecho, "GERAL");


	while(!flagEOF)
		 {
		 //codCirc;  TAXA_MT; TMA_MT;	cli_mt;	clih_mt;	TAXA_BT;	TMA_BT
		 if((codTipoChave = LeDado(fmed, flagEOF)).AnsiCompare("TIPOEQUIPAMENTOPROTECAO") == 0) break;
		 tipoChave = BuscaTipoChave(codTipoChave);
		 if(tipoChave != NULL)
			 {if(dataConfiab->lisTipoChv->IndexOf(tipoChave) == -1) dataConfiab->lisTipoChv->Add(tipoChave);
			 }
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATAEQPTO") == 0) break;
		 dataConfiab->LisDataEqpto->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LISDATAEQPTO");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATAREDE") == 0) break;
		 dataConfiab->LisDataRede->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataFalhaRede");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATATIPO") == 0) break;
		 dataConfiab->LisDataTipo->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataTipo");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATATIPOTRECHO") == 0) break;
		 dataConfiab->LisDataTipoTrecho->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataTipoTrecho");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATATIPOCHAVE") == 0) break;
		 dataConfiab->LisDataTipoChave->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataTipoChave");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATATIPOGERADOR") == 0) break;
		 dataConfiab->LisDataTipoGerador->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataTipoGerador");
		 }

	while(!flagEOF)
		 {
		 if((aux = LeDado(fmed, flagEOF)).AnsiCompare("LISDATATIPOTRAFO") == 0) break;
		 dataConfiab->LisDataTipoTrafo->Add(dataEqpto = NewObjDataEqpto(NULL));
		 LeDataEqpto(fmed, dataEqpto, "LisDataTipoTrafo");
		 }


//	ImprimeDataEqpto(fmed, dataConfiab->dataGeralEqpto);
//	ImprimeDataEqpto(fmed, dataConfiab->dataGeralTrecho);
   fclose(fmed);

	} catch (Exception &e)
			{
			Aviso("Arquivo selecionado pode não ser o correto");
			resposta = false;
			}
	return(resposta);
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::LeDataEqpto(FILE *fmed, VTDataEqpto *dataEqpto, AnsiString tipoLeit)
	{
	AnsiString aux;
	bool flagEOF;

	aux = LeDado(fmed, flagEOF);//dataEqpto->eqpto
	if(aux.AnsiCompare("INDEF")) dataEqpto->eqpto = NULL;
	else dataEqpto->eqpto = BuscaEqpto(aux);


	aux = LeDado(fmed, flagEOF);//dataEqpto->eqpto_aux
	if(aux.AnsiCompare("INDEF") == 0) dataEqpto->eqpto_aux = NULL;
	else
	  {
	  if(tipoLeit.AnsiCompare("LisDataFalhaRede") == 0)
		 {
		 dataEqpto->eqpto_aux = BuscaRede(aux);
		 }
	  if(tipoLeit.AnsiCompare("LisDataTipoChave") == 0)
		 {
		 dataEqpto->eqpto_aux = BuscaTipoChave(aux);
		 }
	  if(tipoLeit.AnsiCompare("LisDataTipoTrecho") == 0)
		 {
		 dataEqpto->eqpto_aux = BuscaTipoTrecho(aux);
		 }
	  }


	dataEqpto->itipo = StrToInt(LeDado(fmed, flagEOF));
	dataEqpto->itipo_aux = StrToInt(LeDado(fmed, flagEOF));
	dataEqpto->codigo    = LeDado(fmed, flagEOF);
	dataEqpto->vmin      = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->vmax      = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.tx_falha  = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.tx_interr = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.treparo   = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.tman      = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.comp      = StrToDouble(LeDado(fmed, flagEOF));
	dataEqpto->pconf.fat_isola = StrToDouble(LeDado(fmed, flagEOF));

	}
//---------------------------------------------------------------------------
VTRede* __fastcall TConfiab::BuscaRede(AnsiString cod)
	{
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede       *rede;
	TList  *lisRedes = redes->LisRede();
	AnsiString    codAux = cod.Trim();

	rede = NULL;
	for(int n = 0; n < lisRedes->Count; n++)
		{
		rede = (VTRede*)lisRedes->Items[n];
		if(rede->Codigo.Trim().AnsiCompare(codAux) == 0) break;
		rede = NULL;
		}
	return(rede);
	}
//---------------------------------------------------------------------------
VTTipoChave* __fastcall TConfiab::BuscaTipoChave(AnsiString cod)
	{
	VTTipos     *tipos  = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();
	VTTipoChave *tipoChave;
	AnsiString  codAux = cod.Trim();

	tipoChave = NULL;
	for(int n = 0; n < lisTIPO->Count; n++)
		{
		tipoChave = (VTTipoChave*)lisTIPO->Items[n];
		if(tipoChave->Codigo.Trim().AnsiCompare(codAux) == 0) break;
		tipoChave = NULL;
		}
	return(tipoChave);
	}
//---------------------------------------------------------------------------
VTCabo* __fastcall TConfiab::BuscaTipoTrecho(AnsiString cod)
	{
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	TList      *lisCabos = arranjos->LisCabo();
	VTCabo     *pcabo;
	AnsiString codAux = cod.Trim();

	pcabo = NULL;
	for(int n = 0; n < lisCabos->Count; n++)
		{
		pcabo = (VTCabo*)lisCabos->Items[n];
		if(pcabo->Codigo.Trim().AnsiCompare(codAux) == 0) break;
		pcabo = NULL;
		}
   return(pcabo);
	}
//---------------------------------------------------------------------------
VTEqpto* __fastcall TConfiab::BuscaEqpto(AnsiString codEqpto)
	{
	VTRedes*      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEqpto *eqpto;
	TList   *lisAux;
	AnsiString codAux = codEqpto.Trim();

	lisAux = new TList();
	redes->LisEqpto(lisAux);
	eqpto = NULL;
	for(int n = 0; n < lisAux->Count; n++)
		{
		eqpto = (VTEqpto*)lisAux->Items[n];
		if(eqpto->Codigo.Trim().AnsiCompare(codAux) == 0) break;
		eqpto = NULL;
		}

	delete lisAux;
	return(eqpto);
	}
//---------------------------------------------------------------------------
AnsiString __fastcall TConfiab::LePathParam(void)
	{
	VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arqParam, dirConf;
	FILE    *fparam;
	bool    flagEOF;

	arqParam = path->DirTmp() + "\\dirConfMalha.tmp";
	if(!FileExists(arqParam)) return(path->DirTmp() + "\\");//hkJunho return(dirConf);

	fparam = fopen(arqParam.c_str(), "r");
	dirConf = "";
	//dirConf = path->DirDat();
	dirConf.cat_sprintf("%s", LeDado(fparam, flagEOF).c_str());
	fclose(fparam);

	if(!DirectoryExists(dirConf)) dirConf = path->DirTmp() + "\\";//hkJunho
	return(dirConf);
	}
//---------------------------------------------------------------------------
AnsiString __fastcall TConfiab::LeDado(FILE* fp, bool &flagEOF)
	{
	AnsiString x;
	char  aux;

	flagEOF = false;

	x = "";
	aux = fgetc(fp);
	while ((aux  != ';') && (aux  != '\n') && (aux  != EOF))
					  {x += AnsiString(aux);
						aux = fgetc(fp);
					  }
	if(aux == EOF) flagEOF = true;

	return(x);
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::GeraCopiaArvores(TList* lisArvs)
	{
	VTArvores *arvs, *arvs_aux;
	VTEqpto   *eqpto, *eqpto_aux;
	TList     *lisCargas;
	int       iaux;
	TList     *lisAux;


	//Deleta arvores "clone"
	for(int n = lisArvs->Count - 1; n >= 0; n--)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		if(arvs->arvs_ori == NULL) continue;
		lisArvs->Delete(n);
		delete arvs;
		}

	lisAux = new TList();
	lisEqpto->Clear();
	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		lisEqpto->Add(eqpto= arvs->EqRaiz());
		lisAux->Add(arvs);
		for(int n = 0; n < lisGrupoCargas->Count; n++)
			{
			lisCargas = (TList*)lisGrupoCargas->Items[n];
			if(lisCargas->Count == 0) continue;
			eqpto_aux = (VTEqpto*)lisCargas->First();
			if(eqpto != eqpto_aux) continue;
			for(int n1 = 1; n1 < lisCargas->Count; n1++)
				{
				eqpto_aux = (VTEqpto*)lisCargas->Items[n1];
				//iaux = lisEqpto->IndexOf(eqpto_aux);
				//if(!(iaux < 0))
				//  {lisEqpto->Remove(eqpto_aux);
				//	lisArvs->Remove(arvs_aux = (VTArvores*)lisArvs->Items[iaux]);
				//	delete arvs_aux; arvs_aux = NULL;
				//  }
				arvs_aux = gerapath->GeraCopia(arvs, eqpto_aux);
				lisEqpto->Add(eqpto_aux);
				lisAux->Add(arvs_aux);
				}
			}
		}
	lisArvs->Clear();
   lisArvs->Assign(lisAux, laOr);

	delete lisAux;
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::ImprimeParametrosRede(FILE *fout)
	{
	VTLigacao *pliga;
	PAR_CONF  pconf;

	for(int n = 0; n < lisLiga->Count; n++)
		{
		pliga = (VTLigacao*)lisLiga->Items[n];
		dataConfiab->ParamEqpto(pconf, pliga);
		fprintf(fout, "%s\t", pliga->Codigo.c_str());
		fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		fprintf(fout, "tinterr:\t%6.3f\t", pconf.tman);
		fprintf(fout, "fat_isola:\t%6.3f\n", pconf.fat_isola);
		}

	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::Inicia(void)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	//Limpa lista de arvores
	LimpaTList(lisArv);
	lisEqpto->Clear();
	//Inicia lista de ligações a serem excluidas
	gerapath->AddLigaExc(NULL, true);

	(geracorte->LisExcl())->Clear();
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::IniciaConfigGerais(void)
	{
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList       *lisExcl;
	//Inicia parametros para calculo básico



	Inicia();
	//Inicia lista com ligacoes a serem desconsideradas nos caminhos
	AddLigaExc(NULL, true);
	//Define lista de equipamentos para não participarem de
	//geração de lista agregada
	lisExcl = new TList();
	redes->LisEqpto(lisExcl, eqptoCHAVE);
	//Gera rede agregada
	GeraRedeAgregada(lisExcl, true);
	delete lisExcl;


	IniciaParamConf(this->dataConfiab);

	DefineCargas();
	DefineSuprimentos();
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::IniciaConfigRede(void)
	{
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList       *lisExcl;
	//Inicia parametros para calculo básico



	Inicia();
	//Inicia lista com ligacoes a serem desconsideradas nos caminhos
	AddLigaExc(NULL, true);
	//Define lista de equipamentos para não participarem de
	//geração de lista agregada
	lisExcl = new TList();
	redes->LisEqpto(lisExcl, eqptoCHAVE);
	//Gera rede agregada
	GeraRedeAgregada(lisExcl, true);
	delete lisExcl;


	//IniciaParamConf(this->dataConfiab);

	DefineCargas();
	DefineSuprimentos();
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::InsereLigaExcl(TList* lis)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	geracorte->InsereLigaExcl(lis);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::LigaExcl(void)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	return(geracorte->LisExcl());
	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TConfiab::GeraArvore(VTEqpto* eqpto)
	{
	VTArvores*      arvs;
	int iaux;

	//Verifica se a arvore já foi construída
	iaux = lisEqpto->IndexOf(eqpto);
	if(!(iaux < 0))
		{
		arvs = (VTArvores*)lisArv->Items[iaux];
		}
	else
		{
		//arvs = gerapath->GeraArvore(eqpto, lisArv);
		arvs = gerapath->GeraArvore(eqpto);
		if(arvs) {lisArv->Add(arvs);
					 lisEqpto->Add(eqpto);
					}
		}
	return(arvs);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::GeraRedeAgregada(TList *lisFora, bool flag)
	{
	VTRedes    *redes;
	VTGeraRede *gerRede = (VTGeraRede*)apl->GetObject(__classid(VTGeraRede));
	VTBlocos   *blocos  = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

	lisLiga->Clear();
	if(flag)
	  {//Monta lista de ligações em série
	  //gerRede->Executa(lisLiga, lisFora);
	  if(dataConfiab->flag_BLOCO)
		  gerRede->ExecutaBlocos(lisLiga, blocos);  //PENDENCIA e lisFora?       //ALTERA BLOCOS
	  else
		  gerRede->Executa(lisLiga, lisFora);
	  }
	else
	  {  //Preenche lista com ligações originais
	  redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
	  redes->LisLigacao(lisLiga);
	  //Remove ligações de lisFora
	  if(lisFora != NULL)
		 {
		 for(int nl = 0; nl < lisFora->Count; nl++)
			 lisLiga->Remove(lisFora->Items[nl]);
		 }
	  }
	//Define ligacoes para gerador de árvores
	gerapath->DefLiga(lisLiga);

	return(lisLiga);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::IniciaParamConf(VTDadoConfiab *dataConfiab)
	{
	PAR_CONF    pconf;
	//VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTTipos));
	VTTipos     *tipos  = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();
	TList *lisCabos;
	TList *lisTipoChaves;
	VTTipoChave* ptipochave;
	//TList *lisLig;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTArranjo  *arranjo;
	VTTrecho   *ptrecho;
	VTCabo     *cabo;

	dataConfiab->Inicia();

	pconf.tx_falha = 0.10;
	pconf.tx_interr= 0.05;
	pconf.treparo  = 1.50;
	pconf.tman     = 0.66;
	pconf.fat_isola= 1.00;
	pconf.comp     = 1.00;

	dataConfiab->DefParamGeralTrecho(pconf);
	dataConfiab->DefParamGeralTipo(pconf, eqptoTRECHO);

	pconf.tx_falha = 0.10;
	pconf.tx_interr= 0.05;
	pconf.treparo  = 1.50;
	pconf.tman     = 0.66;
	pconf.fat_isola= 1.00;
	pconf.comp     = 1.00;

	dataConfiab->DefParamGeralEqpto(pconf);
	dataConfiab->DefParamGeralTipo(pconf, eqptoGERADOR);
	dataConfiab->DefParamGeralTipo(pconf, eqptoTRAFO);
	dataConfiab->DefParamGeralTipo(pconf, eqptoCHAVE);
	pconf.tx_falha = 0.0010;
	pconf.tx_interr= 0.0005;
	pconf.treparo  = 1.50;
	pconf.tman     = 0.66;
	pconf.fat_isola= 1.00;
	pconf.comp     = 1.00;
	dataConfiab->DefParamGeralTipo(pconf, eqptoBARRA);//inclui barras


	//Ordem das contingencias
	dataConfiab->icont = 0;//2;

	//Define se calculo será realizado por bloco ou por carga
	dataConfiab->flag_BLOCO = false;

	//Inicia lista com tipos de chaves
	dataConfiab->lisTipoChv->Clear();
	/*dataConfiab->lisTipoChv->Assign(lisTIPO, laOr);
	dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
	dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
	dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
	dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
	*/

	/*
	lisCabos = arranjos->LisCabo();
	dataConfiab->ParamGeralTipo(pconf, eqptoTRECHO);
	for(int nl = 0; nl < lisCabos->Count; nl++)
		{
		cabo = (VTCabo*)lisCabos->Items[nl];
		dataConfiab->DefParamGeralTipoTrecho(pconf, cabo);
		}


	lisTipoChaves = tipos->LisTipoChave();
	dataConfiab->ParamGeralTipo(pconf, eqptoCHAVE);
	for(int nl = 0; nl < lisTipoChaves->Count; nl++)
		{
		cabo = (VTCabo*)lisTipoChaves->Items[nl];
		dataConfiab->DefParamGeralTipoChave(pconf, cabo);
		}

	//Insere dados de trafos
	dataConfiab->ParamGeralTipo(pconf, eqptoTRAFO);
	dataConfiab->DefParamGeralTipoTrafo(pconf, 0., 1.);
	dataConfiab->DefParamGeralTipoTrafo(pconf, 1., 10.);
	dataConfiab->DefParamGeralTipoTrafo(pconf, 10., 999.);

	//Insere dados de geradores
	dataConfiab->ParamGeralTipo(pconf, eqptoGERADOR);
	dataConfiab->DefParamGeralTipoGerador(pconf, 0., 1.);
	dataConfiab->DefParamGeralTipoGerador(pconf, 1., 10.);
	dataConfiab->DefParamGeralTipoGerador(pconf, 10., 999.);
	*/

	return(true);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::ListaArvores(void)
	{
	return(lisArv);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::ListaCortes(void)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	return(geracorte->LisCortes());
	}
//---------------------------------------------------------------------------
void __fastcall TConfiab::ListaCortes(VTEqpto* eqRaiz, TList* lis1, TList* lis2, TList* lis3)
	{
	VTGeraCorte*    geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	TList* lisCortes;
	TCorteConf *pcorte1, *pcorte2, *pcorte3;
	VTArvores *arvs;

	if(lisArv->Count == 0) return;
	arvs = NULL;
	for(int n = 0; n < lisArv->Count; n++)
		{
		arvs = (VTArvores*)lisArv->Items[n];
		if(arvs->EqRaiz() == eqRaiz) break;
		arvs = NULL;
		}
	if(arvs == NULL) return;

	if(arvs->arvs_ori != NULL) arvs = arvs->arvs_ori;

	lisCortes = geracorte->LisCortes();
	for(int n1 = 0; n1 < lisCortes->Count; n1++)
		{
		pcorte1 = (TCorteConf*)lisCortes->Items[n1];
		if(pcorte1->lisFontes->IndexOf(arvs) != -1) {lis1->Add(pcorte1); continue;}
		for(int n2 = 0; n2 < pcorte1->lisCortes->Count; n2++)
			{
			pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[n2];
			if(pcorte2->lisFontes->IndexOf(arvs) != -1) {lis2->Add(pcorte2); continue;}
			for(int n3 = 0; n3 < pcorte2->lisCortes->Count; n3++)
				{
				pcorte3 = (TCorteConf*)pcorte2->lisCortes->Items[n3];
				if(pcorte3->lisFontes->IndexOf(arvs) != -1) lis3->Add(pcorte3);
				}
			}

		}



	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::ListaLiga(void)
	{
	return(lisLiga);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::ListaGrupoCargas(void)
	{
	return(lisGrupoCargas);
	}
//---------------------------------------------------------------------------
TList* __fastcall TConfiab::ListaSup(void)
	{
	return(lisSUP);
	}
//---------------------------------------------------------------------------
/*TDataEqpto* __fastcall TConfiab::NovoEqpto(VTEqpto* eqpto)
   {
   TDataEqpto* deqpto;
	if(!eqpto)
     {
	  lisDataEqpto->Clear();
	  lisEqpto->Clear();
     return(NULL);
     }

   lisDataEqpto->Add(deqpto = new TDataEqpto(eqpto));
   lisEqpto->Add(eqpto);

   return(deqpto);
   }
*/
//---------------------------------------------------------------------------
//TDataEqpto* __fastcall TConfiab::BuscaDataEqpto(VTEqpto* eqpto)
//   {
//   TDataEqpto* deqpto;
//   int iaux;

//   iaux = lisEqpto->IndexOf(eqpto);
//   if(iaux == -1) return(NULL);

//   deqpto = (TDataEqpto*)lisDataEqpto->Items[iaux];

//   return(deqpto);
//   }
/*
//---------------------------------------------------------------------------
PAR_CONF __fastcall TConfiab::ParEqpto(VTEqpto* eqpto)
   {
   TMetaClass *obj_class;
   AnsiString obj_name, obj_name1;
   TLigaSerie *pligS;
   TList* lisEqpto;
	PAR_CONF   pconf_res, pconf_aux;
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));

   //TDataEqpto* deqpto;

   obj_class = ((TObject*)eqpto)->ClassType();
   obj_name  = obj_class->ClassName();
   if(obj_name.AnsiCompareIC("TLigaSerie") == 0)
    {
    pconf_res.tx_falha  = 0.;
    pconf_res.tx_interr = 0.;
    pconf_res.treparo   = 0.;
    pconf_res.tman      = 0.;
    pligS = (TLigaSerie*)eqpto;
    lisEqpto = pligS->LisEqpto();
    for(int n = 0; n < lisEqpto->Count; n++)
       {
		 dataConfiab->ParamEqpto(pconf_aux, (VTEqpto*)lisEqpto->Items[n]);
		 //deqpto = BuscaDataEqpto((VTEqpto*)lisEqpto->Items[n]);
       int PENDENCIA = 0;
       //pconf_aux = (deqpto == NULL) ? pconf : deqpto->pconf;
       pconf_res.tx_falha  += pconf_aux.tx_falha;
       pconf_res.tx_interr += pconf_aux.tx_interr;
       pconf_res.treparo   += (pconf_aux.tx_falha * pconf_aux.treparo);
       pconf_res.tman      += (pconf_aux.tx_interr* pconf_aux.tman);
       }
    pconf_res.treparo /= pconf_res.tx_falha;
	 pconf_res.tman    /= pconf_res.tx_interr;
    }
   else
    {
    dataConfiab->ParamEqpto(pconf_res, eqpto);
    //deqpto = BuscaDataEqpto(eqpto);
    //pconf_res = (deqpto == NULL) ? pconf : deqpto->pconf;
    }
   return(pconf_res);
   }
*/
//---------------------------------------------------------------------------
//void __fastcall TConfiab::DefParMed(PAR_CONF pconf)
//   {
//   this->pconf = pconf;
//   //Distribui parametros entre equipamentos
//   DefParDist();
//   }
//---------------------------------------------------------------------------
/*void __fastcall TConfiab::DefParDist(void)
	{
	VTRedes*   redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEqpto*   eqpto;
   //TDataEqpto* deqpto;
   TList*     lisAux;
   TList*     lisAux1;
	VTLigacao* plig;
   double     ext, aux, tf_bar, ti_bar;
   double     tf_eqbar, ti_eqbar;
   double     daux;
   TObject*   pobj;
   AnsiString obj_name;
   TList*     lisBarra;
   PAR_CONF   pconf_aux;
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));

   //lisBarra = new TList();
   //Inializa lista de equipamentos
   //NovoEqpto(NULL);
   //for(int n = 0; n < lisLiga


   lisAux = new TList();
	//Inializa lista de equipamentos

	dataConfiab->DefParamEqpto(pconf_aux, NULL);
   //NovoEqpto(NULL);
   //Define parametros para as ligações da rede
   lisAux->Clear();
   redes->LisLigacao(lisAux);
   ext = 0.;
   for(int n = 0; n < lisAux->Count; n++)
		{
      pobj = (TObject*)lisAux->Items[n];
      obj_name = (pobj->ClassType())->ClassName();
      if(obj_name.AnsiCompareIC("TTrecho") == 0)
         daux = ((VTTrecho*)pobj)->Comprimento_km;
      else
         daux = 0.001;
      ext += daux;



      pconf_aux.tx_falha  = pconf.tx_falha  * daux * 0.8;
      pconf_aux.tx_interr = pconf.tx_interr * daux * 0.8;
      pconf_aux.treparo   = pconf.treparo;
      pconf_aux.tman      = pconf.tman;

      dataConfiab->DefParamEqpto(pconf_aux, (VTEqpto*)pobj);

      //deqpto = NovoEqpto((VTEqpto*)pobj);
      //deqpto->pconf.tx_falha  = pconf.tx_falha  * daux * 0.8;
      //deqpto->pconf.tx_interr = pconf.tx_interr * daux * 0.8;
      //deqpto->pconf.treparo   = pconf.treparo;
      //deqpto->pconf.tman      = pconf.tman;
      }
   lisAux->Clear();
   redes->LisEqpto(lisAux);
	aux = 1.;
   int PENDENCIA2 = 0;
   //aux = (double)(lisAux->Count - lisDataEqpto->Count);
   for(int n = 0; n < lisAux->Count; n++)
      {
      eqpto = (VTEqpto*)lisAux->Items[n];
      if(eqpto->Tipo() == eqptoLIGACAO) continue;
      pconf_aux.tx_falha  = (ext * pconf.tx_falha *.2) / aux;
      pconf_aux.tx_interr = (ext * pconf.tx_interr*.2) / aux;
      pconf_aux.treparo   = pconf.treparo;
      pconf_aux.tman      = pconf.tman;
      dataConfiab->DefParamEqpto(pconf_aux, eqpto);
      //deqpto = NovoEqpto(eqpto);
      //deqpto->pconf.tx_falha  = (ext * pconf.tx_falha *.2) / aux;
      //deqpto->pconf.tx_interr = (ext * pconf.tx_interr*.2) / aux;
      //deqpto->pconf.treparo   = pconf.treparo;
      //deqpto->pconf.tman      = pconf.tman;
		}
   delete lisAux;

   }
*/
//---------------------------------------------------------------------------
//double __fastcall TConfiab::DEC(void)
//   {
//   return(res_conf.dec);
//   }
//---------------------------------------------------------------------------
//double __fastcall TConfiab::FEC(void)
//   {
//   return(res_conf.fec);
//   }
//---------------------------------------------------------------------------
//double __fastcall TConfiab::END(void)
//   {
//   return(res_conf.end);
//   }
//---------------------------------------------------------------------------
//void __fastcall TConfiab::DefLigacoes(TList* lisLiga)
//   {
//   gerapath->DefLiga(lisLiga);
//   }
//---------------------------------------------------------------------------
VTBlocos* __fastcall TConfiab::PM_GetBlocos(void)
	{
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

	return (blocos);
	}
//---------------------------------------------------------------------------
bool __fastcall TConfiab::ExecutaCalcIndices(void)
	{
	VTCalcIndCont   *calcind =  (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTGeraCorte     *geracorte    = (VTGeraCorte*)apl->GetObject(__classid(VTGeraCorte));
	TList           *lisCargas;
	VTArvores       *arvs, *arvs_aux;
	AnsiString      aux;
	int             iaux;
	bool            resposta;
	VTEqpto         *eqpto;


	//Calculo dos indicadores
	resposta = calcind->Executa(geracorte->LisCortes(), lisArv, prog);
	//Replica indicadores para cargas vizinhas
	for(int n = 0; n < lisArv->Count; n++)
		{
		arvs = (VTArvores*)lisArv->Items[n];
		if(arvs->arvs_ori != NULL)	arvs->res_conf = arvs->arvs_ori->res_conf;
		}
	return(resposta);
	}
/*//---------------------------------------------------------------------------
bool __fastcall TConfiab::ExecutaCS(void)
	{
	VTEqpto* eqpto;
	TList*   lisAux;

	//Inicia DESTINOS(Suprimentos)
	gerapath->AddFonte(NULL);//DefSuprimento(NULL);
	for(int n = 0; n < lisSUP->Count; n++)
		 {
		 eqpto = (VTEqpto*)lisSUP->Items[n];
		 gerapath->AddFonte(eqpto);//DefSuprimento(eqpto);
		 }

	//Executa (flag_inv = false) para cada ORIGEM (Cargas)
	for(int n = 0; n < lisGrupoCargas->Count; n++)
		 {
		 lisAux = (TList*)lisGrupoCargas->Items[n];
		 //Gera arvores para o primeira carga do grupo
		 eqpto = (VTEqpto*)lisAux->First();

		 //PENDENCIA : barra de progresso

		 //aux = "Carga: ";
		 //aux.cat_sprintf("%s", eqpto->Codigo);
		 //prog->SetSubTitulo(aux);
		 //prog->NextPosition();

		 ExecutaCS(lisAux);
		 }
	return(true);
	}
*/
