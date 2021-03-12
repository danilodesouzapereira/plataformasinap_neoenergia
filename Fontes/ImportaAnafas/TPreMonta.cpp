//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include <algorithm>
#include "TPreMonta.h"
#include "TDbar.h"
#include "TDcir.h"
#include "TDmut.h"
#include "TDtrf.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Classes\VTClasse.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
using namespace std;

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl)
   {
   return(new TPreMonta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta::TPreMonta(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   monta     = (VTMonta*)apl->GetObject(__classid(VTMonta));
   //cria listas
   lisDBAR = new TList();
   lisDCIR = new TList();
   lisDMUT = new TList();
   lisDTRF = new TList();
   lisTMP  = new TList();
   //inicia flag que indica sucesso/falha na montagem da rede
   sucesso        = true;
   coord_definida = false;
   rede_id        = -1;
   num_erro       = 0;
   Sbase          = 100.;   //MVA
   fator_perc_pu  = 100.;
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   //destrói lista e seus objetos
   if (lisDBAR) {LimpaTList(lisDBAR); delete lisDBAR; lisDBAR = NULL;}
   if (lisDCIR) {LimpaTList(lisDCIR); delete lisDCIR; lisDCIR = NULL;}
   if (lisDMUT) {LimpaTList(lisDMUT); delete lisDMUT; lisDMUT = NULL;}
   if (lisDTRF) {LimpaTList(lisDTRF); delete lisDTRF; lisDTRF = NULL;}
   }

//-----------------------------------------------------------------------------
double __fastcall TPreMonta::Converte_pu_ohm(double valor_pu, double Sbase, double Vbase)
   {
   //variáveis locais
   double Zbase = (Vbase * Vbase)/Sbase;
   return(valor_pu * Zbase);
   }

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineCoordenadaDbar(void)
   {
   //variáveis locais
   int     dx, dy, x, y, num_bar;
   VTArea  *area;
   TDbar   *dbar;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //determina número de barras da rede
   if ((num_bar = lisDBAR->Count) == 0) return;
   //define coordenadas iniciais p/ barras
   area = redes->AreaEsquematico();
   x    = area->x1;
   y    = area->y1;
   dx   = area->Width() / num_bar;
   dy   = area->Height() / num_bar;
   //loop p/ todos TDbar
   for (int n = 0; n < lisDBAR->Count; n++)
      {
      dbar = (TDbar*)lisDBAR->Items[n];
      dbar->coord_x = x;
      dbar->coord_y = y;
      x += dx;
      y += dy;
      }
   }
//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefinePrimario(TDtrf *dtrf, TDbar *dbar, int ligacao)
   {
   dtrf->dbar1       = dbar;
   dtrf->pri.vnom    = dbar->VBASE;
   dtrf->pri.ligacao = ligacao;
   }

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineSecundario(TDtrf *dtrf, TDbar *dbar, int ligacao)
   {
   dtrf->dbar2       = dbar;
   dtrf->sec.vnom    = dbar->VBASE;
   dtrf->sec.ligacao = ligacao;
   }

//-----------------------------------------------------------------------------
TDbar* __fastcall TPreMonta::ExisteDbar(int NB)
   {
   //variáveis locais
   TDbar *dbar;

   //procura Dbar na lista lisDBAR
   for (int n = 0; n < lisDBAR->Count; n++)
      {
      dbar = (TDbar*)lisDBAR->Items[n];
      if (dbar->NB == NB) return(dbar);
      }
   //não encontrou Dbar
   return(NULL);
   }

//-----------------------------------------------------------------------------
TDcir* __fastcall TPreMonta::ExisteDcir(int BF, int BT, int NC)
   {
   //variáveis locais
   TDbar  *dbar1, *dbar2;
   TDcir  *dcir;

   //determina Barras do Trecho
   if ((dbar1 = ExisteDbar(BF)) == NULL) return(NULL);
   if ((dbar2 = ExisteDbar(BT)) == NULL) return(NULL);
   //determina Dcir que conectam as Dbar indicadas
   if (ExisteDcir(dbar1, dbar2, lisTMP) == 0) return(NULL);
   //verifica se há um único Dcir entre as Dbar
   if (lisTMP->Count == 1) return((TDcir*)lisTMP->First());
   //determina Dcir c/ memos NC
   for (int n = 0; n < lisTMP->Count; n++)
      {
      dcir = (TDcir*)lisTMP->Items[n];
      if (dcir->NC == NC) return(dcir);
      }
   return((TDcir*)lisTMP->First());
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::ExisteDcir(TDbar *dbar1, TDbar *dbar2, TList *lisSEL)
   {
   //variáveis locais
   TDcir  *dcir;

   //reinicia lisSEL
   lisSEL->Clear();
   //determina Dcir entre as Dbar
   for (int n = 0; n < lisDCIR->Count; n++)
      {
      dcir = (TDcir*)lisDCIR->Items[n];
      if ((dcir->dbar1 == dbar1)&&(dcir->dbar2 == dbar2))
         {//insere Dcir em lisSEL
         lisSEL->Add(dcir);
         }
      else if ((dcir->dbar1 == dbar2)&&(dcir->dbar2 == dbar1))
         {//insere Trecho em lisSEL
         lisSEL->Add(dcir);
         //inverte as Dbar do Dcir
         dcir->dbar1 = dbar2;
         dcir->dbar2 = dbar1;
         }
      }
   return(lisSEL->Count);
   }

//-----------------------------------------------------------------------------
TDtrf* __fastcall TPreMonta::ExisteDtrf(TDbar *dbar_md, TDbar *dbar1, TDbar *dbar2)
   {
   //variáveis locais
   TDtrf *dtrf;

   //verifica se a Barra dbar_md é nula
   if (dbar_md == NULL) return(NULL);
   //procura Dtrf na lista lisDTRF
   for (int n = 0; n < lisDTRF->Count; n++)
      {
      dtrf = (TDtrf*)lisDTRF->Items[n];
      //verifica se dbar_mp de Dtrf é igual Dbar mid-point indicada
      if (dtrf->dbar_md == dbar_md)
         {//proteção: verifica se as duas Dbar dde Dtrf já estão definidas
          //          e uma das novas Dbar não é nula
         if ((dtrf->dbar1 != NULL)&&(dtrf->dbar2 != NULL))
            {//verifica se uma das novas barras não é nula
            if ((dbar1 != NULL)||(dbar2 != NULL)) return(NULL);
            }
         return(dtrf);
         }
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDBAR(int num_seq, TStringList *strList)
   {
   //variáveis locais
   TDbar *dbar;

   try{//cria nova Dbar e insere na Rede
      dbar = new TDbar();
      //lê dados da própria Dbar
      dbar->NB    = StrToInteger(strList->Strings[0]);
      dbar->CHNG  = StrToInteger(strList->Strings[1]);
      dbar->MP    = StrToInteger(strList->Strings[2]);
      dbar->BN    = strList->Strings[3];
      dbar->VPRE  = StrToDouble(strList->Strings[5]);
      dbar->ANG   = StrToDouble(strList->Strings[6]);
      dbar->VBASE = VbaseDbar(strList);
      dbar->IA    = StrToInteger(strList->Strings[8]);
      }catch(Exception &e)
         {
         plog->Add("Erro na leitura do cartão DBAR " + AnsiQuotedStr(strList->Strings[0], '\''));
         //destrói objeto
         if (dbar) delete dbar;
         return(false);
         }
   //define id
   dbar->id = lisDBAR->Count + 1;
   //insere Dbar em lisDBAR
   lisDBAR->Add(dbar);

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDbarRef(void)
   {
   //variáveis locais
   TDbar *dbar;

   try{//cria Dbar de referência
      dbar = new TDbar();
      //define código e nome específicos p/ a Barra de referência
      dbar->NB    = 0;
      dbar->CHNG  = 0;
      dbar->MP    = 3;
      dbar->BN    = "REF";
      dbar->VPRE  = 0.;
      dbar->ANG   = 0.;
      dbar->VBASE = 0.;
      dbar->IA    = 1;
      //define id
      dbar->id = lisDBAR->Count + 1;
      //insere Dbar de referência em lisDBAR
      lisDBAR->Add(dbar);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDCIR(int num_seq, TStringList *strList)
   {
   //variáveis locais
   AnsiString txt;
   TDcir      *dcir;

   try{//cria objeto Dlin
      dcir         = new TDcir();
      dcir->BF     = StrToInteger(strList->Strings[0]);
      dcir->CHNG   = StrToInteger(strList->Strings[1]);
      dcir->BT     = StrToInteger(strList->Strings[2]);
      dcir->NC     = StrToInteger(strList->Strings[3]);
      dcir->TIPC   = strList->Strings[4];
      dcir->R1     = LeValorNaoInfinito(strList->Strings[5], 2) / fator_perc_pu;
      dcir->X1     = LeValorNaoInfinito(strList->Strings[6], 2) / fator_perc_pu;
      dcir->R0     = LeValorNaoInfinito(strList->Strings[7], 2) / fator_perc_pu;
      dcir->X0     = LeValorNaoInfinito(strList->Strings[8], 2) / fator_perc_pu;
      dcir->CN     = strList->Strings[9];
      dcir->S1     = StrToDouble(strList->Strings[10]);
      dcir->S0     = StrToDouble(strList->Strings[11]);
      dcir->TAP    = StrToDouble(strList->Strings[12]);
      dcir->TB     = StrToInteger(strList->Strings[13]);
      dcir->TC     = StrToInteger(strList->Strings[14]);
      dcir->IA     = StrToInteger(strList->Strings[15]);
      dcir->DEF    = StrToInteger(strList->Strings[16]);
      dcir->IE     = strList->Strings[17];
      }catch(Exception &e)
         {//erro na leitura dos dados: nada a fazer
         plog->Add("Erro na leitura do cartão DCIR de " +
                   AnsiQuotedStr(strList->Strings[0], '\'') + " para " +
                   AnsiQuotedStr(strList->Strings[2], '\''));
         return(false);
         }
   //determina DBAR de DCIR
   dcir->dbar1 = ExisteDbar(dcir->BF);
   dcir->dbar2 = ExisteDbar(dcir->BT);
   if ((dcir->dbar1 == NULL)||(dcir->dbar2 == NULL))
      {//incrementa número de erros
      num_erro++;
      txt = "Circuito inválido: ";
      if (dcir->dbar1 == NULL) txt += "falta barra(" + strList->Strings[0] + ") ";
      if (dcir->dbar2 == NULL) txt += "falta barra(" + strList->Strings[2] + ")";
      plog->Add(txt);
      //destrói objeto
      delete dcir;
      return(true);
      }
   //proteção: se necessário, define NC p/ Dcir
   //<su> 2010.08.30 habilitei a instrução abaixo,
   //linhas paralelas sem indicação de númeor de circuito
   if (dcir->NC == 0) dcir->NC = ExisteDcir(dcir->dbar1, dcir->dbar2, lisTMP) + 1;
   //define ID
   dcir->id = lisDCIR->Count + 1;
   //insere Dcir em lisDCIR
   lisDCIR->Add(dcir);
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDMUT(int num_seq, TStringList *strList)
   {
   //variáveis locais
   AnsiString txt;
   TDmut     *dmut;

   try{//cria objeto Dmut
      dmut        = new TDmut();
      dmut->BF1   = StrToInteger(strList->Strings[0]);
      dmut->CHNG  = StrToInteger(strList->Strings[1]);
      dmut->BT1   = StrToInteger(strList->Strings[2]);
      dmut->NC1   = StrToInteger(strList->Strings[3]);
      dmut->BF2   = StrToInteger(strList->Strings[4]);
      dmut->BT2   = StrToInteger(strList->Strings[5]);
      dmut->NC2   = StrToInteger(strList->Strings[6]);
      dmut->RM    = LeValorPtoImplicito(strList->Strings[7], 2) / fator_perc_pu;;
      dmut->XM    = LeValorPtoImplicito(strList->Strings[8], 2) / fator_perc_pu;
      dmut->IA    = StrToInteger(strList->Strings[9]);
      }catch(Exception &e)
         {
         //erro na leitura dos dados: nada a fazer
         plog->Add("Erro na leitura do cartão DMUT de " +
                   AnsiQuotedStr(strList->Strings[0], '\'') + " para " +
                   AnsiQuotedStr(strList->Strings[2], '\''));
         return(true);
         }
   //determina Dcir da Mutua
   dmut->dcir1 = ExisteDcir(dmut->BF1, dmut->BT1, dmut->NC1);
   dmut->dcir2 = ExisteDcir(dmut->BF2, dmut->BT2, dmut->NC2);
   if ((dmut->dcir1 == NULL)||(dmut->dcir2 == NULL)||(dmut->dcir1 == dmut->dcir2))
      {//Dmut sem Dcir
      num_erro++;
      txt  = "Mutua c/ barra inválida: ";
      txt += strList->Strings[0] + ",";
      txt += strList->Strings[2] + ",";
      txt += strList->Strings[4] + ",";
      txt += strList->Strings[5];
      plog->Add(txt);
      //destrói objeto
      delete dmut;
      return(true);
      }
   //define ID
   dmut->id = lisDMUT->Count + 1;
   //insere Dmut em lisDMUT
   lisDMUT->Add(dmut);

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDtrf(TDcir *dcir)
   {
   //variáveis locais
   double r0, x0, r1, x1;
   TDbar  *dbar, *dbar1, *dbar2, *dbar_md;
   TDtrf  *dtrf;

   //determina Dbar de Dcir
   dbar_md = NULL;
   dbar1 = dcir->dbar1;
   dbar2 = dcir->dbar2;
   //verifica se as Dbar são de referência ou de mid-point
   if      (dbar1->Referencia()) {dbar1 = NULL;}
   else if (dbar1->MidPoint())   {dbar_md = dbar1; dbar1 = NULL;}
   if      (dbar2->Referencia()) {dbar2 = NULL;}
   else if (dbar2->MidPoint())   {dbar_md = dbar2; dbar2 = NULL;}
   try{//verifica se já existe Dtrf p/ Dbar "mid-point"
      if ((dtrf = ExisteDtrf(dbar_md, dbar1, dbar2)) == NULL)
         {//cria um novo Dtrf
         dtrf = new TDtrf();
         //NÃO define Dbar do Dtrf
         //dtrf->dbar1 = dbar1;
         //dtrf->dbar2 = dbar2;
         //define codigo
         dtrf->codigo = dcir->CN;
         //define snom
         dtrf->snom = Sbase;   //assume Snom = Sbase
         //salva Dtrf em lisDTRF
         lisDTRF->Add(dtrf);
         }
      //soma as impedâncias de Dcir em Dtrf
      dtrf->z1.r += dcir->R1;
      dtrf->z1.x += dcir->X1;
      dtrf->z0.r += dcir->R0;
      dtrf->z0.x += dcir->X0;
      } catch(Exception &e)
         {
         plog->Add("Erro na inclusão do circuito " + AnsiQuotedStr(dcir->CN, '\''));
         return(false);
         }
   //verifica se os dois Dbar são reais (nem mid-point nem de referência)
   if ((dbar1 != NULL)&&(dbar2 != NULL))
      {//trafo EST_ISO/EST_ISO definido por um único Circuito
      DefinePrimario(dtrf, dbar1, lgEST_ISO);
      DefineSecundario(dtrf, dbar2, lgEST_ISO);
      //garante que Dbar mid-point do Dtrf é nula
      dtrf->dbar_md = NULL;
      return(true);
      }
   //verifica se os dois Dbar do Dtrf estão definidos
   if ((dtrf->dbar1 != NULL)&&(dtrf->dbar2 != NULL)) return(true);
   //define Dbar mid-point do Dtrf
   dtrf->dbar_md = dbar_md;
   //verifica se há uma Barra real (que não seja "mid-point" ou de referência)
   dbar = NULL;
   if      (dbar1 != NULL) dbar = dbar1;
   else if (dbar2 != NULL) dbar = dbar2;
   //verifica se existe uma Barra real
   if (dbar != NULL)
      {//dbar é uma Barra real: verifica se Z0 é nulo
      if ((r0 == 0.)&&(x0 == 0.))
         {//ligação delta: verifica se Dbar do primário está definido
         if (dtrf->dbar1 == NULL) DefinePrimario(dtrf, dbar, lgEST_ISO);
         else                     DefineSecundario(dtrf, dbar, lgEST_ISO);
         }
      else
         {//ligação estrela: verifica se Dbar do secundário está definido
         if (dtrf->dbar2 == NULL) DefineSecundario(dtrf, dbar, lgEST_AT);
         else                     DefinePrimario(dtrf, dbar, lgEST_AT);
         }
      }
   return(true);
   }

//-----------------------------------------------------------------------------
double __fastcall TPreMonta::LeValorNaoInfinito(AnsiString valor, int ncd)
   {
   if (ValorInfinito(valor)) return(0.);
   //return(StrToDouble(valor));
   return(LeValorPtoImplicito(valor, ncd));
   }

//-----------------------------------------------------------------------------
double __fastcall TPreMonta::LeValorPtoImplicito(AnsiString valor, int ncd)
   {
   //variáveis locais
   double val = 0.;

   //proteção: verifica se string nulo
   if ((valor.Trim()).IsEmpty()) return(val);
   //proteção: verifica se valor infinito
   if (ValorInfinito(valor)) return(val);
   try{//converte string p/ double
      val = StrToDouble(valor);
      //verifica se string contém ponto decimal explícito
      if (! PontoDecimalExplicito(valor))
         {//converte valor
         val /= pow(10., double(ncd));
         }
      }catch(Exception &e)
         {
         }
   return(val);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Monta(void)
   {
   //variáveis locais
   TDbar *dbar;
   TDcir *dcir;
   TDtrf *dtrf;
   TDmut *dmut;

   //monta uma Rede
   if (! MontaRede()) return(false);
   //loop p/ todos TDbar em lisDBAR
   monta->InsereBarraStart();
   for (int n = 0; n < lisDBAR->Count; n++)
      {
      dbar = (TDbar*)lisDBAR->Items[n];
      //verifica se Dbar está sendo excluído
      if (dbar->Excluida()) continue;
      //verifica se Dbar é de referência ou mid-point
      if ((dbar->MidPoint())||(dbar->Referencia())) continue;
      if (! MontaBarra(dbar)) return(false);
      }
   monta->InsereBarraStop();
   //loop p/ todos Dcir em lisDCIR
   for (int n = 0; n < lisDCIR->Count; n++)
      {
      dcir = (TDcir*)lisDCIR->Items[n];
      //verifica se Dcir está sendo excluído
      if (dcir->Excluido()) continue;
      if (! MontaEqpto(dcir)) return(false);
      }
   //loop p todos os Dtrf criados e armazenados em lisDTRF
   for (int n = 0 ; n < lisDTRF->Count; n++)
      {
      dtrf = (TDtrf*)lisDTRF->Items[n];
      if (! MontaTrafo(dtrf)) return(false);
      }
   //loop p todos os Dmut m lisDMUT
   for (int n = 0; n < lisDMUT->Count; n++)
      {
      dmut = (TDmut*)lisDMUT->Items[n];
      //verifica se está sendo excluída
      if (dmut->Excluida()) continue;
      if (! MontaMutua(dmut)) return(false);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaBarra(TDbar *dbar)
   {
   //variáveis locais
   strBARRA str_barra;

   //preenche estrutura strREDE
   str_barra.bar_id    = dbar->id;
   str_barra.codigo    = dbar->BN;
   str_barra.extern_id = "";
   str_barra.vnom      = dbar->VBASE;
   str_barra.utm.x     = dbar->coord_x;
   str_barra.utm.y     = dbar->coord_y;
   str_barra.esq.x1    = dbar->coord_x;
   str_barra.esq.y1    = dbar->coord_y;
   str_barra.esq.x2    = dbar->coord_x;
   str_barra.esq.y2    = dbar->coord_y;
   //insere Rede em Monta
   return(monta->InsereBarraEqpto(str_barra));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapacitor(TDcir *dcir, TDbar *dbar)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapserie(TDcir *dcir)
   {
   //variáveis locais
   strCAPSERIE str;

   //monta estrutura strCAPSERIE
   str.id         = -1;
   str.rede_id    = rede_id;
   str.bar1_id    = dcir->dbar1->id;
   str.bar2_id    = dcir->dbar2->id;
   str.codigo     = dcir->CN;
   str.extern_id  = "";
   str.vnom       = dcir->dbar1->VBASE;
   str.snom       = 0.;
   str.z.r        = Converte_pu_ohm(dcir->R1, Sbase, dcir->dbar1->VBASE);
   str.z.x        = Converte_pu_ohm(dcir->X1, Sbase, dcir->dbar1->VBASE);
   str.esq.posbar1 = 50.;
   str.esq.posbar2 = 50.;
   str.esq.posbar3 =  0.;
   //insere Capserie na rede
   if (! monta->InsereCapserie(str))return(false);
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCarga(TDcir *dcir, TDbar *dbar)
   {
   //variáveis locais
   complex<double> z_pu, s_mva;
   strCARGA        str_carga;
   int hh_ini , hh_fim;

   //proteção
   if ((ValorZero(dcir->R0))&&(ValorZero(dcir->X0))) return(true);
   //preenche estrutura strCARGA
   str_carga.car_id     = -1;
   str_carga.bar_id     = dbar->id;
   str_carga.codigo     = dcir->CN;
   //calcula z_pu complexo
   z_pu = complex<double>(dcir->R0, dcir->X0);
   //calcula  Smva
   s_mva = Sbase / z_pu;
   //assume modelo de carga Scte
   str_carga.modelo.icte_pu  = 0.;
   str_carga.modelo.scte_pu  = 1.;
   str_carga.modelo.zcte_pu  = 0.;
   //assume carga equivalente
   str_carga.classe_cons          = consEQUIVALENTE;
   str_carga.num_cons.comercial   = 0;
   str_carga.num_cons.residencial = 0;
   str_carga.num_cons.industrial  = 0;
   str_carga.num_cons.rural       = 0;
   str_carga.num_cons.outros      = 0;
   str_carga.num_cons.a4          = 0;
   //define P/Q da carga nos 4 patamares
   str_carga.curva.id      = -1;
   str_carga.curva.tipica  = false;
   str_carga.curva.num_pat = MAX_PAT;
   hh_ini = 0;
   hh_fim = 0;
   for (int np = 0; np < MAX_PAT; np++)
      {
      hh_ini  = hh_fim;
      hh_fim += 24/MAX_PAT;
      str_carga.curva.patamar[np].hm_ini.hora   = hh_ini;
      str_carga.curva.patamar[np].hm_fim.hora   = hh_fim-1;
      str_carga.curva.patamar[np].hm_ini.minuto = 0;
      str_carga.curva.patamar[np].hm_fim.minuto = 59;
      //str_carga.demanda[np].p_mw        = s_mva.real();
      //str_carga.demanda[np].q_mvar      = s_mva.imag();
      str_carga.curva.patamar[np].valor[indP]    = s_mva.real();
      str_carga.curva.patamar[np].valor[indQ]    = s_mva.imag();
      }
   str_carga.esq.posbar = 50;
   str_carga.esq.dxbar  = 0;
   str_carga.esq.dybar  = 0;
   //insere Carga em Monta
   if (! monta->InsereCarga(str_carga)) return(false);
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaEqpto(TDcir *dcir)
   {
   //variáveis locais
   TDbar *dbar;
   TDtrf *dtrf;

   //verifica se uma das Barras é "mid_point"
   if ((dcir->dbar1->MidPoint())||(dcir->dbar2->MidPoint()))
      {//Trafo
      return(InsereDtrf(dcir));
      }
   //verifica se uma das barras é de referência
   dbar = NULL;
   if (dcir->dbar1->Referencia()) dbar = dcir->dbar2;
   if (dcir->dbar2->Referencia()) dbar = dcir->dbar1;
   if (dbar != NULL)
      {//um Dbar é de referência: verifica se Suprimento ou Gerador
      //if (! Zinfinito(dcir->R1, dcir->X1))
      if ((! ValorZero(dcir->R1))||(! ValorZero(dcir->X1)))
         {//Suprimento ou Gerador
         return(MontaGerador(dcir, dbar));
         }
      //verifica se reator shunt ou Carga
      if (ValorZero(dcir->R0))
         {//Reator
         return(MontaReator(dcir, dbar));
         }
      else
         {//Carga
         return(MontaCarga(dcir, dbar));
         }
      }
   //nenhuma das Barras é de referência
   if ((ValorZero(dcir->R1))&&(dcir->X1  < 0.)&&
       (ValorZero(dcir->R0))&&(dcir->X0  < 0.))
      {//Capacitor Série
      return(MontaCapserie(dcir));
      }
   //IMPORTANTE: p/ trafo de rede equivalente Z0 pode não ser infinito
   if (dcir->TIPC.AnsiCompareIC("T") == 0)
      {//Trafo
      return(InsereDtrf(dcir));
      }
   if (Zinfinito(dcir->R0, dcir->X0))
      {//Trafo
      return(InsereDtrf(dcir));
      }
   else
      {
      return(MontaTrecho(dcir));
      }
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaGerador(TDcir *dcir, TDbar *dbar)
   {
   //variáveis locais
   strGERADOR str_ger;
   int hh_ini , hh_fim;

   //monta strGERADOR
   str_ger.ger_id       = -1;
   str_ger.bar_id       = dbar->id;
   str_ger.codigo       = dcir->CN;
   str_ger.extern_id    = "";
   str_ger.tiposup      = supVF;
   str_ger.tipo_geracao = gerVAPOR;
   str_ger.ligacao      = lgEST_AT;
   str_ger.vnom         = dbar->VBASE;
   str_ger.snom_mva     = 100.;
   str_ger.smax_mva     = str_ger.snom_mva;
   str_ger.fplim        = 0.8;
   str_ger.z0.r         = dcir->R0;
   str_ger.z0.x         = dcir->X0;
   str_ger.z1.r         = dcir->R1;
   str_ger.z1.x         = dcir->X1;
   //define dimensão do vetor
   str_ger.curva.id      = -1;
   str_ger.curva.tipica  = false;
   str_ger.curva.num_pat = MAX_PAT;
   hh_ini = 0;
   hh_fim = 0;
   for (int np = 0; np < MAX_PAT; np++)
      {
      hh_ini  = hh_fim;
      hh_fim += 24/MAX_PAT;
      str_ger.curva.patamar[np].hm_ini.hora   = hh_ini;
      str_ger.curva.patamar[np].hm_fim.hora   = hh_fim-1;
      str_ger.curva.patamar[np].hm_ini.minuto = 0;
      str_ger.curva.patamar[np].hm_fim.minuto = 59;
      //str_ger.demanda[np].p_mw        = 0.;
      //str_ger.demanda[np].q_mvar      = 0.;
      //str_ger.demanda[np].v_pu        = dbar->VPRE;
      //str_ger.demanda[np].t_rad       = DegToRad(dbar->ANG);
      str_ger.curva.patamar[np].valor[indP]   = 0.;
      str_ger.curva.patamar[np].valor[indQ]   = 0.;
      str_ger.curva.patamar[np].valor[indV]   = dbar->VPRE;
      str_ger.curva.patamar[np].valor[indT]   = DegToRad(dbar->ANG);
      }
   str_ger.esq.posbar = 50;
   str_ger.esq.dxbar  = 0;
   str_ger.esq.dybar  = 0;
   //insere Gerador em Monta
   return(monta->InsereGerador(str_ger));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaMutua(TDmut *dmut)
   {
   //variáveis locais
//   int      bar_tre1_id, bar_tre2_id;
   int      num_seq = 1;
   double   comp;
   double   Vbase, Zbase;
   strIMP   zm;
   strMUTUA str;

   //calcula impedância em ohm/km
   Vbase = dmut->dcir1->dbar1->VBASE;
   Zbase = (Vbase * Vbase) / Sbase;
   comp  = 1.;
   zm.r = (dmut->RM * Zbase) / comp;
   zm.x = (dmut->RM * Zbase) / comp;
   //define Barras de referência p/ que a Mutua cause queda de tensão positiva
   if ((zm.r >= 0.)&&(zm.x >= 0.))
      {//queda de tensão positiva
      //str.sentido_tre1 = sentidoB1B2;
      //str.sentido_tre2 = sentidoB1B2;
      str.tre1_bar_id = dmut->dcir1->dbar1->id;
      str.tre2_bar_id = dmut->dcir2->dbar1->id;
      }
   else
      {//queda de tensão negativa: inverte as barras e referência
      //str.sentido_tre1 = sentidoB2B1;
      //str.sentido_tre2 = sentidoB2B1;
      str.tre1_bar_id = dmut->dcir1->dbar2->id;
      str.tre2_bar_id = dmut->dcir2->dbar2->id;
      //inverte sinal da impedância
      zm.r = fabs(zm.r);
      zm.x = fabs(zm.x);
      }
   //preenche strMUTUA
   str.id           = dmut->id;
   str.rede_id      = rede_id;
   str.tre1_id      = dmut->dcir1->id;
   str.tre2_id      = dmut->dcir2->id;
// str.bar_tre1_id  = bar_tre1_id;
//   str.bar_tre2_id  = bar_tre2_id;
   str.codigo       = "M_" + IntToStr(num_seq++);
   str.extern_id    = "";
   str.z0           = zm;
   str.z1           = str.z0;
   str.postre1      = 50.;
   str.postre2      = 50.;
   //insere Mutua em Monta
   return(monta->InsereMutua(str));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaReator(TDcir *dcir, TDbar *dbar)
   {
   //variáveis locais
   strREATOR str_rea;

   //proteção
   if (ValorZero(dcir->X0)) return(false);
   //preenche estrutua strREATOR
   str_rea.id         = -1;
   str_rea.bar_id     = dbar->id;
   str_rea.codigo     = dcir->CN;
   str_rea.extern_id  = "";
   str_rea.ligacao    = lgTRIANG;
   str_rea.vnom       = dbar->VBASE;
   str_rea.q_mvar     = fabs(Sbase / dcir->X0);
   str_rea.esq.posbar = 50;
   str_rea.esq.dxbar  =  0;
   str_rea.esq.dybar  =  0;
   //insere Reator em Monta
   return(monta->InsereReator(str_rea));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaRede(void)
   {
   //variáveis locais
   strREDE str;

   //preenche estrutura strREDE
   str.rede_id           = rede_id;
   str.mrede_id          = -1;
   str.cluster_id        = -1;
   str.barini_id         = -1;
   str.codigo            = "Rede_1";
   str.tiporede.segmento = redeSUB;
   str.estatica          = true;
   str.radial            = false;
   str.equivalente       = false;
   str.color             = clBlack;
   str.area_esq.x1       = str.area_esq.y1 = -1;
   str.area_esq.x2       = str.area_esq.y2 = -1;
   //insere Rede em Monta
   return(monta->InsereRede(str));
   }
   
/*
//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaRegulador(TDcir *dcir, TDbar *dbar)
   {
   //variáveis locais
   int          num_seq = 1;
   strREGULADOR str_reg;
   TDbar        *dbar_ref;
   TDcir        *dcir;

   //loop p/ todos Dlin
   for (int n = 0; n < lisDCIR->Count; n++)
      {
      dcir = (TDcir*)lisDCIR->Items[n];
      //verifica se Dlin está sendo excluído
      if (dcir->Excluida()) continue;
      //verifica se Dlin é um Regulador
      if (! dcir->TipoRegulador()) continue;
      //determina Dbar controlada
      if ((dbar_ref = ExisteDbar(dcir->barra_controlada)) == NULL) continue;
      //preenche estrutura strTRECHO
      str_reg.id                  = -1;
      str_reg.rede_id             = rede_id;
      str_reg.bar1_id             = dcir->dbar1->numero;
      str_reg.bar2_id             = dcir->dbar2->numero;
      str_reg.codigo              = "RT_" + IntToStr(num_seq++);
      str_reg.by_pass             = false;
      str_reg.reversivel          = true;
      str_reg.ligacao             = lgEST_ISO;
      str_reg.snom                = (ValorInfinito(dcir->capacidade_normal)) ? Sbase : dcir->capacidade_normal;
      str_reg.var_tensao          = max(dcir->tap_min_pu, dcir->tap_max_pu) * 100;  //pu ==> %
      str_reg.num_passo           = (dcir->numero_steps) / 2;
      str_reg.ajuste_auto         = true;
      str_reg.cfg_fixo.passo      = 0;
      str_reg.cfg_auto.bar_ref_id = dbar_ref->numero;
      str_reg.cfg_auto.v_pu       = dbar_ref->tensao_pu;
      str_reg.esq.posbar1         = 50.;
      str_reg.esq.posbar2         = 50.;
      //insere Regulador em Monta
      if(! monta->InsereRegulador(str_reg)) return(false);
      }
   return(true);
   }
*/

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaTrafo(TDtrf *dtrf)
   {
   //variáveis locais
   strTRAFO  str_trf;

   //verifica se os dois Dbar do Dtrf estão definidos
   if ((dtrf->dbar1 == NULL)||(dtrf->dbar2 == NULL)) return(false);
   //preenche estrutura strTRAFO
   str_trf.trf_id        = dtrf->id;
   str_trf.rede_id       = rede_id;
   str_trf.bar1_id       = dtrf->dbar1->id;
   str_trf.bar2_id       = dtrf->dbar2->id;
   str_trf.codigo        = dtrf->codigo;
   str_trf.snom_mva      = dtrf->snom;
   str_trf.auto_trafo    = false;
   str_trf.pri.ligacao   = dtrf->pri.ligacao;
   str_trf.pri.defasagem = 0.;
   str_trf.pri.vnom      = dtrf->pri.vnom;
   str_trf.pri.tap       = (dtrf->tap_pu == 0.) ? dtrf->pri.vnom : (dtrf->pri.vnom / dtrf->tap_pu);
   str_trf.pri.zterra.r  = 0.;
   str_trf.pri.zterra.x  = 0.;
   str_trf.sec.ligacao   = dtrf->sec.ligacao;
   str_trf.sec.defasagem = 30.;
   str_trf.sec.vnom      = dtrf->sec.vnom;
   str_trf.sec.tap       = dtrf->sec.vnom;
   str_trf.sec.zterra.r  = 40.;
   str_trf.sec.zterra.x  = 0.;
   str_trf.z1            = dtrf->z1;
   str_trf.z0            = dtrf->z0;
   str_trf.perda_ferro   = 0.1;
   str_trf.esq.posbar1   = 50.;
   str_trf.esq.posbar2   = 50.;
   //dados de LTC: não disponíveis
   str_trf.ltc.tipo      = ltcFIXO;
   str_trf.ltc.bar_id    = -1;

//   str_trf.ltc.v_pu[0]   = 1.;
// str_trf.ltc.v_pu[1]   = 1.;
//   str_trf.ltc.v_pu[2]   = 1.;
// str_trf.ltc.v_pu[3]   = 1.;

   //insere Trafo em Monta
   return( monta->InsereTrafo(str_trf));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaTrecho(TDcir *dtrf)
   {
   //variáveis locais
   strARRANJO1 str_arr;
   strTRECHO   str_tre;

   //preenche estrutura strTRECHO
   str_tre.tre_id      = dtrf->id;
   str_tre.rede_id     = rede_id;
   str_tre.bar1_id     = dtrf->dbar1->id;
   str_tre.bar2_id     = dtrf->dbar2->id;
   str_tre.codigo      = dtrf->CN;
   str_tre.comp_km     = 1.; //km
   //dados de arranjo
   str_arr.tipo_cfg                  = arranjoZ0Z1;
   str_arr.cfg_arranjoZ0Z1.z1.r      = Converte_pu_ohm(dtrf->R1, Sbase, dtrf->dbar1->VBASE);
   str_arr.cfg_arranjoZ0Z1.z1.x      = Converte_pu_ohm(dtrf->X1, Sbase, dtrf->dbar1->VBASE);
   str_arr.cfg_arranjoZ0Z1.c1        = dtrf->S1 / Sbase;   //pu
   str_arr.cfg_arranjoZ0Z1.z0.r      = Converte_pu_ohm(dtrf->R0, Sbase, dtrf->dbar1->VBASE);
   str_arr.cfg_arranjoZ0Z1.z0.x      = Converte_pu_ohm(dtrf->X0, Sbase, dtrf->dbar1->VBASE);
   str_arr.cfg_arranjoZ0Z1.c0        = dtrf->S0 / Sbase;   //pu
   str_arr.cfg_arranjoZ0Z1.iadm_amp  = 1000.;
   str_tre.esq.posbar1 = 50.;
   str_tre.esq.posbar2 = 50.;
   //insere Trecho em Monta
   return(monta->InsereTrecho(str_tre, str_arr));
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::NumeroErros(void)
   {
   return(num_erro);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::PontoDecimalExplicito(AnsiString valor)
   {
   //variáveis locais
   int len = valor.Length();

   //verifica se o string possui o caractere '.' ou o caractere ','
   for (int n = 1; n <= len; n++)
      {
      if ((valor[n] == '.')||(valor[n] == ',')) return(true);
      }
   return(false);
   }

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::Reinicia(void)
   {
   //inicia dados
   sucesso        = true;
   coord_definida = false;
   num_erro       = 0;
   //limpa listas
   LimpaTList(lisDBAR);
   LimpaTList(lisDCIR);
   LimpaTList(lisDMUT);
   //cria Barra de referência
   InsereDbarRef();
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
   {
   return(sucesso);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDados(void)
   {
   //assume erro na pré-montagem dos dados
   sucesso = false;
   //ajuste inicial das coordenadas de Dbar
   if (! coord_definida) DefineCoordenadaDbar();
   //passa os dados p/ Monta
   if (! Monta()) return(false);
   //tratamento final dos dados
   if (! monta->TrataFimDeDados()) return(false);
   //término da pré-montagem dos dados c/ sucesso
   sucesso = true;
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValorInfinito(AnsiString valor)
   {
   return((valor.AnsiCompareIC("X") == 0)||
          (valor.AnsiCompareIC("999999") == 0));
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValorZero(double valor)
   {
   return(fabs(valor) < 1e-8);
   }

//-----------------------------------------------------------------------------
double __fastcall TPreMonta::VbaseDbar(TStringList *strList)
   {
   //variáveis locais
   AnsiString vbase;
   double     valor = 138.;

   try{//verifica se Vbase está definida explicitamente
      vbase = strList->Strings[7].Trim();
      if (vbase.IsEmpty())
         {//tenta extrair Vbase do próprio nome da Barra
         vbase = strList->Strings[4].Trim();
         }
      valor = StrToDouble(vbase);
      }catch(Exception &e)
         {//assume valor default
         plog->Add("Aviso: DBAR " + strList->Strings[0] + "(" + strList->Strings[3] + "): Vbase indefinida");
         valor = 138.;
         }
   return(valor);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Zinfinito(AnsiString r, AnsiString x)
   {
   return((ValorInfinito(r))&&(ValorInfinito(x)));
   }

//-----------------------------------------------------------------------------
// eof
