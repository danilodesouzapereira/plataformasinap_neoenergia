//---------------------------------------------------------------------------
#ifndef VTPerfilAltH
#define VTPerfilAltH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTBarra;
class VTPerfilBarra;
class VTPerfilRegulador;
class VTRegulador;

//---------------------------------------------------------------------------
class VTPerfilAlt : public TObject
   {
   public:  //property
      __property bool Enabled  = {read=PM_GetEnabled};

   public:
									      __fastcall  VTPerfilAlt(void) {};
									      __fastcall ~VTPerfilAlt(void) {};
		virtual void               __fastcall  CalculaDRA_DRP_DRC(int limite_amostra=-1) = 0;
		virtual void               __fastcall  CalculaIndicesQualidadeUmDia(int ind_dia) = 0;
		virtual VTPerfilBarra*     __fastcall  ExistePerfilBarra(VTBarra *barra) = 0;
      virtual VTPerfilRegulador* __fastcall  ExistePerfilReg(VTRegulador *reg) = 0;
		virtual TList*             __fastcall  LisPerfilBarra(void) = 0;
      virtual void               __fastcall  Reinicia(void) = 0;
      virtual bool               __fastcall  SalvaPerfilBarra(TDateTime data) = 0;
		virtual bool               __fastcall  SalvaPerfilRegulador(TDateTime data) = 0;

   protected:  //métodos acessados via property
      virtual bool           __fastcall  PM_GetEnabled(void) = 0;
   };

//---------------------------------------------------------------------------
VTPerfilAlt* __fastcall NewObjPerfilAlt(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


