//---------------------------------------------------------------------------
#ifndef ConstH
#define ConstH

//---------------------------------------------------------------------------
//constantes
#define ANO_0        0
#define ANO_1        1
#define ANO_2        2
#define ANO_3        3
#define ANO_4        4
#define ANO_5        5
#define MAX_ANO      6
#define MAX_PAT      4
/*
#define NUM_FASE     3
#define NUM_FASEN    4
#define MAX_FASE     4
#define MAX_FASEN    4
*/
#define MAX_FAIXA    3       //número de faixas p/ diagnóstico
#define MAX_FAIXA_TR 10      //número de faixas de carregamento de trafos de distribuição
#define MAX_COR      10      //número de cores p/ circuitos
#define MAX_CLASCONS 5
#define MAX_CLASCONSBAND 9   //classe de consumidores p/ Mercado por Energia da Band
#define MAX_ALFA     5
#define MERCADO_MAX_QUAD     5
#define MERCADO_MAX_FATPOND 10
#define MAX_PONTOS    96
#define MAX_FATIAS 60
#define MIN_FATIAS 10
#define MIN_CORREL 0.1

//---------------------------------------------------------------------------
//constantes de identificação de bitmaps
enum enumBMP {bmpCAPACITOR=0, bmpCAPSERIE,    bmpCARGA,      bmpFILTRO,
              bmpGERADOR,     bmpMEDIDOR,     bmpMUTUA,      bmpREATOR,
              bmpREGULADOR,   bmpSUPRIMENTO,  bmpTRAFO_H,    bmpTRAFO_V,
              bmpTRAFO3E_H,   bmpTRAFO3E_V,   bmpTRAFOZZ,    bmpZREF,
              bmpMAX,         bmpMOTOR,       bmpBATERIA};
//---------------------------------------------------------------------------
//constantes de identificação de icones vetoriais
enum enumICONE {iconeCAPACITOR=0,  iconeCAPSERIE,    iconeCARGA,
                iconeFILTRO,       iconeGERADOR,     iconeREATOR,
                iconeMUTUA,        iconeREGULADOR,   iconeSUPRIMENTO,
                //trafo2E
                iconeTRF2E_YaYa,   iconeTRF2E_YaD,   iconeTRF2E_YaY,
                iconeTRF2E_DYa,    iconeTRF2E_DD,    iconeTRF2E_DY,
                iconeTRF2E_YYa,    iconeTRF2E_YD,    iconeTRF2E_YY,
                //trafo3E
                iconeTRF3E_YaYaYa, iconeTRF3E_YaYaD, iconeTRF3E_YaYaY,
                iconeTRF3E_YaDYa,  iconeTRF3E_YaDD,  iconeTRF3E_YaDY,
                iconeTRF3E_YaYYa,  iconeTRF3E_YaYD,  iconeTRF3E_YaYY,
                iconeTRF3E_DYaYa,  iconeTRF3E_DYaD,  iconeTRF3E_DYaY,
                iconeTRF3E_DDYa,   iconeTRF3E_DDD,   iconeTRF3E_DDY,
                iconeTRF3E_DYYa,   iconeTRF3E_DYD,   iconeTRF3E_DYY,
                iconeTRF3E_YYaYa,  iconeTRF3E_YYaD,  iconeTRF3E_YYaY,
                iconeTRF3E_YDYa,   iconeTRF3E_YDD,   iconeTRF3E_YDY,
                iconeTRF3E_YYYa,   iconeTRF3E_YYD,   iconeTRF3E_YYY,
                //
                iconeTRAFOZZ,      iconeZREF,        iconeMAX,
                iconeMOTOR,        iconeBATERIA};

//---------------------------------------------------------------------------
//diagnósticos Barra, Trecho, Trafo, Gerador, Suprimento
enum diagFLOW {diagBOM=0, diagREGULAR=1, diagRUIM=2, diagISOLADO=3};

//---------------------------------------------------------------------------
enum sentidoFLUXO {//sentido do fluxo da Barra_I para Barra_J
                  fluxoNENHUM=0,
                  fluxoPOS,
                  fluxoNEG,
                  fluxoINDEF};
                  
//---------------------------------------------------------------------------
//enumerador p/ tipos de patamares
enum patESCALA  {pat24HORA=1,   pat12HORA=2,    pat08HORA=3,    pat06HORA=4,
                 pat04HORA=6,   pat03HORA=8,    pat02HORA=12,   pat01HORA=24,
                 pat15MIN=96,   pat10MIN=144,   pat05MIN=288,
                 //patamares com período variável
                 patEDITAVEL=0, patMMTN=-1,     patLMP=-2};

//---------------------------------------------------------------------------
//enumerador p/ categorias de Ajuste de Demanda
enum categoriaAJUSTE {
					 catAD_INV = 0,
					 catAD_FRAUDULENTO = 1,
					 catAD_IRREGULAR,
					 catAD_NADAAPARENTE,
					 catAD_NAO_INSPEC,
                     catAD_COUNT
					 };
#endif
//---------------------------------------------------------------------------
//eof
