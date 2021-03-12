//---------------------------------------------------------------------------
#ifndef TCriptStringListH
#define TCriptStringListH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBcd;
class VTCript;

//---------------------------------------------------------------------------
class TCriptStringList : public TStringList
   {
   public:
      __property bool       Enabled = {read=PD.enabled, write=PD.enabled};
      __property AnsiString Chave   = {read=PD.chave, write=PD.chave};

   public:
           __fastcall  TCriptStringList(void);
           __fastcall ~TCriptStringList(void);
      int  __fastcall  Add(AnsiString txt);

   private: //métodos
      void __fastcall BufferDelete(void);
      void __fastcall BufferNew(int len);

   private: //dados locais
      VTBcd      *bcd;
      VTCript    *cript;
      struct{
            int  len;
            char *vet;
            }buffer;
      struct{
            bool       enabled;
            AnsiString chave;
            }PD;
   };


#endif
//---------------------------------------------------------------------------
//eof

