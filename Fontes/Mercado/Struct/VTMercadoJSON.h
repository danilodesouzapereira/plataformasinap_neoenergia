//---------------------------------------------------------------------------
#ifndef VTMercadoJSONH
#define VTMercadoJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class  VTApl;
class  VTSegmentoJSON;

//---------------------------------------------------------------------------
#define ANO_INDEF  -1

//---------------------------------------------------------------------------
class VTMercadoJSON : public TObject
   {
   public:  //property
	  __property int        AnoInicial     = {read=PM_GetAnoInicial, write=PM_SetAnoInicial};
	  __property int        AnoFinal       = {read=PM_GetAnoFinal,   write=PM_SetAnoFinal  };
	  __property TList*     SegmentoJSON   = {read=PM_GetSegmentoJSON };

   public:  //m�todos
					 __fastcall  VTMercadoJSON(void) {};
		virtual      __fastcall ~VTMercadoJSON(void) {};
		virtual bool __fastcall InsereSegmentoJSON(VTSegmentoJSON *segmentoJSON)= 0;


   protected:  //m�todos acessados via property
	  // GET
      virtual int        __fastcall PM_GetAnoFinal(void) = 0;
      virtual int        __fastcall PM_GetAnoInicial(void) = 0;
	  virtual TList*     __fastcall PM_GetSegmentoJSON(void) = 0;
	  // SET
      virtual void       __fastcall PM_SetAnoFinal(int ano_fim) = 0;
      virtual void       __fastcall PM_SetAnoInicial(int ano_ini) = 0;
     
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTMercadoJSON
//---------------------------------------------------------------------------
VTMercadoJSON* __fastcall NewObjMercadoJSON(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
