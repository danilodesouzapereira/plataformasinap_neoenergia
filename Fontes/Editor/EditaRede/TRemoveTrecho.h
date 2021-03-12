//---------------------------------------------------------------------------
#ifndef TRemoveTrechoH
#define TRemoveTrechoH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TRemoveTrecho : public TObject
   {
   public:		// User declarations
           __fastcall TRemoveTrecho(VTApl *apl_owner);
           __fastcall ~TRemoveTrecho(void);
      bool __fastcall Executa(void);

   private:
      void __fastcall CopiaComum(TList* lisEQP1,TList* lisEQP2);
      void __fastcall RemoveEqptos(void);
      void __fastcall RemoveTrechosDuplicados(TList *lisTRECHO);

   private: //obj. externo
      VTApl *apl;

   private: //interno
      TList *lisLIGA_DUP;
   };

//---------------------------------------------------------------------------
#endif
