//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRedeCC.h"
#include "ResCurto.h"
#include "VTBarCC.h"
#include "VTLigCC.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeCC
//-----------------------------------------------------------------------------
VTRedeCC* __fastcall NewObjRedeCC(VTApl *apl)
   {
   return(new TRedeCC(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TRedeCC::TRedeCC(VTApl *apl)
   {
   //variáveis locais
   int num_pat = 24;

   //salva ponteiro dos objetos
   this->apl = apl;
   //cria BarCC para salvar correntes de defeito (NÃO ASSOCIADO a nenhuma barra)
   corrente.barCC = NewObjBarCC(NULL);
   //cria Barra e Trechos fictícios usados p/ defeito em Trecho
   troca.bar_fic  = DLL_NewObjBarra(num_pat);
   troca.tre_fic1 = DLL_NewObjTrecho(num_pat);
   troca.tre_fic2 = DLL_NewObjTrecho(num_pat);
   //cria BarCC p/ Barra fictícia
   troca.barCC_fic = NewObjBarCC(troca.bar_fic);
   //cria LigCC p/ Trechos fictícios
   troca.ligCC_fic1 = NewObjLigCC(troca.tre_fic1);
   troca.ligCC_fic2 = NewObjLigCC(troca.tre_fic2);
   //cria listas
   lisGS      = new TList();
   lisZT      = new TList();
   lisBAR     = new TList();
   lisCAR     = new TList();
   lisLIG     = new TList();
   lisMUT     = new TList();
   lisBAR_CC  = new TList();
   lisLIG_CC  = new TList();
   //cria lista de LigCC dos Trechos fictícos das Reducoes
   ficticia.lisLIG_CC  = new TList();
   //define defeito a ser usado p/ animação gráfica
   ShowDefeito = defeito3F;
   }

//-----------------------------------------------------------------------------
__fastcall TRedeCC::~TRedeCC(void)
   {
   //se necessário, restaura RedeCC original
   RestauraRedeCC();
   //destrói objeto BarCC usada p/ saslvar correntes de defeito
   if (corrente.barCC)   {delete corrente.barCC;   corrente.barCC   = NULL;}
   //destrói objetos usados p/ defeito em um Trecho
   if (troca.ligCC_fic1) {delete troca.ligCC_fic1; troca.ligCC_fic1 = NULL;}
   if (troca.ligCC_fic2) {delete troca.ligCC_fic2; troca.ligCC_fic2 = NULL;}
   if (troca.barCC_fic)  {delete troca.barCC_fic;  troca.barCC_fic  = NULL;}
   if (troca.tre_fic1)   {delete troca.tre_fic1;   troca.tre_fic1   = NULL;}
   if (troca.tre_fic2)   {delete troca.tre_fic2;   troca.tre_fic2   = NULL;}
   if (troca.bar_fic)    {delete troca.bar_fic;    troca.bar_fic    = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisZT)  {delete lisZT;  lisZT  = NULL;}
   if (lisMUT) {delete lisMUT; lisMUT = NULL;}
   if (lisGS)  {delete lisGS;  lisGS  = NULL;}
   //destrói listas e seus objetos
   if (lisBAR_CC) {LimpaTList(lisBAR_CC); delete lisBAR_CC; lisBAR_CC = NULL;}
   if (lisLIG_CC) {LimpaTList(lisLIG_CC); delete lisLIG_CC; lisLIG_CC = NULL;}
   //destrói lista e seus objetos
   if (ficticia.lisLIG_CC) {LimpaTList(ficticia.lisLIG_CC); delete ficticia.lisLIG_CC; ficticia.lisLIG_CC = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::AjustaBarraFicticia(double dist_bar1_km)
   {
   //variáveis locais
   double  fat_mult;
   int     x1, y1, x2, y2;

   //determina coordenadas do Trecho original p/ o esquemático
   troca.tre_orig->CoordenadasEsquematico(x1, y1, x2, y2);
   //calcula fat_mult p/ calcular coordenadas da Barra p/ o diag.esquemático
   if (troca.tre_orig->Comprimento_m < 1.) fat_mult = 0.5;
   else                                    fat_mult = dist_bar1_km / troca.tre_orig->Comprimento_km;
   //define atributos
   //troca.bar_fic->redes = troca.tre_orig->pbarra1->redes;
   troca.bar_fic->zona  = troca.tre_orig->pbarra1->zona;
   troca.bar_fic->vnom  = troca.tre_orig->pbarra1->vnom;
   //define coordenadas da Barra  p/ o esquemático
   troca.bar_fic->esq.x1 = x1 + ((x2 - x1) * fat_mult);
   troca.bar_fic->esq.y1 = y1 + ((y2 - y1) * fat_mult);
   troca.bar_fic->esq.x2 = troca.bar_fic->esq.x1;
   troca.bar_fic->esq.y2 = troca.bar_fic->esq.y1;
   //define coordenadas da Barra p/georreferenciado (DVK 2017.05.29)
   if(troca.tre_orig->CoordenadasUtm_cm(x1, y1, x2, y2))
      {
      troca.bar_fic->utm.x = x1 + ((x2 - x1) * fat_mult);
      troca.bar_fic->utm.y = y1 + ((y2 - y1) * fat_mult);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::AjustaTrechoFicticio(double dist_bar1_km)
   {
   //variáveis locais
   double  fat_mult;

   //calcula fat_mult p/ corrigir coordenadas do esquemático da Barra
   if   (troca.tre_orig->Comprimento_m < 1.) fat_mult = 0.5;
   else                                      fat_mult = dist_bar1_km / troca.tre_orig->Comprimento_km;
   //redefine atributos do Trecho fictício 1
   troca.tre_fic1->rede           = troca.tre_orig->rede;
   troca.tre_fic1->arranjo        = troca.tre_orig->arranjo;
   troca.tre_fic1->pbarra1        = troca.tre_orig->pbarra1;
   troca.tre_fic1->pbarra2        = troca.bar_fic;
   troca.tre_fic1->Codigo         = troca.tre_orig->Codigo + "/1";
   troca.tre_fic1->Comprimento_km = troca.tre_orig->Comprimento_km * fat_mult;
   troca.tre_fic1->esq.posbar1    = troca.tre_orig->esq.posbar1;
   troca.tre_fic1->esq.posbar2    = 50.;
   //redefine atributos do Trecho fictício 2
   troca.tre_fic2->rede           = troca.tre_orig->rede;
   troca.tre_fic2->arranjo        = troca.tre_orig->arranjo;
   troca.tre_fic2->pbarra1        = troca.bar_fic;
   troca.tre_fic2->pbarra2        = troca.tre_orig->pbarra2;
   troca.tre_fic2->Codigo         = troca.tre_orig->Codigo + "/2";
   troca.tre_fic2->Comprimento_km = troca.tre_orig->Comprimento_km - troca.tre_fic1->Comprimento_km;
   troca.tre_fic2->esq.posbar1    = 50.;
   troca.tre_fic2->esq.posbar2    = troca.tre_orig->esq.posbar2;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::AssociaBarraBarCC(void)
   {
   //variáveis locais
   VTBarCC *pbcc;

   //loop p/ todas BarCC
   for (int n = 0; n < lisBAR_CC->Count; n++)
      {
      pbcc = (VTBarCC*)lisBAR_CC->Items[n];
      if (pbcc->Barra) pbcc->Barra->Obj = pbcc;
      }
   //BarCC da barra fictícia para defeitos em trecho
   if (troca.barCC_fic->Barra) troca.barCC_fic->Barra->Obj = troca.barCC_fic;
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TRedeCC::BarraDefeito(void)
   {
   //proteção
   if (defeito.barCC != NULL) return(defeito.barCC->Barra);
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTBarCC* __fastcall TRedeCC::BarCCDefeito(void)
   {
   return(defeito.barCC);
   }

//-----------------------------------------------------------------------------
VTBarCC* __fastcall TRedeCC::CorrenteDefeito(void)
   {
   return(corrente.barCC);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::DefineBarraDefeito(VTBarra *barra)
   {
   //se necessário, restaura RedeCC original
   RestauraRedeCC();
   //salva Eqpto com defeito e define BarCC de defeito igual ao da própria barra
   defeito.eqpto = barra;
   defeito.barCC = (VTBarCC*)(barra->Obj);
   //reinicia valores de BarCC e LigCC
   ReiniciaResultados();
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::DefineTrechoDefeito(VTTrecho *trecho, double dist_bar1_km)
   {
   //se necessário, restaura RedeCC original
   RestauraRedeCC();
   //salva Eqpto com defeito e define BarCC de defeito igual ao da Barra fictícia
   defeito.eqpto = trecho;
   defeito.barCC = troca.barCC_fic;
   //salva Trecho original do defeito
   troca.tre_orig = trecho;
   //redefine nome da Barra fictícia c/ nome do Trecho
   troca.bar_fic->Codigo = trecho->Codigo + ":" + IntToStr(int(dist_bar1_km * 1000));
   //reconfigura RedeCC
   ReconfiguraRedeCC();
   //ajusta atributos da Barra ficticia
   AjustaBarraFicticia(dist_bar1_km);
   //ajusta atributos dos Trechos ficticios
   AjustaTrechoFicticio(dist_bar1_km);
   //reinicia valores de BarCC e LigCC
   ReiniciaResultados();
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TRedeCC::EqptoDefeito(void)
   {
   return(defeito.eqpto);
   }

//-----------------------------------------------------------------------------
VTBarCC* __fastcall TRedeCC::ExisteBarCC(VTBarra *barra)
   {
   //variáveis locais
   VTBarCC *barCC;

   for (int n = 0; n < lisBAR_CC->Count; n++)
      {
      barCC = (VTBarCC*)lisBAR_CC->Items[n];
      //cria objeto BarCC e insere em lisBAR_CC
      if (barCC->Barra == barra) return(barCC);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTLigCC* __fastcall TRedeCC::ExisteLigCC(VTLigacao *ligacao)
   {
   //variáveis locais
   VTLigCC *ligCC;

   for (int n = 0; n < lisLIG_CC->Count; n++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[n];
      //cria objeto BarCC e insere em lisBAR_CC
      if (ligCC->Ligacao == ligacao) return(ligCC);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedeCC::GetZdef(STR_ZDef &Zdef)
   {
   Zdef = defeito.Zdef;
   return(true);
   }
      VTBarCC*  __fastcall  ExisteBarCC(VTBarra *barra);
      VTLigCC*  __fastcall  ExisteLigCC(VTLigacao *ligacao);

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::Inicia(VTRedes *redes)
   {
   //variáveis locais
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));

   //se necessário, restaura RedeCC original
   RestauraRedeCC();
   //inicia lisBAR c/ todas as Barras das Redes e das Reducoes
   lisBAR->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoBARRA), lisBAR);
   //inicia lisCAR c/ todas as Cargas das Redes e das Reducoes
   lisCAR->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoCARGA), lisCAR);
   //inicia lisLIG c/ todas as Ligacoes das Redes e das Reducoes
   lisLIG->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoLIGACAO), lisLIG);
   //inicia lisZT c/ todos Capacitor, Filtro, Reator, TrafoZZ e Zref das Redes e das Reducoes
   lisZT->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoCAPACITOR), lisZT);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoFILTRO),    lisZT);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoREATOR),    lisZT);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoTRAFOZZ),   lisZT);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoYREF),      lisZT);
   //inicia lisGS c/ todos os Geradores e Supimentos das Redes e das Reducoes
   lisGS->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoGERADOR),    lisGS);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoSUPRIMENTO), lisGS);
   //inicia lisMUT c/ todas as Mutuas das Redes (OBS: reduções não tem Mutua)
   lisMUT->Clear();
   redes->LisMutua(lisMUT);
   //cria objetos TBarCC
   IniciaLisBarCC();
   //cria objetos TLigCC
   IniciaLisLigCC();
   //cria objetos TLigCC p/ as Ligacoes fictícias das Reducoes
   IniciaLisLigCC_Ficticia(redes);
   //define ID p/ Barra fictícia
   troca.bar_fic->Id = MaiorId(lisBAR, eqptoBARRA) + 1;
   //define ID p/ TRechos fictícios
   troca.tre_fic1->Id = MaiorId(lisLIG, eqptoTRECHO) + 1;
   troca.tre_fic2->Id = troca.tre_fic1->Id  + 1;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::IniciaLisBarCC(void)
   {
   //variáveis locais
   VTBarra *barra;

   //destrói objetos criados anteriormente
   LimpaTList(lisBAR_CC);
   try{//loop p/ todas Barras
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         //cria objeto BarCC e insere em lisBAR_CC
         lisBAR_CC->Add(NewObjBarCC(barra));
         }
      }catch (Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::IniciaLisLigCC(void)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //destrói objetos criados anteriormente
   LimpaTList(lisLIG_CC);
   try{//loop p/ todas Ligacoes
      for (int n = 0; n < lisLIG->Count; n++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[n];
         //cria objeto LigCC e insere em lisLIG_CC
         lisLIG_CC->Add(NewObjLigCC(ligacao));
         }
      }catch (Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::IniciaLisLigCC_Ficticia(VTRedes *redes)
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
         //cria objetos LigCC
         for (int nl = 0; nl < lisLIG_FIC->Count; nl++)
            {
            ligacao = (VTLigacao*)lisLIG_FIC->Items[nl];
            ficticia.lisLIG_CC->Add(NewObjLigCC(ligacao));
            }
         }
      }
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisBarra(void)
   {
   return(lisBAR);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisBarraCC(void)
   {
   return(lisBAR_CC);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisCarga(void)
   {
   return(lisCAR);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisEqbarAterrado(void)
   {
   return(lisZT);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisGeradorSuprimento(void)
   {
   return(lisGS);
   }
   
//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisLigacao(void)
   {
   return(lisLIG);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisLigacaoCC(void)
   {
   return(lisLIG_CC);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisLigacaoCC_Ficticia(void)
   {
   return(ficticia.lisLIG_CC);
   }
//-----------------------------------------------------------------------------
TList* __fastcall TRedeCC::LisMutua(void)
   {
   return(lisMUT);
   }
   
//-----------------------------------------------------------------------------
int __fastcall TRedeCC::MaiorId(TList *lisEQP, int tipo)
   {
   //variáveis locais
   int      max_id = 0;
   VTEqpto *eqpto;

   //determina mior ID dos Eqptos da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() != tipo) continue;
      if (eqpto->Id > max_id) max_id = eqpto->Id;
      }
   return(max_id);
   }

//-----------------------------------------------------------------------------
VTNet* __fastcall TRedeCC::PM_GetNet(void)
   {
   return(net);
   }

//-----------------------------------------------------------------------------
int __fastcall TRedeCC::PM_GetShowDefeito(void)
   {
   return(PD.show_defeito);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetS3f_mva(void)
   {
   return(PD.s3f_mva);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetSft_mva(void)
   {
   return(PD.sft_mva);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetZ0entrada(void)
   {
   return(PD.z0_entrada);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetZ0entradaSoTrechos(void)
   {
   return(PD.z0_entrada_so_trechos);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetZ1entrada(void)
   {
   return(PD.z1_entrada);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TRedeCC::PM_GetZ1entradaSoTrechos(void)
   {
   return(PD.z1_entrada_so_trechos);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetNet(VTNet *net)
   {
   this->net = net;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetShowDefeito(int defeito)
   {
   //proteção
   if ((defeito < 0)||(defeito >= NUM_MAX_DEF)) return;
   PD.show_defeito = defeito;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetS3f_mva(complex<double> s3f_mva)
   {
   PD.s3f_mva = s3f_mva;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetSft_mva(complex<double> sft_mva)
   {
   PD.sft_mva = sft_mva;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetZ0entrada(complex<double> z0)
   {
   PD.z0_entrada = z0;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetZ0entradaSoTrechos(complex<double> z0)
   {
   PD.z0_entrada_so_trechos = z0;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetZ1entrada(complex<double> z1)
   {
   PD.z1_entrada = z1;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::PM_SetZ1entradaSoTrechos(complex<double> z1)
   {
   PD.z1_entrada_so_trechos = z1;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::ReiniciaResultados(void)
   {
   //variáveis locais
   VTBarCC *pbcc;
   VTLigCC *ligCC;

   //loop p/ todas LigCC em lisLIG_CC
   for (int n = 0; n < lisLIG_CC->Count; n++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[n];
      ligCC->ReiniciaValores();
      }
   //loop p/ todas LigCC em ficticia.lisLIG_CC
   for (int n = 0; n < ficticia.lisLIG_CC->Count; n++)
      {
      ligCC = (VTLigCC*)ficticia.lisLIG_CC->Items[n];
      ligCC->ReiniciaValores();
      }
   //loop p/ todas BarCC
   for (int n = 0; n < lisBAR_CC->Count; n++)
      {
      pbcc = (VTBarCC*)lisBAR_CC->Items[n];
      pbcc->ReiniciaValores();
      }
   //reinicia valores da Barra de defeito
   defeito.barCC->ReiniciaValores();
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::ReconfiguraRedeCC(void)
   {
   //variáveis locais
   VTLigCC *ligCC_tre_orig;

   //retira Trecho original
   lisLIG->Remove(troca.tre_orig);
   //retira ligCC do Trecho original
   ligCC_tre_orig = (VTLigCC*)(troca.tre_orig->Obj);
   lisLIG_CC->Remove(ligCC_tre_orig);
   //insere Barra e Trechos ficticios
   lisBAR->Add(troca.bar_fic);
   lisLIG->Add(troca.tre_fic1);
   lisLIG->Add(troca.tre_fic2);
   //inclui BarCC da Barra fictícia
   lisBAR_CC->Add(troca.barCC_fic);
   //inclui LigCC dos Trechos ficticios
   lisLIG_CC->Add(troca.ligCC_fic1);
   lisLIG_CC->Add(troca.ligCC_fic2);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::RestauraRedeCC(void)
   {
   //variáveis locais
   VTLigCC *ligCC_tre_orig;

   //verifica se RedeCC está alterada p/ defeito em um Trecho
   if (troca.tre_orig == NULL) return;
   //retira Barra fictícia
   lisBAR->Remove(troca.bar_fic);
   //retira BarCC da Barra fictícia
   lisBAR_CC->Remove(troca.barCC_fic);
   //retira Trechos fictícios
   lisLIG->Remove(troca.tre_fic1);
   lisLIG->Remove(troca.tre_fic2);
   //retira LigCC dos Trechos fictícios
   lisLIG_CC->Remove(troca.ligCC_fic1);
   lisLIG_CC->Remove(troca.ligCC_fic2);
   //insere Trecho original
   lisLIG->Add(troca.tre_orig);
   //insere LigCC do Trecho Original
   ligCC_tre_orig = (VTLigCC*)(troca.tre_orig->Obj);
   lisLIG_CC->Add(ligCC_tre_orig);
   //cancela Trecho original p/ troca
   troca.tre_orig = NULL;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaIccBar(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq)
   {
   //variáveis locais
   complex<double> aux_amp;

   //proteção
   if (pbcc == NULL) return;
   //copia correntes de fase e de seq. da Barra
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      if(ifase[ind_def].enabled) //salva correntes de fase (convertendo de kA p/ A)
         {
         pbcc->Ifas_amp[ind_def][0] = 1000. * ifase[ind_def].fasA;
         pbcc->Ifas_amp[ind_def][1] = 1000. * ifase[ind_def].fasB;
         pbcc->Ifas_amp[ind_def][2] = 1000. * ifase[ind_def].fasC;
         }
      if(iseq[ind_def].enabled) //salva correntes de sequência (convertendo de kA p/ A)
         {
         pbcc->Iseq_amp[ind_def][0]  = 1000. * iseq[ind_def].seq0;
         pbcc->Iseq_amp[ind_def][1]  = 1000. * iseq[ind_def].seq1;
         pbcc->Iseq_amp[ind_def][2]  = 1000. * iseq[ind_def].seq2;
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaIccBar_Assimetrico(VTBarCC *pbcc, STR_FASE *assim_ifase)
   {
   //proteção
   if (pbcc == NULL) return;
   //correntes assimétricas: copia correntes de fase da Barra
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      if(assim_ifase[ind_def].enabled) //salva correntes de fase (convertendo de kA p/ A)
         {
         pbcc->Assim_Ifas_amp[ind_def][0] = assim_ifase[ind_def].fasA * 1000.;
         pbcc->Assim_Ifas_amp[ind_def][1] = assim_ifase[ind_def].fasB * 1000.;
         pbcc->Assim_Ifas_amp[ind_def][2] = assim_ifase[ind_def].fasC * 1000.;
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaIccBarDef(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase)
   {
   //CUIDADO: o bar_id recebido é da Barra pai do Nó que pode ser diferente da Barra de defeito
   SalvaIccBar(defeito.barCC, ifase, iseq);
   SalvaIccBar_Assimetrico(defeito.barCC, assim_ifase);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaIccBarSup(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq)
   {
   //verifica se a barra possui geração (a geração pode estar em outra Barra do no)
   if (! pbcc->Barra->ExisteGeracao()) return;
   //salva correntes da barra
   SalvaIccBar(pbcc, ifase, iseq);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaIccLigCC(VTLigCC *plcc, VTBarCC *pbcc_ref, STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG])
   {
   //variáveis locais
   int     ind_bar;
   double  sinal;

   //proteção
   if (plcc == NULL) return;
   //determina índice da Barra de referência da corrente
   for (ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if (pbcc_ref->Barra->No[net] == (plcc->Ligacao->Barra(ind_bar))->No[net]) break;
      }
   //proteção
   if (ind_bar >= NBAR_LIG)
      {return;}
   //inverte sentido da corrente quando a Barra de referência não é a barra1 da
   sinal = (ind_bar == 0) ? 1. : -1.;
   //copia correntes de fase e de seq. da Ligacao
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if(ifase[ind_def][ind_bar].enabled)
            {
            //salva correntes de fase em relação a barra1 da Ligacao ( convertendo de kA p/ A)
            plcc->Ifas_amp[ind_def][ind_bar][0] = (1000. * sinal) * ifase[ind_def][ind_bar].fasA;
            plcc->Ifas_amp[ind_def][ind_bar][1] = (1000. * sinal) * ifase[ind_def][ind_bar].fasB;
            plcc->Ifas_amp[ind_def][ind_bar][2] = (1000. * sinal) * ifase[ind_def][ind_bar].fasC;
            }
         if(iseq[ind_def][ind_bar].enabled)
            {
            //salva correntes de sequencia
            plcc->Iseq_amp[ind_def][ind_bar][0]  = (1000. * sinal) * iseq[ind_def][ind_bar].seq0;
            plcc->Iseq_amp[ind_def][ind_bar][1]  = (1000. * sinal) * iseq[ind_def][ind_bar].seq1;
            plcc->Iseq_amp[ind_def][ind_bar][2]  = (1000. * sinal) * iseq[ind_def][ind_bar].seq2;
            //inverte sinal p/ a próxima barra
            // sinal = - sinal;
            }
         }
      }
   //define sentido do fluxo
   VerificaSentidoFluxo(plcc);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedeCC::SalvaVccBarCC(VTBarCC *pbcc, STR_FASE *vfase, STR_SEQ *vseq)
   {
   //copia tensões de fase e de seq. da Barra
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      if(vfase[ind_def].enabled)
         {
         //salva tensões de fase
         pbcc->Vfas_pu[ind_def][0] = vfase[ind_def].fasA;
         pbcc->Vfas_pu[ind_def][1] = vfase[ind_def].fasB;
         pbcc->Vfas_pu[ind_def][2] = vfase[ind_def].fasC;
         }
      if(vseq[ind_def].enabled)
         {
         //salva tensões de sequencia
           pbcc->Vseq_pu[ind_def][0] = vseq[ind_def].seq0;
           pbcc->Vseq_pu[ind_def][1] = vseq[ind_def].seq1;
           pbcc->Vseq_pu[ind_def][2] = vseq[ind_def].seq2;
         }
      }
/*
   //verifica se é a Barra de defeito
   if (pbcc->Barra == defeito.barCC->Barra)
      {//copia dados tensões p/ barra de defeito
      for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
         {//salva tensões de fase
         defeito.barCC->Vfas_pu[ind_def][0] = vfase[ind_def].fasA;
         defeito.barCC->Vfas_pu[ind_def][1] = vfase[ind_def].fasB;
         defeito.barCC->Vfas_pu[ind_def][2] = vfase[ind_def].fasC;
         //salva tensões de sequencia
         defeito.barCC->Vseq_pu[ind_def][0] = vseq[ind_def].seq0;
         defeito.barCC->Vseq_pu[ind_def][1] = vseq[ind_def].seq1;
         defeito.barCC->Vseq_pu[ind_def][2] = vseq[ind_def].seq2;
         }
      }
*/
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedeCC::SetZdef(STR_ZDef &Zdef)
   {
   //copia dados da estrutura
   defeito.Zdef = Zdef;
   return(true);
   }

//-----------------------------------------------------------------------------
VTTrecho* __fastcall TRedeCC::TrechoDefeito(void)
   {
   return(troca.tre_orig);
   }

//-----------------------------------------------------------------------------
VTTrecho* __fastcall TRedeCC::TrechoFicticio(int ind_trecho)
   {
   return((ind_trecho == 0) ? troca.tre_fic1 : troca.tre_fic2);
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TRedeCC::VerificaSentidoFluxo(VTLigCC *ligCC)
   {
   //variáveis locais
   double          p_mw;
   complex<double> s_mva;
   VTBarra         *barra;
   VTBarCC         *barCC;

   //reinicia todos os valores de sentido de fluxo
   ligCC->ReiniciaSentido();
   //loop p/ todas Barras da Ligacao
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {//determina
      if ((barra = ligCC->Ligacao->Barra(ind_bar)) == NULL) continue;
      //determina BarCC
      if ((barCC = ExisteBarCC(barra->Id)) == NULL) continue;
      //loop p/ todos defeitos
      for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
         {
         //calcula fluxo total nas 3 fases
         s_mva = CZero();
         for (int ind_fase = 0; ind_fase < 3; ind_fase++)
            {
            s_mva += barCC->Vfas_kv[ind_def][ind_fase] * conj(ligCC->Ifas_amp[ind_def][ind_fase]);
            }
         p_mw = s_mva.real();
         if      (p_mw < 0.) ligCC->Sentido[ind_def][ind_bar] = fluxoNEG;
         else if (p_mw > 0.) ligCC->Sentido[ind_def][ind_bar] = fluxoPOS;
         else                ligCC->Sentido[ind_def][ind_bar] = fluxoNENHUM;
         }
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TRedeCC::VerificaSentidoFluxo(VTLigCC *ligCC)
   {
   //variáveis locais
   //double          dif_mw;
   double          s_mw;
   complex<double> s_kva;
   complex<double> s_mva;
   VTBarra         *barra;
   VTBarCC         *barCC;

   //reinicia todos os valores de sentido de fluxo
   ligCC->ReiniciaSentido();
   //loop p/ todos defeitos
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {//loop p/ todas Barras da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {//inicia potência em relação à Barra
         ligCC->S_mva[ind_def][ind_bar] = CZero();
         s_mva = CZero();
         //determina Barra e sua BarCC
         if ((barra = ligCC->Ligacao->Barra(ind_bar)) == NULL) continue;
         if ((barCC = (VTBarCC*)(barra->Obj)) == NULL) continue;
         //calcula fluxo total nas 3 fases
         for (int ind_fase = 0; ind_fase < 3; ind_fase++)
            {
            s_kva  = barCC->Vfas_kv[ind_def][ind_fase] * conj(ligCC->Ifas_amp[ind_def][ind_bar][ind_fase]);
            s_mva += (s_kva / 1000.);
            }
         //salva fluxo na barra
         ligCC->S_mva[ind_def][ind_bar] = s_mva;
         //determina sentido do fluxo na Barra
         s_mw = s_mva.real();
         /*
         if (IsDoubleZero(s_mw, 1e-8))
            {ligCC->Sentido[ind_def][ind_bar] = fluxoNENHUM;}
         else if (p_mw > 0)
            {ligCC->Sentido[ind_def][ind_bar] = fluxoPOS;}
         else
            {ligCC->Sentido[ind_def][ind_bar] = fluxoNEG;}
         */
         //OBS: se for nulo não atualiza sentido: para defeito 3F, o fluxo é nulo
         //     no ponto de defeito mas diferente de nulo no outro ponto
         //if (IsDoubleZero(s_mw, 1e-8)) continue;
         if (IsDoubleZero(s_mw, 1e-6)) continue;
         if (s_mw > 0)
            {
            ligCC->Sentido[ind_def][ind_bar]   = fluxoPOS;
            }
         else if (s_mw < 0)
            {
            ligCC->Sentido[ind_def][ind_bar]   = fluxoNEG;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof