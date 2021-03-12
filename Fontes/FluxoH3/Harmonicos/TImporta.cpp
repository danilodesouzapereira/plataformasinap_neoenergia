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
   // OOPS. Estou passando NULL no apl, para que o monta seja destru�do aqui
   monta  = NewObjMontaRede(apl);
   }

//---------------------------------------------------------------------------
__fastcall TImporta::~TImporta(void)
   {
   //destr�i objetos
    if (lines)  {delete lines;  lines  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TImporta::Executa(AnsiString arq_cnl, TList *lisEXT)
   {
   //vari�veis locais
   VTCargaNL  *cnl;

   //salva  ponteiro p/ lista externa de CargaNL
   lisCNL = lisEXT;
   //verifica se existe arquivo de Cargas n�o linerares
   if (! FileExists(arq_cnl)) return(lisCNL);
   //l� arquivo de cargas n�o lineares
   if (! LeArquivoCarga(arq_cnl)) {LimpaTList(lisCNL);}
   // OOPS! Essas cargas deveriam ser eliminadas da base de dados antes de escrever de novo?
   //ajusta os identificadores das cargas, que ainda n�o existem na base de dados
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
   //vari�veis locais
   VTBarra *barra;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();

   //procura Barra pelo c�digo
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
   //vari�veis locais
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
   //vari�veis locais
   TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

   //reinicia StrinList campos
   campos->Clear();
   //prote��o
   if (txt.Length() < 2) return;
   //verifica se linha de coment�rio
   if ((txt[1] == '\/')&&(txt[2] == '\/')) return;
   //inicia conjunto de caracteres separadores
   Separators << ';';
   //inicia conjunto de caracteres de espa�amento
   WhiteSpace << ' ';
   //separa campos do txt
   ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TImporta::LeArquivoCarga(AnsiString arq_cnl)
   {
   //vari�veis locais
   int           estado = 0;
   TExtraiCampos ExtraiCampos;
   TStrings      *campos;

   //configura extra��o
   ExtraiCampos.SeparadorInsere(';');
	ExtraiCampos.SetOpcaoFiltraComentario(true);
	ExtraiCampos.SetOpcaoTrimCampos(true);
   //l� o arquivo
   lines->LoadFromFile(arq_cnl);
   //loop p/ todas linhas do arquivo selecionado
   for (int nl = 0; nl < lines->Count; nl++)
      {//extrai campos da linha
      if ((campos = ExtraiCampos.Executa(lines->Strings[nl])) == NULL) continue;
      //prote��o: linha vazia ou de coment�rio
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
   //vari�veis locais
   VTBarra           *barra;
   VTCargaNL         *cargaNL;
   struct strCARGANL sttCargaNL;
   AnsiString        strNatureza;
   bool              bolIndutivo;
   VTRedes*          redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //prote��o: verifica se o redes est� ok
   if (!redes) return;
   //prote��o: verifica se o monta est� ok
   if (!monta) return;
   //prote��o: verifica se a lista de cargas est� ok
   if (!lisCNL) return;
   //prote��o: verifica o n�mero de campos
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
         // Essa � uma lista auxiliar, que indica o que foi inserido (por�m,
         // j� foi inserido no VTRedes tamb�m)
         lisCNL->Add(cargaNL);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TImporta::LeHarmonico(TStrings *campos)
   {
   //vari�veis locais
   VTCargaNL         *cargaNL;
   VTCargaNL         *cargaNLcheck;
   struct strCARGANL sttCargaNL;

   //prote��o: verifica se o monta est� ok
   if (!monta) return;
   //prote��o: verifica se a lista de cargas est� ok
   if (!lisCNL) return;
   //prote��o: verifica o n�mero de campos
   if (campos->Count < 8) return;
   //determina carga
   // OOPS! se parar de inserir na lista de cargas, vai dar zebra aqui
   if ((cargaNL = ExisteCarganl(StrToInteger(campos->Strings[0]))) == NULL) return;
   //prote��o: verifica barra da carga
   if (!cargaNL->pbarra) return;

   try{
      //inicia dados
      // OOPS
      sttCargaNL.obra          = false;
      sttCargaNL.car_id        = StrToInteger(cargaNL->Id);
      sttCargaNL.bar_id        = cargaNL->pbarra->Id;
      sttCargaNL.codigo        = "";    // OOPS: n�o deve inserir, visto que se trata de um harm�nico
      sttCargaNL.extern_id     = "";    // OOPS: n�o deve inserir, visto que se trata de um harm�nico
      sttCargaNL.ordem         = StrToInteger(campos->Strings[1]);
      sttCargaNL.fat_pot       = 0;     // OOPS: n�o deve inserir, visto que se trata de um harm�nico
      sttCargaNL.flag_indutivo = true;  // OOPS: n�o deve inserir, visto que se trata de um harm�nico
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

      // Aqui n�o deve criar uma carga nova. Se criar, tem erro
      cargaNLcheck = (VTCargaNL*)monta->InsereCargaNL(sttCargaNL);
      if (cargaNL != cargaNLcheck)
         {
         // OOPS, aqui n�o faz nada. Deveria logar, pelo menos
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
//eof

