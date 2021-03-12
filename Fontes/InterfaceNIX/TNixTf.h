//---------------------------------------------------------------------------
#ifndef TNixTfH
#define TNixTfH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Complexo\Complexo.h"
#include "VTInterfaceNIX.h"
#include "VTNixTf.h"
//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTDemanda;
class VTPatamares;
class VTSecundario;
class VTTrafo;

//---------------------------------------------------------------------------
class TNixTf : public VTNixTf
   {
   public:
				 __fastcall  TNixTf(void);
				 __fastcall  TNixTf(VTApl *apl, VTCarga *carga, strCFGINIX cfg);
                 __fastcall  TNixTf(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg);
                 __fastcall  TNixTf(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg);
                 __fastcall ~TNixTf(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   protected: //métodos
      bool   __fastcall AlocaVectorSmva(int numpat);
      void   __fastcall ClearVetorSmva(void);
      int    __fastcall FasesPrimario(void);
      double __fastcall FatorDemanda(void);
      double __fastcall FatorDemanda(VTCarga *carga);
      double __fastcall FatorPotencia(void);
      double __fastcall FatorPotencia(VTCarga *carga);
      //void   __fastcall LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT);
      void   __fastcall LisCargasBancoTrafo(TList *lisTrafos, TList *lisEXT);
      int    __fastcall NumeroDeCarga(int classe_tag);
      double __fastcall PotenciaInstalada_kva(void);
      double __fastcall PotenciaInstalada_kva(VTCarga *carga);
      int    __fastcall TraduzFases(int fases);

   protected: //dados externos
	  VTDemanda *demanda;
	  VTPatamares *patamares;
      //VTApl     *apl;
   protected: //dados locais
	  TList *lisCAR;
	  TList *lisTRF;
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
            } strTF;
      //struct com o vetor complexo
      struct{
         int    size;
         complex<double> *complex;
         double          *P;
         }smva;
   };

//---------------------------------------------------------------------------
#endif
//eof

