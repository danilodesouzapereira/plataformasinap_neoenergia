//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Potencia\VTFornecimento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilSup.h"
#include "VTMedicao.h"
#include "VTTensao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilSup*  __fastcall NewObjPerfilSup(VTApl *apl, VTSuprimento *suprimento)
   {
   try{
      return(new TPerfilSup(apl, suprimento));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilSup::TPerfilSup(VTApl *apl, VTSuprimento *suprimento)
   {
   //salva ponteiro
   this->apl     = apl;
   PD.suprimento = suprimento;
   //cria objetos
   lisTENSAO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerfilSup::~TPerfilSup(void)
   {
   //destrói lista e seus objetos
   if (lisTENSAO) {LimpaTList(lisTENSAO); delete lisTENSAO; lisTENSAO = NULL;}
   }

//---------------------------------------------------------------------------
VTTensao* __fastcall TPerfilSup::ExisteTensao(TDateTime data)
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
bool __fastcall TPerfilSup::AtualizaTensaoSuprimento(TDateTime data)
   {
   //variáveis locais
   int             num_pat;
   double          mod_vfn_pu, ang_vfn_rad;
   complex<double> vsup_pu;
   strSUP          *str_sup, vet_sup[MAX_FASE];
   VTTensao        *tensao;
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //obtém objeto Tensao p/ a data indicada
   if ((tensao = ExisteTensao(data)) == NULL) return(false);
   //determina número de patamares
   num_pat = tensao->NumeroPatamares;
   //atualliza tensão do Suprimento com os valores salvos em Tensao (OBS: os valores foram salvos em pu)
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se existe  valor salvo no patamar
      if (! tensao->Enabled[ind_pat]) continue;
      //define PQVT por fase do suprimento
      for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
         {
         vsup_pu     = tensao->Vfn_kv[ind_pat][ind_fase];
         mod_vfn_pu  = Abs(vsup_pu);
         ang_vfn_rad = Arg(vsup_pu);
         str_sup     = vet_sup + ind_fase;  // Obtém endereço no vetor
         str_sup->p  = 0.;
         str_sup->q  = 0.;
         str_sup->v  = mod_vfn_pu;
         str_sup->t  = ang_vfn_rad;
         }
      if(! fornecimento->Set_PQVT_porfase(Suprimento, ind_pat, vet_sup)) return(false);
      }
   //desaloca memoria
   //delete fornecimento;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilSup::SalvaTensao(TDateTime data, VTMedicao *medicao)
   {
   //variáveis locais
   int             num_pat;
   double          mod_v_pu, ang_v_rad;
   strSUP          *str_sup, vet_sup[MAX_FASE];
   VTTensao        *tensao;
   complex<double> vfn_pu[MAX_FASE];
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

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
   //salva valores de tensão do Suprimento
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se há medição no patamar
      if (! medicao->ExisteMedicao(data, ind_pat)) continue;
      // Obtém tensão
      if(! (PD.suprimento)->DefinidoPorFase) continue;  // Proteção
      if(! fornecimento->Get_PQVT_porfase(PD.suprimento, ind_pat, vet_sup)) continue;
      for (int nf = 0; nf < MAX_FASE; nf++)
         {//obtém demanda por fase
         str_sup    = vet_sup + nf;
         mod_v_pu   = str_sup->v;
         ang_v_rad  = str_sup->t;
         vfn_pu[nf] = polar(mod_v_pu, ang_v_rad);
         }
      //salva demanda por fase da Carga
      tensao->InsereTensao(ind_pat, vfn_pu);
      }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TPerfilSup::Vfn_pu(int ind_dia, int ind_pat, double v_pu[MAX_FASE])
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
      v_pu[ind_fase] = Abs(tensao->Vfn_kv[ind_pat][ind_fase]);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilSup::Vfn_pu(int ind_dia, int ind_pat, complex<double> v_pu[MAX_FASE])
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
      v_pu[ind_fase] = tensao->Vfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

