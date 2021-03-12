//---------------------------------------------------------------------------
#ifndef TArqMedicaoH
#define TArqMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "MesAno.h"

//---------------------------------------------------------------------------
class TArqMedicao : public TObject
   {
   public:  //property
      __property bool Enabled = {read=PM_GetEnabled};

   public:
                 __fastcall  TArqMedicao(TDateTime dt);
                 __fastcall ~TArqMedicao(void);
      AnsiString __fastcall  DataAsString(void);
      void       __fastcall  InsereArquivo(AnsiString filename);
      void       __fastcall  InsereArquivo(TStrings *lines);

   private:
      bool __fastcall PM_GetEnabled(void);

   public:  //dados
      TStrings   *files;
      strMES_ANO data;
   };

//---------------------------------------------------------------------------
#endif
//eof

