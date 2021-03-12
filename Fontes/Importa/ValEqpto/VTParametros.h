//-----------------------------------------------------------------------------
#ifndef VTParametrosH
#define VTParametrosH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
class VTParametro;

//---------------------------------------------------------------------------
enum tipoEQP {//tipos de equipamentos
					 tipoINDEF=-1,
					 tipoARRANJO,
					 tipoCABO,
					 tipoCAPACITOR,
					 tipoCARGA_A,
					 tipoCARGA_B,
					 tipoCHAVE,
					 tipoGERADOR,
					 tipoREGULADOR,
					 tipoSUPORTE,
					 tipoSUPRIMENTO,
					 tipoTRAFOSE,
					 tipoTRAFOMT,
					 tipoTRECHOMT,
					 tipoTRECHOBT
					 };

//-----------------------------------------------------------------------------
class VTParametros : public TObject
	{
	public:  //property
		__property TList*		 LisParametros   	 = {read=PM_LisParametros};

	public:
									 __fastcall  VTParametros(void) {};
		virtual               __fastcall ~VTParametros(void) {};
		virtual bool       	 __fastcall  Atualiza(int tipo, AnsiString atributo, double valor)=0;
		virtual VTParametro*  __fastcall  Busca(int tipo, AnsiString atributo)=0;
		virtual bool			 __fastcall  Busca(int tipo, TList *lisPAR)=0;
		virtual bool       	 __fastcall  Valor(int tipo, AnsiString atributo, double &valor)=0;

	protected: //dados acessados via property
		virtual TList* __fastcall PM_LisParametros(void)=0;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTParametros
//-----------------------------------------------------------------------------
VTParametros* NewObjParametros(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
