//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasores.h"
#include "TAngulos.h"
#include "VTFasor.h"
#include "TFasorAjuste.h"
#include "VTGrupo.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Fases.h"
#include "..\Flow\VTFlow.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTChave.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasores* __fastcall NewObjFasores(VTApl *apl)
   {
   try{//cria objeto TFasores
      return(new TFasores(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
//---------------------------------------------------------------------------
__fastcall TFasores::TFasores(VTApl *apl)
   {
   this->apl = apl;
   try{//cria listas
      lisFASOR = new TList();
      lisGRUPO = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasores::~TFasores(void)
   {
   //destrói lista de Grupos e seus objetos
   if (lisGRUPO) {LimpaTList(lisGRUPO); delete lisGRUPO; lisGRUPO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::DefineAnguloAjuste_rad(TAngulos *ang)
   {
   //variáveis locais
   TFasorAjuste *fasor_ajuste;
   VTFasor      *fasor;
   VTGrupo      *grupo;
   TList        *lisFASOR;
   double       ang_novo;

   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      //define patamar p/ todos TFasor
      for (int n = 0; n < lisFASOR->Count; n++)
         {
         fasor = (VTFasor*)lisFASOR->Items[n];
         if (fasor->Tipo() != fasorAJUSTE) continue;
         fasor_ajuste = (TFasorAjuste*)fasor;
         //verifica se fasor do tipo fasorAJUSTE
         if (fasor_ajuste->Classe() == 'A')
            {
            if (fasor->ind_seq == indSEQ_0)  ang_novo = ang->ajuste_A_seq0;
            else                             ang_novo = ang->ajuste_A_outros;
            }
         else
            {
            if (fasor->ind_seq == indSEQ_0)  ang_novo = ang->ajuste_B_seq0;
            else                             ang_novo = ang->ajuste_B_outros;
            }
         fasor_ajuste->DefineAnguloAjuste_rad(ang_novo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::DefinePatamar(int ind_pat)
   {
   //variáveis locais
   VTFasor *fasor;
   VTGrupo *grupo;
   TList   *lisFASOR;

   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      //define patamar p/ todos TFasor
      for (int n = 0; n < lisFASOR->Count; n++)
         {
         fasor = (VTFasor*)lisFASOR->Items[n];
         fasor->DefinePatamar(ind_pat);
         }
      }
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorCurtoIfas(int tipo_curto, int fases)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorCURTO_Ifas)&&
             (fasor->tipo_curto == tipo_curto)&&
             (fasor->fases == fases)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorCurtoIseq(int tipo_curto, int ind_seq)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorCURTO_Iseq)&&
             (fasor->tipo_curto == tipo_curto)&&
             (fasor->ind_seq == ind_seq)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorCurtoVfas(int tipo_curto, int fases)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorCURTO_Vfas)&&
             (fasor->tipo_curto == tipo_curto)&&
             (fasor->fases == fases)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorCurtoVseq(int tipo_curto, int ind_seq)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorCURTO_Vseq)&&
             (fasor->tipo_curto == tipo_curto)&&
             (fasor->ind_seq == ind_seq)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorFluxoIfas(int fases)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorFLUXO_Ifas)&&
             (fasor->fases == fases)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorFluxoIseq(int ind_seq)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorFLUXO_Iseq)&&
             (fasor->ind_seq == ind_seq)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorFluxoVfas(int fases)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorFLUXO_Vfas)&&
             (fasor->fases == fases)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTFasor* __fastcall TFasores::ExisteFasorFluxoVseq(int ind_seq)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         if ((fasor->Tipo() == fasorFLUXO_Vseq)&&
             (fasor->ind_seq == ind_seq)) return(fasor);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::IniciaVetorCores(TColor *vet_color, int dim_vet)
   {
   //protecção
   if (dim_vet > 6) dim_vet = 6;
   switch(dim_vet)
      {
      case 6: vet_color[5] = clBlack;
      case 5: vet_color[4] = clOlive;
      case 4: vet_color[3] = clSkyBlue;
      case 3: vet_color[2] = clGreen;
      case 2: vet_color[1] = clRed;
      case 1: vet_color[0] = clNavy;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::InsereFasores(VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //var locais
   VTFlow    *flow   = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedeCC  *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));

   //proteções
   if ((bar_ref == NULL)||(ligacao == NULL)) return(false);
   //insere fasores do fluxo
   if (flow->UpToDate)
      {//monta Grupo de fasores de fluxo
      InsereFasoresFluxo(bar_ref, ligacao);
      }
   //insere fasores do curto
   if (redeCC != NULL)
      {//insere fasores c/curto circuito
      InsereFasoresCurto(bar_ref, ligacao);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::InsereFasoresCurto(VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //loop p/ todos tipos de defeito
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {//cria Grupo de fasores
      MontaGrupoFasorCurtoVfn(ind_def,  bar_ref);
      MontaGrupoFasorCurtoVff(ind_def,  bar_ref);
      MontaGrupoFasorCurtoVseq(ind_def, bar_ref);
      MontaGrupoFasorCurtoIfas(ind_def, bar_ref, ligacao);
      MontaGrupoFasorCurtoIseq(ind_def, bar_ref, ligacao);
      MontaGrupoFasorCurtoAngulo(ind_def);
      MontaGrupoFasorCurtoAjusteA(ind_def);
      MontaGrupoFasorCurtoAjusteB(ind_def);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::InsereFasoresFluxo(VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //cria Grupos de fasores
   MontaGrupoFasorFluxoVfn(bar_ref);
   MontaGrupoFasorFluxoVff(bar_ref);
   MontaGrupoFasorFluxoVseq(bar_ref);
   MontaGrupoFasorFluxoIfas(bar_ref, ligacao);
   MontaGrupoFasorFluxoIseq(bar_ref, ligacao);
   MontaGrupoFasorFluxoAngulo();
   MontaGrupoFasorFluxoAjusteA();
   MontaGrupoFasorFluxoAjusteB();

   return(true);
   }


//---------------------------------------------------------------------------
TList* __fastcall TFasores::LisGrupo(void)
   {
   return(lisGRUPO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFasores::LisFasorGruposSelecionados(void)
   {
   //variáveis locais
   VTGrupo *grupo;

   //reinicia lisFASOR
   lisFASOR->Clear();
   //insere em lisFASOR os objetos VTFasor dos Grupos selecionados
   for (int n = 0; n < lisGRUPO->Count; n++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[n];
      if (grupo->Visible)
         {
         grupo->LisFasor(lisFASOR);
         }
      }
   return(lisFASOR);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoAjusteA(int ind_def)
   {
   //variáveis locais
   char    classe = 'A';
   VTFasor *fasorI, *fasorV;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Ajuste A";
   //cria fasor I0 - V0
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_0);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_0);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I1 - V1
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_1);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_1);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I2 - V2
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_2);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_2);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
//   //cria fasor Ia - Va
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseA);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseAN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(classe, fasorI, fasorV, ang_ajuste));
//   //cria fasor Ib - Vb
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseB);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseBN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(classe, fasorI, fasorV, ang_ajuste));
//   //cria fasor Ic - Vc
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseC);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseCN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(classe, fasorI, fasorV, ang_ajuste));
   //cria fasor Ia - Vbc
   fasorI = ExisteFasorCurtoIfas(ind_def, faseA);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseBC);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ib - Vca
   fasorI = ExisteFasorCurtoIfas(ind_def, faseB);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseCA);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ic - Vab
   fasorI = ExisteFasorCurtoIfas(ind_def, faseC);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseAB);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoAjusteB(int ind_def)
   {
   //variáveis locais
   char    classe = 'B';
   VTFasor *fasorI, *fasorV;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Ajuste B";
   //cria fasor I0 - V0
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_0);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_0);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I1 - V1
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_1);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_1);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I2 - V2
   fasorI = ExisteFasorCurtoIseq(ind_def, indSEQ_2);
   fasorV = ExisteFasorCurtoVseq(ind_def, indSEQ_2);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
//   //cria fasor Ia - Va
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseA);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseAN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ib - Vb
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseB);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseBN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ic - Vc
//   fasorI = ExisteFasorCurtoIfas(ind_def, faseC);
//   fasorV = ExisteFasorCurtoVfas(ind_def, faseCN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
   //cria fasor Ia - Vbc
   fasorI = ExisteFasorCurtoIfas(ind_def, faseA);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseBC);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ib - Vca
   fasorI = ExisteFasorCurtoIfas(ind_def, faseB);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseCA);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ic - Vab
   fasorI = ExisteFasorCurtoIfas(ind_def, faseC);
   fasorV = ExisteFasorCurtoVfas(ind_def, faseAB);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoAngulo(int ind_def)
   {
   //variáveis locais
   VTFasor *fasor;
   VTFasor *fasor1, *fasor2;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Ângulos";
   //cria fasor I0 - V0
   fasor1 = ExisteFasorCurtoIseq(ind_def, indSEQ_0);
   fasor2 = ExisteFasorCurtoVseq(ind_def, indSEQ_0);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor I1 - V1
   fasor1 = ExisteFasorCurtoIseq(ind_def, indSEQ_1);
   fasor2 = ExisteFasorCurtoVseq(ind_def, indSEQ_1);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor I2 - V2
   fasor1 = ExisteFasorCurtoIseq(ind_def, indSEQ_2);
   fasor2 = ExisteFasorCurtoVseq(ind_def, indSEQ_2);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ia - Va
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseA);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseAN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ib - Vb
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseB);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseBN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ic - Vc
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseC);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseCN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ia - Vbc
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseA);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseBC);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ib - Vca
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseB);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseCA);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ic - Vab
   fasor1 = ExisteFasorCurtoIfas(ind_def, faseC);
   fasor2 = ExisteFasorCurtoVfas(ind_def, faseAB);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoIfas(int ind_def, VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //variáveis locais
   int     vet_fase[] = {faseA, faseB, faseC, faseN};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Ifase";
   //cria fasores das correntes de fase
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para correntes de fase
      if ((fasor = NewObjFasorCurto(ligacao, bar_ref)) == NULL) return(false);
      fasor->tipo_curto = ind_def;
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoIseq(int ind_def, VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //variáveis locais
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Iseq.";
   //cria fasores das correntes de sequencia
   for(int ind_seq = indSEQ_0; ind_seq <= indSEQ_2; ind_seq++)
      {//cria Fasor para corrente de sequencia
      if ((fasor = NewObjFasorCurto(ligacao, bar_ref)) == NULL) return(false);
      fasor->tipo_curto = ind_def;
      fasor->ind_seq    = ind_seq;
      fasor->fases      = faseINV;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoVff(int ind_def, VTBarra *barra)
   {
   //variáveis locais
   int     vet_fase[] = {faseAB, faseBC, faseCA};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Vff";
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para tensão de fase
      if ((fasor = NewObjFasorCurto(barra)) == NULL) return(false);
      fasor->tipo_curto = ind_def;
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoVfn(int ind_def, VTBarra *barra)
   {
   //variáveis locais
   int     vet_fase[]  = {faseAN, faseBN, faseCN};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Vfn";
   //cria fasores das tensões de fase
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para tensão de fase
      if ((fasor = NewObjFasorCurto(barra)) == NULL) return(false);
      fasor->tipo_curto = ind_def;
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorCurtoVseq(int ind_def, VTBarra *barra)
   {
   //variáveis locais
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = NomeDefeito(ind_def) + ": Vseq.";
   //cria fasores das tensões de sequencia
   for(int ind_seq = indSEQ_0; ind_seq <= indSEQ_2; ind_seq++)
      {//cria Fasor para tensão de sequencia
      if ((fasor = NewObjFasorCurto(barra)) == NULL) return(false);
      fasor->tipo_curto = ind_def;
      fasor->ind_seq    = ind_seq;
      fasor->fases      = faseINV;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoAjusteA(void)
   {
   //variáveis locais
   char     classe = 'A';
   VTFasor *fasor;
   VTFasor *fasorI, *fasorV;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Ajuste A";
   //cria fasor I0 - V0
   fasorI = ExisteFasorFluxoIseq(indSEQ_0);
   fasorV = ExisteFasorFluxoVseq(indSEQ_0);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I1 - V1
   fasorI = ExisteFasorFluxoIseq(indSEQ_1);
   fasorV = ExisteFasorFluxoVseq(indSEQ_1);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I2 - V2
   fasorI = ExisteFasorFluxoIseq(indSEQ_2);
   fasorV = ExisteFasorFluxoVseq(indSEQ_2);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
//   //cria fasor Ia - Va
//   fasorI = ExisteFasorFluxoIfas(faseA);
//   fasorV = ExisteFasorFluxoVfas(faseAN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ib - Vb
//   fasorI = ExisteFasorFluxoIfas(faseB);
//   fasorV = ExisteFasorFluxoVfas(faseBN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ic - Vc
//   fasorI = ExisteFasorFluxoIfas(faseC);
//   fasorV = ExisteFasorFluxoVfas(faseCN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
   //cria fasor Ia - Vbc
   fasorI = ExisteFasorFluxoIfas(faseA);
   fasorV = ExisteFasorFluxoVfas(faseBC);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ib - Vca
   fasorI = ExisteFasorFluxoIfas(faseB);
   fasorV = ExisteFasorFluxoVfas(faseCA);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ic - Vab
   fasorI = ExisteFasorFluxoIfas(faseC);
   fasorV = ExisteFasorFluxoVfas(faseAB);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoAjusteB(void)
   {
   //variáveis locais
   char     classe = 'B';
   VTFasor *fasorI, *fasorV;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Ajuste B";
   //cria fasor I0 - V0
   fasorI = ExisteFasorFluxoIseq(indSEQ_0);
   fasorV = ExisteFasorFluxoVseq(indSEQ_0);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I1 - V1
   fasorI = ExisteFasorFluxoIseq(indSEQ_1);
   fasorV = ExisteFasorFluxoVseq(indSEQ_1);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor I2 - V2
   fasorI = ExisteFasorFluxoIseq(indSEQ_2);
   fasorV = ExisteFasorFluxoVseq(indSEQ_2);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
//   //cria fasor Ia - Va
//   fasorI = ExisteFasorFluxoIfas(faseA);
//   fasorV = ExisteFasorFluxoVfas(faseAN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ib - Vb
//   fasorI = ExisteFasorFluxoIfas(faseB);
//   fasorV = ExisteFasorFluxoVfas(faseBN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
//   //cria fasor Ic - Vc
//   fasorI = ExisteFasorFluxoIfas(faseC);
//   fasorV = ExisteFasorFluxoVfas(faseCN);
//   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, ang_ajuste, classe));
   //cria fasor Ia - Vbc
   fasorI = ExisteFasorFluxoIfas(faseA);
   fasorV = ExisteFasorFluxoVfas(faseBC);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ib - Vca
   fasorI = ExisteFasorFluxoIfas(faseB);
   fasorV = ExisteFasorFluxoVfas(faseCA);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));
   //cria fasor Ic - Vab
   fasorI = ExisteFasorFluxoIfas(faseC);
   fasorV = ExisteFasorFluxoVfas(faseAB);
   if ((fasorI != NULL)&&(fasorV != NULL)) grupo->InsereFasor(NewObjFasorAjuste(fasorI, fasorV, classe));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoAngulo(void)
   {
   //variáveis locais
   VTFasor *fasor;
   VTFasor *fasor1, *fasor2;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Ângulos";
   //cria fasor I0 - V0
   fasor1 = ExisteFasorFluxoIseq(indSEQ_0);
   fasor2 = ExisteFasorFluxoVseq(indSEQ_0);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor I1 - V1
   fasor1 = ExisteFasorFluxoIseq(indSEQ_1);
   fasor2 = ExisteFasorFluxoVseq(indSEQ_1);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor I2 - V2
   fasor1 = ExisteFasorFluxoIseq(indSEQ_2);
   fasor2 = ExisteFasorFluxoVseq(indSEQ_2);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ia - Va
   fasor1 = ExisteFasorFluxoIfas(faseA);
   fasor2 = ExisteFasorFluxoVfas(faseAN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ib - Vb
   fasor1 = ExisteFasorFluxoIfas(faseB);
   fasor2 = ExisteFasorFluxoVfas(faseBN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ic - Vc
   fasor1 = ExisteFasorFluxoIfas(faseC);
   fasor2 = ExisteFasorFluxoVfas(faseCN);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ia - Vbc
   fasor1 = ExisteFasorFluxoIfas(faseA);
   fasor2 = ExisteFasorFluxoVfas(faseBC);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ib - Vca
   fasor1 = ExisteFasorFluxoIfas(faseB);
   fasor2 = ExisteFasorFluxoVfas(faseCA);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));
   //cria fasor Ic - Vab
   fasor1 = ExisteFasorFluxoIfas(faseC);
   fasor2 = ExisteFasorFluxoVfas(faseAB);
   if ((fasor1 != NULL)&&(fasor2 != NULL)) grupo->InsereFasor(NewObjFasorAngulo(fasor1, fasor2));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoIfas(VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //variáveis locais
   int     vet_fase[] = {faseA, faseB, faseC, faseN};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Ifase";
   //cria fasores das correntes de fase
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para correntes de fase
      if ((fasor = NewObjFasorFluxo(ligacao, bar_ref)) == NULL) return(false);
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoIseq(VTBarra *bar_ref, VTLigacao *ligacao)
   {
   //variáveis locais
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Iseq";
   //cria fasores das correntes de fase
   for(int ind_seq = indSEQ_0; ind_seq <= indSEQ_2; ind_seq++)
      {//cria Fasor para corrente de sequencia
      if ((fasor = NewObjFasorFluxo(ligacao, bar_ref)) == NULL) return(false);
      fasor->ind_seq    = ind_seq;
      fasor->fases      = faseINV;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoVff(VTBarra *barra)
   {
   //variáveis locais
   int     vet_fase[] = {faseAB, faseBC, faseCA};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Vff";
   //cria fasores das tensões de fase
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para tensão de fase
      if ((fasor = NewObjFasorFluxo(barra)) == NULL) return(false);
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoVfn(VTBarra *barra)
   {
   //variáveis locais
   int     vet_fase[] = {faseAN, faseBN, faseCN};
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Vfn";
   //cria fasores das tensões de fase
   for(int ind_fase = 0; ind_fase < sizeof(vet_fase)/sizeof(int); ind_fase++)
      {//cria Fasor para tensão de fase
      if ((fasor = NewObjFasorFluxo(barra)) == NULL) return(false);
      fasor->fases      = vet_fase[ind_fase];
      fasor->ind_seq    = -1;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::MontaGrupoFasorFluxoVseq(VTBarra *barra)
   {
   //variáveis locais
   VTFasor *fasor;
   VTGrupo *grupo;

   //cria Grupo de fasores
   if ((grupo = NewObjGrupo()) == NULL) return(false);
   //insere Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //define código
   grupo->Codigo = "Fluxo: Vseq";
   //cria fasores das tensões de sequencia
   for(int ind_seq = indSEQ_0; ind_seq <= indSEQ_2; ind_seq++)
      {//cria Fasor para tensão de sequencia
      if ((fasor = NewObjFasorFluxo(barra)) == NULL) return(false);
      fasor->ind_seq    = ind_seq;
      fasor->fases      = faseINV;
      grupo->InsereFasor(fasor);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TFasores::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *vet_def[NUM_MAX_DEF] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};

   return(AnsiString("Curto ") + vet_def[ind_def]);
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::RedefineBarraReferencia(VTBarra *bar_ref)
   {
   //variáveis locais
   TList   *lisFASOR;
   VTGrupo *grupo;
   VTFasor *fasor;

   //loop p/ todos Grupos
   for (int i = 0; i < lisGRUPO->Count; i++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[i];
      lisFASOR = grupo->LisFasor();
      for (int j = 0; j < lisFASOR->Count; j++)
         {
         fasor = (VTFasor*)lisFASOR->Items[j];
         fasor->RedefineBarraReferencia(bar_ref);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::Reinicia(void)
   {
   //limpa lista lisFASOR sem destruir seus objetos
   lisFASOR->Clear();
   //destrói todos Grupos existentes
   LimpaTList(lisGRUPO);
   }

//---------------------------------------------------------------------------
//eof
