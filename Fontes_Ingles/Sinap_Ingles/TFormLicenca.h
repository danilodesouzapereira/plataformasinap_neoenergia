//---------------------------------------------------------------------------
#ifndef TFormLicencaH
#define TFormLicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormLicenca : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEdit;
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TImageList *ImageList;
   void __fastcall ButFechaClick(TObject *Sender);

public:		// User declarations
   __fastcall  TFormLicenca(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormLicenca(void);

private:	//métodos
   void __fastcall DadosDaLicenca(void);

private:	//objetos externos
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
