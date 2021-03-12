//---------------------------------------------------------------------------

#ifndef TFormFigurasIOH
#define TFormFigurasIOH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTFigurasIO;
class VTEstudo;
//---------------------------------------------------------------------------
class TFormFigurasIO : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheetImportar;
	TTabSheet *TabSheetExportar;
	TButton *Button2;
	TPanel *Panel2;
	TLabel *Label2;
	TEdit *EditFilename;
	TButton *Button1;
	TPanel *Panel1;
	TImageList *ImageList;
	TFileOpenDialog *FileOpenDialog;
	TButton *Button3;
	TSaveDialog *SaveDialog;
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	bool __fastcall ValidaDiretorio(AnsiString filepath);
public:		// User declarations
	__fastcall TFormFigurasIO(TComponent* Owner, VTApl *apl);
    __fastcall ~TFormFigurasIO(void);

private: //externo
	VTApl *apl;
private: //interno
    VTEstudo *estudo;
	VTFigurasIO *figuraIO;
	AnsiString filenameImp;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFigurasIO *FormFigurasIO;
//---------------------------------------------------------------------------
#endif
