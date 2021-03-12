//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorBarCC.h"
#include "..\Complexo\Complexo.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorCurto(VTBarra *barra)
   {
   try{//cria objeto TFasorBarCC
      return(new TFasorBarCC(barra));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorBarCC::TFasorBarCC(VTBarra *barra)
   {
   //salva Barra
   this->barra   = barra;
   //insere parâmetros default
   color        = clRed;
   ind_pat      = 0;
   codigo       = "";
   enabled      = false;
   visible      = false;
   dim          = 0;
   raio         = 0;
   draw_seta    = true;
   fases        = faseINV;
   ind_seq      = -1;
   tipo_curto   = -1;
   coord.x      = 0.;
   coord.y      = 0.;
   tensao_kv    = CZero();
   }

//---------------------------------------------------------------------------
__fastcall TFasorBarCC::~TFasorBarCC(void)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::AtualizaAngulo(double ang_novo)
   {
   //implementado somente para TFasorAjuste
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   if      (fases != faseINV)                             IniciaFasorVfas();
   else if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) IniciaFasorVseq();
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::IniciaFasorVfas(void)
   {
   //variáveis locais
   VTBarCC *barCC = (VTBarCC*)(barra->Obj);

   //habilita exibição
   //enabled = true;
   //define código do Fasor
   codigo  = NomeDefeito(tipo_curto) + ": ";
   dim     = barra->zona->Vff_kv;
   //dim     = Abs(barCC->Vfas_kv[cc1FTz][ind_fase]);
   switch(fases)
      {
      case faseAN:
         codigo    = codigo + " Van";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_A];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_A];
         break;
      case faseBN:
         codigo    = codigo + " Vbn";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_B];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_B];
         break;
      case faseCN:
         codigo    = codigo + " Vcn";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_C];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_C];
         break;
      case faseAB:
         codigo    = codigo + " Vab";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_A] - barCC->Vfas_kv[tipo_curto][indFASE_B];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_A] - barCC->Vfas_pu[tipo_curto][indFASE_B];
         break;
      case faseBC:
         codigo    = codigo + " Vbc";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_B] - barCC->Vfas_kv[tipo_curto][indFASE_C];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_B] - barCC->Vfas_pu[tipo_curto][indFASE_C];
         break;
      case faseCA:
         codigo    = codigo + " Vca";
         tensao_kv = barCC->Vfas_kv[tipo_curto][indFASE_C] - barCC->Vfas_kv[tipo_curto][indFASE_A];
         tensao_pu = barCC->Vfas_pu[tipo_curto][indFASE_C] - barCC->Vfas_pu[tipo_curto][indFASE_A];
         break;
      default: //desabilita exibição
         tensao_kv  = CZero();
         tensao_pu  = CZero();
      }
   //verifica se valor nulo
   //if (IsDoubleZero(Abs(tensao_kv, V_KV_NULA))) tensao_kv = CZero();
   //if (IsDoubleZero(Abs(tensao_pu, V_PU_NULA))) tensao_pu = CZero();
   //verifica se valor nulo
   if (IsDoubleZero(Abs(tensao_kv), V_KV_NULA))
      {
      tensao_kv = CZero();
      tensao_pu = CZero();
      }
   //define informações para gráfico
   raio    = Abs(tensao_kv);
   coord.x = tensao_kv.real();
   coord.y = tensao_kv.imag();
   enabled = (raio > 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::IniciaFasorVseq(void)
   {
   //variáveis locais
   VTBarCC *barCC = (VTBarCC*)(barra->Obj);

   //habilita exibição
   //enabled = true;
   //define código do Fasor
   codigo    = NomeDefeito(tipo_curto) + ": ";
   dim       = barra->zona->Vff_kv;
   tensao_kv = barCC->Vseq_kv[tipo_curto][ind_seq];
   tensao_pu = barCC->Vseq_pu[tipo_curto][ind_seq];
   switch(ind_seq)
      {
      case indSEQ_0:  codigo  = codigo + " V0"; break;
      case indSEQ_1:  codigo  = codigo + " V1"; break;
      case indSEQ_2:  codigo  = codigo + " V2"; break;
      default: //desabilita exibição
         tensao_kv = CZero();
         tensao_pu = CZero();
      }
   //verifica se valor nulo
   //if (IsDoubleZero(Abs(tensao_kv, V_KV_NULA))) tensao_kv = CZero();
   //if (IsDoubleZero(Abs(tensao_pu, V_PU_NULA))) tensao_pu = CZero();
   //verifica se valor nulo
   if (IsDoubleZero(Abs(tensao_kv), V_KV_NULA))
      {
      tensao_kv = CZero();
      tensao_pu = CZero();
      }
   //define informações para gráfico
   raio      = Abs(tensao_kv);
   coord.x   = tensao_kv.real();
   coord.y   = tensao_kv.imag();
   enabled   = (raio > 0.);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TFasorBarCC::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F", "2F", "1FT", "1FTz", "2FT"};

   return(AnsiString("Curto ") + tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::RedefineBarraReferencia(VTBarra *barra)
   {
   //salva barra
   this->barra = barra;
   //reinicia o fasor
   DefinePatamar(ind_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TFasorBarCC::Tipo(void)
   {
   if (fases   != faseINV)                           return(fasorCURTO_Vfas);
   if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) return(fasorCURTO_Vseq);
   return(fasorINDEFINIDO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorBarCC::Valor(complex<double> &valor, bool pu)
   {
   if (pu) {valor = tensao_pu;}
   else    {valor = tensao_kv;}

   return(true);
   }

//---------------------------------------------------------------------------
//eof
