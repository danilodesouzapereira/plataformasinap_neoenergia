//---------------------------------------------------------------------------
#ifndef EstruturasH
#define EstruturasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TImpressao3;
class VTAtualiza3;
class VTCalculo1;
class VTCalculo3;
class VTGeralC;
class VTMonta1;
class VTMonta3;
class VTRede1;
class VTRede3;

//---------------------------------------------------------------------------

struct Est1_G1_NR1 // Estrutura para passar dados entre Estabilidade, Gauss1 e NR1
   {
   complex<double> *vet_tensoes;
   VTGeralC        *geralC;
   VTRede1         *rede1;
   VTMonta1        *monta1;
   VTCalculo1      *calculo1;
   };

struct Filename  // Estrutura para armazenar todos os filenames
   {
   AnsiString dados;     // Nome do arquivo de dados
   AnsiString log;       // Nome do arquivo de log
   AnsiString log2;      // Nome do arquivo de log - 2
   AnsiString result;    // Nome do arquivo de resultados
   };

struct FluxoH3_G3  // Estrutura para passar dados entre FluxoH3 e Gauss3
   {
   VTGeralC    *geralC;
   VTRede3     *rede3;
   VTAtualiza3 *atual3;
   TImpressao3 *impr3;
   VTMonta3    *monta3;
   VTCalculo3  *calculo3;
   };

#endif
//---------------------------------------------------------------------------


