//---------------------------------------------------------------------------
#ifndef VTRedegraf_H
#define VTRedegraf_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCoordenada.h"
#include "..\Editor\Marca\VTMarca.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"

//---------------------------------------------------------------------------
enum modoGRAFICO {modoGEOREFERENCIADO=1, modoESQUEMATICO, modoESQ_FUNCIONAL};

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTCelula;
class VTFiguraComentario;
class VTLink;
class VTRetangulo;

//---------------------------------------------------------------------------
class VTRedegraf : public TObject
   {
   public: //property
      __property VTCoordenada* CoordBarra[VTEqpto*]     = {read=PM_GetCoordLine};
      __property VTCoordenada* CoordEqbar[VTEqpto*]     = {read=PM_GetCoordLine};
      __property VTCoordenada* CoordEqpto[VTEqpto*]     = {read=PM_GetCoordEqpto};
      __property VTCoordenada* CoordLigacao[VTEqpto*]   = {read=PM_GetCoordEqpto};
      __property VTCoordenada* CoordMutua[VTMutua*]     = {read=PM_GetCoordMutua};
      __property VTCoordenada* CoordMarca[VTMarca*]     = {read=PM_GetCoordMarca};
      __property VTCoordenada* CoordTrafo[VTEqpto*]     = {read=PM_GetCoordLine};
      __property VTCoordenada* CoordTrafo3E[VTTrafo3E*] = {read=PM_GetCoordTrafo3E};
      __property VTCoordenada* CoordTrecho[VTTrecho*]   = {read=PM_GetCoordTrecho};
      __property TColor        CorFundo                 = {read=PM_GetCorFundo,    write=PM_SetCorFundo};
      __property int           ModoGrafico              = {read=PM_GetModoGrafico, write=PM_SetModoGrafico};

   public:
                      __fastcall  VTRedegraf(void) {};
      virtual         __fastcall ~VTRedegraf(void) {};
      virtual bool    __fastcall  Area(TList *lisBAR, VTArea *area) = 0;
      virtual VTArea* __fastcall  Area(VTCelula *celula) = 0;
      virtual VTArea* __fastcall  Area(VTRede *rede) = 0;
      virtual VTArea* __fastcall  Area(VTRedes *redes) = 0;
	  virtual bool    __fastcall  CoordFigComentario(VTFiguraComentario *comentario, double escala, int &x1, int &y1, int &x2, int &y2) = 0;
	  virtual bool    __fastcall  CoordFigLink(VTLink *link, double escala, int &x1, int &y1, int &x2, int &y2) = 0;
	  virtual bool    __fastcall  CoordFigRetangulo(VTRetangulo *retangulo, double escala, int &x1, int &y1, int &x2, int &y2) = 0;

   protected: //métodos acessados via property
      virtual VTCoordenada* __fastcall PM_GetCoordEqpto(VTEqpto *eqpto) = 0;
      virtual VTCoordenada* __fastcall PM_GetCoordLine(VTEqpto *eqpto) = 0;
      virtual VTCoordenada* __fastcall PM_GetCoordMarca(VTMarca *marca) = 0;
      virtual VTCoordenada* __fastcall PM_GetCoordMutua(VTMutua *mutua) = 0;
      virtual VTCoordenada* __fastcall PM_GetCoordTrafo3E(VTTrafo3E *trafo3e) = 0;
      virtual VTCoordenada* __fastcall PM_GetCoordTrecho(VTTrecho *trecho) = 0;
      virtual TColor        __fastcall PM_GetCorFundo(void) = 0;
      virtual int           __fastcall PM_GetModoGrafico(void) = 0;
      virtual void          __fastcall PM_SetCorFundo(TColor color) = 0;
      virtual void          __fastcall PM_SetModoGrafico(int modo) = 0;

   };

//---------------------------------------------------------------------------
//protótipo de função para criar objetos VTRedegraf
//---------------------------------------------------------------------------
VTRedegraf* __fastcall NewObjRedegraf(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
