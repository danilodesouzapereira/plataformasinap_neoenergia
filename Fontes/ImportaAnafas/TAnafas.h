//---------------------------------------------------------------------------
#ifndef TAnafas_H
#define TAnafas_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTAnafas.h"

//---------------------------------------------------------------------------
class VTPreMonta;

//---------------------------------------------------------------------------
class TAnafas : public VTAnafas
   {
   public:
           __fastcall TAnafas(VTApl *apl);
           __fastcall ~TAnafas(void);
      bool __fastcall Executa(TStrings *Lines);
      int  __fastcall NumeroErros(void);

   private: //métodos
      bool __fastcall Comentario(AnsiString txt);
      bool __fastcall FimDeBloco(AnsiString txt);
      int  __fastcall InicioDeBloco(AnsiString txt);
      void __fastcall LeCartaoBarra(AnsiString txt);
      void __fastcall LeCartaoCircuito(AnsiString txt);
      void __fastcall LeCartaoMutua(AnsiString txt);

   private: //objetos externos
      VTApl      *apl;

   private: //dados locais
      enum {estNONE=0, estDBAR, estCIRC, estMUTUA, estFIM};
      int         estado;
      TStringList *strList;
   };

#endif

//-----------------------------------------------------------------------------
// eof
