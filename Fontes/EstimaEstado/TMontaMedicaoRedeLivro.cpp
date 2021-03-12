//---------------------------------------------------------------------------
#pragma hdrstop
#include "TMontaMedicaoRedeLivro.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoRedeLivro::TMontaMedicaoRedeLivro(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria lista
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoRedeLivro::~TMontaMedicaoRedeLivro(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::Executa(bool por_fase)
   {
   //variáveis locais
   VTEdita   *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //retira Medidores atuais
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   if (lisEQP->Count > 0)
      {
      edita->RetiraLisEqpto(lisEQP);
      }
   //insere medidores
   InsereMedidor(por_fase);
   //atualiza o gráfico
   grafico->Redraw();

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedicaoRedeLivro::IniciaCurva(VTCanal *canal, double valor)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar                     = patamares->Patamar[ind_pat];
      canal->ValorMedido[patamar] = valor;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereMedidor(bool por_fase)
   {
   //variáveis locais
   int       num_fase;
   double    Sbase = 100.;
   double    fmult_V, sigma_PQ, sigma_V, tolerancia_V, tolerancia_PQ;
   struct{
         int        tag;
         AnsiString nome;
         }fase[MAX_FASE] = {{faseA, "_A"}, {faseB, "_B"},{faseC, "_C"}, {faseN, "_N"}};
   VTBarra   *bar1, *bar2, *bar3, *bar4, *bar5, *bar6;
   VTChave   *chave36;
   VTTrecho  *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
             *trec35, *trec36, *trec45, *trec56;
   VTMedidor *medidor;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //define número de fases
   if(por_fase)
      {
      num_fase = 3;
      }
   else
      {
      num_fase = 1;
      for (int n = 0; n < MAX_FASE; n++) fase[n].nome = "";
      }
   //localiza as barras da rede
   bar1 = redes->ExisteBarra("B1");
   bar2 = redes->ExisteBarra("B2");
   bar3 = redes->ExisteBarra("B3");
   bar4 = redes->ExisteBarra("B4");
   bar5 = redes->ExisteBarra("B5");
   bar6 = redes->ExisteBarra("B6");
   //localiza os trechos da rede
   trec12  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B1-B2");
   trec14  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B1-B4");
   trec15  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B1-B5");
   trec23  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B2-B3");
   trec24  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B2-B4");
   trec25  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B2-B5");
   trec26  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B2-B6");
   trec35  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B3-B5");
   trec45  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B4-B5");
   trec56  = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, "B5-B6");

   // Usar no caso "WW6_Base"
   trec36  = (VTTrecho *) redes->ExisteEqpto(eqptoTRECHO, "B3-B6");

   // Usar no caso "WW6_Base_ComChave"
   // trec36  = (VTTrecho *) redes->ExisteEqpto(eqptoTRECHO, "B3_CH-B6");
   // chave36 = (VTChave  *) redes->ExisteEqpto(eqptoCHAVE,  "B3-B3_CH");

   // Acerta unidades e define desvios padrão
   fmult_V  = 1. / 230.;              // [kV] de linha -> [pu]
   sigma_V  = 3.83 * fmult_V;         // [pu]
   sigma_PQ = 5. / double(num_fase);  // [MW] ou [MVAr] (trifásico ou por fase)
   //converte sigma para tolerância
   tolerancia_V  = sigma_V  * 100.0 * 1.645;
   tolerancia_PQ = sigma_PQ * 100.0 * 1.645;

   //==========================
   //Medidor e Canais da bar1
   medidor = InsereMedidor(bar1, "Med_bar1");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar1
      InsereCanal_V(medidor, "MV1"   + fase[n].nome, bar1,   fase[n].tag, tolerancia_V, (238.4 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar1
      InsereCanal_P(medidor, "MG1_P" + fase[n].nome, bar1,   fase[n].tag, tolerancia_PQ,  1.131 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "MG1_Q" + fase[n].nome, bar1,   fase[n].tag, tolerancia_PQ,  0.202 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec12
      InsereCanal_P(medidor, "M12_P" + fase[n].nome, trec12, fase[n].tag, tolerancia_PQ,  0.315 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M12_Q" + fase[n].nome, trec12, fase[n].tag, tolerancia_PQ, -0.132 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativA de trec14
      InsereCanal_P(medidor, "M14_P" + fase[n].nome, trec14, fase[n].tag, tolerancia_PQ,  0.389 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M14_Q" + fase[n].nome, trec14, fase[n].tag, tolerancia_PQ,  0.212 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativA de trec15
      InsereCanal_P(medidor, "M15_P" + fase[n].nome, trec15, fase[n].tag, tolerancia_PQ,  0.357 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M15_Q" + fase[n].nome, trec15, fase[n].tag, tolerancia_PQ,  0.094 * (Sbase / num_fase));
      }
   //==========================
   //Medidor e Canais da bar2
   medidor = InsereMedidor(bar2, "Med_bar2");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar2
      InsereCanal_V(medidor, "MV2" + fase[n].nome,   bar2,   fase[n].tag, tolerancia_V, (237.8 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar1
      InsereCanal_P(medidor, "MG2_P" + fase[n].nome, bar2,   fase[n].tag, tolerancia_PQ,  0.484 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "MG2_Q" + fase[n].nome, bar2,   fase[n].tag, tolerancia_PQ,  0.719 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec21
      InsereCanal_P(medidor, "M21_P" + fase[n].nome, trec12, fase[n].tag, tolerancia_PQ, -0.349 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M21_Q" + fase[n].nome, trec12, fase[n].tag, tolerancia_PQ,  0.097 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec23
      InsereCanal_P(medidor, "M23_P" + fase[n].nome, trec23, fase[n].tag, tolerancia_PQ,  0.086 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M23_Q" + fase[n].nome, trec23, fase[n].tag, tolerancia_PQ, -0.119 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec24
      InsereCanal_P(medidor, "M24_P" + fase[n].nome, trec24, fase[n].tag, tolerancia_PQ,  0.328 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M24_Q" + fase[n].nome, trec24, fase[n].tag, tolerancia_PQ,  0.383 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec25
      InsereCanal_P(medidor, "M25_P" + fase[n].nome, trec25, fase[n].tag, tolerancia_PQ,  0.174 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M25_Q" + fase[n].nome, trec25, fase[n].tag, tolerancia_PQ,  0.220 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec26
      InsereCanal_P(medidor, "M26_P" + fase[n].nome, trec26, fase[n].tag, tolerancia_PQ,  0.223 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M26_Q" + fase[n].nome, trec26, fase[n].tag, tolerancia_PQ,  0.150 * (Sbase / num_fase));
      }
   //==========================
   //Medidor e Canais da bar3
   medidor = InsereMedidor(bar3, "Med_bar3");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar3
      InsereCanal_V(medidor, "MV3"   + fase[n].nome, bar3,   fase[n].tag, tolerancia_V, (250.7 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar3
      InsereCanal_P(medidor, "MG3_P" + fase[n].nome, bar3,   fase[n].tag, tolerancia_PQ,  0.551 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "MG3_Q" + fase[n].nome, bar3,   fase[n].tag, tolerancia_PQ,  0.906 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec23
      InsereCanal_P(medidor, "M32_P" + fase[n].nome, trec23, fase[n].tag, tolerancia_PQ, -0.021 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M32_Q" + fase[n].nome, trec23, fase[n].tag, tolerancia_PQ,  0.102 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec35
      InsereCanal_P(medidor, "M35_P" + fase[n].nome, trec35, fase[n].tag, tolerancia_PQ,  0.177 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M35_Q" + fase[n].nome, trec35, fase[n].tag, tolerancia_PQ,  0.239 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec36

      // Usar no caso "WW6_Base"
      InsereCanal_P(medidor, "M36_P" + fase[n].nome, trec36, fase[n].tag, tolerancia_PQ,  0.433 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M36_Q" + fase[n].nome, trec36, fase[n].tag, tolerancia_PQ,  0.583 * (Sbase / num_fase));

      // Usar no caso "WW6_Base_ComChave"
      // InsereCanal_P(medidor, "M36_P" + fase[n].nome, chave36, fase[n].tag, tolerancia_PQ,  0.433 * (Sbase / num_fase));
      // InsereCanal_Q(medidor, "M36_Q" + fase[n].nome, chave36, fase[n].tag, tolerancia_PQ,  0.583 * (Sbase / num_fase));
      }
   //==========================
   //Medidor e Canais da bar4
   medidor = InsereMedidor(bar4, "Med_bar4");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar4
      InsereCanal_V(medidor, "MV4"   + fase[n].nome, bar4,   fase[n].tag, tolerancia_V, (225.7 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar4
      InsereCanal_P(medidor, "ML4_P" + fase[n].nome, bar4,   fase[n].tag, tolerancia_PQ,  0.718 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "ML4_Q" + fase[n].nome, bar4,   fase[n].tag, tolerancia_PQ,  0.719 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec14
      InsereCanal_P(medidor, "M41_P" + fase[n].nome, trec14, fase[n].tag, tolerancia_PQ, -0.401 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M41_Q" + fase[n].nome, trec14, fase[n].tag, tolerancia_PQ, -0.143 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec24
      InsereCanal_P(medidor, "M42_P" + fase[n].nome, trec24, fase[n].tag, tolerancia_PQ, -0.298 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M42_Q" + fase[n].nome, trec24, fase[n].tag, tolerancia_PQ, -0.443 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec45
      InsereCanal_P(medidor, "M45_P" + fase[n].nome, trec45, fase[n].tag, tolerancia_PQ,  0.007 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M45_Q" + fase[n].nome, trec45, fase[n].tag, tolerancia_PQ, -0.174 * (Sbase / num_fase));
      }
   //==========================
   //Medidor e Canais da bar5
   medidor = InsereMedidor(bar5, "Med_bar5");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar5
      InsereCanal_V(medidor, "MV5"   + fase[n].nome, bar5,   fase[n].tag, tolerancia_V, (225.2 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar5
      InsereCanal_P(medidor, "ML5_P" + fase[n].nome, bar5,   fase[n].tag, tolerancia_PQ,  0.720 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "ML5_Q" + fase[n].nome, bar5,   fase[n].tag, tolerancia_PQ,  0.677 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec15
      InsereCanal_P(medidor, "M51_P" + fase[n].nome, trec15, fase[n].tag, tolerancia_PQ, -0.366 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M51_Q" + fase[n].nome, trec15, fase[n].tag, tolerancia_PQ, -0.175 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec25
      InsereCanal_P(medidor, "M52_P" + fase[n].nome, trec25, fase[n].tag, tolerancia_PQ, -0.117 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M52_Q" + fase[n].nome, trec25, fase[n].tag, tolerancia_PQ, -0.222 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec35
      InsereCanal_P(medidor, "M53_P" + fase[n].nome, trec35, fase[n].tag, tolerancia_PQ, -0.251 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M53_Q" + fase[n].nome, trec35, fase[n].tag, tolerancia_PQ, -0.299 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec45
      InsereCanal_P(medidor, "M54_P" + fase[n].nome, trec45, fase[n].tag, tolerancia_PQ, -0.021 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M54_Q" + fase[n].nome, trec45, fase[n].tag, tolerancia_PQ, -0.015 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec56
      InsereCanal_P(medidor, "M56_P" + fase[n].nome, trec56, fase[n].tag, tolerancia_PQ, -0.021 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M56_Q" + fase[n].nome, trec56, fase[n].tag, tolerancia_PQ, -0.008 * (Sbase / num_fase));
      }
   //==========================
   //Medidor e Canais da bar6
   medidor = InsereMedidor(bar6, "Med_bar6");
   for (int n = 0; n < num_fase; n++)
      {//insere Canal de tensão de bar6
      InsereCanal_V(medidor, "MV6"   + fase[n].nome, bar6,   fase[n].tag, tolerancia_V, (228.9 * fmult_V));
      //insere Canal de pot.ativa e reativa de bar6
      InsereCanal_P(medidor, "ML6_P" + fase[n].nome, bar6,   fase[n].tag, tolerancia_PQ,  0.723 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "ML6_Q" + fase[n].nome, bar6,   fase[n].tag, tolerancia_PQ,  0.609 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec26
      InsereCanal_P(medidor, "M62_P" + fase[n].nome, trec26, fase[n].tag, tolerancia_PQ, -0.196 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M62_Q" + fase[n].nome, trec26, fase[n].tag, tolerancia_PQ, -0.223 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec36
      InsereCanal_P(medidor, "M63_P" + fase[n].nome, trec36, fase[n].tag, tolerancia_PQ, -0.468 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M63_Q" + fase[n].nome, trec36, fase[n].tag, tolerancia_PQ, -0.511 * (Sbase / num_fase));
      //insere Canal de pot.ativa e reativa  de trec56
      InsereCanal_P(medidor, "M65_P" + fase[n].nome, trec56, fase[n].tag, tolerancia_PQ,  0.010 * (Sbase / num_fase));
      InsereCanal_Q(medidor, "M65_Q" + fase[n].nome, trec56, fase[n].tag, tolerancia_PQ,  0.029 * (Sbase / num_fase));
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicaoRedeLivro::InsereMedidor(VTBarra *barra, AnsiString codigo)
   {
   //variáveis locais
   VTMedidor *medidor;
   VTEdita   *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   medidor = DLL_NewObjMedidor();
   medidor->Codigo    = codigo;
   medidor->esq.dxbar = 300;
   medidor->esq.dybar = 300;
   edita->InsereEqbar(medidor, barra);

   return(medidor);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereCanal_I(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double i_amp)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalI, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);
   IniciaCurva(canal, i_amp);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double p_mw)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalP, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);
   IniciaCurva(canal, p_mw);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double q_mvar)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalQ, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);
   IniciaCurva(canal, q_mvar);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereCanal_V(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double v_pu)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalV, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);
   IniciaCurva(canal, v_pu);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoRedeLivro::InsereCurva(VTCanal *canal)
   {
   try{//verifica se o Canal não possui Curva
      if (canal->Curva == NULL)
         {//cria Curva p/ a Canal
			canal->Curva                = DLL_NewObjCurva();
         canal->Curva->Tipica        = false;
         canal->Curva->NumeroValores = nvCURVA_CANAL;
         canal->Curva->Escala        = pat01HORA;
         switch(canal->TipoCanal)
            {
            case canalV:
			      canal->Curva->Tipo          = curvaV;
               canal->Curva->Unidade       = unidKV;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalI:
			      canal->Curva->Tipo          = curvaI;
               canal->Curva->Unidade       = unidA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalP:
			      canal->Curva->Tipo          = curvaP;
               canal->Curva->Unidade       = unidMW;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalQ:
			      canal->Curva->Tipo          = curvaQ;
               canal->Curva->Unidade       = unidMVAR;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalS:
			      canal->Curva->Tipo          = curvaS;
               canal->Curva->Unidade       = unidMVA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            default:
               break;
            }
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   return(canal->Curva != NULL);
   }

//---------------------------------------------------------------------------
//eof

