//---------------------------------------------------------------------------
#ifndef VTFiguraComentarioH
#define VTFiguraComentarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFigura.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLink;
class VTRetangulo;
class VTRede;
class VTEqpto;
//---------------------------------------------------------------------------
enum fmtCOMENTARIO {fmtRETANGULO=1, fmtFLAG, fmtELIPSE };

//---------------------------------------------------------------------------
class VTFiguraComentario : public VTFigura
   {
   public:  //property
      __property int          Formato   = {read=PD.formato,   write=PM_SetFormato};
	  __property TFont*       Font      = {read=PD.font,      write=PD.font      };
	  __property VTRede*      Rede      = {read=PD.rede,      write=PD.rede      };
      __property VTRetangulo* Retangulo = {read=PD.retangulo, write=PD.retangulo };

   public:
						__fastcall  VTFiguraComentario(void){};
						__fastcall ~VTFiguraComentario(void){};
      virtual void      __fastcall  AutoSize(void) = 0;
	  virtual void      __fastcall  ClearTexto(void) = 0;
	  virtual VTLink*   __fastcall  ExisteLink(VTEqpto *eqpto) = 0;
	  virtual VTLink*   __fastcall  InsereLink(bool create_estilo = true) = 0;
	  virtual VTLink*   __fastcall  InsereLink(VTEqpto *eqpto) = 0;
      virtual bool      __fastcall  InsereLink(VTLink *link) = 0;
	  virtual bool      __fastcall  InsereTexto(AnsiString txt) = 0;
	  virtual bool      __fastcall  InsereTexto(TStrings *lines) = 0;
      virtual TList*    __fastcall  LisLink(void) = 0;
	  virtual bool      __fastcall  RetiraLink(VTLink *link) = 0;
	  virtual bool      __fastcall  RetiraTexto(AnsiString txt) = 0;
      virtual TStrings* __fastcall  Texto(void) = 0;

   protected:  //métodos acessados via property
      virtual void      __fastcall  PM_SetFormato(int formato) = 0;

   protected:  //dados acessados via property
      struct{
            int          formato;
            TFont       *font;
            VTRetangulo *retangulo;
            VTRede      *rede;
            }PD;
   };

//---------------------------------------------------------------------------
VTFiguraComentario* __fastcall NewObjFiguraComentario(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

