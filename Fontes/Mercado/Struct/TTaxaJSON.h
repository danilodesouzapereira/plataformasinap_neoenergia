//---------------------------------------------------------------------------
#ifndef TTaxaJSONH
#define TTaxaJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTaxaJSON.h"

//---------------------------------------------------------------------------
class TTaxaJSON : public VTTaxaJSON
   {
   public:  //funções públicas
                     __fastcall TTaxaJSON(void);
                     __fastcall ~TTaxaJSON(void);

   protected: //métodos acessados via property
	  // GET
	  int        __fastcall PM_GetAno(void);
	  double     __fastcall PM_GetTaxa_perc(void);
	  // SET
	  void        __fastcall PM_SetAno(int ano);
	  void        __fastcall PM_SetTaxa_perc(double valor);

   private:  //dados acessados via property
		struct {
					int ano;
					double valor;
			   } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
