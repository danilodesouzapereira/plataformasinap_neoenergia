//---------------------------------------------------------------------------
#ifndef TRedegrafH
#define TRedegrafH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTRedegraf.h"

//---------------------------------------------------------------------------
class VTGrid;

//---------------------------------------------------------------------------
class TRedegraf : public VTRedegraf
   {
   public:
              __fastcall  TRedegraf(VTApl *apl);
              __fastcall ~TRedegraf(void);
      bool    __fastcall  Area(TList *lisBAR, VTArea *area);
      VTArea* __fastcall  Area(VTCelula *celula);
	  VTArea* __fastcall  Area(VTRede *rede);
      VTArea* __fastcall  Area(VTRedes *redes);
      bool    __fastcall  CoordFigComentario(VTFiguraComentario *comentario, double escala, int &x1, int &y1, int &x2, int &y2);
      bool    __fastcall  CoordFigLink(VTLink *link, double escala, int &x1, int &y1, int &x2, int &y2);
	  bool    __fastcall  CoordFigRetangulo(VTRetangulo *retangulo, double escala, int &x1, int &y1, int &x2, int &y2);

   private: //métodos acessados via property
      VTCoordenada* __fastcall PM_GetCoordEqpto(VTEqpto *eqpto);
      VTCoordenada* __fastcall PM_GetCoordMarca(VTMarca *marca);
      VTCoordenada* __fastcall PM_GetCoordLine(VTEqpto *eqpto);
      VTCoordenada* __fastcall PM_GetCoordMutua(VTMutua *mutua);
      VTCoordenada* __fastcall PM_GetCoordRede(VTRede *rede);
      VTCoordenada* __fastcall PM_GetCoordTrafo3E(VTTrafo3E *trafo3e);
      VTCoordenada* __fastcall PM_GetCoordTrecho(VTTrecho *trecho);
      TColor        __fastcall PM_GetCorFundo(void);
      int           __fastcall PM_GetModoGrafico(void);
      void          __fastcall PM_SetCorFundo(TColor color);
      void          __fastcall PM_SetModoGrafico(int modo);
      bool          __fastcall RedeGeorefrenciada(void);

   private: //métodos
      void __fastcall CalculaAreaEsquematico(TList *lisBAR, VTArea *area);
      void __fastcall CalculaAreaGeoreferenciado(TList *lisBAR, VTArea *area);
      void __fastcall CoordLine(VTEqpto *eqpto);
      void __fastcall InfosetLoad(void);
      void __fastcall InfosetSave(void);

   private: //objetos externos
      VTApl  *apl;
      VTGrid *grid;

   private: //dados locais
      int          modo_graf;
      TArea        area;
      TPoint       point;
      TLine        line;
      TPolyline    polyline;
      VTCoordenada *coordenada;
      struct   {
               TColor modo_geo;
               TColor modo_esq;
               } cor;
   };

#endif
//-----------------------------------------------------------------------------
// eof
