//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifFase.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TFormGraf.h"
#include "TPadrao.h"
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
VTUnif* NewObjUnifFase(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifFase(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifFase::TUnifFase() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifFase::TUnifFase(VTApl *apl, int tipo_graf, AnsiString nome)
                      :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawTopo(apl);
   }

//---------------------------------------------------------------------------
// TUnifFase::~TUnifFase() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifFase::~TUnifFase(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TUnifFase::Redraw(VTEdt *editor)
   {
   //desabilita animação
   animacao->Stop();
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //atualiza gráfico
   form->RedrawTelaReal();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFase::Redraw(int op, VTEdt *editor)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFase::RedrawAllBarra(void)
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
void __fastcall TUnifFase::RedrawAllEqbar(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFase::RedrawAllChave(void)
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
void __fastcall TUnifFase::RedrawAllLigacao(bool chave_enabled)
   {
   RedrawAllChave();
   RedrawAllTrecho();
   RedrawAllTrafo();
   RedrawAllTrafo3E();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFase::RedrawAllTrafo(void)
   {
   //variáveis locais
   int        num_fase;
   TColor     color;
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
void __fastcall TUnifFase::RedrawAllTrafo3E(void)
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

//-----------------------------------------------------------------------------
void __fastcall TUnifFase::RedrawAllTrecho(void)
   {
   //variáveis locais
   int        dim_orig;
   unsigned   show_orig;
   VTArranjo  *arranjo;
   VTTrecho   *trecho;
   TList      *lisTRE;

   //salva dimensão original p/ trechos
   dim_orig = padrao->ligacao.dim;
   //determina lista de Trechos
   if ((lisTRE = LisEqptoRede(eqptoTRECHO)) != NULL)
      {//loop p/ todos Trechos
      for (int n = 0; n < lisTRE->Count; n++)
         {
         trecho = (VTTrecho*)lisTRE->Items[n];
         arranjo = trecho->arranjo;
         if (arranjo->Show[eqp_VISIBLE])
            {//salva configuração de atributos do Trecho
            show_orig = trecho->ShowAsUnsigned;
            //atualiza atributos do Trecho
            trecho->ShowAsUnsigned = arranjo->ShowAsUnsigned &
                                     (eqp_VISIBLE | eqp_FIXO | tre_ARRANJO | tre_FASES);
            //ajusta dimensão da linha em função da existência de neutro
            padrao->ligacao.dim = ((trecho->arranjo->Fases & faseN) == faseN) ? 2 : 1;
            //desenha o Trecho
            draw->Trecho(trecho, trecho->arranjo->Color);
            //restaura configuração de atributos do Trecho
            trecho->ShowAsUnsigned = show_orig;
            }
         }
      }
   //restaura dimensão original p/ trechos
   padrao->ligacao.dim = dim_orig;
   }

//---------------------------------------------------------------------------
//eof

