//---------------------------------------------------------------------------
#ifndef VTSegmentoJSONH
#define VTSegmentoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include "..\Rede\VTArea.h"

//---------------------------------------------------------------------------
//enum SEG_MERCADO {smDEFAULT=1, smTIPOREDE, smREDE, smAREA, smCARGA, smREGIAO, smINDEF=0};

//---------------------------------------------------------------------------
class VTCrescimentoJSON;

//---------------------------------------------------------------------------
class VTSegmentoJSON : public TObject
   {
   public:  //property
		__property AnsiString Tipo           = {read=PM_GetTipo,           write=PM_SetTipo};
		__property AnsiString Codigo         = {read=PM_GetCodigo,         write=PM_SetCodigo};
		__property AnsiString ExternId       = {read=PM_GetExternId,       write=PM_SetExternId};
		__property TList* Crescimento        = {read=PM_GetListCrescimento };

   public:  //métodos
							 __fastcall  VTSegmentoJSON(void) {};
	  virtual                __fastcall ~VTSegmentoJSON(void) {};
      virtual bool           __fastcall InsereCrescimento(VTCrescimentoJSON *crescimentoJSON) = 0;

   protected:  //métodos acessados via properpy
   		// GET
		virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
		virtual AnsiString __fastcall PM_GetExternId(void) = 0;
		virtual TList*     __fastcall PM_GetListCrescimento(void) = 0;
		virtual AnsiString __fastcall PM_GetTipo(void) = 0;
		// SET
	  	virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetExternId(AnsiString Segmento_id) = 0;
		virtual void       __fastcall PM_SetTipo(AnsiString tipo) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSegmento
//---------------------------------------------------------------------------
VTSegmentoJSON* __fastcall NewObjSegmentoJSON(void);

//---------------------------------------------------------------------------
#endif
//eof
