//---------------------------------------------------------------------------
#ifndef TFormMTH
#define TFormMTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormMT : public TForm
{
__published:   // IDE-managed Components
   TImageList *ImageList;
   TActionList *ActionListMapaTematico;
   TAction *ActionMT_Energia;
   TAction *ActionMT_Tensao;
   TAction *ActionMT_AIS;
   TAction *ActionMP_CCS;
   TAction *ActionMT_Temp;
   TAction *ActionMT_DensidadeCarga;
   TAction *ActionMT_PerfilTensao;
   TAction *ActionMT_PerfilFatpot;
   void __fastcall ActionMT_EnergiaExecute(TObject *Sender);
   void __fastcall ActionMT_TensaoExecute(TObject *Sender);
   void __fastcall ActionMT_AISExecute(TObject *Sender);
   void __fastcall ActionMP_CCSExecute(TObject *Sender);
   void __fastcall ActionMT_TempExecute(TObject *Sender);
   void __fastcall ActionMT_DensidadeCargaExecute(TObject *Sender);
   void __fastcall ActionMT_PerfilTensaoExecute(TObject *Sender);
   void __fastcall ActionMT_PerfilFatpotExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormMT(TComponent *Owner, VTApl *apl_owner);
        __fastcall ~TFormMT(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   void         __fastcall CloseMapaTematico(void);
   TForm*       __fastcall ExisteForm(AnsiString class_name_alvo);
   bool         __fastcall RedeGeoreferenciada(void);
   TWinControl* __fastcall SelecionaParentParaJanela(void);

private:   //objetos externos
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
//eof
