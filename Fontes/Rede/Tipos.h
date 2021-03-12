//---------------------------------------------------------------------------
#ifndef TiposH
#define TiposH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//tipos de redes
enum ENUM_TIPO_CHAVE_SINAP {chaveDISJUNTOR=0,
                           chaveFUSIVEL,
                           chaveRELIGADORA,
                           chaveSECCIONADORA};

//---------------------------------------------------------------------------
//tipos de redes
enum ENUM_TIPO_REDE_SINAP  {redeGERACAO=0,
                           redeTRANSMISSAO,
                           redeSUBTRANSMISSAO,
                           redeDISTRIBUICAO_PRIMARIA,
                           redeDISTRIBUICAO_SECUNDARIA,
                           redeTRANSFORMACAO,
                           redeTRANSFORMACAO_AT_MT,
                           redeTRANSFORMACAO_MT_MT,
                           redeTRANSFORMACAO_MT_BT,
                           redeCONVERSAO_AC_DC,
                           redeCONVERSAO_DC_AC,
                           redeILUMINACAO_PUBLICA,
                           redeCHAVEAMENTO,
                           redeGENERICA
                           };

//---------------------------------------------------------------------------
//tipos de redes p/ Eletropaulo
enum ENUM_TIPO_REDE_ELPA   {redeELPA_MINI=redeGENERICA+1,
                           redeELPA_HIBRIDO,
                           redeELPA_SPOT,
                           };

//---------------------------------------------------------------------------
#endif
//eof


