//---------------------------------------------------------------------------
#ifndef TMedidorH
#define TMedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;

//---------------------------------------------------------------------------
class TMedidor : public TObject
{
public:  // Dados locais
   void       *snp_canal;      // Ponteiro para o objeto VTCanal no Sinap
   AnsiString codigo;          // Código do medidor
   bool       ligado;          // Indica se o medidor está ligado ou não
   // bool       descartavel;     // Indica se o medidor pode ser descartado ou não
   bool       enabled;         // Indica se o canal está habilitado ou não (tratamento de erros)
   int        eqpto_id;        // ID do equipamento medido
   void       *eqpto;          // Ponteiro para o equipamento medido (TBarra, TTrecho, TTrafo2 ou TTrafo32)
   int        tipo_eqpto;      // Tipo do equipamento medido (teBARRA, teTRECHO ou teTRAFO, cf. enum_TIPO_EQPTO em VTGeralC.h)
   int        tipo_medidor;    // Tipo do medidor (Real ou Virtual, cf. enum_TIPO_MEDIDOR_13 em VTGeralC.h)
   int        tipo_medicao;    // Tipo da medição (V, I, P, Q ou S, cf. enum_TIPO_MEDICAO em VTGeralC.h)
   int        id_calc;         // ID dentro do Cálculo (Barra, Trecho ou Trafo)
   TBarra     *bar_ref;        // Ponteiro para a barra onde está instalado o medidor (relev. p/ TTrecho, TTrafo2 e TTrafo32)
   int        fase_int;        // Fase medida (faseA, faseB, faseC ou faseN) - SOMENTE ESTIMADOR3
   double     toler_pu;        // Tolerância da medição (pu = %/100)
   double     sigma1_pu;       // Desvio-padrão do medidor ANTES  da normalização (pu do sistema elétrico)
   double     sigma2_pu;       // Desvio-padrão do medidor DEPOIS da normalização (pu do sistema elétrico)
   double     val_fe;          // Valor de fundo de escala
   double     val_fe_pu;       // Valor de fundo de escala (pu do sistema elétrico)
   double     val_med_pu;      // Valor medido (pu do sistema elétrico)
   double     val_est_pu;      // Valor estimado (pu do sistema elétrico)
   double     val_est_nom_pu;  // Valor estimado NOMINAL (pu do sistema elétrico, serve para retornar as cargas ao Sinap)
   double     desvio_ndp;      // Desvio da estimação em relação à medição (número de desvios-padrão)

public:  // Métodos
              __fastcall  TMedidor(void);
              __fastcall ~TMedidor(void);
         void __fastcall  CalculaDesvioPadrao(double semi_intervalo_normal);
         void __fastcall  CalculaFundoDeEscala1(double Vnom_linha_kV,
                                                double Sbase_3F_MVA,
                                                double semi_intervalo_normal,
                                                double valor_fe);
         void __fastcall  CalculaFundoDeEscala3(double Vnom_fase_kV,
                                                double Sbase_fase_MVA,
                                                double semi_intervalo_normal,
                                                double valor_fe);
         bool __fastcall  TipoMedicaoCorrente(void);
         bool __fastcall  TipoMedicaoPotencia(void);
         bool __fastcall  TipoMedicaoTensao(void);
};

#endif
//---------------------------------------------------------------------------

