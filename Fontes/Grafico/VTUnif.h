//---------------------------------------------------------------------------
#ifndef VTUnifH
#define VTUnifH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class TFormGraf;
class TPadrao;
class VTAnimacao;
class VTEdt;

//---------------------------------------------------------------------------
class VTUnif : public TObject
   {
   public:  //property
      __property AnsiString Nome = {read=PM_GetNome                               };
      __property bool       AnimacaoEnabled     = {write=PM_SetAnimacaoEnabled    };
      __property bool       MapaTematicoEnabled = {write=PM_SetMapaTematicoEnabled};

   public:
                    __fastcall VTUnif(void) {};
      virtual       __fastcall ~VTUnif(void) {};
      virtual void  __fastcall Animacao(void) = 0;
      virtual void  __fastcall DefObjAnimacao(VTAnimacao *animacao) = 0;
      virtual void  __fastcall DefObjFormGraf(TFormGraf *form) = 0;
      virtual void  __fastcall DefObjPadrao(TPadrao *padrao) = 0;
      virtual void  __fastcall DestacaEqpto(TList *lisEQP, TColor color, int dim) = 0;
      virtual void  __fastcall LimpaTela(void) = 0;
      virtual void  __fastcall Moldura(TList *lisEQP) = 0;
      virtual void  __fastcall MolduraSolida(TList *lisEQP, TColor color) = 0;
      virtual void  __fastcall Redraw(VTEdt *editor) = 0;
      virtual void  __fastcall Redraw(int op, VTEdt *editor) = 0;
      virtual int   __fastcall Tipo(void) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetNome(void) = 0;
      virtual void       __fastcall PM_SetAnimacaoEnabled(bool enabled) = 0;
      virtual void       __fastcall PM_SetMapaTematicoEnabled(bool enabled) = 0;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos de classes derivadas de VTUnif
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifCabo(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifConfiab(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifCurto(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifFase(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifFlow(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifRegulador(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifTopo(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifDiag(VTApl *apl, int tipo_graf, AnsiString nome);
VTUnif* NewObjUnifTopobp(VTApl *apl, int tipo_graf, AnsiString nome);

#endif
//---------------------------------------------------------------------------
//eof
