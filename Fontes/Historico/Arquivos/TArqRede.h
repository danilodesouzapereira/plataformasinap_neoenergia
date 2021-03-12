//---------------------------------------------------------------------------
#ifndef TArqRedeH
#define TArqRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "MesAno.h"

//---------------------------------------------------------------------------
class TArqRede : public TObject
   {
   public:  //property
      __property bool Enabled = {read=PM_GetEnabled};

   public:
                 __fastcall  TArqRede(TDateTime dt_ini, TDateTime dt_fim);
                 __fastcall ~TArqRede(void);
      AnsiString __fastcall  DataAsString(void);
      void       __fastcall  InsereArquivo(AnsiString filename);
      void       __fastcall  InsereArquivo(TStrings *lines);

   private:
      bool __fastcall PM_GetEnabled(void);

   public:  //dados
	  TStrings   *files;
      TList      *lisSub;
      strMES_ANO data_ini, data_fim;
   };

//---------------------------------------------------------------------------
#endif
//eof

