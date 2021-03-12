//---------------------------------------------------------------------------
#ifndef TCrescimentoJSONH
#define TCrescimentoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCrescimentoJSON.h"

//---------------------------------------------------------------------------
class VTTaxaJSON;

//---------------------------------------------------------------------------
class TCrescimentoJSON : public VTCrescimentoJSON
   {
   public:  //funções públicas
                     __fastcall TCrescimentoJSON(void);
					 __fastcall ~TCrescimentoJSON(void);
        bool         __fastcall InsereTaxa(VTTaxaJSON *taxaJSON);
//      bool           __fastcall InserePeriodo(int ano_ini, int ano_fim, double taxa_pu);
//      TList*         __fastcall LisPeriodo(void);
//      bool           __fastcall Nulo(void);
//      void           __fastcall RedefinePeriodo(int ano_ini, int ano_fim);
//      void           __fastcall Reinicia(void);

   protected: //métodos acessados via property
      // GET
	  AnsiString  __fastcall PM_GetClasse(void);
	  TList*      __fastcall PM_GetListTaxa(void);
	  // SET
	  void       __fastcall PM_SetClasse(AnsiString classe);

   private:  //dados locais
	  AnsiString classeNome;
	  TList               *lisTAXA_JSON;

   };

//---------------------------------------------------------------------------
#endif
//eof
