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
void __fastcall TParse::AtualizaLogProgresso(AnsiString bloco)
   {
   if (plog)      plog->DefineValor("Bloco", bloco);
   if (progresso) progresso->SetSubTitulo(bloco);
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
   if (progresso) progresso->HabilitaUsuarioCancelar(ActionCancela);
   //exibe janela de progresso
//   if (progresso) progresso->Show(arq_txt, 1, lines->Count);  //DVK 2013.07.12
   if (progresso) progresso->Show(ExtractFileName(arq_txt), 1, lines->Count);
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
      {//atualiza janela de progresso
      if (progresso) progresso->NextPosition();
		//atualiza número da linha em plog
		if (plog) plog->DefineValor("Linha", IntToStr(nl + 1));
      //extrai campos da linha de texto
      ExtraiCampos(lines->Strings[nl], campos);
      if (campos->Count == 0) continue;
      //verifica se a linha possui palavra chave
      if (PalavraChave(novo_estado))
			{//atualiza estado
			estado = novo_estado;
			continue;
         }
      else
         {//trata strings da linha de acordo c/ estado
         switch(estado)
            {
				case estBEGIN:              /*nada a fazer*/                           break;
				case estVERSAO:             resp = extrator->InsereVersao(campos);     break;
				case estCANCELA:            /*nada a fazer*/                           break;
				case estEND:                /*nada a fazer*/                           break;
				//arranjos
				case estCONDUTOR:         resp = extrator->InsereCondutor(campos);     break;
				case estCOORDENADA:       resp = extrator->InsereCoordenada(campos);   break;
				case estCARGA_BT:         resp = extrator->InsereCargaBT(campos);      break;
				case estCARGA_MT:         resp = extrator->InsereCargaMT(campos);      break;
				case estCHAVE_BT:         resp = extrator->InsereChaveBT(campos);      break;
				case estCHAVE_MT:         resp = extrator->InsereChaveMT(campos);      break;
				case estCIRCUITO:         resp = extrator->InsereCircuito(campos);     break;
				case estGERADOR_BT:       resp = extrator->InsereGeradorBT(campos);    break;
				case estGERADOR_MT:       resp = extrator->InsereGeradorMT(campos);    break;
				case estRAMAL_BT:         resp = extrator->InsereRamalBT(campos);      break;
				case estREGULADOR_MT:     resp = extrator->InsereReguladorMT(campos);  break;
				case estSEGMENTO_BT:      resp = extrator->InsereSegmentoBT(campos);   break;
				case estSEGMENTO_MT:      resp = extrator->InsereSegmentoMT(campos);   break;
				case estTRAFO_AT_MT:      resp = extrator->InsereTrafoAT_MT(campos);   break;
				case estTRAFO_MT_BT:      resp = extrator->InsereTrafoMT_BT(campos);   break;
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
   if (progresso) progresso->HabilitaUsuarioCancelar(NULL);
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
	if (str.AnsiCompareIC("Revisao")    == 0) {estado = estVERSAO;       AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("Condutor")   == 0) {estado = estCONDUTOR;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("Coordenada") == 0) {estado = estCOORDENADA;   AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("CargaBT")    == 0) {estado = estCARGA_BT;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("CargaMT")    == 0) {estado = estCARGA_MT;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("ChaveBT")    == 0) {estado = estCHAVE_BT;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("ChaveMT")    == 0) {estado = estCHAVE_MT;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("Circuito")   == 0) {estado = estCIRCUITO;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("GeradorBT")  == 0) {estado = estGERADOR_BT;   AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("GeradorMT")  == 0) {estado = estGERADOR_MT;   AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("RamalBT")    == 0) {estado = estRAMAL_BT;     AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("ReguladorMT")== 0) {estado = estREGULADOR_MT; AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("SegmentoBT") == 0) {estado = estSEGMENTO_BT;  AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("SegmentoMT") == 0) {estado = estSEGMENTO_MT;  AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("TrafoAT_MT") == 0) {estado = estTRAFO_AT_MT;  AtualizaLogProgresso(str); return(true);}
	if (str.AnsiCompareIC("TrafoMT_BT") == 0) {estado = estTRAFO_MT_BT;  AtualizaLogProgresso(str); return(true);}
	//fim de arquivo
	if (str.AnsiCompareIC("END")        == 0) {estado = estEND;          AtualizaLogProgresso(str); return(true);}
	//não encontrou palavra chave
   return(false);
   }

//-----------------------------------------------------------------------------
//eof
