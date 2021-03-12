//---------------------------------------------------------------------------
#pragma hdrstop
#include "TReduzSecao.h"
#include "..\Apl\VTApl.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
//#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TReduzSecao * __fastcall NewObjReduzSecao(VTApl *apl_owner)
   {
   return(new TReduzSecao(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TReduzSecao::TReduzSecao(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria obejtos locais
   lisBAR_FIX  = new TList();
   lisCHV_FIX = new TList();
   lisBAR_ORD  = new TList();
   lisLIG_ORD  = new TList();
   lisEQP      = new TList();
   lisSECAO    = new TList();
   radial      = DLL_NewObjRadial(apl);
   }

//---------------------------------------------------------------------------
__fastcall TReduzSecao::~TReduzSecao(void)
   {
   //destrói objetos
   if (radial) {delete radial; radial = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisBAR_FIX)  {delete lisBAR_FIX;  lisBAR_FIX  = NULL;}
   if (lisCHV_FIX) {delete lisCHV_FIX; lisCHV_FIX = NULL;}
   if (lisBAR_ORD)  {delete lisBAR_ORD;  lisBAR_ORD  = NULL;}
   if (lisLIG_ORD)  {delete lisLIG_ORD;  lisLIG_ORD  = NULL;}
   if (lisEQP)      {delete lisEQP;      lisEQP      = NULL;}
   //destrói lista e seus objetos
   if (lisSECAO) {LimpaTList(lisSECAO); delete lisSECAO; lisSECAO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSecao::Executa(void)
   {
   //variáveis locais
   TList      *lisPRIMARIO;
   VTPrimario *primario;

   try{//inicia objetos
      demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
      redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
      //inicia objeto Radial
      if (! radial->Inicia(redes)) return(false);
      //inicia lisCHV_FIX com todas Chaves que devem permanecer na rede
      IniciaLisChaveFixa();
      //inicia lisBAR_FIX com todas Barras que devem permanecer na rede
      IniciaLisBarraFixa();
      //marca todas as Barras fixas como selecionadas e as demais como não selecionadas
      MarcaBarraComoFixa_NaoFixa();
      //loop para todos Primarios
      lisPRIMARIO = radial->LisPrimario();
      for (int n = 0; n < lisPRIMARIO->Count; n++)
         {
         primario = (VTPrimario*)lisPRIMARIO->Items[n];
         //identifica conjunto de Ligacoes que devem permanecer no Primario
         Executa(primario);
         }
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSecao::Executa(VTPrimario *primario)
   {
   //variáveis locais
   int       count;
   VTBarra   *barra;
   VTChave   *chave;
   VTLigacao *ligacao;

   //reinicia lista de Secoes
   LimpaTList(lisSECAO);
   //ordena Ligacoes e Barras do Primario
   primario->Ordena(lisBAR_ORD, lisLIG_ORD);

//PAREI_AQUI

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::IniciaLisBarraFixa(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTLigacao *ligacao;

   //limpa lista lisBAR_FIX
   lisBAR_FIX->Clear();
   //seleciona Barras dos Eqptos que devem permanecer na rede
   SelecionaBarraChaveFixa();
   SelecionaBarraEqbar(eqptoSUPRIMENTO);
   SelecionaBarraEqbar(eqptoGERADOR);
   SelecionaBarraEqbar(eqptoCAPACITOR);
   SelecionaBarraComBifurcacao();
   //SelecionaBarraLigacoesComFasesDistintas();
   SelecionaBarraLigacao(eqptoREGULADOR);
   SelecionaBarraLigacao(eqptoTRAFO);
   SelecionaBarraTrechoConectaChave();
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::IniciaLisChaveFixa(void)
   {
   //variáveis locais
   TList     *lisLIGACAO;
   TList     *lisPRIMARIO;
   VTChave   *chave;
   VTLigacao *ligacao;
   VTPrimario *primario;

   //limpa lista lisCHV_FIX
   lisCHV_FIX->Clear();
   //loop para todos Primarios
   lisPRIMARIO = radial->LisPrimario();
   for (int np = 0; np < lisPRIMARIO->Count; np++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[np];
      lisLIGACAO = primario->Rede->LisLigacao();
      for (int nl = 0; nl < lisLIGACAO->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
         if (ligacao->Tipo() == eqptoCHAVE)
            {
            chave = (VTChave*)ligacao;
            if (chave->TipoDisjuntor || chave->TipoReligadora)
               {//inclui Chave em lisCHV
               if (lisCHV_FIX->IndexOf(chave) < 0) lisCHV_FIX->Add(chave);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSecao::LigacaoConectaBarraFixa(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      //verifica Tag da Barra
      if (barra->Tag > 0) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::MarcaBarraComoFixa_NaoFixa(void)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBARRA = redes->LisBarra();

   //marca todas Barras da redes como não selecionadas
   for (int n = 0; n < lisBARRA->Count; n++)
      {
      barra = (VTBarra*)lisBARRA->Items[n];
      barra->Tag = 0;
      }
   //marca todas Barras fixas como selecionadas
   for (int n = 0; n < lisBAR_FIX->Count; n++)
      {
      barra = (VTBarra*)lisBAR_FIX->Items[n];
      barra->Tag = 1;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraChaveFixa(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;

   //obtém lista de Geradores e Suprimentos da rede
   for (int n = 0; n < lisCHV_FIX->Count; n++)
      {
      chave = (VTChave*)lisCHV_FIX->Items[n];
      //insere Barras da Chave em lisBAR_FIX
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = chave->Barra(ind_bar)) == NULL) continue;
         if (lisBAR_FIX->IndexOf(barra) < 0)
            {//insere Barra do Eqbar em lisBAR_FIX
            lisBAR_FIX->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraComBifurcacao(void)
   {
   //variáveis locais
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisREDE;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia campo Tag de todas as Barras com valor 0
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->Tag = 0;
      }
   //loop para todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //loop p/ todas Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //loop p/ todas Barras da ligacao
         for (int ind_bar=0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            //incrementa valor de Tag da Barra
            barra->Tag++;
            }
         }
      }
   //seleciona todas as Barras com Tag maior que 2
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Tag > 2)
         {
         if (lisBAR_FIX->IndexOf(barra) < 0)
            {//insere Barra do Eqbar em lisBAR_FIX
            lisBAR_FIX->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraEqbar(int tipo_eqbar)
   {
   //variáveis locais
   VTEqbar *eqbar;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Eqbar da rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, tipo_eqbar);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQP->Items[n];
      if (lisBAR_FIX->IndexOf(eqbar->pbarra) < 0)
         {//insere Barra do Eqbar em lisBAR_FIX
         lisBAR_FIX->Add(eqbar->pbarra);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraLigacao(int tipo_ligacao)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Ligacoes
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, tipo_ligacao);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[n];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         if (lisBAR_FIX->IndexOf(barra) < 0)
            {//insere Barra do Eqbar em lisBAR_FIX
            lisBAR_FIX->Add(barra);
            }
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraLigacoesComFasesDistintas(void)
   {
   //variáveis locais
   int       fases_ref, fases_lig;
   TList     *lisLIG_NET;
   TList     *lisNO_NET;
   VTBarra   *barra;
   VTNet     *net;
   VTNo      *no;
   VTLigacao *ligacao;
   VTOrdena  *ordena;
   VTRedes   *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria objeto VTOrdena
   ordena  = DLL_NewObjOrdena(apl);
   //cria objeto Net, sem Tramos
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   if ((net = ordena->Executa(redes)) == NULL)
      {
      delete ordena;
      return;
      }
   //obtém lista de Nos
   lisNO_NET  = net->LisNo();
   lisLIG_NET = net->LisLigacao();
   //loop p/ todos Nos
   for (int n = 0; n < lisNO_NET->Count; n++)
      {
      no = (VTNo*)lisNO_NET->Items[n];
      //inicia fases de referência e fases de uma ligacao
      fases_ref = fases_lig = faseINV;
      //obtém lista de Ligacoes que conectam as Barras do No
      for (int nb = 0; (fases_ref == fases_lig) && (nb < no->lisBAR->Count); nb++)
         {
         barra = (VTBarra*)no->lisBAR->Items[nb];
         //analisa fases das Ligacoes que conectam o No
         for (int nl = 0; (fases_ref == fases_lig) && (nl < lisLIG_NET->Count); nl++)
            {
            ligacao = (VTLigacao*)lisLIG_NET->Items[nl];
            if (ligacao->ConectaBarra(barra))
               {
               if (fases_ref == faseINV)
                  {
                  fases_ref = fases_lig = ligacao->Fases(barra);
                  }
               else
                  {
                  fases_lig = ligacao->Fases(barra);
                  }
               }
            }

         }
      //verifica se há diferenças entre as fases das ligacações
      if ((fases_ref != fases_lig))
         {//insere Barras do No em lisBAR_FIX
         for (int nb = 0; nb < no->lisBAR->Count; nb++)
            {
            barra = (VTBarra*)no->lisBAR->Items[nb];
            if (lisBAR_FIX->IndexOf(barra) < 0) lisBAR_FIX->Add(barra);
            }
         }
      }
   //destrói objetos criados
   delete net;
   delete ordena;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TReduzSecao::SelecionaBarraTrechoConectaChave(void)
   {
   //variáveis locais
   bool      incluir;
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisREDE;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia campo Tag de todas as Barras com valor 0
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->Tag = 0;
      }
   //loop para todas Chaves em lisCHV
   for (int nl = 0; nl < lisCHV_FIX->Count; nl++)
      {
      ligacao = (VTLigacao*)lisCHV_FIX->Items[nl];
      //incrementa valor de Tag das Barras da Chave
      ligacao->pbarra1->Tag++;
      ligacao->pbarra2->Tag++;
      }
   //loop para todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //loop p/ todas Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao conecta alguma Barra com Tag não nulo
         incluir = false;
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            //verifica Tag da Barra
            if (barra->Tag > 0)
               {
               incluir = true;
               break;
               }
            }
         if (! incluir) continue;
         //inclui Barras da Ligacao em lisBAR_FIX
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            if (lisBAR_FIX->IndexOf(barra) < 0)
               {//insere Barra do Eqbar em lisBAR_FIX
               lisBAR_FIX->Add(barra);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof

