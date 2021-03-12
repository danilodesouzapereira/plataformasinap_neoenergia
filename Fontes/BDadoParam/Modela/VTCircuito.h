//---------------------------------------------------------------------------
#ifndef VTCircuitoH
#define VTCircuitoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class VTCircuito : public TObject
   {
	public:
		__property bool       Carregado = {read=PM_GetCarregado, write=PM_SetCarregado};
		__property AnsiString Codigo    = {read=PM_GetCodigo,    write=PM_SetCodigo};
		__property AnsiString CodigoRede= {read=PM_GetCodigoRede,    write=PM_SetCodigoRede};
		__property VTRede*    Rede      = {read=PM_GetRede,      write=PM_SetRede};

   public:
								 __fastcall VTCircuito(void) {};
		virtual            __fastcall ~VTCircuito(void) {};
		virtual TList*     __fastcall ListaOcorr(void) = 0;

	public:  //métodos acessados via property
		virtual bool       __fastcall PM_GetCarregado(void) = 0;
		virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
		virtual AnsiString __fastcall PM_GetCodigoRede(void) = 0;
		virtual VTRede*    __fastcall PM_GetRede(void) = 0;
		virtual void       __fastcall PM_SetCarregado(bool flag) = 0;
		virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetCodigoRede(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetRede(VTRede *rede) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTCircuito* __fastcall NewObjCircuito(void);

#endif
//---------------------------------------------------------------------------
//eof
