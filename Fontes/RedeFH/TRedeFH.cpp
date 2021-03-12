//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRedeFH.h"
#include "VTBarFH.h"
#include "VTLigFH.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeFH
//-----------------------------------------------------------------------------
VTRedeFH* __fastcall NewObjRedeFH(VTApl *apl)
   {
   return(new TRedeFH(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TRedeFH::TRedeFH(VTApl *apl)
   {
   // Inicialização
   PD.num_pat              = 0;
   PD.num_harmonicas       = 0;
   PD.vet_ordem_harmonicas = NULL;

   // Salva dados
   this->apl = apl;

   //cria listas
   lisBAR_FH    = new TList();
   lisLIG_FH    = new TList();
   lisBAR_FH_NO = new TList();
   //cria lista de LigFH dos Trechos fictícos das Reducoes
   ficticia.lisLIG_FH  = new TList();
   }

//-----------------------------------------------------------------------------
__fastcall TRedeFH::~TRedeFH(void)
   {
   // Libera memória alocada a 'vet_ordem'
   delete[] PD.vet_ordem_harmonicas;

   //destrói listas sem destruir seus objetos
   if (lisBAR_FH_NO) {delete lisBAR_FH_NO; lisBAR_FH_NO = NULL;}
   //destrói listas e seus objetos
   if (lisBAR_FH) {LimpaTList(lisBAR_FH); delete lisBAR_FH; lisBAR_FH = NULL;}
   if (lisLIG_FH) {LimpaTList(lisLIG_FH); delete lisLIG_FH; lisLIG_FH = NULL;}
   //destrói lista e seus objetos
   if (ficticia.lisLIG_FH) {LimpaTList(ficticia.lisLIG_FH); delete ficticia.lisLIG_FH; ficticia.lisLIG_FH = NULL;}
   }

//-----------------------------------------------------------------------------
VTBarFH* __fastcall TRedeFH::ExisteBarFH(VTEqpto *eqpto)
   {
   //variáveis locais
   VTBarFH *barFH;

   //loop p/ todas BarFH
   for (int n = 0; n < lisBAR_FH->Count; n++)
      {
      barFH = (VTBarFH*)lisBAR_FH->Items[n];
      if(barFH->Barra == eqpto) return(barFH);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTLigFH* __fastcall TRedeFH::ExisteLigFH(VTEqpto *eqpto)
   {
   //variáveis locais
   VTLigFH *ligFH;

   //loop p/ todas BarFH
   for (int n = 0; n < lisLIG_FH->Count; n++)
      {
      ligFH = (VTLigFH*)lisLIG_FH->Items[n];
      if(ligFH->Ligacao == eqpto) return(ligFH);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTLigFH* __fastcall TRedeFH::ExisteLigFH_Ficticia(VTLigacao *ligacao)
   {
   //variáveis locais
   VTLigFH *ligFH;

   //procura LigFH da Ligacao fictícia
   for (int n = 0; n < ficticia.lisLIG_FH->Count; n++)
      {
      ligFH = (VTLigFH*)ficticia.lisLIG_FH->Items[n];
      if (ligFH->Ligacao == ligacao) return(ligFH);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedeFH::Inicia(VTRedes *redes,
                                VTNet   *net,
                                int     num_pat,
                                int     num_harmonicas,
                                int     *vet_ordem_harmonicas)
   {
   // Variáveis locais
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));

   //salva ponteiros para objetos
   this->net = net;
   // Libera memória alocada anteriormente
   delete[] PD.vet_ordem_harmonicas;
   // Salva contadores, aloca memória para vetor de ordens harmônicas
   // e salva ordens fornecidas
   PD.num_pat              = num_pat;
   PD.num_harmonicas       = num_harmonicas;
   PD.vet_ordem_harmonicas = new int [num_harmonicas];
   for(int nh=0; nh < num_harmonicas; nh++)
      {
      PD.vet_ordem_harmonicas[nh] = vet_ordem_harmonicas[nh];
      }

   // Cria objetos TBarFH
   IniciaLisBarFH(consulta->LisEqptoRedeReducao(redes, eqptoBARRA));

   // Cria objetos TLigFH
   IniciaLisLigFH(consulta->LisEqptoRedeReducao(redes, eqptoLIGACAO));

   // Cria objetos TLigFH p/ as Ligacoes fictícias das Reducoes
   IniciaLisLigFH_Ficticia(redes);

   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeFH::IniciaLisBarFH(TList *lisBAR)
   {
   //variáveis locais
   VTBarra *barra;

   //destrói objetos criados anteriormente
   LimpaTList(lisBAR_FH);
   try{//loop p/ todas Barras
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         //cria objeto BarFH e insere em lisBAR_FH
         lisBAR_FH->Add(NewObjBarFH(barra, PD.num_pat, PD.num_harmonicas));
         }
      }catch (Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeFH::IniciaLisLigFH(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //destrói objetos criados anteriormente
   LimpaTList(lisLIG_FH);
   try{//loop p/ todas Ligacoes
      for (int n = 0; n < lisLIG->Count; n++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[n];
         //cria objeto LigFH e insere em lisLIG_FH
         lisLIG_FH->Add(NewObjLigFH(ligacao, PD.num_pat, PD.num_harmonicas));
         }
      }catch (Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeFH::IniciaLisLigFH_Ficticia(VTRedes *redes)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisMR;
   TList     *lisREDU;
   TList     *lisLIG_FIC;

   //loop p/ todas Redes carregadas
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //descarta Rede carregada
      if (mrede->Carregada) continue;
      //loop p/ todas Reducoes da Rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém lista de Ligacoes do esquemático da Reducao
         lisLIG_FIC = reducao->LisLigacaoEsquematico();
         //cria objetos LigFH
         for (int nl = 0; nl < lisLIG_FIC->Count; nl++)
            {
            ligacao = (VTLigacao*)lisLIG_FIC->Items[nl];
            ficticia.lisLIG_FH->Add(NewObjLigFH(ligacao, PD.num_pat, PD.num_harmonicas));
            }
         }
      }
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeFH::LisBarFH(void)
   {
   return(lisBAR_FH);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeFH::LisBarFH(VTNo *no)
   {
   //variáveis locais
   VTBarFH *barFH;
   VTEqpto *eqpto;

   //reinicia lisBAR_FH_NO
   lisBAR_FH_NO->Clear();
   //loop p/ todas Barras do Nó
   for (int nb = 0; nb < no->lisBAR->Count; nb++)
      {//determina BarFH da Barra
      eqpto = (VTEqpto*)no->lisBAR->Items[nb];
      if ((barFH = ExisteBarFH(eqpto)) == NULL) continue;
      //insere barFH em lisBAR_FH_NO
      if (lisBAR_FH_NO->IndexOf(barFH) < 0) lisBAR_FH_NO->Add(barFH);
      }
   return(lisBAR_FH_NO);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeFH::LisLigFH(void)
   {
   return(lisLIG_FH);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeFH::LisLigFH_Ficticia(void)
   {
   return(ficticia.lisLIG_FH);
   }

//---------------------------------------------------------------------------
int __fastcall TRedeFH::PM_GetOrdemHarmonica(int ind_freq)
   {
   if((ind_freq < 0) || (ind_freq >= PD.num_harmonicas)) return(-1);
   return(PD.vet_ordem_harmonicas[ind_freq]);
   }

//---------------------------------------------------------------------------
int __fastcall TRedeFH::PM_GetNumHarmonicas(void)
   {
   return(PD.num_harmonicas);
   }

//---------------------------------------------------------------------------
void __fastcall TRedeFH::PM_SetOrdemHarmonica(int ind_freq, int ordem)
   {
   if((ind_freq < 0) || (ind_freq >= PD.num_harmonicas)) return;
   if(ordem <= 0) return;
   PD.vet_ordem_harmonicas[ind_freq] = ordem;
   }

//---------------------------------------------------------------------------
void __fastcall TRedeFH::PM_SetNumHarmonicas(int num_harmonicas)
   {
   if((num_harmonicas < 0) || (num_harmonicas > PD.num_harmonicas)) return;
   PD.num_harmonicas = num_harmonicas;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeFH::ReiniciaResultados(void)
   {
   //variáveis locais
   VTBarFH *barFH;
   VTLigFH *ligFH;

   //loop p/ todas LigFH em lisLIG_FH
   for (int n = 0; n < lisLIG_FH->Count; n++)
      {
      ligFH = (VTLigFH*)lisLIG_FH->Items[n];
      ligFH->ReiniciaValores();
      }
   //loop p/ todas LigFH em ficticia.lisLIG_FH
   for (int n = 0; n < ficticia.lisLIG_FH->Count; n++)
      {
      ligFH = (VTLigFH*)ficticia.lisLIG_FH->Items[n];
      ligFH->ReiniciaValores();
      }
   //loop p/ todas BarFH
   for (int n = 0; n < lisBAR_FH->Count; n++)
      {
      barFH = (VTBarFH*)lisBAR_FH->Items[n];
      barFH->ReiniciaValores();
      }
   }

//---------------------------------------------------------------------------
//eof