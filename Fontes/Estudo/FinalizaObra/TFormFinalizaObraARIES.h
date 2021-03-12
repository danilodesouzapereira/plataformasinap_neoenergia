//---------------------------------------------------------------------------

#ifndef TFormFinalizaObraARIESH
#define TFormFinalizaObraARIESH
//---------------------------------------------------------------------------
#include "TFormFinalizaObra.h"
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
//---------------------------------------------------------------------------
class VTPadrao;
//---------------------------------------------------------------------------
class TFormFinalizaObraARIES : public TFormFinalizaObra
{
__published:	// IDE-managed Components
	TGroupBox *GBoxInsercao;
	TGroupBox *GBoxPadraoIns;
	TComboBox *CBoxPadraoIns;
	TPanel *PanelQntInsercao;
	TLabel *LabelQntInsercao;
	TEdit *EditQntInsercao;
	TGroupBox *GBoxRetirada;
	TPanel *PanelQntRetirada;
	TLabel *LabelQntRetirada;
	TEdit *EditQntRetirada;
	TGroupBox *GBoxPadraoRet;
	TComboBox *CBoxPadraoRet;
	TPanel *PanelCusto;
	TLabel *LabelCusto;
	TEdit *EditCusto;
	TPanel *PanelCustoItem;
	TLabel *LabelCustoItem;
	TEdit *EditCustoItem;
	void __fastcall EditQntInsercaoExit(TObject *Sender);
	void __fastcall EditQntRetiradaExit(TObject *Sender);
	void __fastcall CBoxPadraoInsChange(TObject *Sender);
	void __fastcall CBoxPadraoRetChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
			__fastcall TFormFinalizaObraARIES(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
	virtual __fastcall ~TFormFinalizaObraARIES(void);
protected:
			void __fastcall AtualizaCustosTotal(void);
			void __fastcall AtualizaCustosItem(void);
			void __fastcall AtualizaQuantidade(TEdit *edit);
	virtual void __fastcall ConfirmaItemProprio(void);
	virtual bool __fastcall ConfirmaObraPropria(void);
	virtual void __fastcall EnableDetalheItem(bool enable);
			void __fastcall EscolhePadraoConstrutivo(TComboBox *combo, VTPadrao *padrao);
			void __fastcall MudaPadraoConstrutivo(VTPadrao *antes, VTPadrao *depois, bool insercao);
	AnsiString   __fastcall FormataValorMonetario(double valor);
			void __fastcall PreencheCombosPadrao(VTNodeDetalhe *nodeDet);
	virtual bool __fastcall PreencheInfoItemObra(VTNodeDetalhe *nodeDet);
    virtual bool __fastcall PreencheInfoObra(VTObra *obra);
//			bool __fastcall PadraoesTemTipoEqptoIguais(VTPadrao *padrao1, VTPadrao *padrao2);

	//validacoes
    virtual bool __fastcall ValidaInputObra(void);
	virtual bool __fastcall ValidaInputItemObra(void);
	//validacoes proprias
            bool __fastcall ValidaDouble(AnsiString string);
			bool __fastcall ValidaPadroesConstrutivos(void);
		//	bool __fastcall ValidaPadraoConstrutivo(TComboBox *combo);
			bool __fastcall ValidaQuantidade(TEdit *edt);
			bool __fastcall ValidaQuantidades(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormFinalizaObraARIES *FormFinalizaObraARIES;
//---------------------------------------------------------------------------
#endif
