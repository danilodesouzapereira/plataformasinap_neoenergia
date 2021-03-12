//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifTopo.h"
#include "TFormGraf.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifTopo(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifTopo(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
__fastcall TUnifTopo::TUnifTopo(VTApl *apl, int tipo_graf, AnsiString nome)
                     :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawTopo
   if (draw) delete draw;
   draw = NewObjDrawTopo(apl);
   }

//---------------------------------------------------------------------------
__fastcall TUnifTopo::~TUnifTopo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TUnifTopo::Redraw(VTEdt *editor)
   {
   //desabilita animação
   animacao->Stop();
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //atualiza gráfico
   form->RedrawTelaReal();
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifTopo::Redraw(int op, VTEdt *editor)
   {
   //utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//---------------------------------------------------------------------------
//eof

