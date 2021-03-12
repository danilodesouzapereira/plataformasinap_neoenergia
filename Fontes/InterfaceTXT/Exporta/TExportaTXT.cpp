//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaTXT.h"
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
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTPadraoRede.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\\DLL_Inc\Funcao.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTExportaTXT* NewObjExportaTXT(VTApl *apl)
   {
   return(new TExportaTXT(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaTXT::TExportaTXT(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   //cria objetos
   exporta_eqpto = NewObjExportaEqpto(apl);
   lines         = new TStringList();
   lisREDE       = new TList();
   lisEQP        = new TList();
   lisLIG        = new TList();
   //assume opções default
   ExportaTipoChave      = true;
   ExportaTipoConsumidor = true;
   ExportaTipoRede       = true;
   ExportaCurvaTipica    = true;
   ExportaArranjo        = true;
   ExportaCabo           = true;
   ExportaSuporte        = true;
   ExportaRede           = true;
   ExportaReducao        = false;
   Criptografar          = false;
   Separador             = ';';
   }

//---------------------------------------------------------------------------
__fastcall TExportaTXT::~TExportaTXT(void)
   {
   //destrói objetos
   if (lines) {delete lines; lines = NULL;}
   if (exporta_eqpto) {delete exporta_eqpto; exporta_eqpto = NULL;}
   //destrói lista sem destruir objetos
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaTXT::Cript(AnsiString &txt)
   {
   //verifica se a Criptografia está habilitada
   if (! Criptografar) return(txt);
   //criptografa txt
   //return(cript->Executa(txt));
   return(txt);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaArranjos(AnsiString arq_txt)
   {
   try{//define Extern_id para Cabos, Suportes e Arranjso
      IniciaObjAsIdArranjos();
      //reinicia lines
      lines->Clear();
      //configura ExportaEqpto
      exporta_eqpto->Separador    = Separador;
      //insere Bloco indicando criptografia
      if (! MontaBlocoCriptografia()) return(false);
      //insere Bloco de versão
      if (! MontaBlocoVersao()) return(false);
      //exporta Suporte
      if (! ExportaArranjos()) return(false);
      //insere Bloco de fim de dados
      if (! MontaBlocoFimDados()) return(false);
      //salva dados no arquivo
      lines->SaveToFile(arq_txt);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaArranjos(void)
   {
   try{//exporta Suporte
      if (! MontaBlocoSuporte()) return(false);
      //exporta Cabos unipolares
      if (! MontaBlocoCaboUnipolar()) return(false);
      //exporta Cabos multiplexados
      if (! MontaBlocoCaboZ0Z1()) return(false);
      //exporta Arranjo Z0Z1
      if (! MontaBlocoArranjoZ0Z1()) return(false);
      //exporta Arranjo puSB100
      if (! MontaBlocoArranjoPUSB100()) return(false);
      //exporta Arranjo CaboZ0Z1
      if (! MontaBlocoArranjoCaboZ0Z1()) return(false);
      //exporta Arranjo com SUporte e Cabo unipolar
      if (! MontaBlocoArranjoCaboPorfase()) return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaCurvasTipicas(AnsiString arq_txt)
   {
   try{//define Extern_id para Curvas típicas
      IniciaObjAsIdCurvasTipicas();
      //reinicia lines
      lines->Clear();
      //configura ExportaEqpto
      exporta_eqpto->Separador = Separador;
      //insere Bloco indicando criptografia
      if (! MontaBlocoCriptografia()) return(false);
      //insere Bloco de versão
      if ( ! MontaBlocoVersao()) return(false);
      //insere Blocos de Curvas típicas
      if (! ExportaCurvasTipicas()) return(false);
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
bool __fastcall TExportaTXT::ExportaCurvasTipicas(void)
   {
   try{//exporta Curvas típicas de demanda
      if ( ! MontaBlocoCurvaDemanda()) return(false);
      //exporta Curvas típicas de IP
      if ( ! MontaBlocoCurvaIluninacaoPublica()) return(false);
      //exporta Curvas típicas de capacitor
      if ( ! MontaBlocoCurvaCapacitor()) return(false);
      //exporta Curvas típicas de reator
      if ( ! MontaBlocoCurvaReator()) return(false);
      //exporta Curvas típicas de gerador
      if ( ! MontaBlocoCurvaGerador()) return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaEqptos(TList *lisEXT, AnsiString arq_txt)
   {
   try{//reinicia lines
      lines->Clear();
      //configura ExportaEqpto
      exporta_eqpto->Separador = Separador;
      //insere Bloco indicando criptografia
      if (! MontaBlocoCriptografia()) return(false);
      //insere Bloco de versão
      if ( ! MontaBlocoVersao()) return(false);
      //exporta TipoChave e TipoRede
      //if ( ! ExportaTipos()) return(false);
      //exporta Suporte, Cabos e Arranjos
      //if ( ! ExportaArranjos()) return(false);
      //exporta Curvas típicas
      //if ( ! ExportaCurvasTipicas()) return(false);
      //exporta Redes
      if (! ExportaEqptos(lisEXT)) return(false);
      //exporta especificação Formato do arquivo texto
      //if ( ! ExportaFormato()) return(false);
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
bool __fastcall TExportaTXT::ExportaEqptos(TList *lisEXT)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTEqpto   *eqpto;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//inclui em lisEXT os objetos Rede e Barras associados às Ligacoes
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         if (eqpto->TipoLigacao())
            {
            ligacao = (VTLigacao*)eqpto;
            rede    = ligacao->rede;
            if (rede == redes->RedeIsolada()) continue;
            if (lisEXT->IndexOf(rede) < 0) lisEXT->Add(rede);
            //inclui na lista as Barras
            ligacao->LisBarra(lisEXT);
            //verifica se a Ligacao é um Trecho com Arranjo típico
            if (ligacao->Tipo() == eqptoTRECHO)
               {
               trecho = (VTTrecho*)ligacao;
               if (trecho->ArranjoTipico)
                  {//define ExternId do arranjo igual ao seu Id
                  trecho->arranjo->Extern_id = IntToStr(trecho->arranjo->Id);
                  }
               }
            }
         }
      //define Extern_id p/ todos Eqptos em lisEXT
      IniciaObjAsIdEqptos(lisEXT);
      //monta lista com Redes dos eqptos em lisEXT
      lisREDE->Clear();
      CopiaTList(LisEqpto(lisEXT, eqptoREDE), lisREDE);
      //loop p/ todas Redes em lisREDE
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //monta Bloco da Rede
         lisEQP->Clear();
         lisEQP->Add(rede);
         if (! MontaBlocoEqptoRede(lisEQP)) return(false);
         //monta Blocos de Barras e dos Eqbar somente para a primeira Rede
         if (nr == 0)
            {//monta blocos de Barras
            if (! MontaBlocoEqptoBarra(LisEqpto(lisEXT, eqptoBARRA)))                return(false);
			//monta blocos de Eqbar
			if (! MontaBlocoEqptoBateria(LisEqpto(lisEXT, eqptoBATERIA)))      		 return(false);
            if (! MontaBlocoEqptoCapacitor(LisEqpto(lisEXT, eqptoCAPACITOR)))        return(false);
            if (! MontaBlocoEqptoCargaIP(LisEqpto(lisEXT, eqptoCARGA)))              return(false);
            if (! MontaBlocoEqptoCargaComMedicao(LisEqpto(lisEXT, eqptoCARGA)))      return(false);
            if (! MontaBlocoEqptoCargaSemMedicao(LisEqpto(lisEXT, eqptoCARGA)))      return(false);
            if (! MontaBlocoEqptoFiltro(LisEqpto(lisEXT, eqptoFILTRO)))              return(false);
            if (! MontaBlocoEqptoGerador(LisEqpto(lisEXT, eqptoFILTRO)))             return(false);
            if (! MontaBlocoEqptoReator(LisEqpto(lisEXT, eqptoREATOR)))              return(false);
            if (! MontaBlocoEqptoSuprimento(LisEqpto(lisEXT, eqptoSUPRIMENTO)))      return(false);
            if (! MontaBlocoEqptoTrafoZZ(LisEqpto(lisEXT, eqptoTRAFOZZ)))            return(false);
            if (! MontaBlocoEqptoMedidor(LisEqpto(lisEXT, eqptoMEDIDOR)))            return(false);
            }
         //determina Ligacoes da Rede presentes em lisEXT
         lisLIG->Clear();
         if (LisLigacao(lisEXT, lisLIG, rede) == 0) continue;
         //monta blocos de Ligacao da Rede
         if (! MontaBlocoEqptoChave(LisEqpto(lisLIG, eqptoCHAVE)))                   return(false);
         if (! MontaBlocoEqptoCapserie(LisEqpto(lisLIG, eqptoCAPSERIE)))             return(false);
         if (! MontaBlocoEqptoReguladorAutomatico(LisEqpto(lisLIG, eqptoREGULADOR))) return(false);
         if (! MontaBlocoEqptoReguladorTapFixo(LisEqpto(lisLIG, eqptoREGULADOR)))    return(false);
         if (! MontaBlocoEqptoTrafoMonofasico(LisEqpto(lisLIG, eqptoTRAFO)))         return(false);
         if (! MontaBlocoEqptoTrafo(LisEqpto(lisLIG, eqptoTRAFO)))                   return(false);
         if (! MontaBlocoEqptoTrafo3E(LisEqpto(lisLIG, eqptoTRAFO3E)))               return(false);
         if (! MontaBlocoEqptoTrechoArranjoProprio(LisEqpto(lisLIG, eqptoTRECHO)))   return(false);
         if (! MontaBlocoEqptoTrechoArranjoTipico(LisEqpto(lisLIG, eqptoTRECHO)))    return(false);
         //monta blocos de Mutuas
         if (! MontaBlocoEqptoMutua(LisEqpto(lisLIG, eqptoMUTUA)))                   return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaFormato(void)
   {
   //variáveis locais
   TStrings  *fmt;
   VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_fmt = path->DirUsuario() + "\\FormatoArqTexto.txt";

   //verifica se existe o arquivo com especificação do formato
   if (! FileExists(arq_fmt)) return(true);
   //lê arquivo e inclui
   fmt = new TStringList();
   fmt->LoadFromFile(arq_fmt);
   //insere linhas do arquivo de formato em lines
   lines->Add(AnsiString::StringOfChar('=', 100));
   for (int n = 0; n < fmt->Count; n++)
      {
      lines->Add("// " + fmt->Strings[n]);
      }
   //destrói objeto
   delete fmt;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaRedes(AnsiString arq_txt)
   {
   try{//define Extern_id
      IniciaObjAsIdTipos();
      IniciaObjAsIdArranjos();
      IniciaObjAsIdCurvasTipicas();
      IniciaObjAsIdEqptos();
      //reinicia lines
      lines->Clear();
      //configura ExportaEqpto
      exporta_eqpto->Separador = Separador;
      //insere Bloco indicando criptografia
      if (! MontaBlocoCriptografia()) return(false);
      //insere Bloco de versão
	  if ( ! MontaBlocoVersao()) return(false);
	  //insere Bloco de diagnóstico para fluxo
	  if ( ! MontaBlocoDiagnostico()) return(false);
	  //exporta TipoChave, TipoConsumidor e TipoRede
      if ( ! ExportaTipos()) return(false);
      //exporta Suporte, Cabos e Arranjos
      if ( ! ExportaArranjos()) return(false);
      //exporta Curvas típicas
      if ( ! ExportaCurvasTipicas()) return(false);
      //exporta Redes
      if (! ExportaRedes()) return(false);
      //exporta especificação Formato do arquivo texto
      if ( ! ExportaFormato()) return(false);
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
/*
//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaRedes(void)
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
*/  //DVK 2015.10.26 exporta na sequência: SED, SDMT, SDBT e outros tipos,
//                     contornando o problema de importar com a carga concentrada,
//                     caso alguma rede SDBT seja exportada antes de sua rede SDMT
//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaRedes(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();
   TList   *lisORDENADA = new TList();

   //organiza redes por tipo
   OrdenaRedes(lisREDE, lisORDENADA);
   try{//loop p/ todas Redes ordenadas por tipo
      for (int nr = 0; nr < lisORDENADA->Count; nr++)
         {
         rede = (VTRede*)lisORDENADA->Items[nr];
         if (! ExportaUmaRede(rede)) break;
         }
      //destroi lista ordenada
      delete lisORDENADA;
      }catch(Exception &e)
         {
         if(lisORDENADA) delete lisORDENADA;
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaUmaRede(VTRede *rede, AnsiString arq_txt)
   {
   try{//define Extern_id
      IniciaObjAsIdTipos();
      IniciaObjAsIdArranjos();
      IniciaObjAsIdCurvasTipicas();
      IniciaObjAsIdEqptos(rede);
      //reinicia lines
      lines->Clear();
      //configura ExportaEqpto
      exporta_eqpto->Separador = Separador;
      //insere Bloco indicando criptografia
      if (! MontaBlocoCriptografia()) return(false);
      //insere Bloco de versão
      if ( ! MontaBlocoVersao()) return(false);
      //exporta TipoChave e TipoRede
      if ( ! ExportaTipos()) return(false);
      //exporta Suporte, Cabos e Arranjos
      if ( ! ExportaArranjos()) return(false);
      //exporta Curvas típicas
      if ( ! ExportaCurvasTipicas()) return(false);
      //exporta Redes
      if (! ExportaUmaRede(rede)) return(false);
      //exporta especificação Formato do arquivo texto
      if ( ! ExportaFormato()) return(false);
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
bool __fastcall TExportaTXT::ExportaUmaRede(VTRede *rede)
   {
   try{//verifica se a Rede está carregada
      if (! rede->Carregada) return(true);
      //insere header
      InsereHeader("Rede");
      //insere dados da Rede
      lines->Add(exporta_eqpto->MontaRede(rede));
      //monta blocos de Barras da Rede
      if (! MontaBlocoEqptoBarra(LisEqpto(rede, eqptoBARRA)))                   return(false);
      //monta blocos de Ligacao da Rede
      if (! MontaBlocoEqptoChave(LisEqpto(rede, eqptoCHAVE)))                   return(false);
      if (! MontaBlocoEqptoCapserie(LisEqpto(rede, eqptoCAPSERIE)))             return(false);
      if (! MontaBlocoEqptoReguladorAutomatico(LisEqpto(rede, eqptoREGULADOR))) return(false);
      if (! MontaBlocoEqptoReguladorTapFixo(LisEqpto(rede, eqptoREGULADOR)))    return(false);
      if (! MontaBlocoEqptoTrafoMonofasico(LisEqpto(rede, eqptoTRAFO)))         return(false);
      if (! MontaBlocoEqptoTrafo(LisEqpto(rede, eqptoTRAFO)))                   return(false);
      if (! MontaBlocoEqptoTrafo3E(LisEqpto(rede, eqptoTRAFO3E)))               return(false);
      if (! MontaBlocoEqptoTrechoArranjoProprio(LisEqpto(rede, eqptoTRECHO)))   return(false);
      if (! MontaBlocoEqptoTrechoArranjoTipico(LisEqpto(rede, eqptoTRECHO)))    return(false);
      //monta blocos de Mutuas da Rede
      if (! MontaBlocoEqptoMutua(LisEqpto(rede, eqptoMUTUA)))                   return(false);
	  //monta blocos de Eqbar da Rede
	  if (! MontaBlocoEqptoBateria(LisEqpto(rede, eqptoBATERIA)))		        return(false);
	  if (! MontaBlocoEqptoCapacitor(LisEqpto(rede, eqptoCAPACITOR)))           return(false);
	  if (! MontaBlocoEqptoCargaIP(LisEqpto(rede, eqptoCARGA)))                 return(false);
      if (! MontaBlocoEqptoCargaComMedicao(LisEqpto(rede, eqptoCARGA)))         return(false);
      if (! MontaBlocoEqptoCargaSemMedicao(LisEqpto(rede, eqptoCARGA)))         return(false);
      if (! MontaBlocoEqptoFiltro(LisEqpto(rede, eqptoFILTRO)))                 return(false);
      if (! MontaBlocoEqptoGerador(LisEqpto(rede, eqptoGERADOR)))               return(false);
      if (! MontaBlocoEqptoReator(LisEqpto(rede, eqptoREATOR)))                 return(false);
      if (! MontaBlocoEqptoSuprimento(LisEqpto(rede, eqptoSUPRIMENTO)))         return(false);
      if (! MontaBlocoEqptoTrafoZZ(LisEqpto(rede, eqptoTRAFOZZ)))               return(false);
      if (! MontaBlocoEqptoMedidor(LisEqpto(rede, eqptoMEDIDOR)))               return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaTXT::IniciaObjAsIdArranjos(void)
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
void __fastcall TExportaTXT::IniciaObjAsIdCurvas(TList *lisCURVA)
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
void __fastcall TExportaTXT::IniciaObjAsIdCurvasTipicas(void)
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
void __fastcall TExportaTXT::IniciaObjAsIdEqptos(void)
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
void __fastcall TExportaTXT::IniciaObjAsIdEqptos(TList *lisEQP)
   {
   //variáveis locais
   int     index = -1;
   TList   *lisCANAL;
   VTEqpto *eqpto;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto      = (VTEqpto*)lisEQP->Items[n];
      eqpto->Obj = (TObject*)(index--);
      //verifica se Eqpto é um Medidor
      if (eqpto->Tipo() == eqptoMEDIDOR)
         {
         lisCANAL = ((VTMedidor*)eqpto)->LisCanal();
         for (int nc = 0; nc < lisCANAL->Count; nc++)
            {
            eqpto = (VTEqpto*)lisCANAL->Items[nc];
            eqpto->Obj = (TObject*)(index--);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TExportaTXT::IniciaObjAsIdEqptos(VTRede *rede)
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
void __fastcall TExportaTXT::IniciaObjAsIdTipos(void)
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
void __fastcall TExportaTXT::InsereHeader(char *header)
   {
   lines->Add("//" + AnsiString::StringOfChar('-', 80));
   lines->Add(header + AnsiString(Separador));
   }

//---------------------------------------------------------------------------
TList* __fastcall TExportaTXT::LisEqpto(TList *lisORIG, int tipo_eqpto)
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
int __fastcall TExportaTXT::LisLigacao(TList *lisORIG, TList *lisDEST, VTRede *rede)
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
TList* __fastcall TExportaTXT::LisEqpto(VTRede *rede, int tipo_eqpto)
   {
   //variáveis locais
   bool barra_isolada = true;

   //reinicia lisEQP apenas com os Eqptos do tipo indicado
   lisEQP->Clear();
   rede->LisEqpto(lisEQP, tipo_eqpto, barra_isolada);
   return(lisEQP);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoArranjoCaboPorfase(void)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARRANJO = arranjos->LisArranjo();

   //insere header
   InsereHeader("ArranjoCaboUnipolar");
   //loop p/ todos Arranjos
   for (int n = 0; n < lisARRANJO->Count; n++)
      {
      arranjo = (VTArranjo*)lisARRANJO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (arranjo->TipoCfg == arranjoCABOxFASE)
         {
         lines->Add(exporta_eqpto->MontaArranjoCaboPorFase(arranjo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoArranjoCaboZ0Z1(void)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARRANJO = arranjos->LisArranjo();

   //insere header
   InsereHeader("ArranjoCaboMultiplexado");
   //loop p/ todos Arranjos
   for (int n = 0; n < lisARRANJO->Count; n++)
      {
      arranjo = (VTArranjo*)lisARRANJO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {
         lines->Add(exporta_eqpto->MontaArranjoCaboZ0Z1(arranjo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoArranjoPUSB100(void)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARRANJO = arranjos->LisArranjo();

   //insere header
   InsereHeader("ArranjoPuSb100");
   //loop p/ todos Arranjos
   for (int n = 0; n < lisARRANJO->Count; n++)
      {
      arranjo = (VTArranjo*)lisARRANJO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (arranjo->TipoCfg == arranjoPU_SB100)
         {
         lines->Add(exporta_eqpto->MontaArranjoPUSb100(arranjo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoArranjoZ0Z1(void)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARRANJO = arranjos->LisArranjo();

   //insere header
   InsereHeader("ArranjoZ0Z1");
   //loop p/ todos Arranjos
   for (int n = 0; n < lisARRANJO->Count; n++)
      {
      arranjo = (VTArranjo*)lisARRANJO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (arranjo->TipoCfg == arranjoZ0Z1)
         {
         lines->Add(exporta_eqpto->MontaArranjoZ0Z1(arranjo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCaboUnipolar(void)
   {
   //variáveis locais
   VTCabo     *cabo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisCABO  = arranjos->LisCabo();

   //insere header
   InsereHeader("CaboUnipolar");
   //loop p/ todos Cabos
   for (int n = 0; n < lisCABO->Count; n++)
      {
      cabo = (VTCabo*)lisCABO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (cabo->TipoCfg == caboR_GMR)
         {
         lines->Add(exporta_eqpto->MontaCaboUnipolar(cabo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCaboZ0Z1(void)
   {
   //variáveis locais
   VTCabo     *cabo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisCABO  = arranjos->LisCabo();

   //insere header
   InsereHeader("CaboMultiplexado");
   //loop p/ todos Cabos
   for (int n = 0; n < lisCABO->Count; n++)
      {
      cabo = (VTCabo*)lisCABO->Items[n];
      //verifica se Arranjo é tipo arranjoCABOxFASE
      if (cabo->TipoCfg != caboR_GMR)
         {
         lines->Add(exporta_eqpto->MontaCaboZ0Z1(cabo));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCriptografia(void)
   {
   //variáveis locais
   bool cript_enabled;

   //salva flag de criptografia e desabilita criptografica
   cript_enabled = Criptografar;
   Criptografar  = false;
   //insere header
   InsereHeader("Criptografia");
   //verifica se criptografia está habilitada
   if (cript_enabled) {lines->Add("Enabled");}
   else               {lines->Add("Disabled");}
   //restaura flag de criptografia
   Criptografar = cript_enabled;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaCapacitor(void)
   {
   //variáveis locais
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //insere header
   InsereHeader("CurvaCapacitor");
   return(MontaBlocoCurvaTipica(curvas->ListaCurvaTipicaCapacitor()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaDemanda(void)
   {
   //variáveis locais
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //insere header
   InsereHeader("CurvaCarga");
   return(MontaBlocoCurvaTipica(curvas->ListaCurvaTipicaCarga()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaGerador(void)
   {
   //variáveis locais
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //insere header
   InsereHeader("CurvaGerador");
   return(MontaBlocoCurvaTipica(curvas->ListaCurvaTipicaGeradorFotovoltaico()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaIluninacaoPublica(void)
   {
   //variáveis locais
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //insere header
   InsereHeader("CurvaIP");
   return(MontaBlocoCurvaTipica(curvas->ListaCurvaTipicaIP()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaReator(void)
   {
   //variáveis locais
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //insere header
   InsereHeader("CurvaReator");
   return(MontaBlocoCurvaTipica(curvas->ListaCurvaTipicaReator()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoCurvaTipica(TList *lisCURVA)
   {
   //variáveis locais
   VTCurva  *curva;

   //loop p/ todos Cabos
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      lines->Add(exporta_eqpto->MontaCurvaTipica(curva));
      }
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15928772
bool __fastcall TExportaTXT::MontaBlocoDiagnostico(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTPadraoRede *padraoRede;
   double limites[27];
   int nc = 0;

   //proteção
   if(geral == NULL) return(false);
   //insere header
   InsereHeader("Diagnostico");
   //carrega valores redeSEC
   padraoRede = geral->ExistePadraoRede(redeSEC);
   limites[nc] = padraoRede->V1;
   limites[++nc] = padraoRede->V2;
   limites[++nc] = padraoRede->V3;
   limites[++nc] = padraoRede->V4;
   limites[++nc] = padraoRede->FPI1;
   limites[++nc] = padraoRede->FPI2;
   limites[++nc] = padraoRede->FPS1;
   limites[++nc] = padraoRede->FPS2;
   //carrega valores redePRI
   padraoRede = geral->ExistePadraoRede(redePRI);
   limites[++nc] = padraoRede->V1;
   limites[++nc] = padraoRede->V2;
   limites[++nc] = padraoRede->V3;
   limites[++nc] = padraoRede->V4;
   limites[++nc] = padraoRede->FPI1;
   limites[++nc] = padraoRede->FPI2;
   limites[++nc] = padraoRede->FPS1;
   limites[++nc] = padraoRede->FPS2;
   //carrega valores redeSUB
   padraoRede = geral->ExistePadraoRede(redeSUB);
   limites[++nc] = padraoRede->V1;
   limites[++nc] = padraoRede->V2;
   limites[++nc] = padraoRede->V3;
   limites[++nc] = padraoRede->V4;
   limites[++nc] = padraoRede->FPI1;
   limites[++nc] = padraoRede->FPI2;
   limites[++nc] = padraoRede->FPS1;
   limites[++nc] = padraoRede->FPS2;
   //cores
   limites[++nc] = geral->COR.Faixa[diagBOM];
   limites[++nc] = geral->COR.Faixa[diagREGULAR];
   limites[++nc] = geral->COR.Faixa[diagRUIM];
   lines->Add(exporta_eqpto->MontaDiagnostico(limites));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoEqptoBarra(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoBateria(TList *lisEQP)
   {
   if (lisEQP->Count > 0)
	  {//insere header
	  InsereHeader("Bateria");
	  //loop p/ todos eqptos
	  for (int n = 0; n < lisEQP->Count; n++)
		 {
		 lines->Add(exporta_eqpto->MontaBateria((VTBateria*)lisEQP->Items[n]));
		 }
	  }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoEqptoCapacitor(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoCapserie(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoCargaIP(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoCargaComMedicao(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoCargaSemMedicao(TList *lisEQP)
   {
   //variáveis locais
   VTCarga *carga;

   //mantém em lisEQP somente as Cargas com curva típica
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      if (! carga->curva->Tipica) lisEQP->Delete(n);
      //retira cargas IP  //DVK 2015.10.23
      if (carga->IP) lisEQP->Delete(n);
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
bool __fastcall TExportaTXT::MontaBlocoEqptoChave(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoFiltro(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoGerador(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoMedidor(TList *lisEQP)
   {
   //variáveis locais
   TList     *lisCANAL;
   VTCanal   *canal;
   VTMedidor *medidor;

   if (lisEQP->Count > 0)
      {//loop p/ todos eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {//insere header
         InsereHeader("Medidor");
         medidor = (VTMedidor*)lisEQP->Items[n];
         lines->Add(exporta_eqpto->MontaMedidor(medidor));
         //loop p/ todos Canais do Medidor
         InsereHeader("Canal");
         lisCANAL = medidor->LisCanal();
         for (int nc = 0; nc < lisCANAL->Count; nc++)
            {
            canal = (VTCanal*)lisCANAL->Items[nc];
            lines->Add(exporta_eqpto->MontaMedidorCanal(canal));
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoEqptoMutua(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoReator(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoRede(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoReguladorAutomatico(TList *lisEQP)
   {
   //variáveis locais
   VTRegulador *regulador;

   //mantém em lisEQP somente os Reguladores com ajsute automático
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
	  regulador = (VTRegulador*)lisEQP->Items[n];
	  if ((regulador->modelo == reguENERQCT) || (regulador->modelo == reguFIXO))
		 lisEQP->Delete(n);
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
bool __fastcall TExportaTXT::MontaBlocoEqptoReguladorTapFixo(TList *lisEQP)
   {
   //variáveis locais
   VTRegulador *regulador;

   //mantém em lisEQP somente os Reguladores com ajsute fixo
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
	  regulador = (VTRegulador*)lisEQP->Items[n];
	  if (regulador->modelo != reguFIXO)
		 lisEQP->Delete(n);
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
bool __fastcall TExportaTXT::MontaBlocoEqptoSuprimento(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrafo(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrafo3E(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrafoMonofasico(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrafoZZ(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrechoArranjoProprio(TList *lisEQP)
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
bool __fastcall TExportaTXT::MontaBlocoEqptoTrechoArranjoTipico(TList *lisEQP)
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

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoFimDados(void)
   {
   //insere header
   InsereHeader("END");

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::ExportaTipos(void)
   {
   try{//exporta TipoChave e TipoRede
      if ( ! MontaBlocoTipoChave())      return(false);
      if ( ! MontaBlocoTipoConsumidor()) return(false);
      if ( ! MontaBlocoTipoRede())       return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoSuporte(void)
   {
   //variáveis locais
   VTSuporte  *suporte;
   VTArranjos *arranjos   = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisSUPORTE = arranjos->LisSuporte();

   //inseere header
   InsereHeader("EstruturaSuporte");
   //loop p/ todos Suporte
   for (int n = 0; n < lisSUPORTE->Count; n++)
      {
      suporte = (VTSuporte*)lisSUPORTE->Items[n];
      lines->Add(exporta_eqpto->MontaSuporte(suporte));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoTipoChave(void)
   {
   //variáveis locais
   VTTipoChave *tipochave;
   VTTipos     *tipos      = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoChave();

   //inseere header
   InsereHeader("TipoChave");
   //loop p/ todos TipoChave
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      lines->Add(exporta_eqpto->MontaTipoChave((VTTipoChave*)lisTIPO->Items[n]));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoTipoConsumidor(void)
   {
   //variáveis locais
   VTClasses *classes      = (VTClasses*)apl->GetObject(__classid(VTClasses));
   TList     *lisCLASSE = classes->LisClasse();

   //inseere header
   InsereHeader("TipoConsumidor");
   //loop p/ todas classes
   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      lines->Add(exporta_eqpto->MontaTipoConsumidor((VTClasse*)lisCLASSE->Items[n]));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoTipoRede(void)
   {
   //variáveis locais
   VTTipoRede *tiporede;
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList      *lisTIPO = tipos->LisTipoRede();

   //inseere header
   InsereHeader("TipoRede");
   //loop p/ todos TipoChave
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      lines->Add(exporta_eqpto->MontaTipoRede((VTTipoRede*)lisTIPO->Items[n]));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaTXT::MontaBlocoVersao(void)
   {
   //inseere header
   InsereHeader("Revisao");
   lines->Add(exporta_eqpto->MontaVersao());

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaTXT::OrdenaRedes(TList *lisREDE, TList *lisORDENADA)
   {//DVK 2015.10.26
   //variaveis locais
   TList   *lisSED, *lisSDMT, *lisSDBT, *lisOUTROS;
   VTRede  *rede;

   //cria listas
   lisSED      = new TList();
   lisSDMT     = new TList();
   lisSDBT     = new TList();
   lisOUTROS   = new TList();
   //separa tipos
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      switch(rede->TipoRede->Id)
         {
         case redeETD: lisSED->Add(rede);    break;
         case redePRI: lisSDMT->Add(rede);   break;
         case redeSEC: lisSDBT->Add(rede);   break;
         default:      lisOUTROS->Add(rede); break;
         }
      }
   //monta lista de redes ordenadas por tipo
   CopiaTList(lisSED,lisORDENADA);
   CopiaTList(lisSDMT,lisORDENADA);
   CopiaTList(lisSDBT,lisORDENADA);
   CopiaTList(lisOUTROS,lisORDENADA);
   //destroi listas
   delete lisSED;
   delete lisSDMT;
   delete lisSDBT;
   delete lisOUTROS;
   }

//-----------------------------------------------------------------------------
// eof


