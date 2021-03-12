//---------------------------------------------------------------------------
#ifndef VTBalanceamentoH
#define VTBalanceamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;

//---------------------------------------------------------------------------
class VTBalanceamento : public TObject
   {
   public:
								 __fastcall  VTBalanceamento() {};
								 __fastcall ~VTBalanceamento(void) {};
		virtual bool   	 __fastcall  AlteraRede(TList *lisCAR) = 0;
		virtual bool   	 __fastcall  Executa(VTRede *rede) = 0;
		virtual int    	 __fastcall  IndicePatamarPico(void) = 0;
		virtual TList* 	 __fastcall  LisCarga(void) = 0;
		virtual TList* 	 __fastcall  LisCloneCargaAlterada(void) = 0;
		virtual TList* 	 __fastcall  LisCloneCargaOriginal(void) = 0;
		virtual AnsiString __fastcall  TipoZona(VTRede *rede) = 0;
	};

//---------------------------------------------------------------------------
VTBalanceamento* __fastcall NewObjBalanceamento(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
