//---------------------------------------------------------------------------
#ifndef VTCaboH
#define VTCaboH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
//enum que define conjunto de par�metros do Cabo
enum enumCFG_CABO {caboR_GMR=0, caboZ0Z1, caboPRE_REUNIDO};

//---------------------------------------------------------------------------
class VTCabo : public VTEqpto
   {
   protected: //m�todos acessados via property
      virtual double   __fastcall PM_GetIadm_A(void) = 0;
      virtual int      __fastcall PM_GetTipoCfg(void) = 0;
      virtual double   __fastcall PM_GetVmax_KV(void) = 0;
      virtual double   __fastcall PM_GetVmin_KV(void) = 0;
      virtual void     __fastcall PM_SetIAdm_A(double iadm) = 0;
      virtual void     __fastcall PM_SetTipoCfg(int tipo_cfg) = 0;
      virtual void     __fastcall PM_SetVmax_KV(double vmax) = 0;
      virtual void     __fastcall PM_SetVmin_KV(double vmin) = 0;

   public:  //property
      __property double Vmax_KV  = {read=PM_GetVmax_KV,  write=PM_SetVmax_KV};
      __property double Vmin_KV  = {read=PM_GetVmin_KV,  write=PM_SetVmin_KV};
      __property double Iadm_A   = {read=PM_GetIadm_A,   write=PM_SetIAdm_A};
      __property int    TipoCfg  = {read=PM_GetTipoCfg,   write=PM_SetTipoCfg};

   public: //m�todos
                     __fastcall VTCabo(void) {};
     virtual         __fastcall ~VTCabo(void) {};
     virtual VTCabo* __fastcall Clone(void) = 0;

   public: //dados
      float   r;           //resist�ncia (ohm/km)
      float   raio;         //raio(m)
      float   gmr;         //raio m�dio geom�trico (m)
      strIMP  z0;          //imped�ncia de seq.0 (ohm/km)
      strIMP  z1;          //imped�ncia de seq.1 (ohm/km)
      float   c0;          //capacit�nca de seq.0 (nF/km)
      float   c1;          //capacit�nca de seq.1 (nF/km)
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTCabo
//---------------------------------------------------------------------------
VTCabo* __fastcall NewObjCabo(void);

//---------------------------------------------------------------------------
#endif
//eof

