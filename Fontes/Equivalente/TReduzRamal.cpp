//---------------------------------------------------------------------------
#pragma hdrstop
#include "TReduzRamal.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Flow\VTFlow.h"
//#include "..\Grafico\VTGrafico.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TReduzRamal * __fastcall NewObjReduzRamal(VTApl *apl_owner)
   {
   return(new TReduzRamal(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TReduzRamal::TReduzRamal(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria obejtos locais
   lisBAR_ORD  = new TList();
   lisLIG_ORD  = new TList();
   lisCHV_WISE = new TList();
   lisCAP      = new TList();
   lisEQP      = new TList();
   radial      = DLL_NewObjRadial(apl);
   }

//---------------------------------------------------------------------------
__fastcall TReduzRamal::~TReduzRamal(void)
   {
   //destrói objetos
   if (radial) {delete radial; radial = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)      {delete lisEQP;      lisEQP      = NULL;}
   if (lisCAP)      {delete lisCAP;      lisCAP      = NULL;}
   if (lisBAR_ORD)  {delete lisBAR_ORD;  lisBAR_ORD  = NULL;}
   if (lisLIG_ORD)  {delete lisLIG_ORD;  lisLIG_ORD  = NULL;}
   if (lisCHV_WISE) {delete lisCHV_WISE; lisCHV_WISE = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::Executa(void)
   {
   //variáveis locais
   TList      *lisPRIMARIO;
   VTPrimario *primario;

   try{//inicia objetos
      classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
      demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
      flow      = (VTFlow*)apl->GetObject(__classid(VTFlow));
      //grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
      redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
      topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
      //excuta fluxo de potência, se necessário
      flow->TipoFluxo        = fluxoDESEQUILIBRADO;
      flow->ProgressoEnabled = false;
      if (! flow->CalculaPorRede(redes)) return(false);
      //inicia objeto Radial
      if (! radial->Inicia(redes)) return(false);
      //inicia lisCAP com todos os Capacitores
      IniciaLisCapacitor();
      //inicia lisCHV com todas Chaves relevantes para o Wise
      IniciaLisChaveWise();
      //loop para todos Primarios
      lisPRIMARIO = radial->LisPrimario();
      for (int n = 0; n < lisPRIMARIO->Count; n++)
         {
         primario = (VTPrimario*)lisPRIMARIO->Items[n];
         //identifica conjunto de Ligacoes que devem permanecer no Primario
         Executa(primario);
         }
      //reconfigura Redes
      topologia->Reconfigura(redes);
      //descarta blocos isolados
      redes->ClearEqptosIsolados();
      //reinicia Grafico
      //grafico->AreaRedeDefine();
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::Executa(VTPrimario *primario)
   {
   //variáveis locais
   VTBarra     *barra;
   VTCapacitor *capacitor;
   VTChave     *chave;
   VTLigacao   *ligacao;

   //ordena Ligacoes e Barras do Primario
   primario->Ordena(lisBAR_ORD, lisLIG_ORD);
   //marca todas Barras como não selecionadas
   for (int n = 0; n < lisBAR_ORD->Count; n++)
      {
      barra = (VTBarra*)lisBAR_ORD->Items[n];
      barra->Tag = 0;
      }
   //marca todas Ligacoes como não selecionadas
   for (int n = 0; n < lisLIG_ORD->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG_ORD->Items[n];
      ligacao->Tag = 0;
      }
   //loop p/ todos Capacitores em lisCAP
   for (int nc = 0; nc < lisCAP->Count; nc++)
      {
      capacitor = (VTCapacitor*)lisCAP->Items[nc];
      //marca a Barra do Capacitor como selecionada
      capacitor->pbarra->Tag = 1;
      }
   //loop p/ todas as Chaves relevantes para o Wise
   for (int nc = 0; nc < lisCHV_WISE->Count; nc++)
      {
      chave = (VTChave*)lisCHV_WISE->Items[nc];
      //marca as Barras da Chave como selecionadas
      chave->pbarra1->Tag = 1;
      chave->pbarra2->Tag = 1;
      //verifica se a Chave pertence a este Primario
      if (chave->rede == primario->Rede)
         {//marca que a Chave deve permanecer
         chave->Tag = 1;
         }
      }
   //loop para todas Ligacoes
   for (int n = 0; n < lisLIG_ORD->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG_ORD->Items[n];
      //verifica se Ligacao conecta alguma Barra marcada
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         //verifica Tag da Barra
         if (barra->Tag > 0)
            {//marca Ligacao como selecionada
            ligacao->Tag = 1;
            break;
            }
         }
      }
   //identifica conjunto de Ligacoes que devem permanecer no Primario
   for (int nl = 0; nl < lisLIG_ORD->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG_ORD->Items[nl];
      if (ligacao->Tag == 1)
         {
         while ((ligacao = ligacao->ligaPai) != NULL)
            {
            ligacao->Tag = 1;
            }
         }
      }
   //cria Cargas equivalentes nas Barras onde uma Ligacao que será descartada
   //conecta uma Ligacao que irá permanecer
   for (int nl = 0; nl < lisLIG_ORD->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG_ORD->Items[nl];
      if (ligacao->Tag == 0)
         {
         if ((ligacao->ligaPai != NULL) && (ligacao->ligaPai->Tag == 1))
            {
            barra = ligacao->BarraComum(ligacao->ligaPai);
            InsereCargaEquivalente(barra, ligacao);
            }
         }
      }
   //remove todas Ligacoes do Primario que não foram selecionadas
   RemoveLigacoesNaoSelecionadas(primario);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::IniciaLisCapacitor(void)
   {
   //variáveis locais
   TList     *lisPRIMARIO;
   VTPrimario *primario;

   //reinicia lisCAP
   lisCAP->Clear();
   //loop para todos Primarios
   lisPRIMARIO = radial->LisPrimario();
   for (int np = 0; np < lisPRIMARIO->Count; np++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[np];
      //obtém todos os Capacitore do Primário
      primario->Rede->LisEqbar(lisCAP, eqptoCAPACITOR);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::IniciaLisChaveWise(void)
   {
   //variáveis locais
   TList     *lisLIGACAO;
   TList     *lisPRIMARIO;
   VTChave   *chave;
   VTLigacao *ligacao;
   VTPrimario *primario;

   //reinicia lisCH
   lisCHV_WISE->Clear();
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
               if (lisCHV_WISE->IndexOf(chave) < 0) lisCHV_WISE->Add(chave);
               }
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::InsereCargaEquivalente(VTBarra *barra, VTLigacao *ligacao)
   {
   //variáveis locais
   VTCarga         *carga;
   VTClasse        *classe;
   VTCurva         *curva;
   complex<double> vetor_s_mva[MAX_FASE];
   int             ind_bar = ligacao->IndiceBarra(barra);
   int             num_pat = ligacao->resflow->Max_pat;

   //dfine Classe da Carga como equivalente
   if ((classe = classes->ExisteClasseTag(consEQUIVALENTE)) == NULL) return(false);
   //cria uma nova Carga e uma Curva para a Carga
   if ((carga = DLL_NewObjCarga(num_pat)) == NULL) return(false);
   //cria uma nova Curva
   if((curva = DLL_NewObjCurva()) == NULL)
      {
      DLL_DeleteEqpto(carga);
      return(false);
      }
   //define Classe da Carga
   carga->DefineClasse(classe);
   //associa Curva c/ a Carga
   carga->DefineCurva(curva);
   //insere Carga na Barra
   barra->InsereEqbar(carga);
   //inicia dados da Carga
   carga->Id          = FalsoId();
   carga->Codigo      = "EqvWise";
   carga->Reducao     = false;
   carga->Icte_pu     = 1.;
   carga->Fases       = ligacao->Fases(barra);
   carga->esq.posbar  = 50;
   carga->esq.dxbar   = 4000;
   carga->esq.dybar  = 4000;
   //inicia dados da Curva
   curva->Codigo        = carga->Codigo;
   curva->Id            = carga->Id;    //MUITO IMPORTANTE
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = num_pat;
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRCAR;  //{p,q} por fase
   //atualiza Curva da Carga p/ todos os patamares
   for (int np = 0; np < num_pat; np++)
      {//loop p/ todas as fases da Ligacao
      for (int nf = 0; nf < MAX_FASE; nf++)
         {
         vetor_s_mva[nf]  = ligacao->resflow->Sf_mva[ind_bar][np][nf];
         }
      //atualiza demanda da Carga no patamar
      demanda->Set_S3f_mva(carga, np, vetor_s_mva);
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::MarcaLigacoesInOut(TList *lisLIGACAO)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //marca todas Ligacoes como não selecionadas
   for (int n = 0; n < lisLIGACAO->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[n];
      ligacao->Tag = 0;
      }
   //loop p/ todas Chaves em lisCHAVE e suas Ligacoes a montante
   for (int n = 0; n < lisCHAVE->Count; n++)
      {
      ligacao = (VTLigacao*)lisCHAVE->Items[n];
      ligacao->Tag = 1;
      while ((ligacao = ligacao->ligaPai) != NULL)
         {
         ligacao->Tag = 1;
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TReduzRamal::RemoveLigacoesNaoSelecionadas(VTPrimario *primario)
   {
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisLIGACAO = primario->Rede->LisLigacao();

   //marca todas Ligacoes como não selecionadas
   for (int n = lisLIGACAO->Count - 1; n >= 0; n--)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[n];
      if (ligacao->Tag == 0)
         {
         primario->Rede->RemoveLigacao(ligacao);
         DLL_DeleteEqpto(ligacao);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

