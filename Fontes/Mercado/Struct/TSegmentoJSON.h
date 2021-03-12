//---------------------------------------------------------------------------
#ifndef TSegmentoJSONH
#define TSegmentoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSegmentoJSON.h"

//---------------------------------------------------------------------------
class  VTCrescimentoJSON;

//---------------------------------------------------------------------------
class TSegmentoJSON : public VTSegmentoJSON
   {
   public:  //funções públicas
					 __fastcall  TSegmentoJSON(void);
					 __fastcall ~TSegmentoJSON(void);
                bool __fastcall InsereCrescimento(VTCrescimentoJSON *crescimentoJSON);

   protected:  //métodos acessados via property
        // GET
		AnsiString __fastcall PM_GetCodigo(void);
		AnsiString __fastcall PM_GetExternId(void);
		TList*     __fastcall PM_GetListCrescimento(void);
		AnsiString __fastcall PM_GetTipo(void);
		// SET
		void       __fastcall PM_SetCodigo(AnsiString codigo);
		void       __fastcall PM_SetExternId(AnsiString extern_id);
		void       __fastcall PM_SetTipo(AnsiString tipo);

   private:  //dados acessados via property
		struct   {
					AnsiString extern_id;
					AnsiString tipo;
					AnsiString codigo;
			   } PD;

   private:  //dados locais
	  TList *lisCRES_JSON;
   };

//---------------------------------------------------------------------------
#endif
//eof
