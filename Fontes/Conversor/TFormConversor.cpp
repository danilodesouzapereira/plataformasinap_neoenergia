//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\InterfaceTXT\Exporta\VTExportaTXT.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Redutor\VTRedutor.h>
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\InterfaceTXT.h>
#include <DLL_Inc\Redutor.h>
#include "TFormConversor.h"
#include "VTSinap2.h"
#include "TDEscarte.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormConversor::TFormConversor(TComponent *Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiros p/ objetos
   apl = apl_owner;
   //cria objetos
   StrListRede = new TStringList();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormConversor::~TFormConversor(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos
   if (StrListRede) {delete StrListRede; StrListRede = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionConverteExecute(TObject *Sender)
   {
   //reinicia LBoxRede apenas p/ acertar a cor de apresenta��o dos items
   for (int n = 0; n < LBoxRede->Items->Count; n++)
      {
      LBoxRede->Items->Objects[n] = NULL;
      }
   //importa lista de redes
   ImportaListaDeRedes(StrListRede);
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionClearSelecaoExecute(TObject *Sender)
   {
   //limpa LBoxArquivo e  StrListAllFiles
   LBoxRede->Clear();
   StrListRede->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionExcluiRedeExecute(TObject *Sender)
   {
   //vari�veis locais
   int index;

   //prote��o
   if (LBoxRede->Items->Count == 0)
      {
      Aviso("N�o h� rede selecionada");
      return;
      }
   if ((index = LBoxRede->ItemIndex) < 0)
      {
      Aviso("Selecione a rede ser removida");
      return;
      }
   if (Confirma("Deseja remover a rede selecionada ?",
                LBoxRede->Items->Strings[index]) == ID_YES)
      {//remove arquivo LBoxArquivo e de StrListAllFiles
      LBoxRede->Items->Delete(index);
      StrListRede->Delete(index);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionFechaExecute(TObject *Sender)
   {
   //veri�veis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se h� rede carregada
   if (lisREDE->Count == 0) ModalResult = mrCancel;
   else                     ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionSelRedeExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString dir_rede;
   VTDir      *dir = (VTDir*)apl->GetObject(__classid(VTDir));

   //esconde este Form
   Visible = false;
   //seleciona diret�rio
   if (dir->SelDir(dir_rede))
      {//verifica se est� selecionada a op��o de diret�rio raiz com todas suas redes
      if (CheckBoxDirRaiz->Checked)
         {//monta StrListRede com a lista de subdiret�rios existentes embaixo dos diret�rios selecionados
         IdentificaRedesNoDiretorio(dir_rede, StrListRede);
         //reinicia LBoxRede com todos os diret�rios de rede
         LBoxRede->Clear();
         for (int n = 0; n < StrListRede->Count; n++)
            {
            dir_rede = StrListRede->Strings[n];
            LBoxRede->Items->Add(ExtractFileName(dir_rede));
            }
         }
      else
         {//inclui diret�rio da rede em StrListRede
         if (StrListRede->IndexOf(dir_rede) < 0)
            {
            StrListRede->Add(dir_rede);
            //atualiza LBoxRede
            LBoxRede->Items->Add(ExtractFileName(dir_rede));
            }
         }
      }
   //reapresenta este Form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ActionSelSinapExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString dir_sinap2;
   VTDir      *dir = (VTDir*)apl->GetObject(__classid(VTDir));

   //esconde este Form
   Visible = false;
   //seleciona diret�rio
   if (dir->SelDir(dir_sinap2))
      {//atualiza EdtDir
      EditDirSinap2->Text = dir_sinap2;
      //reinicia sele��o
      ActionClearSelecao->Execute();
      }
   //reapresnta este Form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::DescartaArranjosNaoUsados(int num_arranjos)
   {
   //vari�veis locais
   TDescarte descarte(apl);

   //verifica se foi selecionada op��o p/ descartar arranjos
   if (! CheckBoxDescartaArranjo->Checked) return;
   //elimina Arranjos
   descarte.Arranjos(num_arranjos);
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto superior esquerdo
   Left =  50;
   Top  = 150;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::FormShow(TObject *Sender)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::IdentificaRedesNoDiretorio(AnsiString dir_raiz, TStrings *StrListRede)
   {
   //vari�veis locais
   AnsiString  path, subdir;
   int         attr;
   TSearchRec  sr;


   //inclui wildcard *.* no path do diret�rio raiz
   path = dir_raiz + "\\*.*";
   //define busca somente de diret�rios
   attr = faDirectory;
   if (FindFirst(path, attr, sr) == 0)
      {
      do {
         if ((sr.Attr & attr) == attr)
            {//monta path do diret�rio
            subdir = dir_raiz + "\\" + sr.Name;
            //verifica se existe o arquivo Rede.mdb no subdiret�rio
            if (FileExists(subdir + "\\rede.mdb"))
               {//inclui o subdiret�rio em StrListRede
               StrListRede->Add(subdir);
               }
            }
         } while(FindNext(sr) == 0);
      FindClose(sr);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::ImportaListaDeRedes(TStrings *bases)
   {
   //vari�veis locais
   bool       sucesso;
   int        index;
   int        num_arranjos;
   AnsiString dir_rede;
   VTSinap2   *sinap2;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   try{//exibe PanelProgresso
      PanelProgresso->Visible = true;
      //salva n�mero de arranjos existentes antes da importa��o
      num_arranjos = arranjos->LisArranjo()->Count;
      //cria objeto VTSinap2
      sinap2 = NewObjSinap2(this, apl);
      //loop p/ todas as redes em StrListRede
      for (int n = 0; n < bases->Count; n++)
         {
         dir_rede = bases->Strings[n];
         //atualiza progresso
         LabelProgresso->Caption = "Progresso: " + IntToStr(n+1) + "/" + IntToStr(bases->Count);
         //atualiza tela
         if ((index = n-10) < 0) index = 0;
         LBoxRede->TopIndex = index;
         LBoxRede->Items->Objects[n] = (TObject*)clYellow;
         LBoxRede->Refresh();
         //importa Rede
         sinap2->AbreRedeCompleta(EditDirSinap2->Text, dir_rede);
         //descarta Arranjos n�o utilizados pelas Redes
         DescartaArranjosNaoUsados(num_arranjos);
         //atualiza o gr�fico
         grafico->AreaRedeDefine();
         //salva a Rede em uma base nova
         sucesso = SalvaRedeBaseDado(ExtractFileName(dir_rede));
         //salva a Rede em arquivo TXT
         SalvaRedeArquivoTexto(ExtractFileName(dir_rede));
         LBoxRede->Items->Objects[n] = (sucesso) ? (TObject*)clGreen : (TObject*)clRed;
         LBoxRede->Refresh();
         }
      //destr�i objeto VTSinap2
      delete sinap2;
      //esconde PanelProgresso
      PanelProgresso->Visible = false;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormConversor::LBoxRedeDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
   {
   //vari�veis locais
   TColor      color;
   TCanvas    *Canvas    = LBoxRede->Canvas;
   TColor      color_ori = Canvas->Brush->Color;
   TBrushStyle style_ori = Canvas->Brush->Style;
   TRect       rect_cor;

   //limpa a �rea do item
   Canvas->FillRect(Rect);
   //define �rea p/ desenhar o ret�ngulo c/ a cor da Rede
   rect_cor.Top    = Rect.Top + 1;
   rect_cor.Bottom = Rect.Bottom - 1;
   rect_cor.Left   = Rect.Left;
   rect_cor.Right  = rect_cor.Left + (rect_cor.Bottom - rect_cor.Top);
   //determina cor do ret�ngulo (e descarta cor preta)
   if ((color = TColor(int(LBoxRede->Items->Objects[Index]))) == clBlack) color = color_ori;
   Canvas->Brush->Color = color;
   //pinta a �rea do ret�ngulo
   Canvas->FillRect(rect_cor);
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   //exibe o texto
   Canvas->TextOut(rect_cor.Right + 2, Rect.Top, LBoxRede->Items->Strings[Index]);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormConversor::SalvaRedeArquivoTexto(AnsiString nome_rede)
   {
   //vari�veis locais
   bool         sucesso;
   AnsiString   arq_txt;
   VTExportaTXT *exporta;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   try{//verifica se foi selecionada op��o p/ exportar aquivo TXT
      if (! CheckBoxExportaTXT->Checked) return(true);
      //define nome do arquivo
      arq_txt = path->DirExporta() + "\\" + nome_rede + ".txt";
      //cria objeto p/ exportar arquivo
      exporta = DLL_NewObjExportaTXT(apl);
      exporta->ExportaCurvaTipica = true;
      exporta->ExportaArranjo     = true;
      exporta->ExportaCabo        = true;
      exporta->ExportaSuporte     = true;
      exporta->ExportaRede        = true;
      exporta->ExportaReducao     = true;
      exporta->ExportaTipoChave   = true;
      exporta->ExportaTipoRede    = true;
      sucesso = exporta->ExportaRedes(arq_txt);
      //destr�i objeto
      delete exporta;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormConversor::SalvaRedeBaseDado(AnsiString dir_rede)
   {
   //vari�veis locais
   VTRedutor  *redutor;
   VTSalva    *salva;
   VTDir      *dir   = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath     *path  = (VTPath*)apl->GetObject(__classid(VTPath));

   try{//verifica se foi selecionada op��o p/ gravar base Access
      if (! CheckBoxBaseAccess->Checked) return(true);
      //cria objetos
      if ((redutor =  DLL_NewObjRedutor(apl)) == NULL) return(false);
      //configura mensagen redutor
      redutor->ProgressoEnabled = true;
      redutor->AvisoEnabled     = false;
      //monta redes reduzidas
      if (redutor->ExecutaReducaoMNet())
         {//define diret�rio da Rede
         if (dir_rede.AnsiPos("Rede") == 0) dir_rede = "Rede_" + dir_rede;
         dir_rede = "Sinap2_" + dir_rede;
         //cria uma nova base de dados Access
         if (dir->CriaRedeSinap(dir_rede, false))
            {//atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
            path->DefineDirRede(dir_rede);
            //atualiza base de dados
            salva  = DLL_NewObjSalva(this, apl);
            salva->SalvaRedeComo();
            }
         }
      //destr�i objetos
      delete redutor;
      delete salva;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


