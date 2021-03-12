//---------------------------------------------------------------------------
#ifndef TDrawH
#define TDrawH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDraw.h"
#include "StrPaint.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTEqpto;
class VTRedegraf;

//---------------------------------------------------------------------------
class TDraw : public VTDraw
   {
   public:
           __fastcall TDraw(VTApl *apl);
           __fastcall ~TDraw(void);
      void __fastcall DefObjFormGraf(TFormGraf *form);
      //void __fastcall DefObjGrid(VTGrid *grid);
      void __fastcall DefObjPadrao(TPadrao *pad);
      void __fastcall Area(VTArea *area, TColor cor);
      void __fastcall AreaTransparente(VTArea *area, TColor cor);
	  void __fastcall Barra(VTBarra *pbar, TColor cor);
	  void __fastcall Bateria(VTBateria *pbateria, TColor cor);
      void __fastcall Capacitor(VTCapacitor *pcap, TColor cor);
      void __fastcall Capserie(VTCapserie *capserie, TColor cor);
      void __fastcall Carga(VTCarga *pcar, TColor cor);
      void __fastcall CargaNL(VTCargaNL *cargaNL, TColor cor);
      void __fastcall Chave(VTChave *pchave, TColor cor);
      void __fastcall FigComentario(VTFiguraComentario *comentario, TColor cor);
      void __fastcall FigLink(VTLink *link, TColor cor);
      void __fastcall FigRetangulo(VTRetangulo *retangulo, TColor cor);
      void __fastcall Filtro(VTFiltro *filtro, TColor cor);
      void __fastcall Gerador(VTGerador *gerador, TColor cor);
      void __fastcall HabilitaBarra(bool enabled);
      void __fastcall HabilitaLegenda(bool enabled);
      void __fastcall Ligacao(VTLigacao *ligacao, TColor cor);
      void __fastcall LinhasLigacao(VTLigacao *ligacao, TColor cor);
      void __fastcall Medidor(VTMedidor *medidor, TColor cor);
      void __fastcall Motor(VTMotor *motor, TColor cor);
      void __fastcall Mutua(VTMutua *mutua, TColor cor);
      //void __fastcall Poligono(TList *lisPOINT, TColor cor, bool fill)
      void __fastcall Poligono(TList *lisPOINT, TColor cor, int dim_linha=1, bool fill=false , TColor cor_fill=clBlack);
      void __fastcall Reator(VTReator *reator, TColor cor);
      void __fastcall RedeNome(AnsiString nome, VTBarra *barra, VTLigacao *ligacao, TColor cor);
      void __fastcall Regulador(VTRegulador *regulador, TColor cor);
      void __fastcall Suprimento(VTSuprimento *psup, TColor cor);
      void __fastcall Trafo(VTTrafo *ptrf, TColor cor);
      void __fastcall Trafo3E(VTTrafo3E *ptrf, TColor cor);
      void __fastcall TrafoZZ(VTTrafoZZ *trafoZZ, TColor cor);
      void __fastcall Trecho(VTTrecho *ptre, TColor cor);
      void __fastcall Yref(VTYref *zref, TColor cor);

  private:   //métodos
      bool __fastcall AlocaPolylinePoint(int num_point);

   protected: //método a ser implementado nas classes derivadas
      virtual void __fastcall MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor) = 0;

   protected: //métodos
      void __fastcall BitMap(int bmp_index, int x, int y, int bmp_align, TColor cor);
      void __fastcall BitMap(VTChave *pchv, int x, int y, int dim, TColor cor);
      void __fastcall ChaveNA(VTChave *pchv, TColor cor);
      void __fastcall ChaveNF(VTChave *pchv, TColor cor);
      void __fastcall Codigo(int x, int y, AnsiString txt, TColor cor);
      void __fastcall CodigoUp(int x, int y, AnsiString txt, TColor cor);
      void __fastcall CodigoDown(int x, int y, AnsiString txt, TColor cor);
      int  __fastcall Declividade(int x1, int y1, int x2, int y2);
      void __fastcall Eqbar(VTEqbar *eqbar, int bmp_index, int bmp_align, TColor cor);
      void __fastcall ExibeLegenda(int x, int y, TColor cor);
      bool __fastcall IconeVetorial(int eqpto_id, int index, int x, int y, int len, int bmp_align, TColor cor);
      void __fastcall Ligacao(VTLigacao *ligacao, int bmp_index, int dim_lin, TColor cor);

   protected: //dados externos
      TFormGraf  *form;
      TPadrao    *padrao;
      VTApl      *apl;
      //VTGrid     *grid;
      VTRedegraf *redegraf;

   protected: //dados locais
      bool        barra_enabled;
      bool        legenda_enabled;
	  StrPaint    paint;
      TList       *lisDADO;

   private: //dados
      struct{
            int    size;
            TPoint *point;
            }polyline;
   };

//---------------------------------------------------------------------------
#endif
//eof
