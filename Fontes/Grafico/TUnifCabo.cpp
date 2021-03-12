//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifCabo.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TFormGraf.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTTrecho.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifCabo(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifCabo(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifCabo::TUnifCabo() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifCabo::TUnifCabo(VTApl *apl, int tipo_graf, AnsiString nome)
                      :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawTopo(apl);
   }

//---------------------------------------------------------------------------
// TUnifCabo::~TUnifCabo() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifCabo::~TUnifCabo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TUnifCabo::Redraw(VTEdt *editor)
   {
   //desabilita animação
   animacao->Stop();
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //atualiza gráfico
   form->RedrawTelaReal();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::Redraw(int op, VTEdt *editor)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllBarra(void)
   {
   //variáveis locais
      VTBarra *pbarra;
   TList   *lisBAR;

   //determina lista de Barras das Redes
   if ((lisBAR = LisEqptoRede(eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisBAR->Count; nbar++)
         {
         pbarra = (VTBarra*)lisBAR->Items[nbar];
         draw->Barra(pbarra, clDkGray);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllChave(void)
   {
   //variáveis locais
   VTChave *pchave;
   TList   *lisCHV;

   //determina lista de Chaves
   if ((lisCHV = LisEqptoRede(eqptoCHAVE)) != NULL)
      {//loop p/ todas as Chaves
      for (int nchv = 0; nchv < lisCHV->Count; nchv++)
         {
         pchave = (VTChave*)lisCHV->Items[nchv];
         draw->Chave(pchave, clDkGray);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllLigacao(bool chave_enabled)
   {
   RedrawAllChave();
   RedrawAllTrecho();
   RedrawAllTrafo();
   RedrawAllTrafo3E();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllTrafo(void)
   {
   //variáveis locais
   VTTrafo *ptrafo;
   TList   *lisTRF;

   //determina lista de Trafos
   if ((lisTRF = LisEqptoRede(eqptoTRAFO)) != NULL)
      {//loop p/ todos Trafos
      for (int ntrf = 0; ntrf < lisTRF->Count; ntrf++)
         {
         ptrafo = (VTTrafo*)lisTRF->Items[ntrf];
         draw->Trafo(ptrafo, clDkGray);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllTrafo3E(void)
   {
   //variáveis locais
   VTTrafo3E *ptrf3e;
   TList     *lisT3E;

   //determina lista de Trafo3E
   if ((lisT3E = LisEqptoRede(eqptoTRAFO3E)) != NULL)
      {//loop p/ todas os Trafo3E
      for (int ntrf = 0; ntrf < lisT3E->Count; ntrf++)
         {
         ptrf3e = (VTTrafo3E*)lisT3E->Items[ntrf];
         draw->Trafo3E(ptrf3e, clDkGray);
         }
      }
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllTrecho(void)
   {
   //variáveis locais
   VTTrecho   *trecho;
   TList      *lisEQP;

   //determina lista de Trechos
   if ((lisEQP = LisEqptoRede(eqptoTRECHO)) != NULL)
      {//loop p/ todos Capserie
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trecho = (VTTrecho*)lisEQP->Items[n];
         //verifica se deve apresentar Trecho do Arranjo
         if (trecho->arranjo->Show[eqp_VISIBLE])
            {
            draw->LinhasLigacao(trecho, trecho->arranjo->Color);
            }
         }
      }
   }
*/

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllTrecho(void)
   {
   //variáveis locais
   unsigned   show_orig;
   VTArranjo  *arranjo;
   VTTrecho   *trecho;
   TList      *lisEQP;

   //determina lista de Trechos
   if ((lisEQP = LisEqptoRede(eqptoTRECHO)) != NULL)
      {//loop p/ todos Trechos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trecho  = (VTTrecho*)lisEQP->Items[n];
         arranjo = trecho->arranjo;
         if (arranjo->Show[eqp_VISIBLE])
            {//salva configuração de atributos do Trecho
            show_orig = trecho->ShowAsUnsigned;
            //atualiza atributos do Trecho
            trecho->ShowAsUnsigned = arranjo->ShowAsUnsigned &
                                     (eqp_VISIBLE | eqp_FIXO | tre_ARRANJO | tre_ZSEQ_OHM | tre_ZSEQ_PU);
            //desenha o Trecho
            draw->Trecho(trecho, trecho->arranjo->Color);
            //restaura configuração de atributos do Trecho
            trecho->ShowAsUnsigned = show_orig;
            }
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCabo::RedrawAllEqbar(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof

