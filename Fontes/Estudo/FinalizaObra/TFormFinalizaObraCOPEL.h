//---------------------------------------------------------------------------

#ifndef TFormFinalizaObraCOPELH
#define TFormFinalizaObraCOPELH
#include "TFormFinalizaObraARIES.h"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
#include <map>
#include "TFormFinalizaObraARIES.h"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormFinalizaObraCOPEL : public TFormFinalizaObraARIES
{
__published:	// IDE-managed Components
	TGroupBox *GBoxPrograma;
	TComboBox *CBoxPrograma;
	TGroupBox *GBoxObrEsp;
	TEdit *EditObEspecial;
	TGroupBox *GBoxMunicioPrincipal;
	TComboBox *CBoxVPO;
	TGroupBox *GBoxMunicipios;
	TSplitter *Splitter2;
	TGroupBox *GBoxObra;
	TComboBox *CBoxObra;
	TGroupBox *GBoxRegiao;
	TComboBox *CBoxRegiao;
	TPanel *Panel1;
	TGroupBox *GBoxCroqui;
	TAction *ActionCriarCroqui;
	TAction *ActionCarregarCroqui;
	TAction *ActionLimparCroqui;
	TAction *ActionVisualizarCroqui;
	TToolBar *ToolBar1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TEdit *EditCroqui;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog;
	TCheckListBox *CheckListBoxMunicipios;
	TToolBar *ToolBarCheckMunicipios;
	TBitBtn *ButSubestacaoSelNone;
	TAction *ActionSelNoneMunicpios;
	TGroupBox *GBoxGrupo;
	TEdit *EditGrupo;
	void __fastcall ActionCriarCroquiExecute(TObject *Sender);
	void __fastcall ActionCarregarCroquiExecute(TObject *Sender);
	void __fastcall ActionLimparCroquiExecute(TObject *Sender);
	void __fastcall ActionVisualizarCroquiExecute(TObject *Sender);
	void __fastcall ActionSelNoneMunicpiosExecute(TObject *Sender);
	void __fastcall CheckListBoxMunicipiosClickCheck(TObject *Sender);
	void __fastcall CBoxVPOChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
			__fastcall TFormFinalizaObraCOPEL(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
	virtual __fastcall ~TFormFinalizaObraCOPEL(void);

protected:
// jss20191211
//      método transportado para o ToolKit da COPEL
//      ao final do carregamento da BD
//			bool __fastcall CarregaMunicipios(void);
			void __fastcall CBoxObraInicia(void);
			void __fastcall CBoxProgramaInicia(void);
			void __fastcall CBoxRegiaoInicia(void);
			void __fastcall CBoxVPOInicia(void);
			void __fastcall CheckListMunicipioInicia(void);
			bool __fastcall CriaCroqui(AnsiString filepath);
	virtual void __fastcall ConfirmaItemProprio(void);
	virtual bool __fastcall ConfirmaObraPropria(void);
	virtual void   __fastcall EnableDetalheItem(bool enable);
            bool __fastcall InsereAtributoIPE(AnsiString texto, int tipoAtributoIPE);
			void __fastcall MostraCroqui(AnsiString filepath);
			bool __fastcall PreencheInfoObra(VTObra *obra);
			bool __fastcall PreencheInfoItemObra(VTNodeDetalhe *nodeDet);
			void __fastcall PreencheMapsCOPEL(void);
			void __fastcall SelecionaAtributoObra(AnsiString obraCopel);
			void __fastcall SelecionaAtributoRegiao(AnsiString regiao);
			void __fastcall SelecionaGrupo(AnsiString num_grupo);
			void __fastcall SelecionaMunicipioExtra(AnsiString nomeMunicipio);
			void __fastcall SelecionaMunicipioPrincipal(AnsiString nomeMunicipio);
			void __fastcall SelecionaRegiao(AnsiString nomeMunicipio);
			void __fastcall SelecionaPrograma(AnsiString programa);
			bool __fastcall ValidaInputObra(void);
			bool __fastcall ValidaInputItemObra(void);
			//validacoes proprias
			bool __fastcall ValidaCroqui(void);
			bool __fastcall ValidaGrupo(void);
			bool __fastcall ValidaObraCOPEL(void);
			bool __fastcall ValidaObrigacaoEspecial(double valor);
			bool __fastcall ValidaMunicipioPrincipal(void);
			bool __fastcall ValidaMunicipiosExtras(void);
			bool __fastcall ValidaProgramaCOPEL(void);
			bool __fastcall ValidaRegiaoCOPEL(void);

private:
	std::map<AnsiString,int> mapObraCOPEL;
	std::map<AnsiString,int> mapProgramaCOPEL;
	std::map<AnsiString,int> mapRegiaoCOPEL;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFinalizaObraCOPEL *FormFinalizaObraCOPEL;
//---------------------------------------------------------------------------
#endif
