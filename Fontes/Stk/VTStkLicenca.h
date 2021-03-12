//---------------------------------------------------------------------------
#ifndef VTStkLicencaH
#define VTStkLicencaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTStkLicenca : public TObject
	{
   public :
			                __fastcall  VTStkLicenca(void) {};
		virtual            __fastcall ~VTStkLicenca(void) {};
      virtual int        __fastcall  EmpresaId(void) = 0;
      virtual AnsiString __fastcall  EmpresaNome(void) = 0;
      virtual AnsiString __fastcall  UsuarioNome(void) = 0;
      virtual AnsiString __fastcall  UsuarioSenha(void) = 0;
	};

//---------------------------------------------------------------------------
VTStkLicenca* __fastcall NewObjStkLicenca(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
