//---------------------------------------------------------------------------
#ifndef TFigurasH
#define TFigurasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFiguras.h"

//---------------------------------------------------------------------------
class VTAlternativa;
class VTFiguraComentario;
class VTEstilos;
class VTObra;
class VTElemVisual;
//---------------------------------------------------------------------------
class TFiguras : public VTFiguras
   {
   public:
                    __fastcall  TFiguras(VTApl *apl_owner);
					__fastcall ~TFiguras(void);
      bool          __fastcall  DestroiFigura(VTFigura *figura);
	  bool          __fastcall  ExisteAlteracao(void);
	  VTFiguraComentario* __fastcall  ExisteComentario(int figura_id);
	  VTFiguraComentario* __fastcall  InsereComentario(bool novoEstilo);
	  VTFiguraComentario* __fastcall  InsereComentario(strRETANGULO *ref);
	  bool          __fastcall  InsereComentario(VTFiguraComentario *comentario);
	  VTRetangulo*  __fastcall  InsereRetangulo(bool novoEstilo);
	  VTRetangulo*  __fastcall  InsereRetangulo(strRETANGULO *ref);
      bool          __fastcall  InsereRetangulo(VTRetangulo *retangulo);
      TList*        __fastcall  LisComentario(void);
	  TList*        __fastcall  LisRetangulo(void);
	  void 			__fastcall  ObraAtiva(VTObra *obra);
      bool 			__fastcall  RedefineIds(void);
	  bool          __fastcall  RetiraComentario(VTFiguraComentario *comentario);
      bool          __fastcall  RetiraFigura(VTFigura *figura);
      bool          __fastcall  RetiraLink(VTLink *link);
      bool          __fastcall  RetiraRetangulo(VTRetangulo *retangulo);

   private:
      VTAlternativa* __fastcall AlternativaAtiva(void);
	  VTFiguraComentario* __fastcall  ComentarioPai(VTLink *link);
	  VTFiguraComentario* __fastcall  ComentarioPai(VTRetangulo *retangulo);
	  bool          __fastcall  DeveInserir(strRETANGULO *ref);
	  VTObra* 		__fastcall  ExisteObra(int obra_id);
	  bool 			__fastcall  RedefineIds(TList *lisELEM_VISUAL);
	  bool 			__fastcall  RedefineIds(VTElemVisual *elem);

   private: //objetos externos
      VTApl *apl;
//	  VTEstilos *estilos;
      VTEstudo *estudo;

   private: //dados locais
      TList *lisCOMENTARIO;
      TList *lisRETANGULO;
   };

#endif
//---------------------------------------------------------------------------
//eof

