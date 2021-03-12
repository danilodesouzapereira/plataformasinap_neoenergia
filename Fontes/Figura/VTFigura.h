//---------------------------------------------------------------------------
#ifndef VTFiguraH
#define VTFiguraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTElemVisual.h"
//---------------------------------------------------------------------------
enum TipoFigura {figCOMENTARIO=1, figLINK, figRETANGULO, figELIPSE};
enum indPOSICAO  {posLEFT=1, posTOP, posRIGHT, posBOTTOM, posCENTER, posAUTO,
                  posEQP_X1Y1, posEQP_X2Y2, posEQP_XmYm};

//---------------------------------------------------------------------------
class VTObra;
class VTEstilo;
class VTEstilos;
class VTAlternativa;
class strVALOR_FORMA_ATRIBUTO;
#define DIM_FLAG    5
//---------------------------------------------------------------------------
class VTFigura : public VTElemVisual
   {
   public:  //property
//	  __property VTEstilos* 	Estilos     		   = {read=estilos,    write=estilos};
	  __property VTEstilo*  	Estilo      		   = {read=estilo,    write=estilo};
	  __property int        	EstudoId    		   = {read=estudoId,    write=estudoId};
	  __property TList*     	LisValorFormaAtributo  = {read=lisValorFormaAtributo};
	  __property bool       	Visible     		   = {read=PM_Visible,   write=visible};
	  __property VTObra*    	Obra        		   = {read=obra,      write=obra};
	  __property VTAlternativa* Alternativa        	   = {read=alternativa, write=alternativa};
	  __property int        	TipoFigura  		   = {read=PM_TipoFigura};


   public:
				   __fastcall  VTFigura(void);
				   __fastcall ~VTFigura(void);
      virtual bool __fastcall  AplicaEstilo(VTEstilo *estilo = NULL) = 0;
	  strVALOR_FORMA_ATRIBUTO* __fastcall InsereValorFormaAtributo(int formaAtributoId, AnsiString valor);
			  void __fastcall  LimpaLisValorFormaAtributo(void);
      virtual void __fastcall  Move_esq(int dx, int dy) = 0;
	  virtual void __fastcall  Move_utm(int dx, int dy) = 0;
      virtual void __fastcall  SalvaInfoNoEstilo(void) = 0;

   protected:  //métodos acessados via property
	  virtual int    __fastcall PM_TipoFigura(void) = 0;
      virtual bool   __fastcall PM_Visible(void);
	  strVALOR_FORMA_ATRIBUTO* __fastcall InsereValorFormaAtributo(void);

   protected:  //dados acessados via property
		bool      visible;
		VTObra    *obra;
		VTEstilo  *estilo;
		VTAlternativa *alternativa;
//		VTEstilos  *estilos;
		TList 	  *lisValorFormaAtributo;
		int       estudoId;
   };

#endif
//---------------------------------------------------------------------------
//eof

