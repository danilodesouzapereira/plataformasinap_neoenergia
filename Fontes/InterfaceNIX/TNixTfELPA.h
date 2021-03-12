//---------------------------------------------------------------------------
#ifndef TNixTfELPAH
#define TNixTfELPAH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include "..\Complexo\Complexo.h"
//#include "VTInterfaceNIX.h"
#include "TNixTf.h"
//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTDemanda;
class VTPatamares;
class VTSecundario;
class VTTrafo;
//---------------------------------------------------------------------------
class TNixTfELPA : public TNixTf
   {
   public:
                 __fastcall  TNixTfELPA(VTApl *apl, VTCarga *carga, strCFGINIX cfg);
                 __fastcall  TNixTfELPA(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg);
                 __fastcall  TNixTfELPA(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg);
                 __fastcall ~TNixTfELPA(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos pai
		//bool   __fastcall AlocaVectorSmva(int numpat);
		//void   __fastcall ClearVetorSmva(void);
		//int    __fastcall FasesPrimario(void);
		double __fastcall FatorDemanda(void);  //override
		double __fastcall FatorDemanda(VTCarga *carga);
		//double __fastcall FatorPotencia(void);
		//double __fastcall FatorPotencia(VTCarga *carga);
		//void   __fastcall LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT);
		//void   __fastcall LisCargasBancoTrafo(TList *lisTrafos, TList *lisEXT);
		//int    __fastcall NumeroDeCarga(int classe_tag);
		//double __fastcall PotenciaInstalada_kva(void);
		//double __fastcall PotenciaInstalada_kva(VTCarga *carga);
		//int    __fastcall TraduzFases(int fases);

	private: //metodos
		int    __fastcall FasesPrimario(VTTrafo *trafo);
		double __fastcall FatorDemanda(VTTrafo *trafo, int tipoLig);
		double __fastcall FatorDemandaDiurno(VTTrafo *trafo, int tipoLig);
		double __fastcall FatorDemandaNoturno(VTTrafo *trafo, int tipoLig);
		double __fastcall FatorPotencia(VTTrafo *trafo);
		void	 __fastcall InfoET(TList *lisTRAFOS);
		double __fastcall PotenciaInstalada_kva(VTTrafo *trafo, int tipoLig);
		double __fastcall PotenciaInstaladaDiurno_kva(VTTrafo *trafo, int tipoLig);
		double __fastcall PotenciaInstaladaNoturno_kva(VTTrafo *trafo, int tipoLig);
		int 	 __fastcall TipoET(TList *lisTRAFOS);

   private: //dados externos
	  //VTDemanda *demanda;
	  //VTPatamares *patamares;
      //VTApl     *apl;
   private: //dados locais
	  //TList *lisCAR;
      //TList *lisTRF;
      struct{
            AnsiString tf;
            int        numero_no;
            AnsiString codigo;
            double     potencia;
            double     fp;
            double     fd;
            int        tipo;
            int        proprietario;
            int        fases;
            int        residenciais;
            int        comerciais;
            int        industriais;
            int        rurais;
            int        outros;
            } strTF[3];
	  //enum para diferenciar as ETs
	  enum tipoET {
					tetMONO = 1,
					tetTRIF,
					tetDELTAABERTO,
					tetDELTAFECHADO
					};
	  //enum para diferenciar os trafos dentro de uma ET
	  enum tipoTRAFOELPA {
					tteLUZ = 0,
					tteFORCA
					};
	  //guardar a info da ET
	  struct{
			int numTrafos;
			double potForca_kva;
			double potLuz_kva;
			}infoET;

	  strCFGINIX cfg;

   };

//---------------------------------------------------------------------------
#endif
//eof

