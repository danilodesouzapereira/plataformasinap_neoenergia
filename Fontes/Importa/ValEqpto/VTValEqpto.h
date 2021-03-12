//-----------------------------------------------------------------------------
#ifndef VTValEqptoH
#define VTValEqptoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Importa\Monta\TStruct.h>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTValEqpto : public TObject
	{
	public:  //property
		__property bool		 Enabled  = {read=enabled, write=enabled};

	public:
									 __fastcall  VTValEqpto(void) {};
		virtual               __fastcall ~VTValEqpto(void) {};
		virtual bool   		 __fastcall  ValidaArranjo(strARRANJO2 &str)=0;
		virtual bool 			 __fastcall  ValidaCabo(strCABO &str)=0;
		virtual bool 			 __fastcall  ValidaCapacitor(strCAPACITOR &str)=0;
		virtual bool   		 __fastcall  ValidaCargaA(strCARGA &str)=0;
		virtual bool   		 __fastcall  ValidaCargaB(strCARGA &str)=0;
		virtual bool 				__fastcall ValidaCargaEQV(strCARGA &str) = 0;
		virtual bool 			 __fastcall  ValidaChave(strCHAVE &str)=0;
		virtual bool 			 __fastcall  ValidaGerador(strGERADOR &str)=0;
		virtual bool 			 __fastcall  ValidaRegulador(strREGULADOR &str)=0;
		virtual bool 			 __fastcall  ValidaSuporte(strSUPORTE &str)=0;
		virtual bool 			 __fastcall  ValidaSuprimento(strSUPRIMENTO &str)=0;
		virtual bool 			 __fastcall  ValidaTrafoSED(strTRAFO &str)=0;
		virtual bool 			 __fastcall  ValidaTrafoMTBT(strTRAFO &str)=0;
		virtual bool 			 __fastcall  ValidaTrechoMT(strTRECHO &str, strARRANJO1 &str_arr)=0;
		virtual bool 			 __fastcall  ValidaTrechoBT(strTRECHO &str, strARRANJO1 &str_arr)=0;

	protected: //dados acessados via property
		bool enabled;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTValEqpto
//-----------------------------------------------------------------------------
VTValEqpto* NewObjValEqpto(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
