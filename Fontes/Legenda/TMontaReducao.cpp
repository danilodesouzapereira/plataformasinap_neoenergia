//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaReducao.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTLigacao.h"
//#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTReducao.h"
#include "..\Licenca\VTLicenca.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaReducao(VTApl *apl)
   {
   try{
      return (new TMontaReducao(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaReducao::TMontaReducao(VTApl *apl)
                         :TMontaLigacao(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaReducao::~TMontaReducao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaReducao::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int       ind_bar;
   int       ano     = 0;
   //int       num_pat = 0;
   TList     *lisLIG;
   //VTLigacao *ligacao;
   VTGrupo   *grupo1, *grupo2, *grupo3;

   //salva ponteiro p/ Reducao
   reducao = (VTReducao*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   /*
   //cria Grupo2 c/ dados da Barra
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      }
   */
   //cria Grupo2 c/ dados de fluxo
   if ((grupo2 = NewObjGrupo(eqpto, "Fluxo Potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //cria Grupo3 c/ Ligacoes do esquemático da Reducao
      lisLIG = reducao->LisLigacaoEsquematico();
      for (int n = 0; n < lisLIG->Count; n++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[n];
         //determina ind_bar fictícia
         ind_bar = ligacao->IndiceBarra(reducao->BarraFicticia());
         //cria Grupo3
         //if ((grupo3 = NewObjGrupo(eqpto, ligacao->Alias(ind_bar))) == NULL) continue;
         if ((grupo3 = NewObjGrupo(eqpto, ligacao->Codigo)) == NULL) continue;
         //insere Grupo3 no Grupo2
         grupo2->Add(grupo3);
         //inicia Grupo3 c/ dados do fluxo
         LisVetValFluxo(ligacao, grupo3->LisVetval, lig_S,     ind_bar, ano);
         LisVetValFluxo(ligacao, grupo3->LisVetval, lig_S_ABC, ind_bar, ano);
         LisVetValFluxo(ligacao, grupo3->LisVetval, lig_S_MAX, ind_bar, ano);
       //LisVetValFluxo(ligacao, grupo3->LisVetval, lig_S_MIN, ind_bar, ano);
         }
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Reducao
   reducao = (VTReducao*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //variáveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //salva ponteiro p/ Reducao
   reducao = (VTReducao*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //não permite visualização dos Eqptos da Reducao, exceto p/ usuário privilegiado
   if ((licenca != NULL)&&(licenca->Tipo == tipoMASTER))
      {
      lisATR->Add(new TAtributo(reducao, rdu_EQPTOS,  false, false,  "Exibir equipamentos"));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Reducao
   reducao = (VTReducao*)eqpto;
   //insere atributos de fluxo de Eqpto
   TMontaEqpto::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo da Reducao
   lisATR->Add(new TAtributo(reducao, rdu_P,       false, true,   "Pot.Ativa", "(MW)"));
   lisATR->Add(new TAtributo(reducao, rdu_Q,       false, true,   "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(reducao, rdu_S,       false, true,   "Pot.Aparente", "(MVA)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //variáveis locais
   int       ind_bar;
   TList     *lisLIG;

   //salva ponteiro p/ Reducao
   reducao = (VTReducao*)eqpto;
   //converte atributo de Reducao p/ atributo de ligacao
   switch(ind_atrib)
      {
      case rdu_P: ind_atrib = lig_P; break;
      case rdu_Q: ind_atrib = lig_Q; break;
      case rdu_S: ind_atrib = lig_S; break;
      default: return(false);
      }
   //cria Vetval p/ cada Ligacao do esquemático da Reducao
   lisLIG = reducao->LisLigacaoEsquematico();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      //determina ind_bar fictícia
      ind_bar = ligacao->IndiceBarra(reducao->BarraFicticia());
      //cria Vetval e insere em lisEXT
      LisVetValFluxo(ligacao, lisEXT, ind_atrib, ind_bar, ano);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReducao::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ind_bar, int ano)
   {
   return(TMontaLigacao::LisVetValFluxo(eqpto, lisEXT, ind_atrib, ind_bar, ano));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaReducao::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   return(TMontaEqpto::ValorAtributo(eqpto, ind_atrib));
   }

//---------------------------------------------------------------------------
//eof

