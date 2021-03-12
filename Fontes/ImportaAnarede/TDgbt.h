//---------------------------------------------------------------------------
#ifndef TDgbt_H
#define TDgbt_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDbar;

//---------------------------------------------------------------------------
class TDgbt : TObject
   {
   public:
           __fastcall TDgbt(void);
           __fastcall ~TDgbt(void);
      //bool __fastcall ContemDbar(TDbar *dbar);
      //void __fastcall InsereDbar(TDbar *dbar);

   public: //dados locais
      AnsiString grupo;
      double     tensao_kv;

   public: //dados locais
      int   id;
   };

#endif
//-----------------------------------------------------------------------------
// eof
