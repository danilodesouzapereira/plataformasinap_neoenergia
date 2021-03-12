//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormExportaXML.h"
#include "VTCtrlPanel.h"
#include "VTExportaXML.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormExportaXML::TFormExportaXML(TComponent *Owner, VTApl *apl, AnsiString arq_xml)
   : TForm(Owner)
   {
   //salva dados recebidos
   this->apl     = apl;
   this->arq_xml = arq_xml;
   try{//cria objetos
      exportaXML = NewObjExportaXML(apl);
      ctrlpanel  = NewObjCtrlPanel();
      //configura componentes da tela
      ConfiguraTela();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormExportaXML::~TFormExportaXML(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos
   if (exportaXML) {delete exportaXML; exportaXML = NULL;}
   if (ctrlpanel)  {delete ctrlpanel;  ctrlpanel  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::ActionExportaExecute(TObject *Sender)
   {
   //vari�veis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o arquivo XML a ser gravado est� definido
   if (arq_xml.IsEmpty())
      {//define arquivo XML
      arq_xml = path->DirExporta() + "\\" + ChangeFileExt(ExtractFileName(path->ArqRede), ".xml");
      }
   //pede p/ usu�rio confirmar
   SaveDialog->InitialDir = path->DirExporta();
   SaveDialog->FileName   = arq_xml;
   if (! SaveDialog->Execute())
      {//fecha o form
      ModalResult = mrCancel;
      }
   //salva nome do arquivo XML
   arq_xml = SaveDialog->FileName;
   //executa exporta��o
   if (ExportaXML(arq_xml))
      {
      Aviso("Os dados foram gravados corretamente no arquivo:\n" + arq_xml);
      ModalResult = mrOk;
      }
   else
      {
      Erro("N�o foi poss�vel exportar os dados");
      ModalResult = mrCancel;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::ConfiguraTela(void)
   {
   //vari�veis locais
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //habilita exporta��o de dados b�sicos
   CheckBoxArranjos->Checked       = true;
   CheckBoxCurvasTipicas->Checked  = true;
   CheckBoxTipos->Checked          = true;
   //verifica se existe o objeto VTRedes
   if (redes == NULL)
      {//bloqueia exporta��o de redes
      CheckBoxRedes->Checked    = false;
      CheckBoxRedes->Visible    = false;
      //bloqueia exporta��o de redu��es
      CheckBoxReducoes->Checked = false;
      CheckBoxReducoes->Visible = false;
      }
   else
      {//habilita exporta��o de redes
      CheckBoxRedes->Checked    = true;
      CheckBoxRedes->Visible    = true;
      //bloqueia exporta��o de redu��es
      CheckBoxReducoes->Checked = false;
      CheckBoxReducoes->Visible = (licenca != NULL)&&(licenca->Tipo == tipoMASTER);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormExportaXML::ExportaXML(AnsiString arq_xml)
   {
   //vari�veis locais
   bool sucesso;

   //redefine separador decimal do Windows
   if (! ctrlpanel->DefineSeperadorDecimal('.'))
      {
      Aviso("O Windows deve estar configurado para utilizar 'ponto' como separador decimal");
      return(false);
      }
   //configura op��es do arquivo XML
   exportaXML->ExportaArranjo     = CheckBoxArranjos->Checked;
   exportaXML->ExportaTipoChave   = CheckBoxTipos->Checked;
   exportaXML->ExportaTipoRede    = CheckBoxTipos->Checked;
   exportaXML->ExportaCurvaTipica = CheckBoxCurvasTipicas->Checked;
   exportaXML->ExportaReducao     = CheckBoxReducoes->Checked;
   exportaXML->ExportaRede        = CheckBoxRedes->Checked;
   sucesso = exportaXML->ExportaRedes(arq_xml);
   //restaura separador decimal originalmente configurado no Windows
   ctrlpanel->RestauraSeperadorDecimal();

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaXML::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof

