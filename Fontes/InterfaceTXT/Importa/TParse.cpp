//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include <DLL_Inc\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Progresso\VTProgresso.h>
#include "TParse.h"
#include "VTExtrator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTParse* NewObjParse(VTApl *apl)
   {
   return(new TParse(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TParse::TParse(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   try{//cria objeto TStringList
      campos        = new TStringList();
      lines         = new TStringList();
      ActionCancela = new TAction(NULL);
      ActionCancela->OnExecute = ActionCancelaExecute;
      }catch(Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
__fastcall TParse::~TParse(void)
   {
   //destrói objetos criados
   if (ActionCancela) {delete ActionCancela; ActionCancela = NULL;}
   if (lines)         {delete lines;         lines         = NULL;}
   if (campos)        {delete campos;        campos        = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TParse::ActionCancelaExecute(TObject *Sender)
   {
   //aborta a leitura
   estado = estCANCELA;
   }

//-----------------------------------------------------------------------------
void __fastcall TParse::AtualizaBloco(AnsiString bloco)
   {
   if (plog)      plog->DefineValor("Bloco", bloco);
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TParse::AtualizaLogProgresso(AnsiString bloco)
   {
   if (plog)      plog->DefineValor("Bloco", bloco);
   if (progresso) progresso->Add(bloco);
   }
*/    //DVK 2015.10.23 reescrito para não poluir a barra de progresso
//-----------------------------------------------------------------------------
void __fastcall TParse::AtualizaLogProgresso(void)
   {
   switch(estado)
      {
	  case estBEGIN:             if (progresso) progresso->Add("Inicio"); break;
	  case estVERSAO:
	  case estCANCELA:
	  case estEND:               if (progresso) progresso->Add("Fim"); break;
	  //diagnostico
	  case estDIAGNOSTICO:       if (progresso) progresso->Add("Diagnóstico"); break;
	  //tipos
	  case estTIPO_CHAVE:        if (progresso) progresso->Add("Tipo de chave"); break;
	  case estTIPO_CONSUMIDOR:   if (progresso) progresso->Add("Tipo de consumidor"); break;
      case estTIPO_REDE:         if (progresso) progresso->Add("Tipo de rede"); break;
      //curvas típicas
      case estCURVA_CAPACITOR:
      case estCURVA_CARGA:
      case estCURVA_CARGA_IP:
      case estCURVA_GERADOR:
      case estCURVA_REATOR:      if (progresso) progresso->Add("Leitura de curvas típicas"); break;
      //suportes, cabos e arranjos
      case estSUPORTE:
      case estCABO_UNIPOLAR:
      case estCABO_MULTIPLEXADO:
      case estARRANJO_Z0Z1:
      case estARRANJO_PUSB100:
      case estARRANJO_CABO_Z0Z1:
      case estARRANJO_CABOxFASE: if (progresso) progresso->Add("Leitura de cabos e arranjos"); break;
      //rede
      case estBARRA:             break;
      case estCAPACITOR:
      case estCAPSERIE:          if (progresso) progresso->Add("Leitura de capacitor"); break;
      case estCARGA_IP:
      case estCARGA_MEDICAO:
      case estCARGA_TIPICA:      if (progresso) progresso->Add("Leitura de cargas"); break;
      case estCHAVE:             if (progresso) progresso->Add("Leitura de chave"); break;
      case estGERADOR:           if (progresso) progresso->Add("Leitura de gerador"); break;
      case estMUTUA:
      case estREATOR:            break;
      case estREDE:              if (progresso) progresso->Add("Leitura de rede", true); break;
      case estREGULADOR_AUTO:
      case estREGULADOR_FIXO:    if (progresso) progresso->Add("Leitura de regulador"); break;
      case estSUPRIMENTO:
      case estTRAFO_2E:
      case estTRAFO_3E:
      case estTRAFO_MONO:
      case estTRAFO_ZZ:          if (progresso) progresso->Add("Leitura de trafo"); break;
      case estTRECHO_TIPICO:     if (progresso) progresso->Add("Leitura de trecho típico"); break;
      case estTRECHO_Z0Z1:       if (progresso) progresso->Add("Leitura de trecho Z0Z1"); break;
      default: break;
      }
   }

//-----------------------------------------------------------------------------
bool __fastcall TParse::Executa(AnsiString arq_lista, TStrings *lines)
   {
   //não implementado nesta classe
   return(false);
   }

//-----------------------------------------------------------------------------
bool __fastcall TParse::Executa(AnsiString arq_txt)
   {
   //variáveis locais
   int  novo_estado;
   bool resp   = true;

   //obtém objetos
   extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   //lê arquivo
   lines->LoadFromFile(arq_txt);
   //confira progresso, habilitando a opção de cancelar
   //if (progresso) progresso->HabilitaUsuarioCancelar(ActionCancela);
   //atualiza  progresso
   if (progresso) progresso->Add("Arquivo " + ExtractFileName(arq_txt), true);
   //atualiza linha/Bloco no Resumo
   if (plog) plog->DefineValor("Linha", "");
   if (plog) plog->DefineValor("Bloco", "");
   //DVK 2013.07.12
   if (plog) plog->DefineValor("Arquivo", ExtractFileName(arq_txt));
   //innicia Extrator
   extrator->TrataIniciaDados(ChangeFileExt(ExtractFileName(arq_txt), ""));
   //loop de leitura
   estado = estBEGIN;
   //for (int nl = 0; (resp)&&(estado != estCANCELA)&&(estado != estEND)&&(nl < lines->Count); nl++)
   for (int nl = 0; (resp)&&(estado != estCANCELA)&&(nl < lines->Count); nl++)
      {//atualiza número da linha em plog
      if (plog) plog->DefineValor("Linha", IntToStr(nl + 1));
      //extrai campos da linha de texto
      ExtraiCampos(lines->Strings[nl], campos);
      if (campos->Count == 0) continue;
      //verifica se a linha possui palavra chave
      if (PalavraChave(novo_estado))
         {//mudança de estado: verifica início/fim de inserção de Barra
         if      (novo_estado == estBARRA) extrator->InsereBarraStart();
         else if (estado      == estBARRA) extrator->InsereBarraStop();
         //atualiza estado
         estado = novo_estado;
         AtualizaLogProgresso();
         continue;
         }
      else
         {//trata strings da linha de acordo c/ estado
         switch(estado)
            {
            case estBEGIN:            /*nada a fazer*/                                         break;
            case estVERSAO:           resp = extrator->InsereVersao(campos);                   break;
            case estCANCELA:          /*nada a fazer*/                                         break;
            case estEND:              /*nada a fazer*/                                         break;
			//diagnostico
			case estDIAGNOSTICO:          resp = extrator->InsereDiagnostico(campos);          break;
			//tipos
            case estTIPO_CHAVE:           resp = extrator->InsereTipoChave(campos);            break;
            case estTIPO_CONSUMIDOR:      resp = extrator->InsereTipoConsumidor(campos);       break;
            case estTIPO_REDE:            resp = extrator->InsereTipoRede(campos);             break;
            //curvas típicas
            case estCURVA_CAPACITOR:      resp = extrator->InsereCurvaCapacitor(campos);       break;
            case estCURVA_CARGA:          resp = extrator->InsereCurvaCarga(campos);           break;
            case estCURVA_CARGA_IP:       resp = extrator->InsereCurvaCargaIP(campos);         break;
            case estCURVA_GERADOR:        resp = extrator->InsereCurvaGerador(campos);         break;
            case estCURVA_REATOR:         resp = extrator->InsereCurvaReator(campos);          break;
            //suportes, cabos e arranjos
            case estSUPORTE:             resp = extrator->InsereSuporte(campos);               break;
            case estCABO_UNIPOLAR:       resp = extrator->InsereCaboUnipolar(campos);          break;
            case estCABO_MULTIPLEXADO:   resp = extrator->InsereCaboZ0Z1(campos);              break;
            case estARRANJO_Z0Z1:        resp = extrator->InsereArranjoZ0Z1(campos);           break;
            case estARRANJO_PUSB100:     resp = extrator->InsereArranjoPUSB100(campos);        break;
            case estARRANJO_CABO_Z0Z1:   resp = extrator->InsereArranjoCaboZ0Z1(campos);       break;
            case estARRANJO_CABOxFASE:   resp = extrator->InsereArranjoCaboPorfase(campos);    break;
			//rede
			case estBARRA:               resp = extrator->InsereBarra(campos);                 break;
			case estBATERIA:             resp = extrator->InsereBateria(campos);               break;
            case estCANAL:               resp = extrator->InsereMedidorCanal(campos);          break;
            case estCAPACITOR:           resp = extrator->InsereCapacitor(campos);             break;
            case estCAPSERIE:            resp = extrator->InsereCapacitorSerie(campos);        break;
            case estCARGA_IP:            resp = extrator->InsereCargaIP(campos);               break;
            case estCARGA_MEDICAO:       resp = extrator->InsereCargaComMedicao(campos);       break;
            case estCARGA_TIPICA:        resp = extrator->InsereCargaTipica(campos);           break;
            case estCHAVE:               resp = extrator->InsereChave(campos);                 break;
            case estGERADOR:             resp = extrator->InsereGerador(campos);               break;
            case estMEDIDOR:             resp = extrator->InsereMedidor(campos);               break;
            case estMUTUA:               resp = extrator->InsereMutua(campos);                 break;
            case estREATOR:              resp = extrator->InsereReator(campos);                break;
            case estREDE:                resp = extrator->InsereRede(campos);                  break;
            case estREGULADOR_AUTO:      resp = extrator->InsereReguladorAutomatico(campos);   break;
            case estREGULADOR_FIXO:      resp = extrator->InsereReguladorTapFixo(campos);      break;
            case estSUPRIMENTO:          resp = extrator->InsereSuprimento(campos);            break;
            case estTRAFO_2E:            resp = extrator->InsereTrafo2E(campos);               break;
            case estTRAFO_3E:            resp = extrator->InsereTrafo3E(campos);               break;
            case estTRAFO_MONO:          resp = extrator->InsereTrafoMono(campos);             break;
            case estTRAFO_ZZ:            resp = extrator->InsereTrafoZZ(campos);               break;
            case estTRECHO_TIPICO:       resp = extrator->InsereTrechoTipico(campos);          break;
            case estTRECHO_Z0Z1:         resp = extrator->InsereTrechoZ0Z1(campos);            break;
            default:
               if (plog) plog->Add("Erro no formato do arquivo texto");
               resp = false;
               break;
            }
         }
      }
   //verifica se a importação foi cancelada
   if (estado == estCANCELA) resp = false;
   //confira progresso, desabilitando a opção de cancelar
   //if (progresso) progresso->HabilitaUsuarioCancelar(NULL);
   //atualiza linha/Bloco no Resumo
   if (plog) plog->DefineValor("Linha", "");
   if (plog) plog->DefineValor("Bloco", "");
   //complementa dados p/ diag. esquemático
   if (resp) resp = extrator->TrataFimDados();
   return(resp);
   }

//---------------------------------------------------------------------------
void __fastcall TParse::ExtraiCampos(AnsiString txt, TStrings *campos)
   {
   //variáveis locais
   #define EOS '\0'
   char    *p_ini, *p_fim;
   char    separador = ';';
   int     len       = txt.Length();

   //reinicia StrinList campos
   campos->Clear();
   //filtra linha com menos de 2 caracteres
   if (len < 2) return;
   //filtra linha de comentário
   if ((txt[1] == '/') && (txt[2] == '/')) return;
   //se necessário, inclui separador no final de txt
   if (txt[len] != separador) txt += separador;
   //extrai os campos delimitados por separador em txt
   p_ini = txt.c_str();
   while ((p_fim = strchr(p_ini, separador)) != NULL)
      {//insere string na lista
      *p_fim = EOS; //insere delimitador de string
      campos->Add(AnsiString(p_ini).Trim());
      //copia em pstr o substring p_ini a p_fim-1
      *p_fim = separador;     //restaura separador no string original
      p_ini = p_fim + 1;
      }
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15928772
bool __fastcall TParse::PalavraChave(int &estado)
   {
   //variáveis locais
   AnsiString str;

   //proteção
   if (campos->Count == 0) return(false);
   //verifica se já encontrou palavra chave END
   //if (estado == estEND) return(false);
   //determina primeiro string
   str = campos->Strings[0];
   //versão
   if (str.AnsiCompareIC("Revisao")                  == 0) {estado = estVERSAO;             AtualizaBloco(str); return(true);}
   //diagnostico
   if (str.AnsiCompareIC("Diagnostico")              == 0) {estado = estDIAGNOSTICO;        AtualizaBloco(str); return(true);}
   //tipos
   if (str.AnsiCompareIC("TipoChave")                == 0) {estado = estTIPO_CHAVE;         AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TipoConsumidor")           == 0) {estado = estTIPO_CONSUMIDOR;    AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TipoRede")                 == 0) {estado = estTIPO_REDE;          AtualizaBloco(str); return(true);}
   //curvas típicas
   if (str.AnsiCompareIC("CurvaCapacitor")           == 0) {estado = estCURVA_CAPACITOR;    AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CurvaCarga")               == 0) {estado = estCURVA_CARGA;        AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CurvaIP")                  == 0) {estado = estCURVA_CARGA_IP;     AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CurvaGerador")             == 0) {estado = estCURVA_GERADOR;      AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CurvaReator")              == 0) {estado = estCURVA_REATOR;       AtualizaBloco(str); return(true);}
   //suportes, cabos e arranjos
   if (str.AnsiCompareIC("EstruturaSuporte")         == 0) {estado = estSUPORTE;            AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CaboUnipolar")             == 0) {estado = estCABO_UNIPOLAR;      AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CaboMultiplexado")         == 0) {estado = estCABO_MULTIPLEXADO;  AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ArranjoZ0Z1")              == 0) {estado = estARRANJO_Z0Z1;       AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ArranjoPuSb100")           == 0) {estado = estARRANJO_PUSB100;    AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ArranjoCaboMultiplexado")  == 0) {estado = estARRANJO_CABO_Z0Z1;  AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ArranjoCaboUnipolar")      == 0) {estado = estARRANJO_CABOxFASE;  AtualizaBloco(str); return(true);}
   //equipamentos de rede
   if (str.AnsiCompareIC("Rede")                     == 0) {estado = estREDE;               AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Barra")                    == 0) {estado = estBARRA;              AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Bateria")                  == 0) {estado = estBATERIA;            AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Canal")                    == 0) {estado = estCANAL;              AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Capacitor")                == 0) {estado = estCAPACITOR;          AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CapacitorSerie")           == 0) {estado = estCAPSERIE;           AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CargaComMedicao")          == 0) {estado = estCARGA_MEDICAO;      AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CargaIP")                  == 0) {estado = estCARGA_IP;           AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("CargaSemMedicao")          == 0) {estado = estCARGA_TIPICA;       AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Chave")                    == 0) {estado = estCHAVE;              AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Filtro")                   == 0) {estado = estFILTRO;             AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Gerador")                  == 0) {estado = estGERADOR;            AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Medidor")                  == 0) {estado = estMEDIDOR;            AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Mutua")                    == 0) {estado = estMUTUA;              AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Reator")                   == 0) {estado = estREATOR;             AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ReguladorAutomatico")      == 0) {estado = estREGULADOR_AUTO;     AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("ReguladorTapFixo")         == 0) {estado = estREGULADOR_FIXO;     AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("Suprimento")               == 0) {estado = estSUPRIMENTO;         AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TransformadorTrifasico2E") == 0) {estado = estTRAFO_2E;           AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TransformadorTrifasico3E") == 0) {estado = estTRAFO_3E;           AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TransformadorMonofasico")  == 0) {estado = estTRAFO_MONO;         AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TransformadorZigZag")      == 0) {estado = estTRAFO_ZZ;           AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TrechoArranjoTipico")      == 0) {estado = estTRECHO_TIPICO;      AtualizaBloco(str); return(true);}
   if (str.AnsiCompareIC("TrechoZ0Z1")               == 0) {estado = estTRECHO_Z0Z1;        AtualizaBloco(str); return(true);}
   //fim de arquivo
   if (str.AnsiCompareIC("END")                      == 0) {estado = estEND;                AtualizaBloco(str); return(true);}
   //não encontrou palavra chave
   return(false);
   }

//-----------------------------------------------------------------------------
//eof
