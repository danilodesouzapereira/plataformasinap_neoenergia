//---------------------------------------------------------------------------
#ifndef VTFasoresH
#define VTFasoresH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTBarCC;
class VTLigCC;

//---------------------------------------------------------------------------
class VTFasores : public TObject
   {
   public:  //property
      __property bool TipoCurto  = {read=PM_GetTipoCurto};
      __property bool TipoFluxo  = {read=PM_GetTipoFluxo};

   public:
                     __fastcall VTFasores(void) {};
      virtual        __fastcall ~VTFasores(void) {};
      virtual void   __fastcall DefinePatamar(int ind_pat=0) = 0;
      virtual void   __fastcall Insere(VTBarra *barra) = 0;
      virtual void   __fastcall Insere(VTBarCC *barCC) = 0;
      virtual void   __fastcall Insere(VTLigacao *ligacao) = 0;
      virtual void   __fastcall Insere(VTLigCC *ligCC) = 0;
      virtual TList* __fastcall LisFasor(void) = 0;
      virtual void   __fastcall Reinicia(void) = 0;

   protected:  //m�todos acessados via property
      virtual bool __fastcall PM_GetTipoCurto(void) = 0;
      virtual bool __fastcall PM_GetTipoFluxo(void) = 0;
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTFasores
//---------------------------------------------------------------------------
VTFasores* __fastcall NewObjFasores(void);

//---------------------------------------------------------------------------
#endif
//eof
