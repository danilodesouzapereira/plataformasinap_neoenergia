//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Figura\VTFiguraComentario.h>
#include <Fontes\Figura\VTRetangulo.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include "TFormEdtComentario.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
__fastcall TFormEdtComentario::TFormEdtComentario(TComponent* Owner, VTApl *apl_owner, VTFiguraComentario *comentario)
   : TForm(Owner)
   {
   //salva ponteiros
   this->apl        = apl_owner;
   this->comentario = comentario;
   //inicia CBoxRede
   CBoxRedeInicia(comentario->Rede);
   //exibe dados do Comentario
   ExibeDados();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtComentario::~TFormEdtComentario(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados do Comentario
   if (! LeDados()) return;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Comentario
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionSelCorBordaExecute(TObject *Sender)
   {
   //inicia FontDialog com o Font do Comentario
   ColorDialog->Color = PanelBordaCor->Color;
   //exibe FontDialog
   if (ColorDialog->Execute())
      {
      PanelBordaCor->Color = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionSelCorFundoExecute(TObject *Sender)
   {
   //inicia FontDialog com o Font do Comentario
   ColorDialog->Color = PanelFundoCor->Color;
   //exibe FontDialog
   if (ColorDialog->Execute())
      {
      PanelFundoCor->Color = ColorDialog->Color;
      Memo->Color          = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ActionSelFonteExecute(TObject *Sender)
   {
   //inicia FontDialog com o Font do Comentario
   FontDialog->Font->Assign(Memo->Font);
   //exibe FontDialog
   if (FontDialog->Execute())
      {
      Memo->Font->Assign(FontDialog->Font);
      Memo->Refresh();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::CBoxBordaEspessuraChange(TObject *Sender)
   {
   //qualquer espessura diferente de 1 exige borda psSolid
   if (CBoxBordaEspessura->ItemIndex != 0)
      {
      CBoxBordaEstilo->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::CBoxBordaEstiloChange(TObject *Sender)
   {
   //qualquer estilo diferente de psSolid exige espessura 1
   if (CBoxBordaEstilo->ItemIndex != 0)
      {
	  CBoxBordaEspessura->ItemIndex = 0;
	  CBoxBordaEspessura->Enabled = false;
	  }
   else
	  {
	  CBoxBordaEspessura->Enabled = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::CBoxRedeInicia(VTRede *rede_ref)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBoxRede
   CBoxRede->Clear();
   //insere opção de não selecionar nenhuma rede
   CBoxRede->Items->AddObject("Nenhuma rede", NULL);
   for (int n = 0; n < lisREDE->Count; n++)
      {
	  rede = (VTRede*)lisREDE->Items[n];
	  if (!rede->Carregada)
	  {
        continue;
	  }
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //pré-seleciona Rede indicada
   if (rede_ref != NULL) CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(rede_ref);
   if (CBoxRede->ItemIndex < 0) CBoxRede->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::ExibeDados(void)
   {
   //variáveis locais
   int x1, y1, x2, y2, dx, dy;
   int folga;

   //exibe formato
   switch(comentario->Formato)
      {
	  case fmtRETANGULO: RadioGroupFormato->ItemIndex = 0; break;
//      case fmtELIPSE:    RadioGroupFormato->ItemIndex = 1; break;
	  case fmtFLAG:      RadioGroupFormato->ItemIndex = 1; break;
      default:           RadioGroupFormato->ItemIndex = 0; break;
      }
   //exibe cor da borda e do fundo
   PanelFundoCor->Color = comentario->Retangulo->FundoCor;
   PanelBordaCor->Color = comentario->Retangulo->BordaCor;
   //exibe espessura da borda
   CBoxBordaEspessura->ItemIndex = comentario->Retangulo->BordaEspessura - 1;
   //exibe estilo da borda
   switch(comentario->Retangulo->BordaEstilo)
      {
	  case psSolid:      CBoxBordaEstilo->ItemIndex = 0; break;
	  case psDash:       CBoxBordaEstilo->ItemIndex = 1; break;
      case psDot:        CBoxBordaEstilo->ItemIndex = 2; break;
      case psDashDot:    CBoxBordaEstilo->ItemIndex = 3; break;
      case psDashDotDot: CBoxBordaEstilo->ItemIndex = 4; break;
	  case psClear:      CBoxBordaEstilo->ItemIndex = 5; break;
	  }
   //só permite espessuras diferentes de 1 no caso psSolid
   if (CBoxBordaEstilo->ItemIndex == 0)
	  {CBoxBordaEspessura->Enabled = true;}
   else
	  {
      CBoxBordaEspessura->ItemIndex = 0;
	  CBoxBordaEspessura->Enabled = false;
	  }
   //exibe texto
   Memo->Color = comentario->Retangulo->FundoCor;
   Memo->Font  = comentario->Font;
   Memo->Lines->AddStrings(comentario->Texto());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtComentario::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtComentario::LeDados(void)
   {
   //variáveis locais
   int        width, height, dx, dy;
   AnsiString txt;
   TFont *fontOriginal;
   //VTGrafico  *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //proteção: verifica se foi definido texto
   if (Memo->Lines->Count == 0)
      {
      Aviso("Defina o texto do comentário");
      return(false);
      }
   //define opção de seleção
   switch(RadioGroupFormato->ItemIndex)
      {
	  case 0: comentario->Formato = fmtRETANGULO; break;
//      case 1: comentario->Formato = fmtELIPSE;    break;
	  case 1: comentario->Formato = fmtFLAG;      break;
      default:
         Aviso("Selecione opção de exibição");
         return(false);
      }
   //copia texto
   comentario->InsereTexto(Memo->Lines);
   //atualiza Font do comentário
   comentario->Font->Assign(Memo->Font);
   //define rede associada
   comentario->Rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
   //define cor de fundo
   comentario->Retangulo->FundoCor = PanelFundoCor->Color;
   //define atributos da borda
   comentario->Retangulo->BordaCor       = PanelBordaCor->Color;
   comentario->Retangulo->BordaEspessura = CBoxBordaEspessura->ItemIndex + 1;
   switch(CBoxBordaEstilo->ItemIndex)
      {
      case 0:  comentario->Retangulo->BordaEstilo = psSolid;      break;
      case 1:  comentario->Retangulo->BordaEstilo = psDash;       break;
      case 2:  comentario->Retangulo->BordaEstilo = psDot;        break;
      case 3:  comentario->Retangulo->BordaEstilo = psDashDot;    break;
      case 4:  comentario->Retangulo->BordaEstilo = psDashDotDot; break;
      case 5:  comentario->Retangulo->BordaEstilo = psClear;      break;
      default: comentario->Retangulo->BordaEstilo = psSolid;      break;
      }
   //redefine dimensões da moldura em função das dimensões do texto
   //dy = (abs(Memo->Font->Height)+1) * Memo->Lines->Count;
   dx = dy = 0;
   fontOriginal = Canvas->Font;
   Canvas->Font->Assign(comentario->Font);
   for (int n = 0; n < Memo->Lines->Count; n++)
      {
      txt     = Memo->Lines->Strings[n];
      width  = abs(Canvas->TextWidth(txt))+1;
      height = abs(Canvas->TextHeight(txt)) * Memo->Lines->Count;
      //grafico->SizeOfString(txt, width, height);
      if (height > dy) dy = height;
      if (width  > dx) dx = width;
	  }
   Canvas->Font->Assign(fontOriginal);
   comentario->Retangulo->SetDimensao(dx + 12, dy + 6);

   return(true);
   }

//---------------------------------------------------------------------------
//eof

