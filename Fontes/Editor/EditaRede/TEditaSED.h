//---------------------------------------------------------------------------
#ifndef TEditaSEDH
#define TEditaSEDH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TEditaSED : public TObject
   {
   public:		// User declarations
           __fastcall TEditaSED(VTApl *apl_owner);
           __fastcall ~TEditaSED(void);
      bool __fastcall Executa(AnsiString arquivo);

   private:	// User declarations
      void          __fastcall AlteraZeq(void);
      bool          __fastcall ExisteSED(void);
      bool          __fastcall ImportaZeqSuprimentos(AnsiString arquivo);
      bool          __fastcall LeArqZeq(AnsiString filefullpath);
      void          __fastcall LisSuprimento(AnsiString cod_rede, TList *lisSUP);
      bool          __fastcall VerificaComentario(AnsiString linha);

   private: //obj. externo
      VTApl *apl;

   private://obj. interno
      TStringList *file_lines, *campos;

   };

//---------------------------------------------------------------------------
#endif
