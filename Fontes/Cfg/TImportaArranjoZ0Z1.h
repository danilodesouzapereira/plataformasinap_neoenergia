//---------------------------------------------------------------------------
#ifndef TImportaArranjoZ0Z1_H
#define TImportaArranjoZ0Z1_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TImportaArranjoZ0Z1 : public TObject
   {
   public:
           __fastcall  TImportaArranjoZ0Z1(VTApl *apl_owner);
           __fastcall ~TImportaArranjoZ0Z1(void);
      bool __fastcall  Executa(AnsiString arq_txt);

   private: //métodos
       void   __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TStringList *campos;
      TStringList *lines;
   };

#endif
//-----------------------------------------------------------------------------
// eof


