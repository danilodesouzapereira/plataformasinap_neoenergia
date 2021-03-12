//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormExportaTXT.h"
#include "VTExportaTXT.h"
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
__fastcall TFormExportaTXT::TFormExportaTXT(TComponent *Owner, VTApl *apl, AnsiString arq_txt)
   : TForm(Owner)
   {
   //salva dados recebidos
   this->apl     = apl;
   this->arq_txt = arq_txt;
   try{//cria objetos
      exportaTXT = NewObjExportaTXT(apl);
      //configura componentes da tela
      ConfiguraTela();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormExportaTXT::~TFormExportaTXT(void)
   {
   //destrói objetos
   if (exportaTXT) {delete exportaTXT; exportaTXT = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::ActionExportaExecute(TObject *Sender)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o arquivo TXT a ser gravado está definido
   if (arq_txt.IsEmpty())
      {//define arquivo TXT
      arq_txt = path->DirExporta() + "\\" + ChangeFileExt(ExtractFileName(path->ArqRede), ".txt");
      }
   //pede p/ usuário confirmar
   SaveDialog->InitialDir = path->DirExporta();
   SaveDialog->FileName   = arq_txt;
   if (! SaveDialog->Execute())
      {//fecha o form
      ModalResult = mrCancel;
      return;
      }
   //salva nome do arquivo XML
   arq_txt = SaveDialog->FileName;
   //executa exportação
   if (ExportaTXT(arq_txt))
      {
	  Aviso("Os dados foram gravados corretamente no arquivo:\n" + arq_txt);
      ModalResult = mrOk;
      }
   else
      {
      Erro("Não foi possível exportar os dados");
      ModalResult = mrCancel;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::ConfiguraTela(void)
   {
   //variáveis locais
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //habilita exportação de dados básicos
   CheckBoxArranjos->Checked       = true;
   CheckBoxCurvasTipicas->Checked  = true;
   CheckBoxTipos->Checked          = true;
   CheckBoxCript->Checked          = false;
   //verifica se existe o objeto VTRedes
   if (redes == NULL)
      {//bloqueia exportação de redes
      CheckBoxRedes->Checked    = false;
      CheckBoxRedes->Visible    = false;
      //bloqueia exportação de reduções
      CheckBoxReducoes->Checked = false;
      CheckBoxReducoes->Visible = false;
      }
   else
      {//habilita exportação de redes
      CheckBoxRedes->Checked    = true;
      CheckBoxRedes->Visible    = true;
      //bloqueia exportação de reduções
      CheckBoxReducoes->Checked = false;
      CheckBoxReducoes->Visible = (licenca != NULL)&&(licenca->Tipo == tipoMASTER);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormExportaTXT::ExportaTXT(AnsiString arq_txt)
   {
   //variáveis locais
   bool sucesso;

   //configura opções do arquivo XML
   exportaTXT->ExportaCurvaTipica = CheckBoxCurvasTipicas->Checked;
   exportaTXT->ExportaArranjo     = CheckBoxArranjos->Checked;
   exportaTXT->ExportaCabo        = CheckBoxArranjos->Checked;
   exportaTXT->ExportaSuporte     = CheckBoxArranjos->Checked;
   exportaTXT->ExportaTipoChave   = CheckBoxTipos->Checked;
   exportaTXT->ExportaTipoRede    = CheckBoxTipos->Checked;
   exportaTXT->ExportaReducao     = CheckBoxReducoes->Checked;
   exportaTXT->ExportaRede        = CheckBoxRedes->Checked;
   exportaTXT->Criptografar       = CheckBoxCript->Checked;
   sucesso = exportaTXT->ExportaRedes(arq_txt);

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto superior esquerdo
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaTXT::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof

