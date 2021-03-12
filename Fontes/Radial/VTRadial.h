//-----------------------------------------------------------------------------
#ifndef VTRadialH
#define VTRadialH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRedes;
class VTPrimario;
class VTReticulado;
class VTSecundario;
class VTSubestacao;

//-----------------------------------------------------------------------------
class VTRadial  : public TObject
   {
   public:
                            __fastcall  VTRadial(void)  {};
      virtual               __fastcall ~VTRadial(void) {};
      virtual VTPrimario*   __fastcall  ExistePrimario(VTRede *rede) = 0;
      virtual VTReticulado* __fastcall  ExisteReticulado(VTRede *rede) = 0;
      virtual VTSecundario* __fastcall  ExisteSecundario(VTRede *rede) = 0;
	  virtual VTSubestacao* __fastcall  ExisteSubestacao(VTRede *rede) = 0;
	  virtual bool          __fastcall  Inicia(VTRedes *redes) = 0;
      virtual TList*        __fastcall  LisPrimario(void) = 0;
      virtual void          __fastcall  LisRede(TList *lisEXT, int segmento) = 0;
      virtual TList*        __fastcall  LisSecundario(void) = 0;
      virtual TList*        __fastcall  LisSubestacao(void) = 0;
	};

//-----------------------------------------------------------------------------
//função global
VTRadial* __fastcall NewObjRadial(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
