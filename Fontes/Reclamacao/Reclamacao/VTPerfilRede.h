//---------------------------------------------------------------------------
#ifndef VTPerfilRedeH
#define VTPerfilRedeH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTPerfilBarra;
class VTPerfilCarga;
class VTPerfilRegulador;
class VTPerfilSup;
class VTSuprimento;
class VTRegulador;

//---------------------------------------------------------------------------
class VTPerfilRede : public TObject
   {
   public:  //property
//      __property bool Enabled  = {read=PM_GetEnabled};

   public:
									      __fastcall  VTPerfilRede(void) {};
									      __fastcall ~VTPerfilRede(void) {};
//		virtual void               __fastcall  CalculaDRA_DRP_DRC(int limite_amostra=-1) = 0;
//		virtual void               __fastcall  CalculaIndicesQualidadeUmDia(int ind_dia) = 0;
//		virtual VTPerfilBarra*     __fastcall  ExistePerfilBarra(VTBarra *barra) = 0;
      virtual VTPerfilCarga*     __fastcall  ExistePerfilCarga(VTCarga *carga) = 0;
//      virtual VTPerfilRegulador* __fastcall  ExistePerfilReg(VTRegulador *reg) = 0;
      virtual VTPerfilSup*       __fastcall  ExistePerfilSup(VTSuprimento *sup) = 0;
//		virtual TList*             __fastcall  LisPerfilBarra(void) = 0;
      virtual void               __fastcall  Reinicia(void) = 0;
      virtual bool               __fastcall  RestauraPerfilCarga(TDateTime data) = 0;
//      virtual bool               __fastcall  SalvaPerfilBarra(TDateTime data) = 0;
		virtual bool               __fastcall  SalvaPerfilCarga(TDateTime data) = 0;
//		virtual bool               __fastcall  SalvaPerfilRegulador(TDateTime data) = 0;
		virtual bool               __fastcall  SalvaPerfilSuprimento(TDateTime data) = 0;

   protected:  //métodos acessados via property
//      virtual bool           __fastcall  PM_GetEnabled(void) = 0;
   };

//---------------------------------------------------------------------------
VTPerfilRede* __fastcall NewObjPerfilRede(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


