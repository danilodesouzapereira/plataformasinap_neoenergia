//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TProduto.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTProduto* __fastcall NewObjProduto(void)
   {
   try{
      return (new TProduto());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TProduto::TProduto(void)
   {
   //define revsisao e dadta
   PD.revisao      = "Rev4.3";
   PD.revisao_seq  = "Rev4.3.6";
   PD.data         = "16.01.2017";
   }

//---------------------------------------------------------------------------
__fastcall TProduto::~TProduto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodigo(void)
   {
   return("SINAPgrid");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodLicenca(void)
   {
   return("Sinap");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodRev(void)
   {
   return(Codigo + Revisao);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodRevSeq(void)
   {
   return(Codigo + " " + RevisaoSeq);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodRevSeqData(void)
   {
   return(CodRevSeq + " - " + Data);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetData(void)
   {
   return(PD.data);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetDescricao(void)
   {
   return(" SINAPgrid: Integrated Analysis of HV/MV/LV Networks");
   //return("Análise de Reclamação de Nível de Tensão");
   }

//---------------------------------------------------------------------------
int __fastcall TProduto::PM_GetId(void)
   {
   return(produtoSINAP);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetRevisao(void)
   {
   return(PD.revisao);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetRevisaoSeq(void)
   {
   return(PD.revisao_seq);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetSinapsis(void)
   {
   return("Sinapsis");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetWinRegister(void)
   {
   return(CodRev);
   }

//---------------------------------------------------------------------------
//eof
