//---------------------------------------------------------------------------
#ifndef TIconesH
#define TIconesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <FileCtrl.hpp>
#include "VTIcones.h"

//---------------------------------------------------------------------------
class TIcones : public VTIcones
   {
   public:
               __fastcall  TIcones(TForm *Owner, AnsiString dir_icone);
               __fastcall ~TIcones(void);
      VTIcone* __fastcall  ExisteIcone(AnsiString eqpto_cod, int index);
      VTIcone* __fastcall  ExisteIcone(int eqpto_id, int index);

   private: //dados locais
      int  __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
      void __fastcall MontaIcones(TFileListBox *FileListBox, TStrings *lines, TStrings *campos);
      void __fastcall MontaUmIcone(TStrings *lines, TStrings *campos);

   private: //dados locais
      TList *lisICONE;
   };

#endif
//---------------------------------------------------------------------------
//eof

