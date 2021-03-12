//---------------------------------------------------------------------------
#ifndef TFormReclamacaoBTH
#define TFormReclamacaoBTH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include "TFormAnaliseRT.h"
#include "EstruturaMed.h"
#include <System.ImageList.hpp> //novo FKM 25.03.2015
//---------------------------------------------------------------------------
class VTApl;
class VTAjusta;
class VTBarra;
class VTCarga;
class VTEdt;
class VTEqpto;
class VTEstudo;
class VTLicenca;
class VTMedicao;
class VTMulta;
class VTRede;
class VTRes505;
class VTSuprimento;
//---------------------------------------------------------------------------
class TFormReclamacaoBT : public TFormEdt
{
__published:   // IDE-managed Components
   TPanel *Panel1;
   TSplitter *Splitter1;
   TGroupBox *GBdadosRT;
   TLabel *Label1;
   TLabel *Label3;
   TEdit *EditConsMed;
   TButton *ButConsMed;
   TButton *Button1;
   TGroupBox *GBanalise;
   TLabel *Label2;
   TComboBox *CBoxConsumidores;
   TBitBtn *BitBtnAnalise;
   TPanel *PanelEstudo;
   TOpenDialog *OpenDialogMed;
   TActionList *ActionList1;
   TAction *ActionAnalise;
   TAction *ActionEditaObra;
   TAction *ActionTUSD;
   TAction *ActionSelEqpto;
   TImageList *ImageList1;
   void __fastcall ButConsMedClick(TObject *Sender);
   void __fastcall CBoxConsumidoresChange(TObject *Sender);
   void __fastcall ActionAnaliseExecute(TObject *Sender);
   void __fastcall ActionEditaObraExecute(TObject *Sender);
   void __fastcall ActionTUSDExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormReclamacaoBT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormReclamacaoBT(void);

public:
   bool inicializado;

private:   //métodos
   VTBarra*       __fastcall BarraInicialRedeSec(void);
   VTCarga*       __fastcall DeterminaCargaMedicao(AnsiString codigo);
   int            __fastcall DeterminaFasesRede(void);
   bool           __fastcall ExportaRelatorio(AnsiString nome_arq);
   bool           __fastcall ImportaMedicao(void);
   bool           __fastcall Inicia(void);
//   bool             __fastcall LeArqMedicao(AnsiString arquivo);
   bool           __fastcall LeArqMedicao(AnsiString arquivo, strMEDIDOR *medidor); //novo FKM
   void           __fastcall ListaCargas(void);
   void           __fastcall Moldura(VTEqpto *eqpto);
   void           __fastcall PreencheCBoxConsumidores(void);
   bool           __fastcall PreparaRede(void);
   bool           __fastcall SalvaEstudoBase(VTEstudo *estudo);
   TWinControl*   __fastcall SelecionaParentParaJanela(void);
   bool           __fastcall TrataReducoes(void);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   TForm          *FormEstudo;
   TList          *lisEQP;
   TList          *lisCARGA;
   VTAjusta       *ajusta;
   VTCarga        *carga_analisada;
   VTEdt          *edt;
   VTLicenca      *licenca;
   VTMedicao      *medicao;
   VTMulta        *multa;
   VTRes505       *res505;
   VTRede         *rede_sec;
   VTSuprimento   *supr;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReclamacaoBT *FormReclamacaoBT;
//---------------------------------------------------------------------------
#endif
