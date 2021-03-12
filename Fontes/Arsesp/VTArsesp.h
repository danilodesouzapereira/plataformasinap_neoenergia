//---------------------------------------------------------------------------
#ifndef VTArsespH
#define VTArsespH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMedicao;
class VTAjustaDemanda;
class VTPrimario;

//---------------------------------------------------------------------------
class VTArsesp : public TObject
   {
   public:
                   __fastcall  VTArsesp(void) {};
      virtual      __fastcall ~VTArsesp(void) {};
      virtual bool __fastcall  Executa(VTPrimario *primario)=0;

   public:  //dados
     struct   {
               struct   {
                        AnsiString cod_primario;
                        AnsiString cod_etd;
                        double     vnom_kv;
                        } rede;
               struct   {
                        double i_faseA, i_faseB, i_faseC;
                        } medicao;
               struct   {
                        double fatcor_faseA, fatcor_faseB, fatcor_faseC;
                        double i_faseA, i_faseB, i_faseC;
                        } ajuste;
               struct   {
                        int    ind_pat;
                        double v_min_pu;
                        double dv_perc;
                        } fluxo_minimo;
               struct   {
                        int    ind_pat;
                        double v_min_pu;
                        double dv_perc;
                        } fluxo_maximo;
               struct   {
                        int        num_cons_primario;
                        int        num_cons_sec_res;
                        int        num_cons_sec_com;
                        int        num_cons_sec_ind;
                        int        num_cons_sec_rur;
                        int        num_cons_sec_out;
                        AnsiString cabo_tronco;
                        double     comp_total;
                        double     comp_tronco;
                        double     comp_ramal;
                        } topologia;
               struct   {
                        bool       sucesso;
                        AnsiString observacao;
                        int        t_mseg;
                        } diagnostico;
               }resumo;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTArsesp* __fastcall NewObjArsesp(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 