//---------------------------------------------------------------------------
#ifndef VTDrawH
#define VTDrawH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TFormGraf;
class TPadrao;
class VTApl;
class VTArea;
class VTBarra;
class VTBateria;
class VTCarga;
class VTCargaNL;
class VTCapserie;
class VTCapacitor;
class VTChave;
class VTFiguraComentario;
class VTEqbar;
class VTEqpto;
class VTFiltro;
class VTFigura;
class VTGerador;
class VTLigacao;
class VTLink;
class VTMarca;
class VTMedidor;
class VTMotor;
class VTMutua;
class VTReator;
class VTRede;
class VTRedes;
class VTReducao;
class VTRegulador;
class VTRetangulo;
class VTSuprimento;
class VTTrafo;
class VTTrafo3E;
class VTTrafoZZ;
class VTTrecho;
class VTYref;

//---------------------------------------------------------------------------
class VTDraw : public TObject
   {
   public:
                   __fastcall VTDraw(void) {};
      virtual      __fastcall ~VTDraw(void) {};
      virtual void __fastcall DefObjFormGraf(TFormGraf *form) = 0;
      virtual void __fastcall DefObjPadrao(TPadrao *padrao) = 0;
      virtual void __fastcall Area(VTArea *area, TColor cor) = 0;
      virtual void __fastcall AreaTransparente(VTArea *area, TColor cor) = 0;
	  virtual void __fastcall Barra(VTBarra *pbar, TColor cor) = 0;
	  virtual void __fastcall Bateria(VTBateria *pbateria, TColor cor) = 0;
      virtual void __fastcall Capacitor(VTCapacitor *pcap, TColor cor) = 0;
      virtual void __fastcall Capserie(VTCapserie *capserie, TColor cor) = 0;
      virtual void __fastcall Carga(VTCarga *pcar, TColor cor) = 0;
      virtual void __fastcall CargaNL(VTCargaNL *cargaNL, TColor cor) = 0;
      virtual void __fastcall Chave(VTChave *pchave, TColor cor) = 0;
      virtual void __fastcall EqptoNovo(VTEqpto *eqpto) = 0;
	  virtual void __fastcall FigComentario(VTFiguraComentario *comentario, TColor cor) = 0;
	  virtual void __fastcall FigNova(VTFigura *figura) = 0;
      virtual void __fastcall FigLink(VTLink *link, TColor cor) = 0;
	  virtual void __fastcall FigRetangulo(VTRetangulo *retangulo, TColor cor) = 0;
      virtual void __fastcall Filtro(VTFiltro *filtro, TColor cor) = 0;
      virtual void __fastcall Gerador(VTGerador *gerador, TColor cor) = 0;
      virtual void __fastcall HabilitaBarra(bool enabled) = 0;
      virtual void __fastcall HabilitaLegenda(bool enabled) = 0;
      virtual void __fastcall Ligacao(VTLigacao *ligacao, TColor cor) = 0;
      virtual void __fastcall LinhasLigacao(VTLigacao *ligacao, TColor cor) = 0;
      virtual void __fastcall Marca(VTMarca *marca) = 0;
      virtual void __fastcall Medidor(VTMedidor *medidor, TColor cor) = 0;
      virtual void __fastcall Motor(VTMotor *motor, TColor cor) = 0;
      virtual void __fastcall Mutua(VTMutua *mutua, TColor cor) = 0;
      //virtual void __fastcall Poligono(TList *lisPOINT, TColor cor) = 0;
      virtual void __fastcall Poligono(TList *lisPOINT, TColor cor, int dim_linha=1, bool fill=false , TColor cor_fill=clBlack) = 0;
      virtual void __fastcall Reator(VTReator *reator, TColor cor) = 0;
      virtual void __fastcall RedeNome(AnsiString nome, VTBarra *barra, VTLigacao *ligacao, TColor cor) = 0;
      virtual void __fastcall Regulador(VTRegulador *regulador, TColor cor) = 0;
      virtual void __fastcall Suprimento(VTSuprimento *psup, TColor cor) = 0;
      virtual void __fastcall Trafo(VTTrafo *ptrf, TColor cor) = 0;
      virtual void __fastcall Trafo3E(VTTrafo3E *ptrf, TColor cor) = 0;
      virtual void __fastcall TrafoZZ(VTTrafoZZ *trafoZZ, TColor cor) = 0;
      virtual void __fastcall Trecho(VTTrecho *ptre, TColor cor) = 0;
      virtual void __fastcall Yref(VTYref *yref, TColor cor) = 0;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos da classe VTDraw
//-----------------------------------------------------------------------------
VTDraw* NewObjDrawCurto(VTApl *apl);
VTDraw* NewObjDrawFlow(VTApl *apl);
VTDraw* NewObjDrawTopo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
