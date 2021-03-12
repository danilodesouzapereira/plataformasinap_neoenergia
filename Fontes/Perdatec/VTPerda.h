//---------------------------------------------------------------------------
#ifndef VTPerdaH
#define VTPerdaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum SEG_ID {segCABO, segTRF, segTRF_CU, segTRF_FE, segRAMAL, segMEDIDOR, segOUTROS, segFLUXO, segTOTAL, segFORA};

//---------------------------------------------------------------------------
class VTPerda : public TObject
   {
   public:  //property
      __property TObject*   Object                   = {read=PM_GetObject,          write=PM_SetObject};
      __property AnsiString Descricao                = {read=PM_GetDescricao,       write=PM_SetDescricao};
      __property double     Energia_mwh[int seg_id]  = {read=PM_GetEnergia_mwh,     write=PM_SetEnergia_mwh};
      //__property VTPerda*   PerdaMontante          = {read=PM_GetPerdaMontante,  write=PM_SetPerdaMontante};

   public:
                     __fastcall  VTPerda(void) {};
      virtual        __fastcall ~VTPerda(void) {};
      virtual void   __fastcall  Soma(VTPerda *perda) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetDescricao(void) = 0;
      virtual double     __fastcall PM_GetEnergia_mwh(int seg_id) = 0;
      virtual TObject*   __fastcall PM_GetObject(void) = 0;
      virtual void       __fastcall PM_SetDescricao(AnsiString descricao) = 0;
      virtual void       __fastcall PM_SetObject(TObject *object) = 0;
      virtual void       __fastcall PM_SetEnergia_mwh(int seg_id, double energia_mwh) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerda
//---------------------------------------------------------------------------
VTPerda* __fastcall NewObjPerda(void);

//---------------------------------------------------------------------------
#endif
//eof



 