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
	//define revisao e data
	PD.revisao      = "";
	PD.revisao_seq  = "6.1.8";
	PD.data         = "16/05/2020";
//   PD.revisao      = "Rev1.0";
//   PD.revisao_seq  = "Rev1.0.1";
//   PD.data         = "20.01.2017";
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
//   return("SINAPrnt");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TProduto::PM_GetCodLicenca(void)
   {
   return("Sinap");
//   return("SinapRNT");
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
   return("Análise Integrada de Redes AT/MT/BT");
//   return("Análise de Reclamação de Nível de Tensão");
   }

//---------------------------------------------------------------------------
int __fastcall TProduto::PM_GetId(void)
   {
   return(produtoSINAP);
//   return(produtoSINAP_RNT);
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
