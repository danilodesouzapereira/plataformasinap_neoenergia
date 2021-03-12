//---------------------------------------------------------------------------
#ifndef VTGrupoH
#define VTGrupoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTDado;
class VTVetval;

//---------------------------------------------------------------------------
class VTGrupo : public TObject
   {
   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetDescricao(void) = 0;
      virtual VTEqpto*   __fastcall PM_GetEqpto(void) = 0;
      virtual TList*     __fastcall PM_GetLisDado(void) = 0;
      virtual TList*     __fastcall PM_GetLisGrupo(void) = 0;
      virtual TList*     __fastcall PM_GetLisVetval(void) = 0;

   __published:  //property
      __property AnsiString Descricao  = {read=PM_GetDescricao};
      __property VTEqpto*   Eqpto      = {read=PM_GetEqpto};
      __property TList*     LisDado    = {read=PM_GetLisDado};
      __property TList*     LisGrupo   = {read=PM_GetLisGrupo};
      __property TList*     LisVetval  = {read=PM_GetLisVetval};

   public:
                   __fastcall VTGrupo(void) {};
      virtual      __fastcall ~VTGrupo(void) {};
      virtual void __fastcall Add(VTDado   *dado) = 0;
      virtual void __fastcall Add(VTGrupo  *grupo) = 0;
      virtual void __fastcall Add(VTVetval *vetval) = 0;
   };
   
//---------------------------------------------------------------------------
//função global p/ criar objetos VTGrupo
//---------------------------------------------------------------------------
VTGrupo* __fastcall NewObjGrupo(VTEqpto *eqpto, AnsiString descricao);

//---------------------------------------------------------------------------
#endif
//eof
