//---------------------------------------------------------------------------
#ifndef VTEstiloH
#define VTEstiloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstilos.h"
#include "..\Figura\VTElemVisual.h"
//---------------------------------------------------------------------------
//enums
//---------------------------------------------------------------------------
enum estiloSTATUS {//status do Estilo
				 sttEstNOVO          =0x00000001,
				 sttEstALTERADO      =0x00000002,
				 sttEstEXCLUIDO      =0x00000004,
				 };
//---------------------------------------------------------------------------
enum estiloTIPO {//tipo de estilo
				 estiloCOMENTARIO = 0,
				 estiloLINK,
				 estiloRETANGULO,
				 estiloCOUNT
			   };
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class VTEstilo : public VTElemVisual
{

	public:  //property
	  __property AnsiString Descricao		          = {read=descricao, write=descricao};
	  __property int        TipoEstilo  			  = {read=tipo,      write=tipo};
	  __property TList*     LisEstiloAtributo         = {read=lisESTILO_ATRIBUTO};

	public:
				   __fastcall  VTEstilo(void){};
	virtual		   __fastcall ~VTEstilo(void){};

	virtual bool   __fastcall  LeEstiloAtributos(void) = 0;
	virtual bool   __fastcall  SalvaEstiloAtributos(void) = 0;
	virtual bool   __fastcall  InsereCopiaEstiloAtributo(strESTILO_ATRIBUTO *ref) = 0;
//	virtual bool   __fastcall  IsEqual(VTEstilo &ref) = 0;

	protected:
    	virtual strESTILO_ATRIBUTO* __fastcall ExisteAtributo(int atributoGraficoId) =0;
        virtual bool   __fastcall  InsereEstiloAtributo(strESTILO_ATRIBUTO *atributo) = 0;
		virtual bool   __fastcall  LeEstiloAtributos(strESTILO_ATRIBUTO *ref) = 0;
		virtual void   __fastcall  LimpaLisAtributos(void) = 0;

	protected:  //dados acessados via property
		int       tipo;
		AnsiString descricao;
		TList *lisESTILO_ATRIBUTO;

};
//---------------------------------------------------------------------------
VTEstilo* __fastcall NewObjEstiloComentario(VTApl *apl);
VTEstilo* __fastcall NewObjEstiloLink(VTApl *apl);
VTEstilo* __fastcall NewObjEstiloRetangulo(VTApl *apl);
//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------
//eof

