//-----------------------------------------------------------------------------
#ifndef VTParametroH
#define VTParametroH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
//-----------------------------------------------------------------------------
class VTParametro : public TObject
	{
	public:  //property
		__property int   		   TipoEqpto  = {read=PD.tipo_eqpto, write=PD.tipo_eqpto};
		__property AnsiString   TipoStr    = {read=PM_Tipo};
		__property AnsiString   Atributo   = {read=PD.atributo,   write=PD.atributo};
		__property AnsiString   Formato    = {read=PD.fmt,        write=PD.fmt};
		__property double       Valor      = {read=PD.valor,      write=PD.valor};
		__property TStringList* Opcoes     = {read=PD.opcoes,     write=PD.opcoes};

	public:
								 __fastcall  VTParametro(void) {};
		virtual            __fastcall ~VTParametro(void) {};
		virtual AnsiString __fastcall PM_Tipo(void)=0;

	protected: //dados acessados via property
		struct   {
					AnsiString  atributo, fmt;
					double	   valor;
					int         tipo_eqpto;
					TStringList *opcoes;
					} PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTParametro
//-----------------------------------------------------------------------------
VTParametro* NewObjParametro(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
