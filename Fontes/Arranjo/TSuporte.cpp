//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSuporte.h"
#include "VTPtoFixacao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//funções com escopo de arquivo
static int __fastcall OrdenaPtoFixacao(void *pf1, void *pf2)
   {
   //variáveis locais
   int          compare;
   VTPtoFixacao *pto_fixacao1 = (VTPtoFixacao*)pf1;
   VTPtoFixacao *pto_fixacao2 = (VTPtoFixacao*)pf2;

   if      (pto_fixacao1->id < pto_fixacao2->id) compare = -1;
   else if (pto_fixacao1->id > pto_fixacao2->id) compare =  1;
   else                                          compare =  0;
                                          
   return(compare);
   }

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TSuporte
//---------------------------------------------------------------------------
VTSuporte* __fastcall NewObjSuporte(void)
   {
   return(new TSuporte());
   }

//---------------------------------------------------------------------------
__fastcall TSuporte::TSuporte(void)
   {
   try{//cria objetos
      lisPTO = new TList();
      //inicia dados
      Id             = -1;
      Codigo         = "";
      TipoDeSuporte  = suporteAEREO;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TSuporte::~TSuporte(void)
   {
   //destrói lista e seus objetos
   if (lisPTO) {LimpaTList(lisPTO); delete lisPTO; lisPTO = NULL;}
   }

//---------------------------------------------------------------------------
VTSuporte* __fastcall TSuporte::Clone(void)
   {
   //variáveis locais
   VTSuporte *suporte;

   //cria um novo Suporte
   if ((suporte = NewObjSuporte()) != NULL)
      {//copia dados deste Suporte p/ o novo Suporte
		suporte->CopiaAtributosDe(*this);
      }
   return(suporte);
   }

//---------------------------------------------------------------------------
void __fastcall TSuporte::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	VTPtoFixacao *pto_fixacao, *pto_fixacao_ext;
	TSuporte     &suporte    = (TSuporte&)ref;
	TList        *lisPTO_EXT = suporte.LisPtoFixacao();

	//proteção
	if (ref.Tipo() != eqptoSUPORTE) return;
	//copia atributos do Eqpto
	VTEqpto::CopiaAtributosDe(suporte);
	//copia atributos do Suporte
	PD = suporte.PD;
	//se necessário, cria novos PtoFixacao
	while (lisPTO->Count < lisPTO_EXT->Count)
		{//cria novos PtoFixacao
		lisPTO->Add(NewObjPtoFixacao());
		}
	//se necessário, destrói PtoFixacao excedente
	while (lisPTO->Count > lisPTO_EXT->Count)
		{//destrói PtoFixacao excedente
		pto_fixacao = (VTPtoFixacao*)lisPTO->Last();
		lisPTO->Delete(lisPTO->Count - 1);
		delete pto_fixacao;
		}
	//copia conjunto de PtoFixacao
	for (int n = 0; (n < lisPTO->Count)&&(n < lisPTO_EXT->Count); n++)
		{
		pto_fixacao     = (VTPtoFixacao*)lisPTO->Items[n];
		pto_fixacao_ext = (VTPtoFixacao*)lisPTO_EXT->Items[n];
		pto_fixacao->CopiaAtributosDe(*pto_fixacao_ext);
		}
	}

//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall TSuporte::ExistePtoFixacao(AnsiString codigo)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;

   //loop p/ todos Arranjos em lisARR
   for (int n = 0; n < lisPTO->Count; n++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPTO->Items[n];
      if (pto_fixacao->codigo.AnsiCompareIC(codigo) == 0)return(pto_fixacao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall TSuporte::ExistePtoFixacao(int id)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;

   //loop p/ todos Arranjos em lisARR
   for (int n = 0; n < lisPTO->Count; n++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPTO->Items[n];
      if (pto_fixacao->id == id) return(pto_fixacao);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall TSuporte::ExistePtoFixacaoFase(int ind_fase)
   {
   //proteção
   if ((ind_fase < 0)||(ind_fase >= lisPTO->Count)) return(NULL);
   //retorno PtoFixacao do índice da fase
   return((VTPtoFixacao*)lisPTO->Items[ind_fase]);
   }
*/
//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall TSuporte::ExistePtoFixacaoIndex(int index)
   {
   //proteção
   if ((index < 0)||(index >= lisPTO->Count)) return(NULL);
   //retorno PtoFixacao do índice da fase
   return((VTPtoFixacao*)lisPTO->Items[index]);
   }

//---------------------------------------------------------------------------
bool __fastcall TSuporte::InserePtoFixacao(VTPtoFixacao *pto_fixacao)
   {
   if (lisPTO->IndexOf(pto_fixacao) < 0)
      {
      lisPTO->Add(pto_fixacao);
      }
   //ordena PtoFixacao pelo Id
   if (lisPTO->Count > 1) lisPTO->Sort(OrdenaPtoFixacao);
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSuporte::LisPtoFixacao(void)
   {
   return(lisPTO);
   }

//---------------------------------------------------------------------------
int __fastcall TSuporte::PM_GetTipoDeSuporte(void)
   {
   return(PD.tiposuporte);
   }

//---------------------------------------------------------------------------
double __fastcall TSuporte::PM_GetVmax_KV(void)
   {
   return(PD.vmax);
   }

//---------------------------------------------------------------------------
double __fastcall TSuporte::PM_GetVmin_KV(void)
   {
   return(PD.vmin);
   }

//---------------------------------------------------------------------------
void __fastcall TSuporte::PM_SetCodigo(AnsiString codigo)
   {
   //proteção contra dimensão do string
   if (codigo.Length() > 128) codigo = codigo.SubString(1, 128);
   VTEqpto::PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TSuporte::PM_SetTipoDeSuporte(int tipo)
   {
   PD.tiposuporte = tipo;
   }

//---------------------------------------------------------------------------
void __fastcall TSuporte::PM_SetVmax_KV(double vmax)
   {
   PD.vmax = vmax;
   }

//---------------------------------------------------------------------------
void __fastcall TSuporte::PM_SetVmin_KV(double vmin)
   {
   PD.vmin = vmin;
   }

//---------------------------------------------------------------------------
int __fastcall TSuporte::Tipo(void)
   {
   return(eqptoSUPORTE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TSuporte::TipoAsString(void)
   {
   return("Estrutura de Suporte");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TSuporte::TipoDeSuporteAsString(void)
   {
   switch(TipoDeSuporte)
      {
      case suporteAEREO:       return("aéreo");
      case suporteSUBTERRANEO: return("subterrâneo");
      case suporteSUBMERSO:    return("submerso");
      case suporteINDEFINIDO:  return("indefinido");
      default:                 return("indefinido");
      }
   }

//---------------------------------------------------------------------------
//eof
