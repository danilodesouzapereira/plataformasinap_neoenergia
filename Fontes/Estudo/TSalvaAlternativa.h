//---------------------------------------------------------------------------
#ifndef TSalvaAlternativaH
#define TSalvaAlternativaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TSalvaAlternativa : public TObject
{
   public:      // User declarations
               __fastcall  TSalvaAlternativa(VTApl *apl_owner);
               __fastcall ~TSalvaAlternativa(void);
      void     __fastcall Executa(void);

   private:
      bool     __fastcall SalvaRedeBase(bool base_nova);
      bool     __fastcall SalvaRedeBaseNova(void);
      bool     __fastcall SalvaRedeComo(void);
      bool     __fastcall ValidaEqptoIsolado(void);

   private:    //obj externo
      VTApl *apl;

};
//---------------------------------------------------------------------------
#endif
