//---------------------------------------------------------------------------
#ifndef VTGrupoH
#define VTGrupoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArranjo;
class VTMutua;
class VTLigacao;

//---------------------------------------------------------------------------
class VTGrupo : public TObject
   {
   public:  //property
      __property VTArranjo *Arranjo = {read=PM_GetArranjo, write=PM_SetArranjo};

   public:
                     __fastcall VTGrupo(void) {};
      virtual        __fastcall ~VTGrupo(void) {};
      virtual bool   __fastcall ExisteLigacao(VTLigacao *ligacao) = 0;
      virtual bool   __fastcall ExisteMutua(VTMutua *mutua) = 0;
      virtual int    __fastcall IndexOf(VTMutua *mutua) = 0;
      virtual void   __fastcall InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void   __fastcall InsereMutua(VTMutua *mutua) = 0;
      virtual TList* __fastcall LisLigacao(void) = 0;
      virtual TList* __fastcall LisMutua(void) = 0;
      virtual void   __fastcall RemoveMutua(VTMutua *mutua) = 0;
      virtual void   __fastcall Reinicia(void) = 0;

   protected:  //métodos acessados via property
      virtual VTArranjo* __fastcall PM_GetArranjo(void) = 0;
      virtual void       __fastcall PM_SetArranjo(VTArranjo *arranjo) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTGrupo* __fastcall NewObjGrupo(void);

//---------------------------------------------------------------------------
#endif
//eof
