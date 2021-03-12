//---------------------------------------------------------------------------
#ifndef TFormEstudoH
#define TFormEstudoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAbre;
class VTEstudo;
class VTRede;

//---------------------------------------------------------------------------
class TFormEstudo : public TForm
{
__published:	// IDE-managed Components
   TTreeView *TView;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);

public:      // User declarations
        __fastcall  TFormEstudo(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormEstudo(void);
   bool __fastcall  AbreEstudo(void);
   void __fastcall  Desabilita(void);
   void __fastcall  Habilita(void);
   void __fastcall  ExcluiEstudo(void);

private:   // métodos
   void       __fastcall IniciaLisRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ);
   void       __fastcall ReiniciaEsquematico(VTEstudo *estudo, TList* lisREDE);
   void       __fastcall TViewEliminaTreeNodeRede(void);
   TTreeNode* __fastcall TViewExisteTreeNode(int level, AnsiString text);
   void       __fastcall TViewInicia(void);
   void       __fastcall TViewInsereRedeDoEstudo(TTreeNode *node_estudo);
   void       __fastcall TViewLisRedes(TTreeNode *node_ref, TList *lisEXT);

private:   // objetos externos
   VTAbre         *abre;

private:    //dados locais
   enum enumLEVEL {LevelAutor=0,  LevelEstudo, LevelRede};
   TList     *lisEQP;
};

//---------------------------------------------------------------------------
#endif
//eof

