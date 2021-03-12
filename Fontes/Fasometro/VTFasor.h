//---------------------------------------------------------------------------
#ifndef VTFasorH
#define VTFasorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;
class VTBarra;
class VTLigacao;
class VTBarCC;
class VTLigCC;

//---------------------------------------------------------------------------
class VTFasor : public TObject
   {
   public:  //property
      __property bool TipoCurto  = {read=PM_GetTipoCurto};
      __property bool TipoFluxo  = {read=PM_GetTipoFluxo};

   public:
                   __fastcall VTFasor(void) {};
      virtual      __fastcall ~VTFasor(void) {};
      virtual void __fastcall DefinePatamar(int ind_pat=0) = 0;

   protected:  //métodos acessados via property
      virtual bool __fastcall PM_GetTipoCurto(void) = 0;
      virtual bool __fastcall PM_GetTipoFluxo(void) = 0;

   public: //dados
      VTEqpto    *eqpto;
      AnsiString codigo;
      bool       visible;
      TColor     color;
      int        atr;
      int        ind_bar;
      double     dim, raio;
      struct   {
               bool       enabled;
               double     x, y;
               AnsiString codigo;
               } orig, a, b, c;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFasor
//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasor(VTEqpto *eqpto, TColor color, int atr, int ind_bar=0, bool visible=true);
//VTFasor* __fastcall NewObjFasor(VTBarra *barra, TColor color, int atr, int ind_bar=0, bool visible=true);
//VTFasor* __fastcall NewObjFasor(VTLigacao *ligacao, TColor color, int atr, int ind_bar=0, bool visible=true);
VTFasor* __fastcall NewObjFasor(VTBarCC *barCC, TColor color, int atr, int ind_bar=0, bool visible=true);
VTFasor* __fastcall NewObjFasor(VTLigCC *ligCC, TColor color, int atr, int ind_bar=0, bool visible=true);

//---------------------------------------------------------------------------
#endif
//eof
