//---------------------------------------------------------------------------
#ifndef TBuscaH
#define TBuscaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBusca.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTLigacao;
class VTMutua;
class VTGrafico;
class VTRede;
class VTRedes;
class VTRedegraf;
class VTReducao;
class VTTrecho;
class VTConsulta;
class VTMedicao;
class VTFiguraComentario;
class VTLink;
class VTRetangulo;
class VTFiguras;

//---------------------------------------------------------------------------
class TBusca : public VTBusca
   {
   public:
              __fastcall TBusca(VTApl *apl);
              __fastcall ~TBusca(void);
      TList*  __fastcall Barra(VTArea *area);
      TList*  __fastcall Capacitor(VTArea *area);
      TList*  __fastcall Carga(VTArea *area);
      TList*  __fastcall Chave(VTArea *area);
      TList*  __fastcall Chave(VTBarra *pbar);
      TList*  __fastcall Eqbar(VTArea *area);
      TList*  __fastcall Eqpto(VTArea *area);
      TList*  __fastcall Eqpto(VTBarra *pbar);
	  TList*  __fastcall FigComentario(VTArea *area);
	  TList*  __fastcall FigLink(VTArea *area);
	  TList*  __fastcall FigRetangulo(VTArea *area);
	  TList*  __fastcall Figura(VTArea *area);
      TList*  __fastcall Gerador(VTArea *area);
      TList*  __fastcall Ligacao(VTArea *area);
      TList*  __fastcall Ligacao(VTBarra *pbar, bool visible);
      TList*  __fastcall Ligacao(TList *lisBAR);
      TList*  __fastcall LigacaoEsquematicoReducao(VTBarra *pbar);
      TList*  __fastcall LigacaoNaoChave(VTArea *area);
      TList*  __fastcall Mutua(VTArea *area);
      TList*  __fastcall Mutua(VTBarra *pbar);
      TList*  __fastcall Reducao(VTArea *area);
      TList*  __fastcall ReducaoAll(VTArea *area);
      TList*  __fastcall Suprimento(VTArea *area );
      TList*  __fastcall Trafo(VTArea *area);
      TList*  __fastcall Trafo3E(VTArea *area);
      TList*  __fastcall Trecho(VTArea *area);
      TList*  __fastcall Yref(VTArea *area);
      //função específica p/ editor: TMarca não é um eqpto da RedeSUb
      virtual VTMarca* __fastcall Marca(TList *lisMAR, VTArea *area);

   private: //métodos
      void   __fastcall AchaBarra(TList *lisBAR);
      void   __fastcall AchaEqbar(TList *lisEQB);
      void  __fastcall  AchaFigComentario(void);
      void  __fastcall  AchaFigLink(void);
      void  __fastcall  AchaFigRetangulo(void);
      void   __fastcall AchaLigacao(TList *lisLIG);
      void   __fastcall AchaMutua(TList *lisMUTUA);
      void   __fastcall AchaRedeEsquematico(TList *lisREDE);
      void   __fastcall AchaReducao(TList *lisREDU);
      bool   __fastcall AchaNaDiagonal(int x1, int y1, int x2, int y2);
      bool   __fastcall AreaContemBarra(VTBarra *pbar);
      bool   __fastcall AreaContemEqbar(VTEqbar *eqbar);
      bool   __fastcall AreaContemFigComentario(VTFiguraComentario *comentario);
      bool   __fastcall AreaContemFigLink(VTLink *link);
      bool   __fastcall AreaContemFigRetangulo(VTRetangulo *retangulo);
      bool   __fastcall AreaContemLigacao(VTLigacao *pliga);
      bool   __fastcall AreaContemMarca(VTMarca *marca);
      bool   __fastcall AreaContemMutua(VTMutua *mutua);
      bool   __fastcall AreaContemRedeEsquematico(VTRede *rede);
      bool   __fastcall AreaContemReducao(VTReducao *reducao);
      bool   __fastcall AreaContemReta(int x1, int y1, int x2, int y2);
      bool   __fastcall AreaContemRetangulo(int x1, int y1, int x2, int y2);
      void   __fastcall DefineDelta(void);

   private: //dados externos
      VTArea     *area;
      VTApl      *apl;
      VTGrafico  *graf;
      VTMedicao  *medicao;
      VTRedes    *redes;
      VTRedegraf *redegraf;
      VTFiguras  *figuras;

   private: //dados locais
      int         delta;
      VTConsulta *consulta;
      TList      *lis;
   };

#endif
//---------------------------------------------------------------------------
//eof
