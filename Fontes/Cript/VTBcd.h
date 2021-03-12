//-----------------------------------------------------------------------------
#ifndef VTBcd_H
#define VTBcd_H

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTBcd  : public TObject
   {
   public:
                   __fastcall VTBcd(void)  {};
      virtual      __fastcall ~VTBcd(void) {};
      virtual bool __fastcall BinToHexa(char *orig, int len_orig, char *dest, int len_dest) = 0;
      virtual bool __fastcall HexaToBin(char *orig, int len_orig, char *dest, int len_dest) = 0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTBcd
//-----------------------------------------------------------------------------
VTBcd* __fastcall NewObjBcd(void);

#endif
//---------------------------------------------------------------------------
//eof
