///---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaMedidor.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTGerador.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaMedidor(VTApl *apl)
   {
   try{
      return (new TMontaMedidor(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedidor::TMontaMedidor(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedidor::~TMontaMedidor(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaMedidor::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTCanal *canal;
   VTGrupo *grupo1, *grupo2;
   TList   *lisCANAL;

   //verifica se Eqpto é um Medidor ou um Canal
   if (eqpto->Tipo() == eqptoCANAL)
      {
      canal = (VTCanal*)eqpto;
      //cria Grupo p/ o Canal
      if ((grupo1 = IniciaGrupoCanal(canal)) == NULL)  return(NULL);
      //inicia Grupo1 c/ dados do Canal
      LisValoresCanal(canal, grupo1->LisDado, ano, num_pat);
      //valores da Curva do Canal
      LisVetValCanal(canal, grupo1->LisVetval);
      }
   else
      {//salva ponteiro p/ Medidor
      medidor = (VTMedidor*)eqpto;
      //cria Grupo básico
      grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
      if (grupo1 == NULL) return(NULL);
      //cria Grupo2 c/ dados do Medidor
      if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
         {//insere Grupo2 no Grupo1
         grupo1->Add(grupo2);
         //inicia Grupo2 c/ dados do Eqpto
         LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
         }
      //cria conjunto de Grupos, um para cada Canal
      lisCANAL = medidor->LisCanal();
      for (int nc = 0; nc < lisCANAL->Count; nc++)
         {
         canal = (VTCanal*)lisCANAL->Items[nc];
         //cria Grupo p/ o Canal
         if ((grupo2 = IniciaGrupoCanal(canal)) == NULL) continue;
         //insere Grupo2 no Grupo1
         grupo1->Add(grupo2);
         //inicia Grupo2 c/ dados do Canal
         LisValoresCanal(canal, grupo2->LisDado, ano, num_pat);
         //valores da Curva do Canal
         LisVetValCanal(canal, grupo2->LisVetval);
         }
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaMedidor::IniciaGrupoCanal(VTCanal *canal)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria Grupo
   if ((grupo = NewObjGrupo(canal, "Canal " + canal->Codigo)) == NULL) return(NULL);
   //insere dados do Canal no Grupo
   return(grupo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Medidor
   this->medidor = (VTMedidor*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Medidor
   this->medidor = (VTMedidor*)eqpto;
   //atributos de Eqbar
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos do Medidor
   lisATR->Add(new TAtributo(medidor, med_CANAL, true,  false, "Canais"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Medidor
   this->medidor = (VTMedidor*)eqpto;
   //atributos de fluxo de Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Capacitor

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisValoresCanal(VTCanal *canal, TList *lisEXT, int ano, int num_pat)
   {
   try{//dados do Eqpto
      TMontaEqpto::LisValoresEqpto(canal, lisEXT, ano, num_pat);
      //dados do Canal
      ValorEqptoCanal(canal);
      ValorTipoCanal(canal);
      ValorFasesCanal(canal);
      ValorEnabledCanal(canal);
      ValorToleranciaCanal(canal);
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->medidor     = (VTMedidor*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Medidor
      if (medidor->Show[med_CANAL]) ValorNumeroCanal();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorEnabledCanal(VTCanal *canal)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Habilitado"));
   //define valor
   dado->valor = (canal->Enabled) ? "Sim" : "Não";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorEqptoCanal(VTCanal *canal)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(canal->Eqpto->TipoAsString()));
   //define valor
   dado->valor = canal->Eqpto->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorFasesCanal(VTCanal *canal)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Fases"));
   //define valor
   dado->valor = fases->AsStrUpper[canal->Fases];
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorNumeroCanal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Canais"));
   //define valor
   dado->valor = IntToStr((medidor->LisCanal())->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorToleranciaCanal(VTCanal *canal)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tolerância(%)"));
   //define valor
   dado->valor = DoubleToStr("%3.2f", canal->Tolerancia_perc);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::ValorTipoCanal(VTCanal *canal)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo canal"));
   //define valor
   dado->valor = canal->TipoCanalAsString;
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedidor::LisVetValCanal(VTCanal *canal, TList *lisEXT)
   {
   //salva parâmetros
   this->lisEXT    = lisEXT;
   this->ano       = ano;
	//verifica atributo selecionado
   switch (canal->TipoCanal)
      {
      case canalV:
         VetValorTensaoModulo(canal, true);
         VetValorTensaoModulo(canal, false);
         break;
      //case canalVF:
      //   VetValorTensaoModulo(canal);
      //   VetValorTensaoFase(canal);
      //   break;
      case canalP:
         VetValorPotAtiva(canal, true);
         VetValorPotAtiva(canal, false);
         break;
      case canalQ:
         VetValorPotReativa(canal, true);
         VetValorPotReativa(canal, false);
         break;
      case canalS:
         VetValorPotAparente(canal, true);
         VetValorPotAparente(canal, false);
         break;
		case canalI:
			VetValorCorrenteModulo(canal, true);
			VetValorCorrenteModulo(canal, false);
			break;
		//case canalIF:
		//	VetValorCorrenteModulo(canal);
		//	VetValorCorrenteFase(canal);
		//	break;
		default: return (false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaMedidor::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   return(TMontaEqpto::ValorAtributo(eqpto, ind_atrib));
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::VetValorCorrenteModulo(VTCanal *canal, bool medido)
	{
	//variáveis locais
	AnsiString  val_str;
	int         max_pat;
   int         ind_atrib = -1;
	double      valor;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
	max_pat = patamares->NumPatamar();
	//cria um novo Vetval e insere em lisEXT
   if (medido) lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Medido: I", max_pat));
   else        lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Estimado: I", max_pat));
	//preenche valores
	for (int np = 0; np < max_pat; np++)
		{
      if (medido) valor = canal->ValorMedido[patamares->Patamar[np]];
      else        valor = canal->ValorEstimado[patamares->Patamar[np]];
      //define valores em vetval
      vetval->Valor[np]      = aue.Corrente_A(valor, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::VetValorPotAparente(VTCanal *canal, bool medido)
   {
   //variáveis locais
   AnsiString  val_str;
   int         ind_atrib = -1;
   int         max_pat;
   double      valor;
   VTVetval    *vetval;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   if (medido) lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Medido: S",   max_pat));
   else        lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Estimado: S", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      if (medido) valor = canal->ValorMedido[patamares->Patamar[np]];
      else        valor = canal->ValorEstimado[patamares->Patamar[np]];
      //define valores em vetval
      vetval->Valor[np]      = aue.PotAparente_MVA(valor, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::VetValorPotAtiva(VTCanal *canal, bool medido)
   {
   //variáveis locais
   AnsiString  val_str;
   int         ind_atrib = -1;
   int         max_pat;
   double      valor;
   VTVetval    *vetval;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   if (medido) lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Medido: P",   max_pat));
   else        lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Estimado: P", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      if (medido) valor = canal->ValorMedido[patamares->Patamar[np]];
      else        valor = canal->ValorEstimado[patamares->Patamar[np]];
      //define valores em vetval
      vetval->Valor[np]      = aue.PotAtiva_MW(valor, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::VetValorPotReativa(VTCanal *canal, bool medido)
   {
   //variáveis locais
   AnsiString  val_str;
   int         ind_atrib = -1;
   int         max_pat;
   double      valor;
   VTVetval    *vetval;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   num_pat = canal->Curva->NumeroPontos();
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   if (medido) lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Medido: Q",   max_pat));
   else        lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Estimado: Q", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      if (medido) valor = canal->ValorMedido[patamares->Patamar[np]];
      else        valor = canal->ValorEstimado[patamares->Patamar[np]];
      //define valores em vetval
      vetval->Valor[np]      = aue.PotReativa_MVAR(valor, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedidor::VetValorTensaoModulo(VTCanal *canal, bool medido)
	{
	//variáveis locais
	AnsiString  val_str;
	int         ind_atrib = -1;
	int         max_pat;
	double      valor;
   VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
	max_pat = patamares->NumPatamar();
	//cria um novo Vetval e insere em lisEXT
	if (medido) lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Medido: V",   max_pat));
	else        lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Estimado: V", max_pat));
	//preenche valores
	for (int np = 0; np < max_pat; np++)
      {
      if (medido) valor = canal->ValorMedido[patamares->Patamar[np]];
      else        valor = canal->ValorEstimado[patamares->Patamar[np]];
      //define valores em vetval
		vetval->Valor[np]      = aue.Tensao_PU(valor, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
		}
   }

//---------------------------------------------------------------------------
//eof

