//---------------------------------------------------------------------------
#ifndef TFormEditaRedeH
#define TFormEditaRedeH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormEditaRede : public TForm
{
__published:	// IDE-managed Components
   TOpenDialog *OpenDialog;
   TSaveDialog *SaveDialogRede;

private:	// User declarations
   AnsiString    __fastcall ImportaArquivo(void);

public:		// User declarations
        __fastcall TFormEditaRede(TComponent* Owner, VTApl *apl_owner);
        __fastcall ~TFormEditaRede(void);
   bool __fastcall EditaSED(void);
   bool __fastcall EquivalentaBT(void);
   bool __fastcall InsereFusivelTrafos(void);
   bool __fastcall RemoveBT(void);
   bool __fastcall RemoveTrechos(void);

private: //obj. externo
   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
