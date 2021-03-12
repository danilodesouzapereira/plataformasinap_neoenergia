//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
// DLL include
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Importa.h>
#include <DLL_Inc\Log.h>
#include <DLL_Inc\MontaZ.h>
#include <DLL_Inc\Progresso.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Rede.h>
#include <DLL_Inc\Redutor.h>
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Importa\ExportaSec\VTExportaSec.h>
#include <Fontes\Importa\IntegraEtd\VTIntegraEtd.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Importa\ValFases\VTValFases.h>
#include <Fontes\Importa\ValPerdafe\VTValPerdafe.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\MontaZ\VTMontaZ.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Redutor\VTRedutor.h>
#include <Fontes\Topologia\VTTopologia.h>
//---------------------------------------------------------------------------
#include "TConversor.h"
#include "VTImportaTXT.h"
#include "VTParse.h"
#include "VTPreMonta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTConversor* NewObjConversor(VTApl *apl)
   {
   return (new TConversor(apl));
   }
//---------------------------------------------------------------------------
__fastcall TConversor::TConversor(VTApl *apl_owner)
   {
	//salva ponteiros
   this->apl_owner = apl_owner;
   //cria objeto locais da classe
	log_resumo = DLL_NewObjLog(apl);
   lisEQP     = new TList();
   files_pri  = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TConversor::~TConversor(void)
   {
   //destr�i lista sem destruir seus objetos
   if(lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destr�i objetos
   if(files_pri)   {delete files_pri;  files_pri  = NULL;}
   if(log_resumo)  {delete log_resumo; log_resumo = NULL;}
   if (apl)        {delete apl;        apl        = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TConversor::DefineCor(void)
   {
   //vari�veis locais
   unsigned short int indexColor;
   TColor  cores[] = {clBlue,   clRed,    clGreen,  clYellow,  clSkyBlue, clFuchsia,
                      clOlive,  clPurple, clNavy,   clMaroon,  clTeal, clLime
                     };
   VTSubestacao *se;
   VTPrimario   *pri;
   VTReticulado *ret;
   TList        *lisSe, *lisRet, *lisPri;
   VTRadial     *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));

   //prote��o
   if(! radial) return;
   lisSe = radial->LisSubestacao();
   for(int nse = 0; nse < lisSe->Count; nse++)
      {
      se = (VTSubestacao*)lisSe->Items[nse];
      //reinicia indice para cores
      indexColor = 0;
      //loop p/ lista de alimentadores da se
      lisPri = se->LisPrimario();
      for(int npri = 0; npri < lisPri->Count; npri++)
         {
         pri = (VTPrimario*)lisPri->Items[npri];
         //define a cor para cada rede primaria
         if (indexColor >= (sizeof(cores) / sizeof(cores[0]))) indexColor = 0;
         pri->Rede->Color = cores[indexColor++];
         }
      //loop p/ lista de reticulados da se
      lisRet = se->LisReticulado();
      for(int nret = 0; nret < lisRet->Count; nret++)
         {
         ret = (VTReticulado*) lisRet->Items[nret];
         //loop p/ lista de redes do reticulado
         lisPri = ret->LisPrimario();
         for(int n = 0; n < lisPri->Count; n++)
            {
            pri = (VTPrimario*)lisPri->Items[n];
            //define a cor para cada rede primaria
            if (indexColor >= (sizeof(cores) / sizeof(cores[0]))) indexColor = 0;
            pri->Rede->Color = cores[indexColor++];
            }
         }
      }
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TConversor::DeterminaBarraInicial(VTRede *redePrimaria)
   {
   //variaveis locais
   VTRedes     *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede      *rede;
   VTBarra     *barra;
   TList       *lisBar;

   //prote��o
   if(! redes) return NULL;
   TList *lisRedes =  redes->LisRede();

   for(int nr = 0; nr < lisRedes->Count; nr++)
      {
      rede = (VTRede*) lisRedes->Items[nr];
      if(rede->TipoRede->Segmento != redeETD) continue;
      lisBar = rede->LisBarra();
      //verifica se a rede a ser removida cont�m barra da ETD
      for(int nb = 0; nb < lisBar->Count; nb++)
         {
         barra = (VTBarra*) lisBar->Items[nb];
         if( (redePrimaria->LisBarra()->IndexOf(barra)) >= 0)
            return (barra); //barra inicial
         }
      }
   return (NULL);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TConversor::ExportaRedesSecundarias(void)
   {
   //vari�veis locais
	bool          sucesso;
	VTExportaSec  *exporta_sec;

   //verifica a op��o
   if (! opcao->gerarTXT) return (true);
	try{//cria objeto p/ exportar redes secund�rias p/ arquivos TXT
		if ((exporta_sec = DLL_NewObjExportaSec(apl)) == NULL) return(false);
		sucesso     = exporta_sec->Executa(true);
      //destr�i objeto criado
		delete exporta_sec;
		}
      catch(Exception &e)
         {
			if (exporta_sec) {delete exporta_sec;}
         return false;
         }
   return (sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::ExtractTXT(AnsiString filefullpath, int ind_arq, int num_arq)
   {
   //vari�veis locais
   bool          sucesso;
   AnsiString    filename;
   TDateTime     start;
   VTImportaTXT     *importa;

   try{//salva apenas nome do arquivo
      filename = ExtractFileName(filefullpath);
      //salva instante inicial
      start = Now();
      //prote��o: arquivo inexistente
      if(! FileExists(filefullpath))
         {
         LogErro(filename, "Arquivo inexistente");
         return false;
         }
      //cria objeto ImportaTtx p/ importar txt
      if ((importa = NewObjImportaTXT(apl)) == NULL)
         {
         LogErro(filename, "Erro ao criar Classe: VTImportaTXT");
         return false;
         }
      sucesso  = importa->ExecutaImportacao(filefullpath);
      //if (sucesso) sucesso = monta->TrataFimDeDados(true);
      }
      catch(Exception &e)
         {
         LogErro(filename, e.Message);
         return(false);
         }
   //destr�i objeto ImportaTxt
   if (importa) {delete importa;}
   if (sucesso) LogSucesso(filename, start, Now());
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaLisArqPrimario(TStrings *files_pri, strOPImporta *opcao)
   {
   //vari�veis locais
   bool        sucesso;
   VTLog       *log_completo;
   //VTPosMonta  *pos_monta;
   VTPreMonta  *pre_monta;
   VTProgresso *progresso;
   VTRadial    *radial;
   VTRedes     *redes;

   try{//salva op��o
      this->opcao = opcao;
      //cria objeto Apl e todos os objetos necess�rios p/ a importa��o
      if (! IniciaObjApl()) return(false);
      //obt�m objetos criados
      log_completo = (VTLog*)apl->GetObject(__classid(VTLog));
      progresso    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
      pre_monta    = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
      radial       = (VTRadial*)apl->GetObject(__classid(VTRadial));
      redes        = (VTRedes*)apl->GetObject(__classid(VTRedes));
      //progresso
      switch(opcao->ihm.progresso)
         {
         case progDEFAULT: if (progresso) progresso->Start(progDEFAULT); break;
         case progTEXTO:   if (progresso) progresso->Start(progTEXTO);   break;
         default: //elimina objeto Progresso
            if (progresso) {apl->Remove(progresso); delete progresso;}
         }
      //inicia log
      LogInicia();
      //configura op��es de PreMonta
      switch(opcao->pri.consA4)
         {
         case opENERGIA:        pre_monta->ConsA4_Energia = true;  break;
         case opCURVA:          pre_monta->ConsA4_Energia = false; break;
         case opDEM_CONTRATADA: pre_monta->ConsA4_Energia = false; break;
         default:               pre_monta->ConsA4_Energia = false; break;
         }
      //importa arquivo txt
      sucesso = ImportaLisArquivoTxt(files_pri);
      //inicia objeto Radial
      if (sucesso) radial->Inicia(redes);
      //verifica consist�ncia das fases dos equipamentos
      if (sucesso) VerificaFases();
      //verifica consist�ncia das perdas no ferro dos trafos
      if (sucesso) VerificaPerdaFerro();
      //define uma cor diferente para cada rede prim�ria
      //if (sucesso) DefineCor();
      //cria redes secund�rias em arquivo texto
      if (sucesso) ExportaRedesSecundarias();
      //integra��o com ETD completa
      if (sucesso) IntegraETDcompleta();
      //executa PorMonta (se existir)
      //if (pos_monta) pos_monta->Executa(opcao);
      //fecha progresso
      if (progresso) progresso->Stop();
      //fecha log resumo e completo
      LogFecha();
      //exibe relat�rio do log
//      if (log_completo) log_completo->ShowModal("Log de Importa��o");  //DVK 2013.07.12
      if (log_completo) log_completo->ShowModal("Log de Importa��o");  //DVK 2013.08.01
      //destr�i objeto VTApl e todos seus objetos
      delete apl;
      apl = NULL;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaLisArqSubestacao(TStrings *files_sub, strOPImporta *opcao)
   {
   //vari�veis locais
   bool        sucesso = true;
   VTLog       *log_completo;
   VTPreMonta  *pre_monta;
   VTProgresso *progresso;
   VTRadial    *radial;
   VTRedes     *redes;

   try{//salva op��o
      this->opcao = opcao;
      //cria objeto Apl e todos os objetos necess�rios p/ a importa��o
      if (! IniciaObjApl()) return(false);
      //obt�m objetos criados
      log_completo = (VTLog*)apl->GetObject(__classid(VTLog));
      progresso    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
      pre_monta    = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
      radial       = (VTRadial*)apl->GetObject(__classid(VTRadial));
      redes        = (VTRedes*)apl->GetObject(__classid(VTRedes));
      //progresso
      switch(opcao->ihm.progresso)
         {
         case progDEFAULT: if (progresso) progresso->Start(progDEFAULT); break;
         case progTEXTO:   if (progresso) progresso->Start(progTEXTO);   break;
         default: //elimina objeto Progresso
            if (progresso) {apl->Remove(progresso); delete progresso;}
         }
      //inicia log
      LogInicia();
      //configura op��es de PreMonta
      switch(opcao->pri.consA4)
         {
         case opENERGIA:        pre_monta->ConsA4_Energia = true;  break;
         case opCURVA:          pre_monta->ConsA4_Energia = false; break;
         case opDEM_CONTRATADA: pre_monta->ConsA4_Energia = false; break;
         default:               pre_monta->ConsA4_Energia = false; break;
         }
      //loop p/ todos arquivos de subestacao
      for (int n = 0; (sucesso)&&(n < files_sub->Count); n++)
         {//monta StringList com nomes dos arqwuivos dos prim�rios da subesta��o
         if (LeListaArquivos(files_sub->Strings[n], files_pri))
            {//executa importa��o dos arquivos
            sucesso = ImportaLisArquivoTxt(files_pri);
            }
         }
      //inicia objeto Radial
      if (sucesso) radial->Inicia(redes);
      //verifica consist�ncia das fases dos equipamentos
      if (sucesso) VerificaFases();
      //verifica consist�ncia das perdas no ferro dos trafos
      if (sucesso) VerificaPerdaFerro();
      //cria redes secund�rias em arquivo texto
      if (sucesso) ExportaRedesSecundarias();
      //integra��o com ETD completa
      if (sucesso) IntegraETDcompleta();
      //executa PorMonta (se existir)
      //if (pos_monta) pos_monta->Executa(opcao);
      //fecha progresso
      if (progresso) progresso->Stop();
      //fecha log resumo e completo
      LogFecha();
      //exibe relat�rio do log
//      if (log_completo) log_completo->ShowModal("Log de Importa��o");  //DVK 2013.07.12
      if (log_completo) log_completo->ShowModal("Log de Importa��o");  //DVK 2013.08.01
      //destr�i objeto VTApl e todos seus objetos
      delete apl;
      apl = NULL;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaLisArquivoTxt(TStrings *files)
   {
   //vari�veis locais
   bool       sucesso = true;
	VTMonta    *monta  = (VTMonta*)apl->GetObject(__classid(VTMonta));

   //verifica se existe files txt
   if (files->Count == 0) return(true);
   try{//loop para arquivos de alimentador
      for (int n = 0; (sucesso)&&(n < files->Count); n++)
         {//verifica se usu�rio cancelou a opera��o
         //Application->ProcessMessages();
         //processa alimentador
         if (! FileExists(files->Strings[n]))  continue;
         sucesso = ExtractTXT(files->Strings[n], n, files->Count);
         }
      if (sucesso) sucesso = monta->TrataFimDeDados(true);
      }
   catch(Exception &e)
      {
      sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::IniciaObjApl(void)
	{
	//variaveis locais
	VTPreMonta *pre_monta;

	try{//cria objeto Apl
		apl  = NewObjApl(NULL, apl_owner);
		//cria objetos opcionais e insere em Apl
		if (opcao->ihm.progresso) apl->Add(DLL_NewObjProgresso(NULL, NULL));
		if (opcao->ihm.log)       apl->Add(DLL_NewObjLog(apl));
      //cria objetos do importador
		apl->Add(DLL_NewObjMontaZ(apl));
		apl->Add(DLL_NewObjMontaRede(apl));
		apl->Add(NewObjExtrator(apl));
      apl->Add(NewObjParse(apl));
      //apl->Add(NewObjPosMonta(apl));
      apl->Add(DLL_NewObjRadial(apl));
      //cria objeto PreMonta conforme a op��o da rede secund�ria
      switch(opcao->sec.topologia)
         {
         case opSEC_CARGA:
				apl->Add(pre_monta = NewObjPreMonta1(apl));
            break;
         case opSEC_UTC:
				apl->Add(pre_monta = NewObjPreMonta2(apl));
            break;
         case opSEC_GIS:
				apl->Add(pre_monta = NewObjPreMonta(apl));
            break;
         default:
				apl->Add(pre_monta = NewObjPreMonta1(apl));
            break;
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TConversor::IntegraETDcompleta(void)
   {
   //vari�veis locais
	bool         sucesso;
	VTIntegraEtd *integra_etd;

   //verifica se deve integrar ETD
   if (opcao->sub.topologia != opSUB_TROCAR) return(true);
   try{//cria objeto p/ fazer integra��o
		if ((integra_etd = DLL_NewObjIntegraEtd(apl)) == NULL) return(false);
		sucesso = integra_etd->InsereETDcompleta();
		//destr�i objeto criado
		delete integra_etd;
		}catch(Exception &e)
			{
			if (integra_etd) {delete integra_etd;}
			sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TConversor::LeListaArquivos(AnsiString arq_sub, TStringList *files_pri)
   {
   //vari�veis locais
   bool    sucesso;
   VTParse *parse;

   try{//reinicia StringList
      files_pri->Clear();
      //cria objeto ParsePertec para ler arquivo tipo Pertec
      parse = NewObjParseLista(apl);
      //l� a lista de arquivos que comp�em a subesta��o
      sucesso = parse->Executa(arq_sub, files_pri);
      //destroi objetos criados
      delete parse;
      }
      catch(Exception &e)
         {//destr�i objetos
         if (parse) delete parse;
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::LogErro(AnsiString arq_txt, AnsiString msg_erro)
   {
   //vsri�veis locais
   AnsiString msg;

   //insere mensagem de log
   log_resumo->Add(msg.sprintf("Erro:    %-20s, Diagn�stico: %s", arq_txt.c_str(), msg_erro.c_str()));
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::LogFecha(void)
   {
   //vari�veis locais
   VTLog *log_completo = (VTLog*)apl->GetObject(__classid(VTLog));

   //fecha Log completo
   if (log_completo) log_completo->Close();
   //fecha Log resumo
   log_resumo->Close();
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::LogInicia(void)
   {
   //vari�veis locais
   VTLog *log_completo = (VTLog*)apl->GetObject(__classid(VTLog));
   VTPath *path        = (VTPath*)apl->GetObject(__classid(VTPath));

   //inicia Log resumo
   log_resumo->Open(path->DirTmp() + "\\ImportaTXT_Resumo.log");
   //inicia Log completo
   if (log_completo)
      {//abre arquivo
      log_completo->Open(path->DirTmp() + "\\ImportaTXT_Detalhado.log");
      //inicia campos do Log completo
//      log_completo->InsereCampo("Arquivo");
//      log_completo->InsereCampo("Tabela");
//      log_completo->InsereCampo("Registro");
//      log_completo->InsereCampo("Rede");
//      log_completo->InsereCampo("Descri��o");   //DVK 2013.07.12
      log_completo->InsereCampo("Arquivo");
      log_completo->InsereCampo("Bloco");
      log_completo->InsereCampo("Linha");
      log_completo->InsereCampo("Rede");
		log_completo->InsereCampo("Eqpto");
		log_completo->InsereCampo("C�digo");  //DVK 2015.10.22
      log_completo->InsereCampo("Descri��o");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::LogSucesso(AnsiString arq_txt, TDateTime start, TDateTime stop)
   {
   //vari�veis locais
   AnsiString     msg;
   int            t_mseg;
   unsigned short hour, min, sec, msec;

   //determina intervalo de tempo em mseg
   TDateTime dif = stop - start;
   dif.DecodeTime(&hour, &min, &sec, &msec);
   t_mseg = (1000*((60*((60*hour) + min)) + sec)) + msec;
   //insere mensagem de log
   log_resumo->Add(msg.sprintf("Sucesso: %-20s, Tempo: %d ms", arq_txt.c_str(), t_mseg));
   }

//---------------------------------------------------------------------------
strOPImporta* __fastcall TConversor::OpcoesImportacao(void)
   {
   return(opcao);
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::VerificaFases(void)
   {
   //vari�veis locais
	int        num_eqpto;
	TList      *lisEXT;
	VTEqpto    *eqpto;
	VTValFases *valfases;
	VTLog      *log_completo = (VTLog*)apl->GetObject(__classid(VTLog));


    //verifica a op��o
   if ((! opcao->valida_fase)&&(! opcao->corrige_fase)) return;

   try{//cria objeto VTValfase
		valfases = DLL_NewObjValFases(apl);
		valfases->CorrigeFases = opcao->corrige_fase;
		valfases->Executa();
		if ((num_eqpto = valfases->ExisteEqptoComFaseInvalida()) > 0)
			{//atualiza campos do Log completo
         if (log_completo) log_completo->DefineValor("Bloco", "Valida��o");
         if (log_completo) log_completo->DefineValor("Linha", -1);
			if (log_completo) log_completo->DefineValor("Rede", "");
			if (log_completo) log_completo->DefineValor("C�digo", "");
			//preenche log
			lisEXT = valfases->LisEqptoComFaseInvalida();
         for (int n = 0; n < lisEXT->Count; n++)
            {
				eqpto = (VTEqpto*)lisEXT->Items[n];
				//DVK 2015.10.23 insere c�digo no log e n�o altera c�digo do eqpto
				if (log_completo) log_completo->DefineValor("C�digo", eqpto->Codigo);
            if (log_completo) log_completo->DefineValor("Eqpto", eqpto->TipoAsString());
            if (log_completo) log_completo->Add("Fase Incompat�vel");
            //altera c�digo do eqpto p/ facilitar localiza��o
//            eqpto->Codigo = "Fase Incompat�vel";
            }
         if (log_completo)
				{
//				Aviso("Existem " + IntToStr(num_eqpto) + " eqptos com fases incompat�veis\n" +
//					"Estes eqptos receberam o c�digo 'Fase Incompat�vel'\n\n" +
//					"IMPORTANTE: Este tipo de problema inviabiliza o c�lculo de fluxo desequilibrado");
				Aviso("Existem " + IntToStr(num_eqpto) + " eqptos com fases incompat�veis\n" +
					"IMPORTANTE: Este tipo de problema inviabiliza o c�lculo de fluxo desequilibrado");
				}
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   //destr�i objeto criado
	if (valfases) {delete valfases;}
   }

//---------------------------------------------------------------------------
void __fastcall TConversor::VerificaPerdaFerro(void)
   {
   //vari�veis locais
   int          num_eqpto;
   TList        *lisEXT;
   VTEqpto      *eqpto;
   VTValPerdafe *valperdafe;
   VTLog        *log_completo = (VTLog*)apl->GetObject(__classid(VTLog));


    //verifica a op��o
   if (! opcao->corrige_perda_fe) return;

	try{//cria objeto VTValPerdafe
		valperdafe = DLL_NewObjValPerdafe(apl);
      valperdafe->Executa();
      if ((num_eqpto = valperdafe->ExisteTrafoComPerdaFeInvalida()) > 0)
         {//atualiza campos do Log completo
         if (log_completo) log_completo->DefineValor("Bloco", "Valida��o");
         if (log_completo) log_completo->DefineValor("Linha", -1);
			if (log_completo) log_completo->DefineValor("Rede", "");
			if (log_completo) log_completo->DefineValor("C�digo", "");
			//preenche log
         lisEXT = valperdafe->LisTrafoComPerdaFeInvalida();
         for (int n = 0; n < lisEXT->Count; n++)
            {
				eqpto = (VTEqpto*)lisEXT->Items[n];
				//DVK 2015.10.23 insere c�digo no log e n�o altera c�digo do eqpto
				if (log_completo) log_completo->DefineValor("C�digo", eqpto->Codigo);
            if (log_completo) log_completo->DefineValor("Eqpto",  eqpto->TipoAsString());
            if (log_completo) log_completo->Add("Perda no ferro inv�lida");
            //altera c�digo do eqpto p/ facilitar localiza��o
//				eqpto->Codigo = "Perda ferro";
            }
         if (log_completo)
				{
//				Aviso("Existem " + IntToStr(num_eqpto) + " trafos com perda no ferro inv�lida\n" +
//						"Estes eqptos receberam o c�digo 'Perda ferro'");
				Aviso("Existem " + IntToStr(num_eqpto) + " trafos com perda no ferro inv�lida\n");
            }
         }
      }catch(Exception &e)
			{//nada a fazer
         }
   //destr�i objeto criado
   if (valperdafe) {delete valperdafe;}
   }

//---------------------------------------------------------------------------
//eof

