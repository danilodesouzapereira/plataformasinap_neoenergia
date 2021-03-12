//---------------------------------------------------------------------------
#ifndef TTramoH
#define TTramoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTTramo.h"

//---------------------------------------------------------------------------
class VTMatC;
class VTMatZ;

//---------------------------------------------------------------------------
class TTramo : public VTTramo
   {
   public:
                 __fastcall TTramo(int num_pat);
                 __fastcall ~TTramo(void);
      VTTramo*   __fastcall Clone(void);
      void       __fastcall CopiaAtributosDe(VTEqpto &ref);
      bool       __fastcall InsereTrecho(VTTrecho *trecho);
      TList*     __fastcall LisBarraInterna(void);
      TList*     __fastcall LisTrecho(void);
      void       __fastcall LisTrecho(TList *lisEXT);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);
      bool       __fastcall Z0_ohm(double &r0, double &x0, double &yc0);
      bool       __fastcall Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0);
      bool       __fastcall Z1_ohm(double &r1, double &x1, double &yc1);
      bool       __fastcall Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1);

	protected: //métodos virtuais redefinidos
		int        __fastcall Fases(VTBarra *barra);

	protected: //métodos acessados via property
      double          __fastcall PM_GetCapacidade_amp(void);
      double          __fastcall PM_GetComprimento_m(void);
      double          __fastcall PM_GetComprimento_km(void);
      bool            __fastcall PM_GetEquilibrado(void);
      double          __fastcall PM_GetIadm_amp(int nf);
      VTMatZ*         __fastcall PM_GetMatZ(void);
      int             __fastcall PM_GetTipoCfg(void);
      bool            __fastcall PM_GetUtilizaNeutro(void);
      complex<double> __fastcall PM_GetZ_ohm(int i, int j);

   private:
      float  iadm_min;      //capacidade mínima, em A
      float  comp_km;
      strIMP z0;            //impedância de seq.0  (ohm)
      strIMP z1;            //impedância de seq.1  (ohm)
      float  ycap0;         //capacitânca de seq.0 (mho)
      float  ycap1;         //capacitânca de seq.1 (mho)
      VTMatZ *matZ;         //matriz de impedâncias(4x4) em ohm
      TList  *lisBAR;       //lista de Barras internas (NÃO inclui pbarra1 e pbarra2)
      TList  *lisTRE;       //lista de Trechos ordenados
   };

//---------------------------------------------------------------------------
#endif
//eof

