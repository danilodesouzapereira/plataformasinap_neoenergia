//---------------------------------------------------------------------------
#ifndef VTMotorH
#define VTMotorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCurva;

//---------------------------------------------------------------------------
enum enumTIPO_MOTOR         {tmGAIOLA=0, tmBOBINADO};
enum enumTIPO_EQPTO_PARTIDA {eqptoNENHUM=0, eqptoCHAVE_YD, eqptoAUTO_TRAFO, eqptoSOFT_STARTER};

//---------------------------------------------------------------------------
class VTMotor : public TEqbar
   {
   public:  //property
      //tipo do motor
      __property int        TipoMotor          = {read=motor.tipo ,                 write= motor.tipo };
      __property AnsiString TipoMotorAsString  = {read=PM_GetTipoMotorAsString                        };
      //circuito equivalente
      __property double r1_pu              = {read=circ_eqv.r1_pu ,             write=circ_eqv.r1_pu };
      __property double x1_pu              = {read=circ_eqv.x1_pu ,             write=circ_eqv.x1_pu };
      __property double r2_pu              = {read=circ_eqv.r2_pu ,             write=circ_eqv.r2_pu };
      __property double x2_pu              = {read=circ_eqv.x2_pu ,             write=circ_eqv.x2_pu };
      __property double rp_pu              = {read=circ_eqv.rp_pu ,             write=circ_eqv.rp_pu };
      __property double xm_pu              = {read=circ_eqv.xm_pu ,             write=circ_eqv.xm_pu };
      __property double dr2_s1_pur2        = {read=circ_eqv.dr2_s1_pur2,        write=circ_eqv.dr2_s1_pur2};
      __property double dx2_s1_pux2        = {read=circ_eqv.dx2_s1_pux2,        write=circ_eqv.dx2_s1_pux2};
      //valores nominais
      __property int    num_polos          = {read=valor_nom.num_polos,          write=valor_nom.num_polos};
      __property double pmec_nom_kW        = {read=valor_nom.pmec_nom_kW,        write=valor_nom.pmec_nom_kW};
      __property double vnom_V             = {read=valor_nom.vnom_V,             write=valor_nom.vnom_V};
      __property double freq_Hz            = {read=valor_nom.freq_Hz,            write=valor_nom.freq_Hz};
      __property double s100_pu            = {read=valor_nom.s100_pu,            write=valor_nom.s100_pu};
      __property double inom_A             = {read=valor_nom.inom_A,             write=valor_nom.inom_A};
      __property double ipart_A            = {read=valor_nom.ipart_A,            write=valor_nom.ipart_A};
      __property double fpot100_pu         = {read=valor_nom.fpot100_pu,         write=valor_nom.fpot100_pu};
      __property double fpot75_pu          = {read=valor_nom.fpot75_pu,          write=valor_nom.fpot75_pu};
      __property double fpot50_pu          = {read=valor_nom.fpot50_pu,          write=valor_nom.fpot50_pu};
      __property double rend100_pu         = {read=valor_nom.rend100_pu,         write=valor_nom.rend100_pu};
      __property double rend75_pu          = {read=valor_nom.rend75_pu,          write=valor_nom.rend75_pu};
      __property double rend50_pu          = {read=valor_nom.rend50_pu,          write=valor_nom.rend50_pu};
      __property double conj_part_Nm       = {read=valor_nom.conj_part_Nm,       write=valor_nom.conj_part_Nm};
      __property double conj_max_Nm        = {read=valor_nom.conj_max_Nm,        write=valor_nom.conj_max_Nm};
      __property double J_kgm2             = {read=valor_nom.J_kgm2,             write=valor_nom.J_kgm2};

      //dados da carga mecânica
      __property double J_carga_kgm2       = {read=carga_mec.J_carga_kgm2,       write=carga_mec.J_carga_kgm2};
      __property double conj_carga_nom_Nm  = {read=carga_mec.conj_carga_nom_Nm,  write=carga_mec.conj_carga_nom_Nm};
      __property double conj_carga_part_Nm = {read=carga_mec.conj_carga_part_Nm, write=carga_mec.conj_carga_part_Nm};

   public:
                       __fastcall VTMotor(void) {};
      virtual          __fastcall ~VTMotor(void) {};
      virtual VTMotor* __fastcall Clone(void) = 0;
      virtual void     __fastcall DefineCurva(VTCurva *curva)=0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetTipoMotorAsString(void) = 0;

   public: //objetos externos
      VTCurva   *curva;
      struct{//eqpto auxiliar de partida
            int            tipo;
            struct{
                  double tap_inicial;
                  }auto_trafo;
            struct{
                  int ligacao_inicial;
                  }chave_YD;
            struct{
                  bool   freq_variavel;
                  int    duracao_partida_ms;
                  double tensao_inicial_pu;
                  }soft_starter;
            }eqpto_partida;

   protected:  //dados acessados via property
      struct{
            int tipo;
            }motor;
      struct{//circuito equivalente
            double r1_pu;
            double x1_pu;
            double r2_pu;
            double x2_pu;
            double rp_pu;
            double xm_pu;
            double dr2_s1_pur2;
            double dx2_s1_pux2;
            }circ_eqv;
      struct{//valores nominais
            int    num_polos;
            double pmec_nom_kW;
            double vnom_V;
            double freq_Hz;
            double s100_pu;
            double inom_A;
            double ipart_A;
            double fpot100_pu;
            double fpot75_pu;
            double fpot50_pu;
            double rend100_pu;
            double rend75_pu;
            double rend50_pu;
            double conj_part_Nm;
            double conj_max_Nm;
            double J_kgm2;
            }valor_nom;
      struct{//dados da carga mecânica
            double J_carga_kgm2;
            double conj_carga_nom_Nm;
            double conj_carga_part_Nm;
            }carga_mec;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Motor
//---------------------------------------------------------------------------
VTMotor* __fastcall NewObjMotor(void);

//---------------------------------------------------------------------------
#endif
//eof

 