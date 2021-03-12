//---------------------------------------------------------------------------
#ifndef TFormGrupoH
#define TFormGrupoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <FileCtrl.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class TFormEditor;
class VTApl;
class VTEdt;

//---------------------------------------------------------------------------
class TFormGrupo : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExibeModulo;
   TTreeView *TViewGrupo;
   TAction *ActionOnDone;
   TDirectoryListBox *DirectoryListBox;
   TFileListBox *FileListBox;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionExibeModuloExecute(TObject *Sender);
   void __fastcall ActionOnDoneExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormGrupo(TFormEditor *FormEditor, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormGrupo(void);
   void __fastcall  Atualiza(void);
   int  __fastcall  NumeroGrupo(void);
   void __fastcall  TViewGrupoInicia(TTreeView *TView);

private:   //métodos
   void __fastcall CancelaAssociacaoLigacaoRede(TList *lisEXT);
   void __fastcall CarregaArquivoXML(AnsiString arq_xml);
   void __fastcall CarregaArquivoTXT(AnsiString arq_txt);
   void __fastcall ExibeGrupo(TTreeNode *Node);

private:   //objetos externos
   TFormEditor *FormEditor;
   VTApl       *apl_owner;

private:   //dados locais
   TList *lisEQP;
   VTApl *apl_local;
   VTEdt *edt_cola;
   VTEdt *edt_copia;
};

//---------------------------------------------------------------------------
#endif
//eof

