//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImporta.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Importa\Monta\VTMonta.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCargaNL.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Util\TExtraiCampos.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TImporta::TImporta(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria lista p/ objetos THarmonico
   lines  = new TStringList();
   // OOPS. Estou passando NULL no apl, para que o monta seja destruído aqui
   monta  = NewObjMontaRede(apl);
   }

//---------------------------------------------------------------------------
__fastcall TImporta::~TImporta(void)
   {
   //destrói objetos
    if (lines)  {delete lines;  lines  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TImporta::Executa(AnsiString arq_cnl, TList *lisEXT)
   {
   //variáveis locais
   VTCargaNL  *cnl;

   //salva  ponteiro p/ lista externa de CargaNL
   lisCNL = lisEXT;
   //verifica se existe arquivo de Cargas não linerares
   if (! FileExists(arq_cnl)) return(lisCNL);
   //lê arquivo de cargas não lineares
   if (! LeArquivoCarga(arq_cnl)) {LimpaTList(lisCNL);}
   // OOPS! Essas cargas deveriam ser eliminadas da base de dados antes de escrever de novo?
   //ajusta os identificadores das cargas, que ainda não existem na base de dados
   for (int i = 0; i < lisCNL->Count; i++)
      {
      if ((cnl = (VTCargaNL*)lisCNL->Items[i]) == NULL) return(NULL);
      // "Reseta" o identificador da carga
      cnl->Id = FalsoId();
      }
   return(lisCNL);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TImporta::ExisteBarra(AnsiString codigo)
   {
   //variáveis locais
   VTBarra *barra;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();

   //procura Barra pelo código
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Codigo.AnsiCompareIC(codigo) == 0) return(barra);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCargaNL* __fastcall TImporta::ExisteCarganl(int cnl_id)
   {
   //variáveis locais
   VTCargaNL *cnl;

   //procura Carganl pelo id
   for (int n = 0; n < lisCNL->Count; n++)
      {
      cnl = (VTCargaNL*)lisCNL->Items[n];
      if (cnl->Id == cnl_id) return(cnl);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TImporta::ExtraiCampos(AnsiString txt, TStringList *campos)
   {
   //variáveis locais
   TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

   //reinicia StrinList campos
   campos->Clear();
   //proteção
   if (txt.Length() < 2) return;
   //verifica se linha de comentário
   if ((txt[1] == '\/')&&(txt[2] == '\/')) return;
   //inicia conjunto de caracteres separadores
   Separators << ';';
   //inicia conjunto de caracteres de espaçamento
   WhiteSpace << ' ';
   //separa campos do txt
   ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TImporta::LeArquivoCarga(AnsiString arq_cnl)
   {
   //variáveis locais
   int           estado = 0;
   TExtraiCampos ExtraiCampos;
   TStrings      *campos;

   //configura extração
   ExtraiCampos.SeparadorInsere(';');
	ExtraiCampos.SetOpcaoFiltraComentario(true);
	ExtraiCampos.SetOpcaoTrimCampos(true);
   //lê o arquivo
   lines->LoadFromFile(arq_cnl);
   //loop p/ todas linhas do arquivo selecionado
   for (int nl = 0; nl < lines->Count; nl++)
      {//extrai campos da linha
      if ((campos = ExtraiCampos.Executa(lines->Strings[nl])) == NULL) continue;
      //proteção: linha vazia ou de comentário
      if (campos->Count == 0) continue;
      //verifica se linha c/ palavra chave
		if ((campos->Count == 1)&&(campos->Strings[0].CompareIC("CNL") == 0))
         {
         estado = 1;
         continue;
         }
      else if ((campos->Count == 1)&&(campos->Strings[0].CompareIC("HAR") == 0))
         {
         estado = 2;
         continue;
         }
      //trata campos de acordo c/ estado
      switch(estado)
         {
         case 1: LeCargaNaoLinear(campos); break;
         case 2: LeHarmonico(campos);      break;
         default:                          break;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TImporta::LeCargaNaoLinear(TStrings *campos)
   {
   //variáveis locais
   VTBarra           *barra;
   VTCargaNL         *cargaNL;
   struct strCARGANL sttCargaNL;
   AnsiString        strNatureza;
   bool              bolIndutivo;
   VTRedes*          redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //proteção: verifica se o redes está ok
   if (!redes) return;
   //proteção: verifica se o monta está ok
   if (!monta) return;
   //proteção: verifica se a lista de cargas está ok
   if (!lisCNL) return;
   //proteção: verifica o número de campos
   if (campos->Count < 11) return;
   //determina Barra
   if ((barra = ExisteBarra(campos->Strings[2])) == NULL) return;
   try{
      //inicia dados
      strNatureza = campos->Strings[4];
      strNatureza = strNatureza.UpperCase();
      bolIndutivo = (strNatureza.Pos(strNatureza) > 0);

      // OOPS
      sttCargaNL.obra          = false;
      sttCargaNL.car_id        = StrToInteger(campos->Strings[0]);
      sttCargaNL.bar_id        = barra->Id;
      sttCargaNL.codigo        = campos->Strings[1];
      sttCargaNL.extern_id     = "";
      sttCargaNL.ordem         = 1;
      sttCargaNL.fat_pot       = StrToDouble(campos->Strings[3]);
      sttCargaNL.flag_indutivo = bolIndutivo;
      sttCargaNL.flag_kVA      = true;
      sttCargaNL.snomA         = StrToDouble(campos->Strings[5]);
      sttCargaNL.angA          = StrToDouble(campos->Strings[6]);
      sttCargaNL.snomB         = StrToDouble(campos->Strings[7]);
      sttCargaNL.angB          = StrToDouble(campos->Strings[8]);
      sttCargaNL.snomC         = StrToDouble(campos->Strings[9]);
      sttCargaNL.angC          = StrToDouble(campos->Strings[10]);
      sttCargaNL.status        = sttNOVO;
      // OOPS
      // sttCargaNL.esq;
      // sttCargaNL.comentario;

      cargaNL = (VTCargaNL*)monta->InsereCargaNL(sttCargaNL);
      if (cargaNL)
         {
         // Essa é uma lista auxiliar, que indica o que foi inserido (porém,
         // já foi inserido no VTRedes também)
         lisCNL->Add(cargaNL);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TImporta::LeHarmonico(TStrings *campos)
   {
   //variáveis locais
   VTCargaNL         *cargaNL;
   VTCargaNL         *cargaNLcheck;
   struct strCARGANL sttCargaNL;

   //proteção: verifica se o monta está ok
   if (!monta) return;
   //proteção: verifica se a lista de cargas está ok
   if (!lisCNL) return;
   //proteção: verifica o número de campos
   if (campos->Count < 8) return;
   //determina carga
   // OOPS! se parar de inserir na lista de cargas, vai dar zebra aqui
   if ((cargaNL = ExisteCarganl(StrToInteger(campos->Strings[0]))) == NULL) return;
   //proteção: verifica barra da carga
   if (!cargaNL->pbarra) return;

   try{
      //inicia dados
      // OOPS
      sttCargaNL.obra          = false;
      sttCargaNL.car_id        = StrToInteger(cargaNL->Id);
      sttCargaNL.bar_id        = cargaNL->pbarra->Id;
      sttCargaNL.codigo        = "";    // OOPS: não deve inserir, visto que se trata de um harmônico
      sttCargaNL.extern_id     = "";    // OOPS: não deve inserir, visto que se trata de um harmônico
      sttCargaNL.ordem         = StrToInteger(campos->Strings[1]);
      sttCargaNL.fat_pot       = 0;     // OOPS: não deve inserir, visto que se trata de um harmônico
      sttCargaNL.flag_indutivo = true;  // OOPS: não deve inserir, visto que se trata de um harmônico
      sttCargaNL.flag_kVA      = false;
      sttCargaNL.snomA         = StrToDouble(campos->Strings[2]);
      sttCargaNL.angA          = StrToDouble(campos->Strings[3]);
      sttCargaNL.snomB         = StrToDouble(campos->Strings[4]);
      sttCargaNL.angB          = StrToDouble(campos->Strings[5]);
      sttCargaNL.snomC         = StrToDouble(campos->Strings[6]);
      sttCargaNL.angC          = StrToDouble(campos->Strings[7]);
      // OOPS
      // sttCargaNL.esq;
      // sttCargaNL.comentario;

      // Aqui não deve criar uma carga nova. Se criar, tem erro
      cargaNLcheck = (VTCargaNL*)monta->InsereCargaNL(sttCargaNL);
      if (cargaNL != cargaNLcheck)
         {
         // OOPS, aqui não faz nada. Deveria logar, pelo menos
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
//eof

