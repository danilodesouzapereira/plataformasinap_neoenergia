//---------------------------------------------------------------------------
#ifndef TMercadoJSONH
#define TMercadoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMercadoJSON.h"

//---------------------------------------------------------------------------
class  VTSegmentoJSON;

//---------------------------------------------------------------------------
class TMercadoJSON : public VTMercadoJSON
   {
   public:  //métodos
						__fastcall  TMercadoJSON(VTApl *apl);
						__fastcall ~TMercadoJSON(void);
		bool __fastcall InsereSegmentoJSON(VTSegmentoJSON *segmentoJSON);

   protected:  //métodos acessados via property
      // GET
	  int        __fastcall PM_GetAnoFinal(void);
      int        __fastcall PM_GetAnoInicial(void);
	  TList*     __fastcall PM_GetSegmentoJSON(void);
	  // SET
      void       __fastcall PM_SetAnoFinal(int ano_fim);
      void       __fastcall PM_SetAnoInicial(int ano_ini);

   protected:  //dados acessados via property
	  struct   {
               int        ano_inicial;
			   int        ano_final;
               }PD;

	private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
      TList *lisSEGMENTO_JSON;

   };

//---------------------------------------------------------------------------
#endif
//eof
