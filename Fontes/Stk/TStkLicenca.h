//---------------------------------------------------------------------------
#ifndef TStkLicencaH
#define TStkLicencaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTStkLicenca.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLicenca;

//---------------------------------------------------------------------------
class TStkLicenca : public VTStkLicenca
	{
   public :
			        __fastcall  TStkLicenca(VTApl *apl_owner);
			        __fastcall ~TStkLicenca(void);
      int        __fastcall  EmpresaId(void);
      AnsiString __fastcall  EmpresaNome(void);
      AnsiString __fastcall  UsuarioNome(void);
      AnsiString __fastcall  UsuarioSenha(void);

	private: //objetos externos
      VTApl     *apl;
      VTLicenca *licenca;

	private: //métodos

	private: //dados locais
	};

//---------------------------------------------------------------------------
#endif
//eof
