//---------------------------------------------------------------------------
#ifndef TArqH
#define TArqH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class TArq 
   {
   public:
      __fastcall TArq(void);
      __fastcall ~TArq(void);
      bool         __fastcall Abre(AnsiString arq);
      void         __fastcall Fecha(void);
      TStringList* __fastcall LeLinha(char chr);
      TStringList* __fastcall LeLinha(char *p_ini, char chr);

   private: //dados
      FILE        *fp;
      char        buf[2048];
      TStringList *strList;
   };

#endif
//---------------------------------------------------------------------------
//eof
