//---------------------------------------------------------------------------
#ifndef TParseZonaH
#define TParseZonaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTScg.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TParseZona
   {
   public:
                 __fastcall  TParseZona(VTApl *apl);
                 __fastcall ~TParseZona(void);
      double     __fastcall  CentralMeridian(void);
      bool       __fastcall  ReadProjCSParms(TProjCS &ProjCS);
      bool       __fastcall  ExisteEmpresa(AnsiString empresa);
      double     __fastcall  FalseEasting(void);
      double     __fastcall  FalseNorthing(void);
      double     __fastcall  OriginLatitude(void);
      int        __fastcall  TipoCoord(void);
      AnsiString __fastcall  UTMZoneX(void);
      AnsiString __fastcall  UTMZoneY(void);


   private: //métodos
      int  __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
      void __fastcall InsereColunas(AnsiString linha, int tipo, double fE, double fN, double cM, double oL);
//      void __fastcall InsereColunaTipoCoord(AnsiString linha, int tipo);
      void __fastcall VerificaArquivoZonas(void);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TStrings   *campos;
      TStrings   *lines, *new_lines;
      int         tipo_coord;
		struct{
            AnsiString x, y;
            }utm_zone;
      struct{
            double falseEasting;  //metro
            double falseNorthing; //metro
            double centralMeridian; //grau decimal
            double originLatitude;  //grau decimal
            }polyc;
   };

#endif
//---------------------------------------------------------------------------
//eof

