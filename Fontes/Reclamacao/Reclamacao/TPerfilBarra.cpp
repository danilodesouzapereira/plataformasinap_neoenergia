//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <DateUtils.hpp>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilBarra.h"
#include "VTMedicao.h"
#include "VTRes505.h"
#include "VTTensao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilBarra*  __fastcall NewObjPerfilBarra(VTBarra *barra)
   {
   try{
      return(new TPerfilBarra(barra));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilBarra::TPerfilBarra(VTBarra *barra)
   {
   //salva ponteiro p/ Barra
   PD.barra = barra;
   //cria objetos
   lisTENSAO = new TList();
   //inicia dados dos índices
   PD.num_amostra = 0;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      PD.dra[ind_fase] = 0;
      PD.drc[ind_fase] = 0;
      PD.drp[ind_fase] = 0;
      }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilBarra::~TPerfilBarra(void)
   {
   //destrói lista e seus objetos
   if (lisTENSAO) {LimpaTList(lisTENSAO); delete lisTENSAO; lisTENSAO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilBarra::CalculaDRA_DRP_DRC(VTRes505 *res505, int limite_amostra)
   {
   //variáveis locais
   double   vfn_kv;
   VTTensao *tensao;

   //inicia dados dos índices
   PD.num_amostra = 0;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      PD.dra[ind_fase] = 0;
      PD.drc[ind_fase] = 0;
      PD.drp[ind_fase] = 0;
      }
   //proteção: verifica se a Barra possui nominal diferente da definida e/ Res505
   if (Barra->zona == NULL) return(true);
   //DVK 2015.03.31 linha comentada: verificação desnecessária, pois os
   //indicadores são calculados p/ barra da carga analisada
//   if (! IsDoubleZero(Barra->zona->Vff_kv - res505->Vnom_ff_kv)) return(true);
   //loop p/ todos objetos Tensao
   for (int n = 0; n < lisTENSAO->Count; n++)
      {//verifica limite de amostras
//      if ((limite_amostra > 0)&&(PD.num_amostra >= limite_amostra)) break;
      //determina objeto Tensao
      tensao = (VTTensao*)lisTENSAO->Items[n];
      //loop p/ todos patamares
      for (int ind_pat = 0; ind_pat < tensao->NumeroPatamares; ind_pat++)
         {//verifica limite de amostras
         if ((limite_amostra > 0)&&(PD.num_amostra >= limite_amostra)) break;
         //verifica se existe tensão no patamar
         if (! tensao->Enabled[ind_pat]) continue;
         //incrementa número de amostragens
         PD.num_amostra++;
         //loop p/ todas fases
         for (int ind_fase = 0; ind_fase < (MAX_FASE-1); ind_fase++)
            {//determina tensão na fase
            vfn_kv = tensao->ModVfn_kv[ind_pat][ind_fase];
            //verifica se tensão aceitável, precária ou crítica
            switch(res505->ClassificaAdequadaPrecariaCritica(vfn_kv))
               {
               case eADEQUADA: PD.dra[ind_fase]++; break;
               case ePRECARIA: PD.drp[ind_fase]++; break;
               case eCRITICA:  PD.drc[ind_fase]++; break;
               }
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilBarra::CalculaIndicesQualidadeUmDia(VTRes505 *res505, int ind_dia)
   {
   //variáveis locais
   double   vfn_kv;
   VTTensao *tensao;

   //inicia dados dos índices
   PD.num_amostra = 0;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      PD.dra[ind_fase] = 0;
      PD.drc[ind_fase] = 0;
      PD.drp[ind_fase] = 0;
      }
   //proteção: verifica se ind_dia é válido
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   //proteção: verifica se a Barra possui nominal diferente da definida e/ Res505
   if (Barra->zona == NULL) return(true);
   if (! IsDoubleZero(Barra->zona->Vff_kv - res505->Vnom_ff_kv)) return(true);
   //determina objeto Tensao
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < tensao->NumeroPatamares; ind_pat++)
      {//verifica se existe tensão no patamar
      if (! tensao->Enabled[ind_pat]) continue;
      //incrementa número de amostragens
      PD.num_amostra++;
      //loop p/ todas fases
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {//determina tensão na fase
         vfn_kv = tensao->ModVfn_kv[ind_pat][ind_fase];
         //verifica se tensão aceitável, precária ou crítica
         switch(res505->ClassificaAdequadaPrecariaCritica(vfn_kv))
            {
            case eADEQUADA: PD.dra[ind_fase]++; break;
            case ePRECARIA: PD.drp[ind_fase]++; break;
            case eCRITICA:  PD.drc[ind_fase]++; break;
            }
         }
      }
   return(true);
   }             

//---------------------------------------------------------------------------
int __fastcall TPerfilBarra::DRA(int fases)
   {
   //variáveis locais
   int dra;

   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina um valor de DRA de uma fase
   if      ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) dra = PD.dra[indFASE_A];
   else if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) dra = PD.dra[indFASE_B];
   else if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) dra = PD.dra[indFASE_C];
   else                                                    return(0);
   //determina valor mínimo de DRA das fases
   if ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) dra = min(dra, PD.dra[indFASE_A]);
   if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) dra = min(dra, PD.dra[indFASE_B]);
   if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) dra = min(dra, PD.dra[indFASE_C]);

   return(dra);
   }

//---------------------------------------------------------------------------
double __fastcall TPerfilBarra::DRA_perc(int fases)
   {
   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina valor de DRA
   return((double(DRA(fases))/double(PD.num_amostra)) * 100.);
   }

//---------------------------------------------------------------------------
int __fastcall TPerfilBarra::DRC(int fases)
   {
   //variáveis locais
   int drc;

   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina um valor de DRA de uma fase
   if      ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) drc = PD.drc[indFASE_A];
   else if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) drc = PD.drc[indFASE_B];
   else if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) drc = PD.drc[indFASE_C];
   else                                                    return(0);
   //determina valor máximo de DRC das fases
   if ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) drc = max(drc, PD.drc[indFASE_A]);
   if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) drc = max(drc, PD.drc[indFASE_B]);
   if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) drc = max(drc, PD.drc[indFASE_C]);

   return(drc);
   }

//---------------------------------------------------------------------------
double __fastcall TPerfilBarra::DRC_perc(int fases)
   {
   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina valor de DRC
   return(double((DRC(fases))/double(PD.num_amostra)) * 100.);
   }

//---------------------------------------------------------------------------
int __fastcall TPerfilBarra::DRP(int fases)
   {
   //variáveis locais
   int drp;

   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina um valor de DRA de uma fase
   if      ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) drp = PD.drp[indFASE_A];
   else if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) drp = PD.drp[indFASE_B];
   else if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) drp = PD.drp[indFASE_C];
   else                                                    return(0);
   //determina valor máximo de DRC das fases
   if ((PD.fase_enabled[indFASE_A])&&(fases & faseA)) drp = max(drp, PD.drp[indFASE_A]);
   if ((PD.fase_enabled[indFASE_B])&&(fases & faseB)) drp = max(drp, PD.drp[indFASE_B]);
   if ((PD.fase_enabled[indFASE_C])&&(fases & faseC)) drp = max(drp, PD.drp[indFASE_C]);

   return(drp);
   }

//---------------------------------------------------------------------------
double __fastcall TPerfilBarra::DRP_perc(int fases)
   {
   //proteção
   if (PD.num_amostra == 0) return(0);
   //determina valor de DRP
   return((double(DRP(fases))/double(PD.num_amostra)) * 100.);
   }

//---------------------------------------------------------------------------
VTTensao* __fastcall TPerfilBarra::ExisteTensao(TDateTime data)
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
int __fastcall TPerfilBarra::NumeroAmostragem(void)
   {
   return(PD.num_amostra);
   }

//---------------------------------------------------------------------------
bool  __fastcall TPerfilBarra::PM_GetFaseEnabled(int ind_fase)
   {
   return(PD.fase_enabled[ind_fase]);
   }

//---------------------------------------------------------------------------
void  __fastcall TPerfilBarra::PM_SetFaseEnabled(int ind_fase, bool enabled)
    {
   PD.fase_enabled[ind_fase] = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilBarra::SalvaTensao(TDateTime data, VTMedicao *medicao, bool padrao)
   {
   //variáveis locais
   int             num_pat = Barra->resflow->Max_pat;
   complex<double> vfn_kv[MAX_PAT];
   double          mod_vfn_kv[MAX_FASE];
   VTTensao        *tensao;

   //salva informação de fase disponível
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //FaseEnabled[ind_fase] = medicao->FaseEnabled[ind_fase];
      FaseEnabled[ind_fase] = true;
      }
   //verifica se já existe Tensao p/ a data indicada
   if ((tensao = ExisteTensao(data)) == NULL)
      {//cria novo objeto Tensao
      tensao      = NewObjTensao(num_pat);
      tensao->Dia = DayOf(data);
      tensao->Mes = MonthOf(data);
      tensao->Ano = YearOf(data);
      //para cada dia, 1 novo objeto tensao
      lisTENSAO->Add(tensao);
      }
   //salva valores de tensão da Barra
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se há medição no patamar
      if (! medicao->ExisteMedicao(data, ind_pat)) continue;
      //DVK 2015.03.27
      //verifica se a rede original está ativa e se a barra é da medição
      if((padrao) && (medicao->Carga->pbarra == Barra))
         {//salva tensões da medição
         medicao->ExisteMedicao(lisTENSAO->IndexOf(tensao), ind_pat, mod_vfn_kv);
         tensao->InsereTensao(ind_pat, mod_vfn_kv);
         }
      else
         {//há medição: salva tensões da barra no patamar
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            //if (! FaseEnabled[ind_fase]) continue;
            vfn_kv[ind_fase] = Barra->resflow->Vfn_kv[ind_pat][ind_fase];
            }
         //salva tensões de fase da Barra
         tensao->InsereTensao(ind_pat, vfn_kv);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilBarra::Vfn_kv(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //determina Tensao do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= tensao->NumeroPatamares)) return(false);
   if(! tensao->Enabled[ind_pat]) return(false);      //DVK 2015.03.27
   //copia valores de tensão das fases
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vfn_kv[ind_fase] = tensao->ModVfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilBarra::Vfn_kv(int ind_dia, int ind_pat, complex<double> vfn_kv[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //determina Tensao do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= tensao->NumeroPatamares)) return(false);
   if(! tensao->Enabled[ind_pat]) return(false);      //DVK 2015.03.27
   //copia valores de tensão das fases
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vfn_kv[ind_fase] = tensao->Vfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

