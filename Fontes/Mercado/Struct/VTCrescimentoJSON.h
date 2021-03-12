//---------------------------------------------------------------------------
#ifndef VTCrescimentoJSONH
#define VTCrescimentoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <sysdyn.h>
//---------------------------------------------------------------------------
//class VTArea;
class VTTaxaJSON;

//---------------------------------------------------------------------------
class VTCrescimentoJSON : public TObject
   {
   public:  //property
		__property AnsiString    Classe  = {read=PM_GetClasse,     write=PM_SetClasse};
		__property TList*        Taxa    = {read=PM_GetListTaxa};

   public:  //funções públicas
                             __fastcall VTCrescimentoJSON(void) {};
	  virtual                __fastcall ~VTCrescimentoJSON(void) {};
	  virtual   bool         __fastcall InsereTaxa(VTTaxaJSON *taxaJSON) = 0;
               

   protected: //métodos acessados via property
      // GET
	  virtual AnsiString  __fastcall PM_GetClasse(void) = 0;
	  virtual TList*      __fastcall PM_GetListTaxa(void) = 0;
	  // SET
	  virtual void       __fastcall PM_SetClasse(AnsiString classe) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCrescimentoJSON
//---------------------------------------------------------------------------
VTCrescimentoJSON* __fastcall NewObjCrescimentoJSON(void);

//---------------------------------------------------------------------------
#endif
//eof
