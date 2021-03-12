//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TUnifTopobp.h"
#include "VTDraw.h"
#include "TPadrao.h"
#include "TFormGraf.h"
#include "VTGrafico.h"
#include "VTGrid.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifTopobp(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifTopobp(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifTopobp::TUnifTopobp() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifTopobp::TUnifTopobp(VTApl *apl, int tipo_graf, AnsiString nome)
                       :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawFlow
   if (draw) delete draw;
   draw = NewObjDrawTopo(apl);
   //inicia dados locais
   cor_fundo = clWhite;
   cor_eqpto = clBlack;

   }

//---------------------------------------------------------------------------
// TUnifTopobp::~TUnifTopobp() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifTopobp::~TUnifTopobp(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
TColor __fastcall TUnifTopobp::Color(VTEqpto *eqpto, TColor cor)
   {
   return(cor_eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TUnifTopobp::LimpaTela(void)
   {
   //variáveis locais
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //limpa a tela
   form->LimpaTela(cor_fundo, false);
   }

//---------------------------------------------------------------------------
void __fastcall TUnifTopobp::Redraw(VTEdt *editor)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //atualiza gráfico
   form->RedrawTelaReal();
   }

//---------------------------------------------------------------------------
void __fastcall TUnifTopobp::Redraw(int op, VTEdt *edt)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(op, edt);
   }

//---------------------------------------------------------------------------
//eof

