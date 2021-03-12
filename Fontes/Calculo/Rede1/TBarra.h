//---------------------------------------------------------------------------
#ifndef TBarraH
#define TBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

#include "..\Comum13\VTGeralC.h"

class TBateriaC;
class TSup;
class VTEqpto;

//---------------------------------------------------------------------------
class TBarra : public TObject
{
public:  // Definição de dados locais

public:  // Dados locais
   int             id;              // ID interno da barra (gerado pelo programa)
   VTEqpto         *eqpto;          // Ponteiro para equipamento SINAP
   AnsiString      codigo;          // Código externo da barra (definido pelo usuário)
   int             offset;          // Posição da barra na lista (primeira = 0)
   int             linhaY;          // Linha da barra na matriz Ynodal (primeira = 0):
                                    //    NÃO É obrigatoriamente igual a 'offset' por
                                    //    causa das barras desconexas (linhaY = -1)
   int             tipo;            // Tipo da barra (cf. enum_TIPO_BARRA)
   int             tipo_original;   // Para controlar mudanças
   bool            mantida;         // Flag que indica se a barra será mantida ou não (Redutor1)
   bool            sup_equivalente; // Flag que indica cálculo de suprimento equivalente em barra mantida (só tem sentido para mantida == true)
   bool            flag_conexa;     // Flag que indica se a barra é conexa (true) ou não (false);
                                    //    usado somente no módulo Estabilidade
   double          vnom_kv;         // Tensao nominal de LINHA da barra (kV)
   double          ang_ref_rad;     // Zona de referência da barra (rad)
   double          best0_pu;        // Susceptância dos estáticos de barra - seq. zero   (cap. e reator, pu)
   double          best1_pu;        // Susceptância dos estáticos de barra - seq. direta (cap. e reator, pu)
   complex<double> yref_reduc0_pu;  // Admit. para a ref. gerada na redução de rede, seq. 0 (pu)
   complex<double> yref_reduc1_pu;  // Admit. para a ref. gerada na redução de rede, seq. 1 (pu)
   complex<double> yref_soma0_pu;   // Soma dos elementos da linha da barra em Ynodal, seq. 0 (pu)
   complex<double> yref_soma1_pu;   // Soma dos elementos da linha da barra em Ynodal, seq. 1 (pu)
   complex<double> vet_carga_pu[NUM_MODELO_CARGA]; // Potência nominal de carga nos 3 modelos (Icte, Scte e Zcte) (DADO) (pu)
   complex<double> tensao_pu;       // Tensão calculada (pu)
   complex<double> corr_a;          // Corrente calculada (A)
   complex<double> scalc_mva;       // Potência complexa calculada (carga ou geração, MVA)
   TSup            *sup;            // Ponteiro para suprimento (NULL se não for suprimento)
   TList           *lisLIG;         // Lista de ligações da barra (classe TLigacaoC)
   TList           *lisBATERIA;     // Lista de baterias da barra (classe TBateriaC)

public:  // Métodos
                              __fastcall  TBarra(void);
                              __fastcall ~TBarra(void);
              void            __fastcall  AddBateria(TBateriaC *bateria);
              complex<double> __fastcall  CalculaCorrente(double          Sbase_mva,
                                                          complex<double> v1);
              bool            __fastcall  CargaNula(void);
              bool            __fastcall  TipoCarga(void);
              bool            __fastcall  TipoGeracao(void);
              void            __fastcall  ZeraCapacitorReator(int seq);
              void            __fastcall  ZeraCarga(void);
};

#endif
//---------------------------------------------------------------------------

