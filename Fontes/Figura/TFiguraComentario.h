//---------------------------------------------------------------------------
#ifndef TFiguraComentarioH
#define TFiguraComentarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFiguraComentario.h"
//---------------------------------------------------------------------------
class VTEstilos;
//---------------------------------------------------------------------------
class TFiguraComentario : public VTFiguraComentario
   {
   public:
				__fastcall  TFiguraComentario(VTApl *apl_owner);
				__fastcall ~TFiguraComentario(void);
	  bool 		__fastcall  AplicaEstilo(VTEstilo *estilo);
	  void      __fastcall  AutoSize(void);
	  void      __fastcall  ClearTexto(void);
	  VTLink*   __fastcall  ExisteLink(VTEqpto *eqpto);
	  VTLink*   __fastcall  InsereLink(bool create_estilo);
      VTLink*   __fastcall  InsereLink(VTEqpto *eqpto);
	  bool      __fastcall  InsereLink(VTLink *link);
      bool      __fastcall  InsereTexto(AnsiString txt);
      bool      __fastcall  InsereTexto(TStrings *lines);
      TList*    __fastcall  LisLink(void);
      void      __fastcall  Move_esq(int dx, int dy);
      void      __fastcall  Move_utm(int dx, int dy);
	  bool      __fastcall  RetiraLink(VTLink *link);
	  bool      __fastcall  RetiraTexto(AnsiString txt);
	  void 		__fastcall SalvaInfoNoEstilo(void);
	  TStrings* __fastcall  Texto(void);

   protected:  //métodos acessados via property
      int  __fastcall PM_Tipo(void) {return(elemvFIGURA);};
      int  __fastcall PM_TipoFigura(void) {return(figCOMENTARIO);};
      void __fastcall PM_SetFormato(int formato);


   private: //métodos

   private: //objetos externos
	  VTApl *apl;
	  VTEstilos *styles;

   private: //dados locais
      TList    *lisLINK;
      TStrings *texto;
      struct{//dx,dy definem distância do centro às bordas
            int dx;
            int dy;
            }dim;
   };

#endif
//---------------------------------------------------------------------------
//eof

