//---------------------------------------------------------------------------
#ifndef VTBuscaH
#define VTBuscaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArea;
class VTBarra;
class VTApl;
class VTMarca;

//---------------------------------------------------------------------------
class VTBusca : public TObject
   {
   public:
                      __fastcall VTBusca(void) {};
      virtual         __fastcall ~VTBusca(void) {};
      virtual TList*  __fastcall Barra(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Capacitor(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Carga(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Chave(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Chave(VTBarra *pbar) = 0;
      virtual TList*  __fastcall Eqbar(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Eqpto(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Eqpto(VTBarra *pbar) = 0;
      virtual TList*  __fastcall FigComentario(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall FigLink(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall FigRetangulo(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Figura(VTArea *area) = 0;
      virtual TList*  __fastcall Gerador(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Ligacao(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Ligacao(VTBarra *pbar, bool visible=true) = 0;
      virtual TList*  __fastcall Ligacao(TList *lisBAR) = 0;                    
      //virtual TList*  __fastcall LigacaoEsquematicoRede(VTBarra *pbar) = 0;
      virtual TList*  __fastcall LigacaoEsquematicoReducao(VTBarra *pbar) = 0;
      virtual TList*  __fastcall LigacaoNaoChave(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Mutua(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Mutua(VTBarra *pbar) = 0;
      virtual TList*  __fastcall Reducao(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall ReducaoAll(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Suprimento(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Trafo(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Trafo3E(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Trecho(VTArea *area = NULL) = 0;
      virtual TList*  __fastcall Yref(VTArea *area) = 0;
      //função específica p/ editor: TMarca não é um eqpto da Rede
      virtual VTMarca* __fastcall Marca(TList *lisMAR, VTArea *area) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTBusca
VTBusca *NewObjBusca(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
