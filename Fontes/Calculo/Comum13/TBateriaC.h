//---------------------------------------------------------------------------
#ifndef TBateriaCH
#define TBateriaCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class  TBarra;
class  VTData;
class  VTEqpto;
struct smcBATERIA;

//---------------------------------------------------------------------------
class TBateriaC : public TObject
{
public:  // Dados
   // Dados nominais (fixos)
   VTEqpto    *eqpto;           // VTEqpto do Sinap
   AnsiString codigo;           // Código da bateria
   int        id;               // ID da bateria
   TBarra     *barra;           // Barra à qual a bateria está ligada
   int        fases;            // Fases da bateria (faseAN, faseAB, etc.)
   int        ligacao;          // Cf. enum_LIG_CARGA             em VTGeralC.h
   int        tipo_operacao;    // Cf. enum_BATERIA_TIPO_OPERACAO em VTGeralC.h
   int        modelo_carga;     // Icte, Scte ou Zcte quando no estado "Carga"
   int        modelo_desc;      // Icte, Scte ou Zcte quando no estado "Descarga"
   double     pnom_kw;          // Potência ativa nominal (valor de referência)
   double     enom_kwh;         // Energia nominal (capacidade)
   double     scap_inv_kva;     // Potência aparente máxima do inversor (capacidade)
   double     en_armaz_inic_pu; // Energia inicial armazenada (pu de 'enom_kwh')
   double     pext_carga_kw;    // Potência ativa   externa absorvida durante a carga (só tipo AUTO)
   double     qext_carga_kvar;  // Potência reativa externa absorvida durante a carga (só tipo AUTO)
   double     pext_desc_kw;     // Potência ativa   externa fornecida durante a descarga (só tipo AUTO)
   double     qext_desc_kvar;   // Potência reativa externa fornecida durante a descarga (só tipo AUTO)
   double     p_ociosa_pu;      // Potência ativa   absorvida no estado "ociosa" (pu de 'pnom_kw')
   double     q_ociosa_pu;      // Potência reativa absorvida no estado "ociosa" (pu de 'pnom_kw')
   double     rend_carga_pu;    // Rendimento na etapa de carga
   double     rend_desc_pu;     // Rendimento na etapa de descarga
   double     gatilho_carga_pu; // Valor mínimo de energia armazenada para início de carga    (pu de 'enom_kwh')
   double     gatilho_desc_pu;  // Valor máximo de energia armazenada para início de descarga (pu de 'enom_kwh'
   // Dados da operação (variáveis)
   bool       inverte_modo;     // Flag que sinaliza que o modo de operação deverá ser invertido no próximo patamar
   int        estado;           // Estado atual (cf. enum_BATERIA_ESTADO em VTGeralC.h)
   int        modo_operacao;    // Modo de operação atual (cf. enum_BATERIA_MODO_OPERACAO em VTGeralC.h)
   double     pext_kw;          // Potência ativa   externa (absorvida/fornecida)
   double     qext_kvar;        // Potência reativa externa (absorvida/fornecida)
   double     pcarga_kw;        // Potência efetiva de carga    (interna)
   double     pdesc_kw;         // Potência efetiva de descarga (interna)
   double     en_armaz_kwh;     // Energia armazenada NO FIM DO PATAMAR

public:  // Métodos
              __fastcall  TBateriaC(TBarra *barra);
              __fastcall ~TBateriaC(void);
         bool __fastcall  Atualiza(int    ind_pat,
                                   double duracao_h,
                                   VTData *data);
         void __fastcall  AtualizaDados(smcBATERIA *bateria);

private:  // Dados
   double en_armaz_max_kwh;  // Energia maxima armazenada
   double en_armaz_min_kwh;  // Energia mínima armazenada

private:  // Métodos
         bool __fastcall AtualizaTipoAUTO(int    ind_pat,
                                          double duracao_h,
                                          VTData *data);
         bool __fastcall AtualizaTipoMANUAL(int    ind_pat,
                                            double duracao_h,
                                            VTData *data);
         // void __fastcall DefineEstadoManual(int ind_pat);
         void __fastcall VerificaLimitePotenciasExternas(double *px_kw,
                                                         double *qx_kvar);
};

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TBateriaC
//---------------------------------------------------------------------------
TBateriaC * __fastcall NewObjBateriaC(TBarra *barra);

#endif
//---------------------------------------------------------------------------

