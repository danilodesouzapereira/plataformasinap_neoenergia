//---------------------------------------------------------------------------
#ifndef TEdtLigaH
#define TEdtLigaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRedegraf;
class VTRede;

//---------------------------------------------------------------------------
class TEdtLiga : public TEdt
{
public:
              __fastcall TEdtLiga(VTApl *apl, TCursor cursor, TAction *ResetFocus,
                                  TAction *SetFocus, TAction *OnDone);
      virtual __fastcall ~TEdtLiga(void);

public: //metodos que podem ser redefinidos pelas classes derivadas
   virtual void __fastcall EvtMouseDownMove(void);
   virtual void __fastcall EvtMouseUp(void);
   virtual void __fastcall EvtMouseDown(void);
   virtual void __fastcall EvtMouseMove(void);

protected: //metodos a serem definidos pelas classes derivadas
   virtual bool __fastcall ConfirmaLigacao(void)=0;
   virtual void __fastcall CriaLigacao(void)=0;

private: //metodos
   bool       __fastcall BarraInicial(VTBarra *barra);
   VTBarra*   __fastcall BuscaBarra(void);
   VTLigacao* __fastcall BuscaLigacao(void);
   VTBarra*   __fastcall ClonaBarra1(void);
   void       __fastcall InsereLigacao(bool barra_clone);
   void       __fastcall MoveLigacao(VTLigacao *ligacao, VTBarra *barra_destino);
   void       __fastcall RemoveLigacaoRecemInserida(VTLigacao *lig);
   void       __fastcall RetiraLigacao(void);
   void       __fastcall OrdenaBarras(void);
   bool       __fastcall VnomDiferente(double vnom1, double vnom2);
   bool       __fastcall VnomIgual(double vnom1, double vnom2);

protected:  //objetos externos
      VTRedegraf *redegraf;

protected: //dados locais
   enum      enumEST {SelecaoBarraInicial=0, SelecaoBarraFinal};
   int       estado;
   VTBarra   *pbarra1, *pbarra2, *bartemp;
   VTLigacao *pliga;
};

#endif
//---------------------------------------------------------------------------
//eof
