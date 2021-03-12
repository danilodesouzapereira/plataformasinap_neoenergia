//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Zona\VTZona.h>
#include <Fontes\Complexo\Complexo.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilCarga.h"
#include "VTMedicao.h"
#include "VTTensao.h"
//#include "VTRes505.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilCarga*  __fastcall NewObjPerfilCarga(VTApl *apl, VTCarga *carga)
   {
   try{
      return(new TPerfilCarga(apl, carga));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilCarga::TPerfilCarga(VTApl *apl, VTCarga *carga)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   PD.carga  = carga;
   //cria objetos
   lisTENSAO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerfilCarga::~TPerfilCarga(void)
   {
   //destrói lista e seus objetos
   if (lisTENSAO) {LimpaTList(lisTENSAO); delete lisTENSAO; lisTENSAO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilCarga::AtualizaDemandaDaCarga(TDateTime data)
   {
   //variáveis locais
   int             num_pat;
   complex<double> saux_mva[MAX_FASE];
   VTTensao        *tensao;
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //obtém objeto Tensao p/ a data indicada
   if ((tensao = ExisteTensao(data)) == NULL) return(false);
   //determina número de patamares
   num_pat = tensao->NumeroPatamares;
   //atualiza demanda da Carga com os valores salvos em Tensao (OBS: é feio mas funciona)
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se existe  valor salvo no patamar
      if (! tensao->Enabled[ind_pat]) continue;
      //monta valores de demanda por fase
      for (int ind_fase = indFASE_A, fase = faseA; ind_fase <= indFASE_C; ind_fase++, fase++)
         {//obtém demanda por fase armazenada em Tensao
         saux_mva[ind_fase] = tensao->Vfn_kv[ind_pat][ind_fase];
         }
      saux_mva[indNEUTRO] = CZero();

      // Atualiza demanda por fase
      if(! demanda->Set_S3f_mva(Carga, ind_pat, saux_mva)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTTensao* __fastcall TPerfilCarga::ExisteTensao(TDateTime data)
   {
   //variáveids locais
   VTTensao *tensao;

   for (int n = 0; n < lisTENSAO->Count; n++)
      {
      tensao = (VTTensao*)lisTENSAO->Items[n];
      if (tensao->Data == data) return(tensao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilCarga::SalvaDemanda(TDateTime data, VTMedicao *medicao)
   {
   //variáveis locais
   int             num_pat;
   VTTensao        *tensao;
   complex<double> sf_mva[MAX_FASE];
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //determina número de patamares
   num_pat = medicao->NumeroMedicaoPorDia;
   //verifica se já existe Tensao p/ a data indicada
   if ((tensao = ExisteTensao(data)) == NULL)
      {//cria novo pbjeto Tensao
      tensao      = NewObjTensao(num_pat);
      tensao->Dia = DayOf(data);
      tensao->Mes = MonthOf(data);
      tensao->Ano = YearOf(data);
      lisTENSAO->Add(tensao);
      }
   //salva valores de demanda da Carga em Tensao (OBS: é feio mas funciona)
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se há medição no patamar
      if (! medicao->ExisteMedicao(data, ind_pat)) continue;
      // Obtém demanda por fase
      if(! demanda->Get_S3f_mva(Carga, ind_pat, sf_mva)) continue;
      //salva demanda por fase da Carga
      tensao->InsereTensao(ind_pat, sf_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilCarga::Sf_mva(int ind_dia, int ind_pat, double sf_mva[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //determina Tensao do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= tensao->NumeroPatamares)) return(false);
   //copia valores de tensão das fases
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      sf_mva[ind_fase] = tensao->ModVfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilCarga::Sf_mva(int ind_dia, int ind_pat, complex<double> sf_mva[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //determina Tensao do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= tensao->NumeroPatamares)) return(false);
   //copia valores de tensão das fases
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      sf_mva[ind_fase] = tensao->Vfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

