//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Figura\VTRetangulo.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include "TFormEdtRetangulo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtRetangulo::TFormEdtRetangulo(TComponent* Owner, VTApl *apl_owner, VTRetangulo *retangulo)
   : TForm(Owner)
   {
   //salva ponteiros
   this->apl        = apl_owner;
   this->retangulo = retangulo;
   //exibe dados do Retangulo
   ExibeDados();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtRetangulo::~TFormEdtRetangulo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados do Retangulo
   if (! LeDados()) return;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Retangulo
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ActionSelCorBordaExecute(TObject *Sender)
   {
   //inicia FontDialog com o Font do Retangulo
   ColorDialog->Color = PanelBordaCor->Color;
   //exibe FontDialog
   if (ColorDialog->Execute())
      {
      PanelBordaCor->Color = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ActionSelCorFundoExecute(TObject *Sender)
   {
   //inicia FontDialog com o Font do Retangulo
   ColorDialog->Color = PanelFundoCor->Color;
   //exibe FontDialog
   if (ColorDialog->Execute())
      {
      PanelFundoCor->Color = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::CBoxBordaEspessuraChange(TObject *Sender)
   {
   //qualquer espessura diferente de 1 exige borda psSolid
   if (CBoxBordaEspessura->ItemIndex != 0)
      {
      CBoxBordaEstilo->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::CBoxBordaEstiloChange(TObject *Sender)
   {
   //qualquer estilo diferente de psSolid exige espessura 1
   if (CBoxBordaEstilo->ItemIndex != 0)
      {
      CBoxBordaEspessura->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::ExibeDados(void)
   {
   //exibe cor da borda e do fundo
   PanelFundoCor->Color = retangulo->FundoCor;
   PanelBordaCor->Color = retangulo->BordaCor;
   //exibe indicação de fundo transparente
   CheckBoxTransparente->Checked = retangulo->Transparente;
   //exibe espessura da borda
   CBoxBordaEspessura->ItemIndex = retangulo->BordaEspessura - 1;
   //exibe estilo da borda
   switch(retangulo->BordaEstilo)
      {
      case psSolid:      CBoxBordaEstilo->ItemIndex = 0; break;
      case psDash:       CBoxBordaEstilo->ItemIndex = 1; break;
      case psDot:        CBoxBordaEstilo->ItemIndex = 2; break;
      case psDashDot:    CBoxBordaEstilo->ItemIndex = 3; break;
      case psDashDotDot: CBoxBordaEstilo->ItemIndex = 4; break;
      case psClear:      CBoxBordaEstilo->ItemIndex = 5; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRetangulo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtRetangulo::LeDados(void)
   {
   //variáveis locais
   int        width, height, dx, dy;

   //define cor de fundo
   retangulo->FundoCor     = PanelFundoCor->Color;
   retangulo->Transparente = CheckBoxTransparente->Checked;
   //define atributos da borda
   retangulo->BordaCor       = PanelBordaCor->Color;
   retangulo->BordaEspessura = CBoxBordaEspessura->ItemIndex + 1;
   switch(CBoxBordaEstilo->ItemIndex)
      {
      case 0:  retangulo->BordaEstilo = psSolid;      break;
      case 1:  retangulo->BordaEstilo = psDash;       break;
      case 2:  retangulo->BordaEstilo = psDot;        break;
      case 3:  retangulo->BordaEstilo = psDashDot;    break;
      case 4:  retangulo->BordaEstilo = psDashDotDot; break;
      case 5:  retangulo->BordaEstilo = psClear;      break;
      default: retangulo->BordaEstilo = psSolid;      break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

