//---------------------------------------------------------------------------
#ifndef VTEloH
#define VTEloH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBloco;
class VTChave;
class VTRede;

//---------------------------------------------------------------------------
class VTElo : public TObject
   {
   public:
      __property  VTBloco *Bloco1 = {read=PM_GetBloco1, write=PM_SetBloco1};
      __property  VTBloco *Bloco2 = {read=PM_GetBloco2, write=PM_SetBloco2};
      __property  VTChave *Chave  = {read=PM_GetChave,  write=PM_SetChave};

   public:
                   __fastcall VTElo(void) {};
      virtual      __fastcall ~VTElo(void) {};
      virtual bool __fastcall ConectaBloco(VTBloco *bloco) = 0;
      virtual bool __fastcall ConectaRede(VTRede *rede) = 0;
      virtual bool __fastcall ConectaRede1Rede2(VTRede *rede1, VTRede *rede2) = 0;

   public:  //métodos acessados via property
      virtual VTBloco* __fastcall PM_GetBloco1(void) = 0;
      virtual VTBloco* __fastcall PM_GetBloco2(void) = 0;
      virtual VTChave* __fastcall PM_GetChave(void)  = 0;
      virtual void     __fastcall PM_SetBloco1(VTBloco *bloco) = 0;
      virtual void     __fastcall PM_SetBloco2(VTBloco *bloco) = 0;
      virtual void     __fastcall PM_SetChave(VTChave *chave) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTElo* __fastcall NewObjElo(void);

#endif
//---------------------------------------------------------------------------
//eof
