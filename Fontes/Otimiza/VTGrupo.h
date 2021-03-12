//---------------------------------------------------------------------------
#ifndef VTGrupoH
#define VTGrupoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTEqpto;
//---------------------------------------------------------------------------
enum tipoGrupo  {//tipos de grupo
				 tipoGer=0,
				 tipoChv
				};

//---------------------------------------------------------------------------
enum tipoAtua  {//tipos de grupo
				 tipoManual=0,
				 tipoAuto
				};

//---------------------------------------------------------------------------
class VTGrupo: public TObject
   {
   public:
		 __fastcall VTGrupo(void) {};
		 __fastcall ~VTGrupo(void) {};
		 virtual void       __fastcall AddEqpto(VTEqpto* eqpto) = 0;
		 virtual void       __fastcall AtivaEqpto(VTEqpto* eqpto) = 0;
		 virtual AnsiString __fastcall Codigo(void) = 0;
		 virtual void       __fastcall DesatEqpto(VTEqpto* eqpto) = 0;
		 virtual VTEqpto*   __fastcall Eqpto(int neq) = 0;
		 virtual TList*     __fastcall LisEqpto(void) = 0;
		 virtual int        __fastcall NEq(void) = 0;
		 virtual void       __fastcall RemEqpto(VTEqpto* eqpto) = 0;
		 virtual bool       __fastcall VerEqpto(VTEqpto* eqpto) = 0;
   public:
	  AnsiString nome;
	  double custo;
	  int      itipo;
	  int      itipoVar;
	  VTGrupo* pgrupo_aux;
	  bool     enabled;//Se entra na otimização para candidatos
	  bool     ativo;//Excluído da otimização
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDespacho
//---------------------------------------------------------------------------
VTGrupo* NewObjGrupo(VTEqpto* eqpto);
VTGrupo* NewObjGrupoChv(VTEqpto* eqpto);
VTGrupo* NewObjGrupoChvManobra(VTEqpto* eqpto);

//---------------------------------------------------------------------------
#endif

 