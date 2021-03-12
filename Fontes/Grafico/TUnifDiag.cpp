//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifDiag.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "VTDiagEqpto.h"
#include "VTDiagRede.h"
#include "TFormGraf.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTUnif* NewObjUnifDiag(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifDiag(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
__fastcall TUnifDiag::TUnifDiag(VTApl *apl, int tipo_graf, AnsiString nome)
                     :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawCurto(apl);
   }

//---------------------------------------------------------------------------
__fastcall TUnifDiag::~TUnifDiag(void)
   {
   //nada a fazer
	}

//---------------------------------------------------------------------------
void __fastcall TUnifDiag::Redraw(VTEdt *editor)
	{
   //obtém objeto VTDiagRede
   diag_rede = (VTDiagRede*)apl->GetObject(__classid(VTDiagRede));
	//desabilita animação
	animacao->Stop();
	//utiliza método da classe base
	TUnifEditor::Redraw(editor);
	//atualiza gráfico
	form->RedrawTelaReal();
	}

//-----------------------------------------------------------------------------
void __fastcall TUnifDiag::Redraw(int op, VTEdt *editor)
   {
   //obtém objeto VTDiagRede
   diag_rede = (VTDiagRede*)apl->GetObject(__classid(VTDiagRede));
	//utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifDiag::RedrawAllBarra(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifDiag::RedrawAllLigacao(bool chave_enabled)
   {
   //variáveis locais
   TList       *lisDIAG_EQPTO;
   VTDiagEqpto *diag_eqpto;

   //proteção
   if (diag_rede == NULL) return;
   //obtém lista de DiagEqpto
   lisDIAG_EQPTO = diag_rede->LisDiagEqpto();
   //desenha todas as Ligacoes
   for (int n = 0; n < lisDIAG_EQPTO->Count; n++)
      {
      diag_eqpto = (VTDiagEqpto*)lisDIAG_EQPTO->Items[n];
      if (diag_eqpto->Eqpto->TipoLigacao())
         {
         //draw->LinhasLigacao((VTLigacao*)diag_eqpto->Eqpto, diag_eqpto->Color);
         draw->Ligacao((VTLigacao*)diag_eqpto->Eqpto, diag_eqpto->Color);
         }
      }
	}

//-----------------------------------------------------------------------------
void __fastcall TUnifDiag::RedrawAllEqbar(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifDiag::RedrawAllMutua(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof

