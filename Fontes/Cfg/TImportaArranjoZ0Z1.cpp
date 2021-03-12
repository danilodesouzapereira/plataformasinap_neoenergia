//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <Algorithm>
#include "TImportaArranjoZ0Z1.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//TImportaArranjoZ0Z1* NewObjImportaArranjoZ0Z1(VTApl *apl_owner)
//   {
//   return(new TImportaArranjoZ0Z1(apl_owner));
//   }

//---------------------------------------------------------------------------
__fastcall TImportaArranjoZ0Z1::TImportaArranjoZ0Z1(VTApl *apl_owner)
   {
   //salva objeto Apl
   apl = apl_owner;
   //cria objetos
   campos = new TStringList();
   lines  = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TImportaArranjoZ0Z1::~TImportaArranjoZ0Z1(void)
   {
   //destrói objetos
   if (campos) {delete campos; campos = NULL;}
   if (lines ) {delete lines ; lines  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaArranjoZ0Z1::Executa(AnsiString arq_txt)
   {
   //variáveis locais
   AnsiString  codigo;
   AnsiString  texto;
   double      s_mva, q_mvar;
   double      Sbase_mva, Vbase_kv, Zbase_ohm;
   double      freq_60Hz = 60;
   double      w         = 2 * M_PI * freq_60Hz;
   double      c0, c1;   //unidade: mvar
   strIMP      z0, z1;   //unidade: porcentagem / km
   VTArranjo   *arranjo;
   VTArranjos  *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));

   //verifica se o arquivo existe
   if (! FileExists(arq_txt)) return(false);
   //carrega o arquivo no StringList
   lines->LoadFromFile(arq_txt);
   //calcula Zbase
   Sbase_mva = 100.;
   Vbase_kv  = 88.;
   Zbase_ohm = (Vbase_kv * Vbase_kv) / Sbase_mva;

   //loop p/ todas as linhas
   for (int n = 0; n < lines->Count; n++)
      {//extrai Campos da linha
      try{
         ExtraiCampos(lines->Strings[n], campos);
         if (campos->Count < 9) continue;
         //lê valores de impedância: //Nº;CÓDIGO;CAPAC.;R+;X+;Ro;Xo;Qc;Vbase;
         codigo   = campos->Strings[1];
         s_mva    = StrToDouble(campos->Strings[2]);
         z1.r     = StrToDouble(campos->Strings[3]);
         z1.x     = StrToDouble(campos->Strings[4]);
         z0.r     = StrToDouble(campos->Strings[5]);
         z0.x     = StrToDouble(campos->Strings[6]);
         q_mvar   = StrToDouble(campos->Strings[7]);
         Vbase_kv = StrToDouble(campos->Strings[8]);
         //proteção
         if (IsDoubleZero(Vbase_kv)) continue;
         //verifica se já existe um Arranjo c/ mesmo código
         if ((arranjo = arranjos->ExisteArranjo(codigo, arranjoZ0Z1)) == NULL)
            {//cria um novo Arranjo
            if ((arranjo = DLL_NewObjArranjo()) == NULL) continue;
            arranjo->Id              = FalsoId();
            arranjo->Status[sttNOVO] = true;
            //insere Arranjo em Arranjos
            arranjos->InsereArranjo(arranjo);
            }
         arranjo->Codigo          = codigo;
         arranjo->TipoCfg         = arranjoZ0Z1;
         arranjo->Fases           = faseABCN;
         arranjo->Iadm_amp        = (s_mva / (sqrt(3.) * Vbase_kv)) * 1000.;
         arranjo->Vmin_KV         = Vbase_kv;
         arranjo->Vmax_KV         = Vbase_kv;
         //converte impedâncias de porcentagem/km para ohm/km
         arranjo->z0.r            = (z0.r / 100.) * Zbase_ohm;
         arranjo->z0.x            = (z0.x / 100.) * Zbase_ohm;
         arranjo->z1.r            = (z1.r / 100.) * Zbase_ohm;
         arranjo->z1.x            = (z1.x / 100.) * Zbase_ohm;
         //converte capacitância de mvar para nF/km
         arranjo->c0              = (1.0e9 * q_mvar) /(w * Vbase_kv * Vbase_kv);
         arranjo->c1              = arranjo->c0;
         //indica que o Arranjo foi alterado
         arranjo->Status[sttALTERADO] = true;
         }catch(Exception &e)
            {//nada a fazer
            }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TImportaArranjoZ0Z1::ExtraiCampos(AnsiString txt, TStringList *campos)
   {
   //variáveis locais
   TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

   //reinicia StrinList campos
   campos->Clear();
   //verifica linha de comentário
   if ((txt[1] == '/')&&(txt[2] == '/')) return;
   if ((txt[1] == '/')&&(txt[2] == '*')) return;
   //inicia conjunto de caracteres separadores
   Separators << ';';
   //inicia conjunto de caracteres de espaçamento
   //WhiteSpace << ' ';
   //separa campos do txt
   ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
   }

//-----------------------------------------------------------------------------
// eof


