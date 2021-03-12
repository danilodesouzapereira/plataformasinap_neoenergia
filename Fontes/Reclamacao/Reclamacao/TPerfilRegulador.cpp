//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTResFlowLig.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilRegulador.h"
#include "VTMedicao.h"
#include "VTCarreg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilRegulador*  __fastcall NewObjPerfilRegulador(VTRegulador *reg)
   {
   try{
      return(new TPerfilRegulador(reg));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilRegulador::TPerfilRegulador(VTRegulador *reg)
   {
   //salva ponteiro p/ regulador
   PD.reg = reg;
   //cria objetos
   lisCARREG = new TList();
   //inicia dados dos índices
   PD.num_amostra = 0;

   }

//---------------------------------------------------------------------------
__fastcall TPerfilRegulador::~TPerfilRegulador(void)
   {
   //destrói lista e seus objetos
   if (lisCARREG) {LimpaTList(lisCARREG); delete lisCARREG; lisCARREG = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilRegulador::Corrente(int ind_dia, int ind_pat, int ind_fase, double* i_antes, double* i_depois)
   {
   //variáveis locais
   VTCarreg *carreg;

   //determina carregamento do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisCARREG->Count)) return(false);
   carreg = (VTCarreg*)lisCARREG->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= carreg->NumeroPatamares)) return(false);
   //copia valores de corrente
   *i_antes = carreg->ModI_Antes[ind_pat][ind_fase];
   *i_depois = carreg->ModI_Depois[ind_pat][ind_fase];

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilRegulador::Carregamento(int ind_dia, int ind_pat, double* cargto)
   {
   //variáveis locais
   VTCarreg *carreg;

   //determina carregamento do dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisCARREG->Count)) return(false);
   carreg = (VTCarreg*)lisCARREG->Items[ind_dia];
   //verifica se o patamar indicado é válido
   if ((ind_pat < 0)||(ind_pat >= carreg->NumeroPatamares)) return(false);
   //copia valores de carregamento
   *cargto = carreg->Carregamento[ind_pat];
   return(true);
   }

//---------------------------------------------------------------------------
VTCarreg* __fastcall TPerfilRegulador::ExistePerfilReg(TDateTime data)
   {
   //variáveids locais
   VTCarreg *carreg;

   for (int n = 0; n < lisCARREG->Count; n++)
      {//verifica se existe valores de carregamento na mesma data
      carreg = (VTCarreg*)lisCARREG->Items[n];
      if (carreg->Data == data) return(carreg);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TPerfilRegulador::NumeroAmostragem(void)
   {
   return(PD.num_amostra);
   }

//---------------------------------------------------------------------------
bool  __fastcall TPerfilRegulador::PM_GetFaseEnabled(int ind_fase)
   {
   return(PD.fase_enabled[ind_fase]);
   }

//---------------------------------------------------------------------------
void  __fastcall TPerfilRegulador::PM_SetFaseEnabled(int ind_fase, bool enabled)
    {
   PD.fase_enabled[ind_fase] = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilRegulador::SalvaCarreg(TDateTime data, VTMedicao *medicao)
   {
   //variáveis locais
   bool            ok;
   int            num_pat = Reg->resflow->Max_pat;
   double          valor, I_ANTES[MAX_FASE], I_DEPOIS[MAX_FASE];
   VTCarreg       *carreg;
   int            ind_bar1 = Reg->IndiceBarra(Reg->pbarra1);
   int            ind_bar2 = Reg->IndiceBarra(Reg->pbarra2);

   //verifica se já existe carregamento p/ a data indicada
   if ((carreg = ExistePerfilReg(data)) == NULL)
      {//cria novo objeto Carreg
      carreg      = NewObjCarreg(num_pat);
      carreg->Dia = DayOf(data);
      carreg->Mes = MonthOf(data);
      carreg->Ano = YearOf(data);
      lisCARREG->Add(carreg);
      }
   //salva valores de carregamento do regulador
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {//verifica se há medição no patamar
      if (! medicao->ExisteMedicao(data, ind_pat)) continue;
      //há medição: salva carregamento e correntes no patamar
      valor = Reg->resflow->Cargto[ind_pat];
      if(Reg->resflow->SentidoP[ind_bar1][ind_pat] == fluxoNEG)  //bar1=1 e bar2=0
         {//verifica o sentido do fluxo, para definir a barra antes e depois do regulador
         ind_bar1 = 1 - ind_bar1;     //barra antes  -> bar1=0
         ind_bar2 = 1 - ind_bar2;     //barra depois -> bar2=1
         }
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {//define correntes antes e depois do regulador
         I_ANTES[ind_fase]  = Abs(Reg->resflow->If_amp[ind_bar1][ind_pat][ind_fase]);
         I_DEPOIS[ind_fase] = Abs(Reg->resflow->If_amp[ind_bar2][ind_pat][ind_fase]);
         if(I_ANTES[ind_fase] != 0.0)         PD.fase_enabled[ind_fase] = true;
         }
      //salva carregamento e correntes
      ok = carreg->InsereCarreg(ind_pat, valor);
      ok = carreg->InsereCorrenteAntes(ind_pat, I_ANTES);
      ok = carreg->InsereCorrenteDepois(ind_pat, I_DEPOIS);
      }
   return(ok);
   }

//---------------------------------------------------------------------------
//eof

