//---------------------------------------------------------------------------
#ifndef TFormCustosH
#define TFormCustosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormCustos : public TForm
{
__published:	// IDE-managed Components public:		// User declarations
   TListView *ListView;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionSelObra;
   TImageList *ImageList1;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);

public:
              __fastcall  TFormCustos(TComponent* Owner, VTApl *apl);
              __fastcall ~TFormCustos(void);
   void       __fastcall  LeArquivoObrasCustos(AnsiString arq_txt);
   AnsiString __fastcall  ObraCodigo(void);
   double     __fastcall  ObraCusto(void);

private:	//objetos externos     métodos
   int __fastcall ExtraiCampos(AnsiString linha);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   TStrings *campos;
   TStrings *lines;
};

//---------------------------------------------------------------------------
#endif
