//---------------------------------------------------------------------------
#ifndef TAnaredeH
#define TAnaredeH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTAnarede.h"

//---------------------------------------------------------------------------
class VTLog;
class VTPreMonta;

//---------------------------------------------------------------------------
class TAnarede : public VTAnarede
   {
   public:
           __fastcall TAnarede(VTApl *apl);
           __fastcall ~TAnarede(void);
      bool __fastcall Executa(TStrings *Lines, int num_area);

   private: //métodos
      bool __fastcall Comentario(AnsiString txt);
      bool __fastcall FimDeBloco(AnsiString txt);
      int  __fastcall InicioDeBloco(AnsiString txt);
      bool __fastcall JaTratou(int estado);
      void __fastcall LeCartaoDBAR(AnsiString txt);
      void __fastcall LeCartaoDBSH(AnsiString txt);
      void __fastcall LeCartaoDBSH2(AnsiString txt);
      void __fastcall LeCartaoDGBA(AnsiString txt);
      void __fastcall LeCartaoDGBT(AnsiString txt);
      void __fastcall LeCartaoDINJ(AnsiString txt);
      void __fastcall LeCartaoDLIN(AnsiString txt);
      void __fastcall RedeAlterada(void);

   private: //objetos externos
      VTApl      *apl;
      VTLog      *plog;
      VTPreMonta *pre_monta;

   private: //dados
      enum {estDBAR=0, estDLIN, estDGBT, estDINJ, estDGBA, estDBSH, estNONE, estERRO, estFIM};
      int         estado;
      bool        ja_tratou[estFIM+1];
      TStringList *strList;
   };

#endif

//-----------------------------------------------------------------------------
// eof
