//---------------------------------------------------------------------------
#ifndef TCPEqptoH
#define TCPEqptoH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;
class VTScg;
class VTApl;
//---------------------------------------------------------------------------
class TCPEqpto
   {
   public:
                 __fastcall  TCPEqpto(VTApl *apl);
                 __fastcall ~TCPEqpto(void);
      AnsiString __fastcall  LinhaArqCsv(bool utm);

   private: //métodos
      void  __fastcall ConverteUTMtoLatLon(void);
   private: //dados externos
      //VTBarra *barra;
      VTScg *scg;

   public: //dados locais
	  VTApl *apl;
	  struct{
            AnsiString subscriber;
            int        numero;
            AnsiString userData1;
            AnsiString userData2;
            AnsiString userData3;
            AnsiString userData4;
            AnsiString userData5;
            AnsiString userData6;
            AnsiString userData7;
            AnsiString userData8;
            int        act;
            int        fase;
            int        area;
            int       flag1;  //consumidor?
            int       flag2;  //chave?
            int       flag3;  //gerador?
            int       flag4;  //regulador
            int       flag5;  //capacitor
            int       flag6;  //subestacao
            int       flag7;  //trafo
            int       flag8;  //
            int        col;
            double     coordUTM_x;
            double     coordUTM_y;
            int        coordGMS_G_lat;
            int        coordGMS_M_lat;
            double     coordGMS_S_lat;
            int        coordGMS_G_lon;
            int        coordGMS_M_lon;
            double     coordGMS_S_lon;
            AnsiString coordGMS_lat;
            AnsiString coordGMS_lon;
            } strEQPTO_CP;
   };
//função global p/ criar objeto da classe VTRede
//---------------------------------------------------------------------------
//VTCPEqpto* __fastcall NewObjCPEqpto(VTApl *apl, VTEqpto *eqpto);
//---------------------------------------------------------------------------
#endif
//eof
