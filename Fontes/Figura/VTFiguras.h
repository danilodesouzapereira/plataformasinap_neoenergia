//---------------------------------------------------------------------------
#ifndef VTFigurasH
#define VTFigurasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTFiguraComentario;
class VTFigura;
class VTLink;
class VTRetangulo;
class VTObra;
class VTEstudo;
//---------------------------------------------------------------------------
class strFIGURA
{
	public:
		int estudoId;
		int alternativaId;
		int obraId;
		int redeId;
		int estiloId;
		int formaId;
		int figuraId;
		bool visible;
		int esqX;
		int esqY;
		int utmX;
        int utmY;
};
//---------------------------------------------------------------------------
class strRETANGULO: public strFIGURA
{
	public:
		int dx;
        int dy;
};
//---------------------------------------------------------------------------
class VTFiguras : public TObject
   {
   public:
	   __property VTEstudo* Estudo  = {read=PD.estudo,          write=PD.estudo};

   public:
                            __fastcall  VTFiguras(void) {};
							__fastcall ~VTFiguras(void) {};
	  virtual bool          __fastcall  DestroiFigura(VTFigura *figura)=0;
	  virtual bool          __fastcall  ExisteAlteracao(void) = 0;
	  virtual VTFiguraComentario* __fastcall  ExisteComentario(int figura_id) = 0;
	  virtual VTFiguraComentario* __fastcall  InsereComentario(bool novoEstilo = true) = 0;
	  virtual VTFiguraComentario* __fastcall  InsereComentario(strRETANGULO *ref) = 0;
	  virtual bool          __fastcall  InsereComentario(VTFiguraComentario *comentario) = 0;
	  virtual VTRetangulo*  __fastcall  InsereRetangulo(bool novoEstilo = true) = 0;
	  virtual VTRetangulo*  __fastcall  InsereRetangulo(strRETANGULO *ref) = 0;
      virtual bool          __fastcall  InsereRetangulo(VTRetangulo *retangulo) = 0;
      virtual TList*        __fastcall  LisComentario(void) = 0;
	  virtual TList*        __fastcall  LisRetangulo(void) = 0;
	  virtual void 			__fastcall  ObraAtiva(VTObra *obra) = 0;
	  virtual bool 			__fastcall  RedefineIds(void) = 0;
	  virtual bool          __fastcall  RetiraComentario(VTFiguraComentario *comentario) = 0;
      virtual bool          __fastcall  RetiraFigura(VTFigura *figura) = 0;
      virtual bool          __fastcall  RetiraLink(VTLink *link) = 0;
	  virtual bool          __fastcall  RetiraRetangulo(VTRetangulo *retangulo) = 0;

   protected:
	 struct
	 {
		VTEstudo *estudo;

     }PD;
   };

//---------------------------------------------------------------------------
VTFiguras* __fastcall NewObjFiguras(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

