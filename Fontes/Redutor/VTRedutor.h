//---------------------------------------------------------------------------
#ifndef VTRedutorH
#define VTRedutorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;

//---------------------------------------------------------------------------
enum modeloEQV {eqvAUTO=1, eqvEQUILIBRADO, eqvDESEQUILIBRADO};

//---------------------------------------------------------------------------
class VTRedutor : public TObject
   {
   public:  //property
      __property bool AvisoEnabled      = {write=PM_SetAvisoEnabled};
      __property bool ProgressoEnabled  = {write=PM_SetProgressoEnabled};
      __property int  ModeloEqv         = {write=PM_SetModeloEqv, read=PM_GetModeloEqv};

   public:
                   __fastcall  VTRedutor(void) {};
                   __fastcall ~VTRedutor(void) {};
      virtual bool __fastcall  ExecutaReducaoMNet(void) = 0;
      virtual bool __fastcall  ExecutaReducaoAllRede(TList *lisBAR, bool calcular_fluxo) = 0;
      virtual bool __fastcall  ExecutaReducaoUmaRede(VTRede *rede, TList *lisBAR, bool calcular_fluxo) = 0;

   protected:  //métodos acessados via property
      virtual int   __fastcall PM_GetModeloEqv(void) = 0;
      virtual void  __fastcall PM_SetAvisoEnabled(bool enabled) = 0;
      virtual void  __fastcall PM_SetModeloEqv(int modelo_eqv) = 0;
      virtual void  __fastcall PM_SetProgressoEnabled(bool enabled) = 0;
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTRedutor
//---------------------------------------------------------------------------
VTRedutor * __fastcall NewObjRedutor(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 