//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TConsulta.h"
#include "..\BDihm\Esquematico\VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTConsulta* __fastcall NewObjConsulta(VTApl *apl)
   {
   return(new TConsulta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TConsulta::TConsulta(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisEQP = new TList();
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TConsulta::~TConsulta(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TConsulta::ExisteCarga(VTReducao *reducao, VTBarra *pbarra)
   {
   //variáveis locais
   VTEqbar *eqbar;
   TList   *lisEQB;

   //obtém lista de Eqbar da Reducao
   lisEQB = reducao->LisEqbar();
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->Tipo() != eqptoCARGA) continue;
      if (eqbar->pbarra == pbarra) return((VTCarga*)eqbar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracao(VTMRede *mrede)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisEXT;

   //loop p/ todas as Redes de VTMRede
   lisEXT = mrede->LisRede();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      rede = (VTRede*)lisEXT->Items[n];
      if (ExisteGeracao(rede)) return(true);
      }
   //loop p/ todas as Reducoes de VTMRede
   lisEXT = mrede->LisReducao();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      reducao = (VTReducao*)lisEXT->Items[n];
      if (ExisteGeracao(reducao)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracao(VTRede *rede)
   {
   //obtém lista de Geradores da Rede
   LisEqpto(rede, eqptoGERADOR);
   if (lisEQP->Count > 0) return(true);
   //obtém lista de Suprimentos da Rede
   LisEqpto(rede, eqptoSUPRIMENTO);
   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracaoReducaoVizinha(VTMRede *mrede)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtém Reducoes vizinhas da MRede
   lisEQP->Clear();
   mrede->LisReducaoVizinha(lisEQP);
   //loop p/ todas Reducoes
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reducao = (VTReducao*)lisEQP->Items[n];
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracaoReducaoVizinha(VTRede *mrede)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtém lista de Reducoes vizinhas da Rede em todos Cortes
   lisEQP->Clear();
   rede->LisReducaoVizinha(lisEQP);
   //loop p/ todas Reducoes
   for (int nr = 0; nr < lisEQP->Count; nr++)
      {
      reducao = (VTReducao*)lisEQP->Items[nr];
      //verifica se Reducao ativa (isto é, Rede não carregada)
      if (! reducao->Carregada) continue;
      //verifica se a Reducao possi geração
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracaoReducaoVizinha(VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtém Reducoes vizinhas da MRede
   lisEQP->Clear();
   mrede->LisReducaoVizinha(lisEQP);
   //loop p/ todas Reducoes
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reducao = (VTReducao*)lisEQP->Items[n];
      if (reducao->Corte != corte) continue;
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracaoReducaoVizinha(VTRede *rede, VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtém lista de Reducoes vizinhas da Rede no Corte
   lisEQP->Clear();
   corte->LisReducaoVizinha(rede, lisEQP);
   //loop p/ todas Reducoes
   for (int nr = 0; nr < lisEQP->Count; nr++)
      {
      reducao = (VTReducao*)lisEQP->Items[nr];
      //verifica se Reducao ativa (isto é, Rede não carregada)
      if (! reducao->Carregada) continue;
      //verifica se a Reducao possi geração
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TConsulta::ExisteGeracao(VTReducao *reducao)
   {
   return(reducao->ExisteGeracao());
   }
/*
//---------------------------------------------------------------------------
VTReducao* __fastcall TConsulta::ExisteReducao(VTRede *rede_mestre, VTRede *rede_reduzida)
   {
   //variáveis locais
   VTCorte   *corte;
   VTReducao *reducao;
   TList     *lisREDU;

   //obtém lista de Reducoes da Rede mestre
   lisREDU = rede_mestre->LisReducao();
   //analisa o Corte de cada Reducao
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      corte   = reducao->Corte;
      //verifica se a Rede rede_reduzida também possui uma Reducao p/ o mesmo Corte
      if ((reducao = rede_reduzida->ExisteReducao(corte)) != NULL) return(reducao);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTSuprimento* __fastcall TConsulta::ExisteSuprimento(VTReducao *reducao, VTBarra *pbarra)
   {
   //variáveis locais
   VTEqbar *eqbar;
   TList   *lisEQB;

   //obtém lista de Eqbar da Reducao
   lisEQB = reducao->LisEqbar();
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->Tipo() != eqptoSUPRIMENTO) continue;
      if (eqbar->pbarra == pbarra) return((VTSuprimento*)eqbar);
      }
   return(NULL);
   }
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqpto(VTRede *rede, int tipo)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado de VTRede
   (tipo < 0) ? rede->LisEqpto(lisEQP) : rede->LisEqpto(lisEQP, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqpto(VTReducao *reducao, int tipo)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado de VTReducao
   (tipo < 0) ? reducao->LisEqpto(lisEQP) : reducao->LisEqpto(lisEQP, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoEsquematicoReducao(VTRedes *redes)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //obtêm lista de objetos VTReducao que estão ativas (em lisEQP)
   LisReducao(redes);
   CopiaTList(lisEQP, lisTMP);
   lisEQP->Clear();
   //obtêm lista de objetos VEqpto dos objetos VTReducao cujas Redes são visíveis
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      //NÃO copia Barra fictícia
      //lisEQP->Add(reducao->BarraFicticia());
      CopiaTList(reducao->LisLigacaoEsquematico(), lisEQP);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoIsolado(VTRedes *redes, int tipo)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisEqptoIsolado(lisEQP) : redes->LisEqptoIsolado(lisEQP, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRede(VTRedes *redes, int tipo)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisEqpto(lisEQP) : redes->LisEqpto(lisEQP, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

/*
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTEqpto   *barra;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisREDE;
   TList     *lisBAR;

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtém lista de objetos VTRede
   lisREDE = redes->LisRede();
   //loop p/ toda VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se VTRede está visível
      if (! rede->Visible()) continue;
      //obtêm objetos VTEqpto do tipo indicado de VTRede
      (tipo < 0) ? rede->LisEqpto(lisEQP) : rede->LisEqpto(lisEQP, tipo);
      }
   //obtém objetos VTEqpto isolados
   redes->LisEqptoIsolado(lisEQP, tipo);
   //verifica se estão sendo solicitadas as Barras
   if (tipo == eqptoBARRA)
      {//inclui Barras de Corte das Reducoes visíveis
      lisTMP->Clear();
      redes->LisReducaoCarregada(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         reducao = (VTReducao*)lisTMP->Items[nr];
         if (! reducao->Visible()) continue;
         lisBAR = reducao->Corte->LisBarra();
         //insere em lisEQP as Barras de lisBAR
         for (int nb = 0; nb < lisBAR->Count; nb++)
            {
            barra = (VTEqpto*)lisBAR->Items[nb];
            if (lisEQP->IndexOf(barra) < 0) lisEQP->Add(barra);
            }
         }
      }
   //retorna lisEQP
   return(lisEQP);
   }
*/

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeVisible(VTRede *rede, int tipo)
   {
   //variáveis locais
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //verifica se VTRede está visível
   if (rede->Visible())
      {//obtêm objetos VTEqpto do tipo indicado de VTRede
      celulas->LisEqptoRedeAreaAtiva(lisEQP, rede, tipo);
      }
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTRede    *rede;
   TList     *lisREDE;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtém lista de objetos VTRede
   lisREDE = redes->LisRede();
   //loop p/ toda VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se VTRede está visível
      if (! rede->Visible()) continue;
      //obtêm objetos VTEqpto do tipo indicado de VTRede
      celulas->LisEqptoRedeAreaAtiva(lisEQP, rede, tipo);
      }
   //obtém objetos VTEqpto isolados
   rede = redes->RedeIsolada();
   //obtêm objetos VTEqpto do tipo indicado de VTRede
   celulas->LisEqptoRedeAreaAtiva(lisEQP, rede, tipo);
   /*
   redes->LisEqptoIsolado(lisEQP, tipo);
   //verifica se estão sendo solicitadas as Barras
   if (tipo == eqptoBARRA)
      {//inclui Barras de Corte das Reducoes visíveis
      lisTMP->Clear();
      redes->LisReducaoCarregada(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         reducao = (VTReducao*)lisTMP->Items[nr];
         if (! reducao->Visible()) continue;
         lisBAR = reducao->Corte->LisBarra();
         //insere em lisEQP as Barras de lisBAR
         for (int nb = 0; nb < lisBAR->Count; nb++)
            {
            barra = (VTEqpto*)lisBAR->Items[nb];
            if (lisEQP->IndexOf(barra) < 0) lisEQP->Add(barra);
            }
         }
      }
   */
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeReducao(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtêm lista de objetos VTReducao que estão ativas (em lisEQP)
   LisReducaoCarregada(redes);
   //transfere lisEQP p/ lisTMP
   CopiaTList(lisEQP, lisTMP);
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisEqpto(lisEQP) : redes->LisEqpto(lisEQP, tipo);
   //obtêm lista de objetos VEqpto dos objetos VTReducao
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      reducao->LisEqpto(lisEQP, tipo);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeReducao(VTRedes *redes, VTArea *area, int tipo)
   {
   //variáveis locais
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado localizados na Area
   celulas->LisEqptoArea(lisEQP, area, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

/*
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeReducaoVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //obtêm lista de objetos VTReducao que estão ativas (em lisEQP)
   LisReducaoCarregada(redes);
   CopiaTList(lisEQP, lisTMP);
   //obtém em lisEQP e Eqptos de VTRede que estão visíveis
   LisEqptoRedeVisible(redes, tipo);
   //inclui em lisEQP os objetos VEqpto das Reducoes visíveis e que estejam exibindo os Eqptos
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      if (reducao->Visible() && reducao->Show[rdu_EQPTOS]) reducao->LisEqpto(lisEQP, tipo);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }
*/

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeReducaoVisible(VTRedes *redes, int tipo)
   {
   //obtém em lisEQP os Eqptos de VTReducao que estão visíveis
   LisEqptoReducaoVisible(redes, tipo);
   //salva lisEQP em lisTMP
   lisTMP->Clear();
   CopiaTList(lisEQP, lisTMP);
   //obtém em lisEQP os Eqptos de VTRede que estão visíveis
   LisEqptoRedeVisible(redes, tipo);
   //inclui Eqptos de lisTMP em lisEQP
   CopiaTList(lisTMP, lisEQP);
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoRedeReducaoVisible(VTRedes *redes, VTArea *area, int tipo)
   {
   //variáveis locais
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado localizados na Area e cujas Redes estejam visíveis
   celulas->LisEqptoVisivelArea(lisEQP, area, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoReducao(TList *lisREDU, int tipo)
   {
   //variáveis locais
   VTReducao *reducao;

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTEqpto do tipo indicado da lista de objetos VTReducao
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      (tipo < 0) ? reducao->LisEqpto(lisEQP) : reducao->LisEqpto(lisEQP, tipo);
      }
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoReducaoCarregada(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTReducao *reducao;

   //obtêm lista de objetos VTReducao que estão ativas (em lisEQP)
   LisReducaoCarregada(redes);
   CopiaTList(lisEQP, lisTMP);
   lisEQP->Clear();
   //obtêm lista de objetos VEqpto dos objetos VTReducao
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      reducao->LisEqpto(lisEQP, tipo);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoReducaoVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //obtêm lista de objetos VTReducao que estão ativas (em lisEQP)
   LisReducaoCarregada(redes);
   CopiaTList(lisEQP, lisTMP);
   lisEQP->Clear();
   //obtêm lista de objetos VEqpto dos objetos VTReducao cujas Redes são visíveis
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      if (reducao->Visible()) reducao->LisEqpto(lisEQP, tipo);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }
*/

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoReducaoVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTReducao *reducao;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lisEQP
   lisEQP->Clear();
   //obtêm lista de objetos VTReducao que estão carregadas
   redes->LisReducaoCarregada(lisTMP);
   //obtêm lista de objetos VEqpto dos objetos VTReducao cujas Redes são visíveis
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      if (! reducao->Visible()) continue;
      //obtêm objetos VTEqpto do tipo indicado de VTReducao
      celulas->LisEqptoReducaoAreaAtiva(lisEQP, reducao, tipo);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisEqptoReducaoVisible(VTReducao *reducao, int tipo)
   {
   //variáveis locais
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lisEQP
   lisEQP->Clear();
   //verifica se a Reducao está visível
   if (reducao->Visible())
      {//obtêm objetos VTEqpto do tipo indicado de VTReducao
      celulas->LisEqptoReducaoAreaAtiva(lisEQP, reducao, tipo);
      }
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisGeradorSuprimento(VTRedes *redes)
   {
   //variáveis locais
   VTReducao *reducao;

   //reinicia lisEQP
   lisEQP->Clear();
   //obtém todos Geradores e Suprimentos das redes carregadas
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   redes->LisEqbar(lisEQP, eqptoGERADOR);
   //obtém todos Suprimentos das Reducoes carregadas
   lisTMP->Clear();
   redes->LisReducaoCarregada(lisTMP);
   //obtêm lista de objetos VTSuprimento dos objetos VTReducao
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      }
   //limpa lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisRede(VTRedes *redes, int tipo)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTRede do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisRede(lisEQP) : redes->LisRede(lisEQP, tipo);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisRedeCarregada(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTRede *rede;

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTRede do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisRede(lisEQP) : redes->LisRede(lisEQP, tipo);
   //mantém em lisEQP somente as redes carregadas
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      rede = (VTRede*)lisEQP->Items[n];
      if (! rede->Carregada) lisEQP->Delete(n);
      }
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisRedeNaoCarregada(VTRedes *redes, int tipo)
   {
   //variáveis locais
   VTRede *rede;

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTRede do tipo indicado de VTRedes
   (tipo < 0) ? redes->LisRede(lisEQP) : redes->LisRede(lisEQP, tipo);
   //mantém em lisEQP somente as Redes não carregadas
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      rede = (VTRede*)lisEQP->Items[n];
      if (rede->Carregada) lisEQP->Delete(n);
      }
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisRedeEsquematico(VTRedes *redes)
   {
   //variáveis locais
   VTEsquematico *esquematico = (VTEsquematico*)apl->GetObject(__classid(VTEsquematico));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //verifica se existe o objeto Esquematico
   if (esquematico != NULL)
      {//obtém todas as Redes do Esquematico
      esquematico->LisRede(lisEQP);
      }
   //retorna lisEQP
   return(lisEQP);
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisRedeEsquematicoArea(VTRedes *redes, VTArea *area)
   {
   //variáveis locais
   VTEsquematico *esquematico = (VTEsquematico*)apl->GetObject(__classid(VTEsquematico));


   //reinicia lista lisEQP
   lisEQP->Clear();
   //verifica se existe o objeto Esquematico
   if (esquematico != NULL)
      {//obtém as Redes do Esquematico que estão na área indicada
      esquematico->LisRedeArea(area, lisEQP);
      }
   //retorna lisEQP
   return(lisEQP);
   }
*/
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducao(VTMRede *mrede)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTReducao de VTRede
   mrede->LisReducao(lisEQP);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducao(VTRedes *redes)
   {
   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos VTReducao de VTRedes
   redes->LisReducao(lisEQP);
   //retorna lisEQP
   return(lisEQP);
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducao(VTRedes *redes, VTRede *rede_reduzida)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisREDE;
   TList     *lisREDU;

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtém lista de objetos VTRede de VTRedes
   lisREDE = redes->LisRede();
   //loop p/ todos objetos VTRede em lisREDE
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //descarta rede igual a rede_reduzida
      if (rede == rede_reduzida) continue;
      //loop p/ todas Reducoes da Rede
      lisREDU = rede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         if (reducao->RedeReduzida() == rede_reduzida)
            {//insere Reducao na lista lisEQP
            lisEQP->Add(reducao);
            }
         }
      }
   return(lisEQP);
   }
*/
//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducaoCarregada(VTRedes *redes)
   {
   //variáveis locais
   VTReducao *reducao;

   //reinicia listas lisEQP e lisTMP
   lisEQP->Clear();
   lisTMP->Clear();
   //obtêm objetos VTReducao de VTRedes
   redes->LisReducao(lisTMP);
   //transfere p/ lisEQP somente os objetos VTReducao cuja rede
   //NÃO esteja carregada
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      if (reducao->Carregada) lisEQP->Add(reducao);
      }
   //limpa lista lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducaoCarregadaArea(VTRedes *redes, VTArea *area)
   {
   //variáveis locais
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //reinicia lista lisEQP
   lisEQP->Clear();
   //obtêm objetos Reducao existentes na Area
   celulas->LisReducaoArea(lisEQP, area);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducaoNaoCarregada(VTRedes *redes)
   {
   //variáveis locais
   VTReducao *reducao;

   //reinicia listas lisEQP e lisTMP
   lisEQP->Clear();
   lisTMP->Clear();
   //obtém objetos VTReducao de VTRedes
   redes->LisReducao(lisTMP);
   //transfere p/ lisEQP somente os objetos VTReducao cuja Rede reduzida
   //esteja carregada
   for (int n = 0; n < lisTMP->Count; n++)
      {
      reducao = (VTReducao*)lisTMP->Items[n];
      if (! reducao->Carregada) lisEQP->Add(reducao);
      }
   //limpa lista lisTMP
   lisTMP->Clear();
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducaoVizinha(VTMRede *mrede, VTCorte *corte)
   {
   //reinicia listas lisEQP
   lisEQP->Clear();
   //obtém lista de Reducoes vizinhas da Rede no Corte
   corte->LisReducaoVizinha(mrede, lisEQP);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TConsulta::LisReducaoVizinha(VTMRede *mrede)
   {
   //reinicia listas lisEQP
   lisEQP->Clear();
   //obtém lista de Reducoes vizinhas da Rede
   mrede->LisReducaoVizinha(lisEQP);
   //retorna lisEQP
   return(lisEQP);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TConsulta::PatamarDemandaMaxima(VTRedes *redes)
   {
   //variáveis locais
   double      s_mva, s_max_mva = -1;
   VTCarga     *carga;
   VTPatamar   *patamar, *pat_dem_max = NULL;
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   //inicia liSEQP c/ todas Cargas
   LisEqptoRedeReducao(redes, eqptoCARGA);
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (! patamar->Enabled) continue;
      //determina demanda de todas Cargas do patamar
      s_mva = 0;
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga = (VTCarga*)lisEQP->Items[nc];
         s_mva += Abs(demanda->S_mva[carga][np]);
         }
      if (s_mva > s_max_mva)
         {
         s_max_mva   = s_mva;
         pat_dem_max = patamar;
         }
      }
   return(pat_dem_max);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TConsulta::PatamarDemandaMinima(VTRedes *redes)
   {
   //variáveis locais
   double      s_mva, s_min_mva = -1;
   VTCarga     *carga;
   VTPatamar   *patamar, *pat_dem_min = NULL;
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   //inicia liSEQP c/ todas Cargas
   LisEqptoRedeReducao(redes, eqptoCARGA);
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (! patamar->Enabled) continue;
      //determina demanda de todas Cargas do patamar
      s_mva = 0;
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga = (VTCarga*)lisEQP->Items[nc];
         s_mva += Abs(demanda->S_mva[carga][np]);
         }
      if (s_mva < s_min_mva)
         {
         s_min_mva   = s_mva;
         pat_dem_min = patamar;
         }
      }
   return(pat_dem_min);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TConsulta::PatamarFluxoMaximo(VTRedes *redes)
   {
   //variáveis locais
   double       p_mw, p_max_mw = -1;
   TList        *lisGS;
   VTEqpto      *eqpto;
   VTGerador    *gerador;
   VTSuprimento *suprimento;
   VTPatamar    *patamar, *pat_dem_max = NULL;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList        *lisPAT    = patamares->LisPatamar();

   //obtém lista de Geradores e Suprimentos
   lisGS = LisGeradorSuprimento(redes);
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (! patamar->Enabled) continue;
      //determina demanda de todas Cargas do patamar
      p_mw = 0;
      for (int n = 0; n < lisGS->Count; n++)
         {
         eqpto = (VTEqpto*)lisGS->Items[n];
         if (eqpto->Tipo() == eqptoSUPRIMENTO)
            {
            suprimento  = (VTSuprimento*)eqpto;
            p_mw       += suprimento->resflow->P_mw[np];
            }
         else if (eqpto->Tipo() == eqptoGERADOR)
            {
            gerador  = (VTGerador*)eqpto;
            p_mw    += gerador->resflow->P_mw[np];
            }
         }
      if (p_mw > p_max_mw)
         {
         p_max_mw   = p_mw;
         pat_dem_max = patamar;
         }
      }
   return(pat_dem_max);
   }

//---------------------------------------------------------------------------
int __fastcall TConsulta::QtdeEqptoRedeVisible(VTRedes *redes, int tipo)
   {
   //variáveis locais
   int       count = 0;
   VTRede    *rede;
   TList     *lisREDE;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //obtém lista de objetos VTRede
   lisREDE = redes->LisRede();
   //loop p/ toda VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se VTRede está visível
      if (! rede->Visible()) continue;
      //obtêm objetos VTEqpto do tipo indicado de VTRede
      count += celulas->QtdeEqptoRedeAreaAtiva(rede, tipo);
      }
   //obtém objetos VTEqpto isolados
   rede = redes->RedeIsolada();
   //obtêm objetos VTEqpto do tipo indicado de VTRede
   count += celulas->QtdeEqptoRedeAreaAtiva(rede, tipo);
   //retorna count
   return(count);
   }

//---------------------------------------------------------------------------
//eof

