//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixPrEDPSP.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTNixPr* __fastcall NewObjNixPrEDPSP(VTChave *chave, int index, bool pr_com_corrente)
   {
   return(new TNixPrEDPSP(chave, index, pr_com_corrente));
   }

//---------------------------------------------------------------------------
__fastcall TNixPrEDPSP::TNixPrEDPSP(VTChave *chave, int index, bool pr_com_corrente)
: TNixPr()
   {
   //variáveis locais
   complex<double> i_max_amp;

   //determina corrente máxima na chave
   if (pr_com_corrente) i_max_amp = CorrenteMaxima(chave);
   else                 i_max_amp = CZero();
   //inicia dados
   strPR.pr        = "PR";
   strPR.tipo      = 0;
   strPR.codigo    = TraduzCodigo(chave, index);
   strPR.no_inicio = chave->pbarra1->Id;
   strPR.no_fim    = chave->pbarra2->Id;
   strPR.i_mod     = Abs(i_max_amp);
   strPR.i_ang     = Arg(i_max_amp);
   strPR.cons      = 0;
   //strPR.elo       = (chave->Aberta) ? "NA" : "";
   if(chave->Aberta)
	  {
      strPR.elo ="NA";
      }
   else
	  {//se a chave for base fusivel...
	  if(chave->TipoChave->Tipo == chaveBF)
		 {//salva o elo de acordo com a existencia do ExternId
		strPR.elo   = chave->EloFusivel;
		//depende do tipo de elo
		if(strPR.elo.Pos("K") > 0)
			{
			strPR.tipo  = 0;
			}
		else if(strPR.elo.Pos("H") > 0)
			{
			strPR.tipo  = 0;
			}
		 }
	  else
         {
		 strPR.elo = "#";
		 }
	  }
   }

//---------------------------------------------------------------------------
__fastcall TNixPrEDPSP::~TNixPrEDPSP(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18622645
AnsiString __fastcall TNixPrEDPSP::TraduzCodigo(VTChave *chave, int index)
   {
   //variáveis locais
   AnsiString codigo;
   int   tipoSuposto;

   //inicia código c/ a parte referente ao index
   //codigo = IntToStr(index);
   //para facilitar a localização da chave e a unicidade do codigo
   //codigo = chave->Codigo + "(id:" + IntToStr(chave->Id) +")";
   codigo = chave->Codigo.Trim();
   codigo = StringReplace(codigo, " ","", TReplaceFlags() << rfReplaceAll);
   //codigo = codigo.SubString(
   switch(chave->TipoChave->Tipo)
      {
	  case chaveDJ:
		if(codigo.Pos("DJ") < 0)
		{//uma vez que o código do disjuntor criado pelo importador já é único
        //(depende do nome do alimentador), não é mais necessário vincular ao id
			codigo = "DJ" + codigo;
		}
		strPR.tipo  = 0;
		break;
	  case chaveRA:
		codigo = "RE_" + codigo;
		strPR.tipo  = 0;
		break;
	  case chaveBF:
		codigo = "BF_" + codigo;
		strPR.tipo  = 0;
		break;
	  case chaveSEC:
		codigo = "CS_" + codigo;
		strPR.tipo  = 0;
		break;
	  case chaveFACA:
		codigo = "CF_" + codigo;
		strPR.tipo  = 0;
		break;
      //default:        codigo = "BF" + codigo; break;
      default:
         tipoSuposto = SupoeTipo(chave);
            switch(tipoSuposto)
            {
			case chaveDJ:
				codigo = "DJ" + codigo;
				strPR.tipo  = 0;
				break;
			case chaveRA:
				codigo = "RE_" + codigo;
				strPR.tipo  = 0;
				break;
			case chaveBF:
				codigo = "BF_" + codigo;
				strPR.tipo  = 0;
				break;
			case chaveSEC:
				codigo = "CS_" + codigo;
				strPR.tipo  = 0;
				break;
			default:
				codigo = "CF_" + codigo;
				strPR.tipo  = 0;
				break;
            }
	  }

   //reduz o tamanho do codigo caso tenha ficado grande demais
   if(codigo.Length() > 20)
   {
      codigo = codigo.SetLength(20);
   }
   return(codigo);
   }
//---------------------------------------------------------------------------
