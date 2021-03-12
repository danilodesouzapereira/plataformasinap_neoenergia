//---------------------------------------------------------------------------
#ifndef TImportaH
#define TImportaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCargaNL;
class VTMonta;

//---------------------------------------------------------------------------
class TImporta : public TObject
   {
   public:  //m�todos
           __fastcall TImporta(VTApl *apl);
           __fastcall ~TImporta(void);
      bool __fastcall Executa(AnsiString arq_cnl, TList *lisEXT);

   private:  //m�todos
      VTBarra*   __fastcall ExisteBarra(AnsiString codigo);
      VTCargaNL* __fastcall ExisteCarganl(int cnl_id);
      bool       __fastcall LeArquivoCarga(AnsiString arq_cnl);
      void       __fastcall LeCargaNaoLinear(TStrings *campos);
      void       __fastcall LeHarmonico(TStrings *campos);

   private:  //objetos externos
      VTApl *apl;

   private:  //dados externos
      TList *lisCNL;

   private:  //dados locais
      TStrings *lines;
      VTMonta  *monta;
   };


#endif
//---------------------------------------------------------------------------
//eof
