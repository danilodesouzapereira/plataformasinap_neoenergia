//---------------------------------------------------------------------------
#ifndef TBcdH
#define TBcdH

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBcd.h"

//---------------------------------------------------------------------------
class TBcd : public VTBcd
   {
   //func�es
   public :
           __fastcall TBcd(void);
           __fastcall ~TBcd(void);
      bool __fastcall BinToHexa(char *orig, int len_orig, char *dest, int len_dest);
      bool __fastcall HexaToBin(char *orig, int len_orig, char *dest, int len_dest);
   };

//---------------------------------------------------------------------------
#endif
//eof
