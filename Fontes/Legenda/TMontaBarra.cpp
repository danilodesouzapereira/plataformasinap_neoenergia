//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaBarra.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaBarra(VTApl *apl)
   {
   try{
      return (new TMontaBarra(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaBarra::TMontaBarra(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   }

//---------------------------------------------------------------------------
__fastcall TMontaBarra::~TMontaBarra(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaBarra::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ Barra
   pbarra = (VTBarra*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados da Barra
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      }
   //cria Grupo2 c/ dados de fluxo
   if ((grupo2 = NewObjGrupo(eqpto, "Fluxo Potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do fluxo
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_DESEQ, ano);
      //tensões fase/neutro
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_PU,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_PU_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_PU_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_KV,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_KV_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFN_KV_MAX, ano);
      //tensões fase/terra
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_PU,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_PU_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_PU_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_KV,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_KV_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFT_KV_MAX, ano);
      //tensões de linha
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_PU,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_PU_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_PU_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_KV,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_KV_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, bar_VFF_KV_MAX, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Barra
   pbarra = (VTBarra*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Barra
   pbarra = (VTBarra*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //insere atributos da Barra
   lisATR->Add(new TAtributo(pbarra, bar_UTM,        true,  false, "Coordenadas UTM", "(cm)"));
   lisATR->Add(new TAtributo(pbarra, bar_VNOM,       true,  false, "Tensão nominal",  "(kV)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Barra
   pbarra = (VTBarra*)eqpto;
   //insere atributos de fluxo de Eqpto
   TMontaEqpto::LisAtributosFluxo(eqpto, lisATR);
   //desequilíbrio
   lisATR->Add(new TAtributo(pbarra, bar_DESEQ,      false, true, "Desequilíbrio", "(%)"));
   //tensões fase/neutro em pu
   lisATR->Add(new TAtributo(pbarra, bar_VFN_PU,     false, true, "Tensão fase/neutro", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFN_PU_MAX, false, true, "Tensão fase/neutro máxima", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFN_PU_MIN, false, true, "Tensão fase/neutro mínima", "(pu)"));
   //tensões fase/neutro em kV
   lisATR->Add(new TAtributo(pbarra, bar_VFN_KV,     false, true, "Tensão fase/neutro", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFN_KV_MAX, false, true, "Tensão fase/neutro máxima", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFN_KV_MIN, false, true, "Tensão fase/neutro mínima", "(kV)"));
   //tensões de linha em pu
   lisATR->Add(new TAtributo(pbarra, bar_VFF_PU,     false, true, "Tensão linha", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFF_PU_MAX, false, true, "Tensão linha máxima", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFF_PU_MIN, false, true, "Tensão linha mínima", "(pu)"));
   //tensões de linha em kV
   lisATR->Add(new TAtributo(pbarra, bar_VFF_KV,     false, true, "Tensão linha", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFF_KV_MAX, false, true, "Tensão linha máxima", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFF_KV_MIN, false, true, "Tensão linha mínima", "(kV)"));
   //tensões fase/terra em pu
   lisATR->Add(new TAtributo(pbarra, bar_VFT_PU,     false, true, "Tensão fase/terra", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFT_PU_MAX, false, true, "Tensão fase/terra máxima", "(pu)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFT_PU_MIN, false, true, "Tensão fase/terra mínima", "(pu)"));
   //tensões fase/terra em kV
   lisATR->Add(new TAtributo(pbarra, bar_VFT_KV,     false, true, "Tensão fase/terra", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFT_KV_MAX, false, true, "Tensão fase/terra máxima", "(kV)"));
   lisATR->Add(new TAtributo(pbarra, bar_VFT_KV_MIN, false, true, "Tensão fase/terra mínima", "(kV)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->pbarra  = (VTBarra*)eqpto;
   try{//insere dados do Eqpto
      TMontaEqpto::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //tensão nominal e coordenadas UTM
      if (pbarra->Show[bar_UTM])        ValorCoordenada_utm();
      if (pbarra->Show[bar_VNOM])       ValorTensaoNominal();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->pbarra  = (VTBarra*)eqpto;
   this->lisEXT = lisEXT;
   this->ano     = ano;
   this->num_pat = num_pat;
   try{//insere dados do Eqpto
      TMontaEqpto::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
      //dados de fluxo
      if (pbarra->Show[bar_DESEQ])      ValorDesequilibrio();
      //tensões fase/neutro em pu
      if (pbarra->Show[bar_VFN_PU])     ValorTensaoFaseNeutro_pu();
      if (pbarra->Show[bar_VFN_PU_MAX]) ValorTensaoFaseNeutro_pu_max();
      if (pbarra->Show[bar_VFN_PU_MIN]) ValorTensaoFaseNeutro_pu_min();
      //tensões fase/neutro em kV
      if (pbarra->Show[bar_VFN_KV])     ValorTensaoFaseNeutro_kv();
      if (pbarra->Show[bar_VFN_KV_MAX]) ValorTensaoFaseNeutro_kv_max();
      if (pbarra->Show[bar_VFN_KV_MIN]) ValorTensaoFaseNeutro_kv_min();
      //tensões de linha em pu
      if (pbarra->Show[bar_VFF_PU])     ValorTensaoLinha_pu();
      if (pbarra->Show[bar_VFF_PU_MAX]) ValorTensaoLinha_pu_max();
      if (pbarra->Show[bar_VFF_PU_MIN]) ValorTensaoLinha_pu_min();
      //tensões de linha em kV
      if (pbarra->Show[bar_VFF_KV])     ValorTensaoLinha_kv();
      if (pbarra->Show[bar_VFF_KV_MAX]) ValorTensaoLinha_kv_max();
      if (pbarra->Show[bar_VFF_KV_MIN]) ValorTensaoLinha_kv_min();
      //tensões fase/terra em pu
      if (pbarra->Show[bar_VFT_PU])     ValorTensaoFaseTerra_pu();
      if (pbarra->Show[bar_VFT_PU_MAX]) ValorTensaoFaseTerra_pu_max();
      if (pbarra->Show[bar_VFT_PU_MIN]) ValorTensaoFaseTerra_pu_min();
      //tensões fase/terra em kV
      if (pbarra->Show[bar_VFT_KV])     ValorTensaoFaseTerra_kv();
      if (pbarra->Show[bar_VFT_KV_MAX]) ValorTensaoFaseTerra_kv_max();
      if (pbarra->Show[bar_VFT_KV_MIN]) ValorTensaoFaseTerra_kv_min();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBarra::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->pbarra  = (VTBarra*)eqpto;
   //insere dados do Eqpto
   TMontaEqpto::LisVetValFluxo(eqpto, lisEXT, ano, num_pat);
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case bar_DESEQ:      VetValorDesequilibrio(ind_atrib);           break;
      case bar_VFN_PU:     VetValorTensaoFaseNeutro_pu(ind_atrib);     break;
      case bar_VFN_PU_MAX: VetValorTensaoFaseNeutro_pu_max(ind_atrib); break;
      case bar_VFN_PU_MIN: VetValorTensaoFaseNeutro_pu_min(ind_atrib); break;
      case bar_VFN_KV:     VetValorTensaoFaseNeutro_kv(ind_atrib);     break;
      case bar_VFN_KV_MAX: VetValorTensaoFaseNeutro_kv_max(ind_atrib); break;
      case bar_VFN_KV_MIN: VetValorTensaoFaseNeutro_kv_min(ind_atrib); break;
      case bar_VFF_PU:     VetValorTensaoLinha_pu(ind_atrib);          break;
      case bar_VFF_PU_MAX: VetValorTensaoLinha_pu_max(ind_atrib);      break;
      case bar_VFF_PU_MIN: VetValorTensaoLinha_pu_min(ind_atrib);      break;
      case bar_VFF_KV:     VetValorTensaoLinha_kv(ind_atrib);          break;
      case bar_VFF_KV_MAX: VetValorTensaoLinha_kv_max(ind_atrib);      break;
      case bar_VFF_KV_MIN: VetValorTensaoLinha_kv_min(ind_atrib);      break;
      case bar_VFT_PU:     VetValorTensaoFaseTerra_pu(ind_atrib);      break;
      case bar_VFT_PU_MAX: VetValorTensaoFaseTerra_pu_max(ind_atrib);  break;
      case bar_VFT_PU_MIN: VetValorTensaoFaseTerra_pu_min(ind_atrib);  break;
      case bar_VFT_KV:     VetValorTensaoFaseTerra_kv(ind_atrib);      break;
      case bar_VFT_KV_MAX: VetValorTensaoFaseTerra_kv_max(ind_atrib);  break;
      case bar_VFT_KV_MIN: VetValorTensaoFaseTerra_kv_min(ind_atrib);  break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaBarra::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   VTBarra    *barra = (VTBarra*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);               break;
      case eqp_CODIGO: valor = eqpto->Codigo;                     break;
      case bar_VNOM:   valor = DoubleToStr("%5.4f", barra->vnom); break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorCoordenada_utm(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Coord.x,y"));
   //define unidade e valor
   aue.CoordenadaUtm(pbarra->utm.x, pbarra->utm.y, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorDesequilibrio(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Deseq."));
   //define unidade e valor
   //aue.Desequilibrio_PERC(pbarra->resflow->Desequilibrio[num_pat], dado->valor, dado->unidade);
   aue.ValorPorcentual(pbarra->resflow->Desequilibrio[num_pat], dado->valor, dado->unidade);
   }
   
//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_kv(void)
   {
   //variáveis locais
   complex<double> vkv;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Van"}, {faseB, "Vbn"}, {faseC, "Vcn"}};

   //tensões fase/neutro em kV
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if (pbarra->resflow->Fases & fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/neutro em kV
         vkv = pbarra->resflow->Vfn_kv[num_pat][nf];
         //define unidade e valor
         aue.Tensao_KV(vkv, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_kv_max(void)
   {
   //variáveis locais
   double vkv_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vfn_max"));
      //determina tensão fase/neutro máxima em kV
      vkv_max = Abs(pbarra->resflow->Vfn_kv_max[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_kv_min(void)
   {
   //variáveis locais
   double vkv_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vfn_min"));
      //determina tensão fase/neutro mínima em kV
      vkv_min = Abs(pbarra->resflow->Vfn_kv_min[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_pu(void)
   {
   //variáveis locais
   complex<double> vpu;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Van"}, {faseB, "Vbn"}, {faseC, "Vcn"}};

   //tensões fase/neutro em pu
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if (pbarra->resflow->Fases & fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/neutro em pu
         vpu = pbarra->resflow->Vfn_pu[num_pat][nf];
         //define unidade e valor
         aue.Tensao_PU(vpu, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_pu_max(void)
   {
   //variáveis locais
   double vpu_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vfn_max"));
      //determina tensão fase/neutro máxima em pu
      vpu_max = Abs(pbarra->resflow->Vfn_pu_max[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseNeutro_pu_min(void)
   {
   //variáveis locais
   double vpu_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vfn_min"));
      //determina tensão fase/neutro mínima em pu
      vpu_min = Abs(pbarra->resflow->Vfn_pu_min[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_kv(void)
   {
   //variáveis locais
   complex<double> vkv;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Vat"}, {faseB, "Vbt"}, {faseC, "Vct"}};

   //tensões fase/neutro em pu
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if (pbarra->resflow->Fases & fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/terra em kV
         vkv = pbarra->resflow->Vft_kv[num_pat][nf];
         //define unidade e valor
         aue.Tensao_KV(vkv, dado->valor, dado->unidade);
         }
      }
   //tensão neutro/terra em kV
   lisEXT->Add(dado = NewObjDado("Vnt"));
   //determina tensão neutro/terra em kV
   vkv = pbarra->resflow->Vnt_kv[num_pat];
   //define unidade e valor
   aue.Tensao_KV(vkv, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_kv_max(void)
   {
   //variáveis locais
   double vkv_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vft_max"));
      //determina tensão fase/terra máxima em kV
      vkv_max = Abs(pbarra->resflow->Vft_kv_max[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_kv_min(void)
   {
   //variáveis locais
   double vkv_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vft_min"));
      //determina tensão fase/terra mínima em kV
      vkv_min = Abs(pbarra->resflow->Vft_kv_min[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_pu(void)
   {
   //variáveis locais
   complex<double> vpu;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Vat"}, {faseB, "Vbt"}, {faseC, "Vct"}};

   //tensões fase/neutro em pu
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if (pbarra->resflow->Fases & fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/terra em kV
         vpu = pbarra->resflow->Vft_pu[num_pat][nf];
         //define unidade e valor
         aue.Tensao_PU(vpu, dado->valor, dado->unidade);
         }
      }
   //tensão neutro/terra em pu
   lisEXT->Add(dado = NewObjDado("Vnt"));
   //determina tensão neutro/terra em pu
   vpu = pbarra->resflow->Vnt_pu[num_pat];
   //define unidade e valor
   aue.Tensao_PU(vpu, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_pu_max(void)
   {
   //variáveis locais
   double vpu_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vft_max"));
      //determina tensão fase/terra máxima em pu
      vpu_max = Abs(pbarra->resflow->Vft_pu_max[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoFaseTerra_pu_min(void)
   {
   //variáveis locais
   double vpu_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vft_min"));
      //determina tensão fase/terra mínima em pu
      vpu_min = Abs(pbarra->resflow->Vft_pu_min[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_kv(void)
   {
   //variáveis locais
   complex<double> vkv;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseAB, "Vab"}, {faseBC, "Vbc"}, {faseCA, "Vca"}};

   //tensões fase/fase em kV
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/neutro em kV
         vkv = pbarra->resflow->Vff_kv[num_pat][nf];
         //define unidade e valor
         aue.Tensao_KV(vkv, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_kv_max(void)
   {
   //variáveis locais
   double vkv_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vff_max"));
      //determina tensão de linha máxima em kV
      vkv_max = Abs(pbarra->resflow->Vff_kv_max[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_kv_min(void)
   {
   //variáveis locais
   double vkv_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vff_min"));
      //determina tensão fase/neutro mínima em kV
      vkv_min = Abs(pbarra->resflow->Vff_kv_min[num_pat]);
      //define unidade e valor
      aue.Tensao_KV(vkv_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_pu(void)
   {
   //variáveis locais
   complex<double> vpu;
   VTDado          *dado;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseAB, "Vab"}, {faseBC, "Vbc"}, {faseCA, "Vca"}};

   //tensões fase/fase em kV
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina tensão fase/neutro em pu
         vpu = pbarra->resflow->Vff_pu[num_pat][nf];
         //define unidade e valor
         aue.Tensao_PU(vpu, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_pu_max(void)
   {
   //variáveis locais
   double vpu_max;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vff_max"));
      //determina tensão de linha máxima em pu
      vpu_max = Abs(pbarra->resflow->Vff_pu_max[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_max, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoLinha_pu_min(void)
   {
   //variáveis locais
   double vpu_min;
   VTDado  *dado;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Vff_min"));
      //determina tensão fase/neutro mínima em pu
      vpu_min = Abs(pbarra->resflow->Vff_pu_min[num_pat]);
      //define unidade e valor
      aue.Tensao_PU(vpu_min, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::ValorTensaoNominal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Vnom"));
   //define unidade e valor
   aue.Tensao_KV(pbarra->vnom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorDesequilibrio(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Desequilíbrio", pbarra->resflow->Max_pat));
   //define unidade
   //vetval->unidade = "%";
   //preenche valores
   for (int np = 0; np < pbarra->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = pbarra->resflow->Desequilibrio[np];
      //preenche valor como string
      aue.ValorPorcentual(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_kv(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vkv;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Van"}, {faseB, "Vbn"}, {faseC, "Vcn"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vkv = pbarra->resflow->Vfn_kv[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vkv);
            //preenche valor como string
            aue.Tensao_KV(vkv, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_kv_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vfn_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kV";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vfn_kv_max[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_kv_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vfn_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kV";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vfn_kv_min[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_pu(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vpu;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Van"}, {faseB, "Vbn"}, {faseC, "Vcn"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vpu = pbarra->resflow->Vfn_pu[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vpu);
            //preenche valor como string
            aue.Tensao_PU(vpu, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_pu_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vfn_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vfn_pu_max[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseNeutro_pu_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vfn_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vfn_pu_min[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_kv(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vkv;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Vat"}, {faseB, "Vbt"}, {faseC, "Vct"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vkv = pbarra->resflow->Vft_kv[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vkv);
            //preenche valor como string
            aue.Tensao_KV(vkv, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_kv_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vft_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kV";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vft_kv_max[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_kv_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vft_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kV";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vft_kv_min[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_pu(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vpu;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseA, "Vat"}, {faseB, "Vbt"}, {faseC, "Vct"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vpu = pbarra->resflow->Vft_pu[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vpu);
            //preenche valor como string
            aue.Tensao_PU(vpu, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_pu_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vft_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vft_pu_max[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoFaseTerra_pu_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 2 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 2)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vft_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vft_pu_min[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_kv(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vpu;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseAB, "Vab"}, {faseBC, "Vbc"}, {faseCA, "Vca"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vpu = pbarra->resflow->Vff_kv[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vpu);
            //preenche valor como string
            aue.Tensao_KV(vpu, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_kv_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 3 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vff_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kv";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vff_kv_max[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_kv_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 3 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vff_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "kv";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vff_kv_min[np];
         //preenche valor como string
         aue.Tensao_KV(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_KV();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_pu(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> vpu;
   VTVetval       *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE-1] = {{faseAB, "Vab"}, {faseBC, "Vbc"}, {faseCA, "Vca"}};

   //cria um Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < (MAX_FASE - 1); nf++)
      {//verifica se existe a fase na Barra
      if ((pbarra->resflow->Fases & fase[nf].tag) == fase[nf].tag)
         {//cria um Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, pbarra->resflow->Max_pat));
         //loop p/ todos patamares
         for (int np = 0; np < pbarra->resflow->Max_pat; np++)
            {
            vpu = pbarra->resflow->Vff_pu[np][nf];
            //preenche módulo da tensão linha em pu
            vetval->Valor[np] = Abs(vpu);
            //preenche valor como string
            aue.Tensao_PU(vpu, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_pu_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 3 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vff_max", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vff_pu_max[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBarra::VetValorTensaoLinha_pu_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //verifica se há pelo menos 3 fases na barra
   if (fases->NumeroDeFases(pbarra->resflow->Fases) >= 3)
      {//cria um novo Vetval e insere em lisEXT
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Vff_min", pbarra->resflow->Max_pat));
      //define unidade
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < pbarra->resflow->Max_pat; np++)
         {
         vetval->Valor[np] = pbarra->resflow->Vff_pu_min[np];
         //preenche valor como string
         aue.Tensao_PU(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
//eof

