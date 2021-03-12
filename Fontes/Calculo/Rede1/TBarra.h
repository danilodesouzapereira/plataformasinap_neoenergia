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
public:  // Defini��o de dados locais

public:  // Dados locais
   int             id;              // ID interno da barra (gerado pelo programa)
   VTEqpto         *eqpto;          // Ponteiro para equipamento SINAP
   AnsiString      codigo;          // C�digo externo da barra (definido pelo usu�rio)
   int             offset;          // Posi��o da barra na lista (primeira = 0)
   int             linhaY;          // Linha da barra na matriz Ynodal (primeira = 0):
                                    //    N�O � obrigatoriamente igual a 'offset' por
                                    //    causa das barras desconexas (linhaY = -1)
   int             tipo;            // Tipo da barra (cf. enum_TIPO_BARRA)
   int             tipo_original;   // Para controlar mudan�as
   bool            mantida;         // Flag que indica se a barra ser� mantida ou n�o (Redutor1)
   bool            sup_equivalente; // Flag que indica c�lculo de suprimento equivalente em barra mantida (s� tem sentido para mantida == true)
   bool            flag_conexa;     // Flag que indica se a barra � conexa (true) ou n�o (false);
                                    //    usado somente no m�dulo Estabilidade
   double          vnom_kv;         // Tensao nominal de LINHA da barra (kV)
   double          ang_ref_rad;     // Zona de refer�ncia da barra (rad)
   double          best0_pu;        // Suscept�ncia dos est�ticos de barra - seq. zero   (cap. e reator, pu)
   double          best1_pu;        // Suscept�ncia dos est�ticos de barra - seq. direta (cap. e reator, pu)
   complex<double> yref_reduc0_pu;  // Admit. para a ref. gerada na redu��o de rede, seq. 0 (pu)
   complex<double> yref_reduc1_pu;  // Admit. para a ref. gerada na redu��o de rede, seq. 1 (pu)
   complex<double> yref_soma0_pu;   // Soma dos elementos da linha da barra em Ynodal, seq. 0 (pu)
   complex<double> yref_soma1_pu;   // Soma dos elementos da linha da barra em Ynodal, seq. 1 (pu)
   complex<double> vet_carga_pu[NUM_MODELO_CARGA]; // Pot�ncia nominal de carga nos 3 modelos (Icte, Scte e Zcte) (DADO) (pu)
   complex<double> tensao_pu;       // Tens�o calculada (pu)
   complex<double> corr_a;          // Corrente calculada (A)
   complex<double> scalc_mva;       // Pot�ncia complexa calculada (carga ou gera��o, MVA)
   TSup            *sup;            // Ponteiro para suprimento (NULL se n�o for suprimento)
   TList           *lisLIG;         // Lista de liga��es da barra (classe TLigacaoC)
   TList           *lisBATERIA;     // Lista de baterias da barra (classe TBateriaC)

public:  // M�todos
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

