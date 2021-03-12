//---------------------------------------------------------------------------
#ifndef TReguladorH
#define TReguladorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <map>
#include "VTRegulador.h"

//---------------------------------------------------------------------------
class TRegulador : public VTRegulador
   {
   public:
				   __fastcall TRegulador(int num_pat);
				   __fastcall ~TRegulador(void);
	  VTRegulador* __fastcall Clone(void);
	  void         __fastcall CopiaAtributosDe(VTEqpto &eqpto);
	  void         __fastcall DefineBarraEntrada(VTBarra *barra_ent);
	  void         __fastcall IniciaLisUM(void);
	  double       __fastcall InomA(void);
	  TList*       __fastcall LisUM(void);
	  double       __fastcall NivelTensaoDiretoV(void);
	  double       __fastcall NivelTensaoInversoV(void);
	  int          __fastcall NumUM(void);
	  int          __fastcall Passo(int fase);
	  int          __fastcall Tipo(void);
	  AnsiString   __fastcall TipoAsString(void);
	  double       __fastcall LarguraBandaMorta_pu(void);
	  double       __fastcall LarguraBandaMortaInv_pu(void);

   private: //métodos acessados via property
	  AnsiString __fastcall PM_GetPadrao(void);
	  double     __fastcall PM_GetSpas_mva(void);
	  //set
	  void       __fastcall PM_SetPadrao(AnsiString padrao);
	  void       __fastcall PM_SetSpas_mva(double s_mva);
	  void       __fastcall PM_SetMaxPat(int max_pat);

   protected: //métodos virtuais redefinidos
	  int         __fastcall Fases(VTBarra *barra);

   private: //métodos
	  double __fastcall DeterminaTensao(void);
	  void   __fastcall InsereUM(int ligacao, int fase, int fase_comum, double snom_mva);
	  void   __fastcall LimpaLisUM(void);

   private:
	  AnsiString padrao;
	  std::map <double, double> capacidade_max; //faixa de tensão (key) e capacidade (value)
   };

//---------------------------------------------------------------------------
#endif
//eof

 